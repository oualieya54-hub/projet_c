#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include "course_callbacks.h"
#include "course_interface.h"
#include "../common/support.h"
#include "gcour.h"
#include <stdio.h>


#define FICHIER_INSCRIPTIONS "data/inscrit_cours.txt"

// Structure pour une inscription
typedef struct {
    char nom[100];
    char prenom[100];
    char type_cours[100];
    char niveau[20];
    char inscription_auto[10];
} InscriptionCours;


// ============================================================
// GLOBAL VARIABLES
// ============================================================
char cours_id_a_supprimer[30] = "";
char selected_cours_id[30] = "";

// ============================================================
// TREEVIEW FUNCTIONS
// ============================================================
void afficher_cours_treeview(GtkWidget *treeview)
{
    GtkListStore *store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), NULL);
    while (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0) != NULL) {
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), 
            gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0));
    }
    
    store = gtk_list_store_new(7, 
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    
    FILE *f = fopen("data/cours.txt", "r");
    if (f != NULL) {
        Cours c;
        while (fscanf(f, "%s %s %d %d %d %f %s %d %d %d\n",
                c.id, c.nom, &c.jour, &c.mois, &c.annee,
                &c.heure_depart, c.type_cour,
                &c.nb_personnes_max, &c.mixte, &c.nbPlacesOccupees) == 10) {
            
            char date_str[50], heure_str[20], mixte_str[20], max_str[20];
            sprintf(date_str, "%02d/%02d/%d", c.jour, c.mois, c.annee);
            sprintf(heure_str, "%.0f:00", c.heure_depart);
            sprintf(mixte_str, "%s", c.mixte ? "Oui" : "Non");
            sprintf(max_str, "%d", c.nb_personnes_max);
            
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                0, c.id, 1, c.nom, 2, date_str, 3, heure_str,
                4, c.type_cour, 5, mixte_str, 6, max_str, -1);
        }
        fclose(f);
    }
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Date", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Heure", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Mixte", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Max", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
}

// ============================================================
// TREEVIEW SELECTION HANDLER
// ============================================================
void on_treeview1_row_activated(GtkTreeView *treeview, GtkTreePath *path,
                                 GtkTreeViewColumn *column, gpointer user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *id;
    
    model = gtk_tree_view_get_model(treeview);
    
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(model, &iter, 0, &id, -1);
        strcpy(selected_cours_id, id);
        g_free(id);
        printf("DEBUG: Row activated - Selected ID: %s\n", selected_cours_id);
        
        // Show a confirmation message
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
            "Cours sélectionné : %s\nVous pouvez maintenant le modifier ou le supprimer.", 
            selected_cours_id);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}
gboolean on_cour_sportif_acceuil_focus_in_event(GtkWidget *widget, GdkEventFocus *event, gpointer user_data)
{
    printf("DEBUG: Main window gained focus - refreshing treeview\n");
    fflush(stdout);
    
    // Find the treeview in this window
    GtkWidget *treeview = lookup_widget(widget, "treeview1");
    
    if (treeview != NULL) {
        printf("DEBUG: Treeview found, refreshing...\n");
        // Refresh it!
        afficher_cours_treeview(treeview);
    } else {
        printf("ERROR: Treeview not found!\n");
    }
    
    // Return FALSE to let other handlers process this event too
    return FALSE;
}

// ============================================================
// AJOUTER UN COURS
// ============================================================
void on_button36_clicked(GtkButton *button, gpointer user_data)
{
    Cours c;
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "ajouter_cour");
    
    GtkWidget *entry_nom = lookup_widget(GTK_WIDGET(button), "entry42");
    GtkWidget *entry_id = lookup_widget(GTK_WIDGET(button), "entry43");
    
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entry_id));
    
    if (strlen(nom) == 0 || strlen(id) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Veuillez remplir tous les champs obligatoires (Nom et ID du cours)");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    Cours existing = chercher_cours((char*)id);
    if (strlen(existing.id) > 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Un cours avec cet ID existe déjà");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    strcpy(c.id, id);
    strcpy(c.nom, nom);
    
    GtkWidget *spin_jour = lookup_widget(GTK_WIDGET(button), "spinbutton3");
    GtkWidget *spin_mois = lookup_widget(GTK_WIDGET(button), "spinbutton4");
    GtkWidget *spin_annee = lookup_widget(GTK_WIDGET(button), "spinbutton5");
    
    c.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_jour));
    c.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_mois));
    c.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_annee));
    
    if (c.jour < 1 || c.jour > 31 || c.mois < 1 || c.mois > 12 || c.annee < 2024) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Date invalide. Vérifiez jour (1-31), mois (1-12) et année");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    GtkWidget *spin_heure = lookup_widget(GTK_WIDGET(button), "spinbutton1");
    c.heure_depart = (float)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_heure));
    
    if (c.heure_depart < 0 || c.heure_depart > 23) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Heure invalide (0-23)");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    GtkWidget *combo_type = lookup_widget(GTK_WIDGET(button), "comboboxentry7");
    gchar *type = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_type));
    if (type != NULL) {
        strcpy(c.type_cour, type);
        g_free(type);
    } else {
        strcpy(c.type_cour, "");
    }
    
    if (strlen(c.type_cour) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Veuillez sélectionner un type de cours");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    GtkWidget *radio_mixte = lookup_widget(GTK_WIDGET(button), "radiobutton2");
    c.mixte = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_mixte)) ? 1 : 0;
    
    GtkWidget *spin_nb = lookup_widget(GTK_WIDGET(button), "spinbutton2");
    c.nb_personnes_max = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_nb));
    
    if (c.nb_personnes_max < 1 || c.nb_personnes_max > 100) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Le nombre maximum de personnes doit être entre 1 et 100");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    c.nbPlacesOccupees = 0;
    
    ajouter_cours(c);
    
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
        GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
        "Cours enregistré avec succès !");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    GtkWidget *main_window = lookup_widget(GTK_WIDGET(button), "cour_sportif_acceuil");
    if (main_window == NULL) {
        main_window = lookup_widget(GTK_WIDGET(button), "cour_sportif_accueil");
    }
    if (main_window != NULL) {
        GtkWidget *treeview = lookup_widget(main_window, "treeview1");
        if (treeview != NULL) {
            afficher_cours_treeview(treeview);
        }
    }
    
    gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
    gtk_entry_set_text(GTK_ENTRY(entry_id), "");
    
    gtk_widget_hide(window);
}

void on_button37_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "ajouter_cour");
    gtk_widget_hide(window);
}

// ============================================================
// MODIFIER UN COURS
// ============================================================
void on_button39_clicked(GtkButton *button, gpointer user_data)
{
    Cours c;
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "modifer_cour");
    
    GtkWidget *entry_nom = lookup_widget(GTK_WIDGET(button), "entry44");
    GtkWidget *entry_id = lookup_widget(GTK_WIDGET(button), "entry45");
    
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const gchar *id_original = gtk_entry_get_text(GTK_ENTRY(entry_id));
    
    if (strlen(id_original) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Veuillez saisir un ID de cours");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    Cours c_ancien = chercher_cours((char*)id_original);
    if (strlen(c_ancien.id) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Cours introuvable avec cet ID");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    strcpy(c.id, id_original);
    strcpy(c.nom, nom);
    
    GtkWidget *spin_jour = lookup_widget(GTK_WIDGET(button), "spinbutton19");
    GtkWidget *spin_mois = lookup_widget(GTK_WIDGET(button), "spinbutton20");
    GtkWidget *spin_annee = lookup_widget(GTK_WIDGET(button), "spinbutton21");
    
    c.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_jour));
    c.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_mois));
    c.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_annee));
    
    GtkWidget *spin_heure = lookup_widget(GTK_WIDGET(button), "spinbutton22");
    c.heure_depart = (float)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_heure));
    
    GtkWidget *combo_type = lookup_widget(GTK_WIDGET(button), "comboboxentry8");
    gchar *type = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_type));
    if (type != NULL) {
        strcpy(c.type_cour, type);
        g_free(type);
    }
    
    GtkWidget *radio_mixte = lookup_widget(GTK_WIDGET(button), "radiobutton8");
    c.mixte = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_mixte)) ? 1 : 0;
    
    GtkWidget *spin_nb = lookup_widget(GTK_WIDGET(button), "spinbutton23");
    c.nb_personnes_max = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_nb));
    
    c.nbPlacesOccupees = c_ancien.nbPlacesOccupees;
    
    modifier_cours(c);
    
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
        GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
        "Cours modifié avec succès !");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    GtkWidget *main_window = lookup_widget(GTK_WIDGET(button), "cour_sportif_acceuil");
    if (main_window == NULL) {
        main_window = lookup_widget(GTK_WIDGET(button), "cour_sportif_accueil");
    }
    if (main_window != NULL) {
        GtkWidget *treeview = lookup_widget(main_window, "treeview1");
        if (treeview != NULL) {
            afficher_cours_treeview(treeview);
        }
    }
    
    gtk_widget_hide(window);
}

void on_button40_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "modifer_cour");
    gtk_widget_hide(window);
}

// ============================================================
// MAIN WINDOW BUTTONS
// ============================================================
void on_button1_clicked(GtkButton *button, gpointer user_data)
{
    printf("DEBUG: Search button clicked\n");
    GtkWidget *entry_recherche = lookup_widget(GTK_WIDGET(button), "entry46");

printf("DEBUG: Entry widget pointer: %p\n", entry_recherche);  // ADD THIS
    
    if (entry_recherche == NULL) {  // ADD THIS CHECK
        printf("ERROR: Could not find entry1!\n");
        return;}

    const gchar *recherche = gtk_entry_get_text(GTK_ENTRY(entry_recherche));
    
printf("DEBUG: Entry text is: '%s'\n", recherche);  // ADD THIS
    printf("DEBUG: Entry text length: %zu\n", strlen(recherche));  // ADD THIS

    if (strlen(recherche) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Veuillez saisir un critère de recherche");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    Cours c = chercher_cours((char*)recherche);
    
    if (strlen(c.id) > 0) {
        gchar *message = g_strdup_printf(
            "Cours trouvé :\n\n"
            "ID : %s\nNom : %s\nDate : %02d/%02d/%d\n"
            "Heure : %.2f\nType : %s\nMixte : %s\n"
            "Max : %d personnes\nPlaces occupées : %d",
            c.id, c.nom, c.jour, c.mois, c.annee,
            c.heure_depart, c.type_cour, 
            c.mixte ? "Oui" : "Non",
            c.nb_personnes_max, c.nbPlacesOccupees);
        
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
            "%s", message);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_free(message);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
            "Aucun cours trouvé");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void course_on_button2_clicked(GtkButton *button, gpointer user_data)
{
    printf("\n");
    printf("====================================================\n");
    printf("*** course_on_button2_clicked CALLED (ADD BUTTON) ***\n");
    printf("====================================================\n");
    fflush(stdout);
    
    GtkWidget *window = create_ajouter_cour();
    gtk_widget_show(window);
    
    printf("*** course_on_button2_clicked FINISHED - Add window shown ***\n\n");
    fflush(stdout);
}

void course_on_button3_clicked(GtkButton *button, gpointer user_data)
{
    printf("DEBUG: Modify button clicked\n");
    
    if (strlen(selected_cours_id) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Veuillez sélectionner un cours dans la liste avant de modifier");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    GtkWidget *window = create_modifer_cour();
    
    Cours c = chercher_cours(selected_cours_id);
    if (strlen(c.id) > 0) {
        GtkWidget *entry_id = lookup_widget(window, "entry45");
        GtkWidget *entry_nom = lookup_widget(window, "entry44");
        
        if (entry_id) gtk_entry_set_text(GTK_ENTRY(entry_id), c.id);
        if (entry_nom) gtk_entry_set_text(GTK_ENTRY(entry_nom), c.nom);
        
        GtkWidget *spin_jour = lookup_widget(window, "spinbutton19");
        GtkWidget *spin_mois = lookup_widget(window, "spinbutton20");
        GtkWidget *spin_annee = lookup_widget(window, "spinbutton21");
        GtkWidget *spin_heure = lookup_widget(window, "spinbutton22");
        GtkWidget *spin_nb = lookup_widget(window, "spinbutton23");
        
        if (spin_jour) gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_jour), c.jour);
        if (spin_mois) gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_mois), c.mois);
        if (spin_annee) gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_annee), c.annee);
        if (spin_heure) gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_heure), c.heure_depart);
        if (spin_nb) gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_nb), c.nb_personnes_max);
    }
    
    gtk_widget_show(window);
}

void on_button4_clicked(GtkButton *button, gpointer user_data)
{
    printf("\n");
    printf("====================================================\n");
    printf("*** on_button4_clicked CALLED (DELETE BUTTON) ***\n");
    printf("====================================================\n");
    printf("selected_cours_id = '%s'\n", selected_cours_id);
    printf("====================================================\n");
    fflush(stdout);
    
    if (strlen(selected_cours_id) == 0) {
        printf("ERROR: No course selected\n");
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Veuillez sélectionner un cours dans la liste avant de supprimer");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    printf("Creating delete confirmation dialog...\n");
    fflush(stdout);
    
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
        "Voulez-vous vraiment supprimer le cours '%s' ?", selected_cours_id);
    
    printf("Showing delete confirmation dialog...\n");
    fflush(stdout);
    
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    printf("User response: %d (YES=%d, NO=%d)\n", response, GTK_RESPONSE_YES, GTK_RESPONSE_NO);
    fflush(stdout);
    
    if (response == GTK_RESPONSE_YES) {
        printf("Deleting course: %s\n", selected_cours_id);
        fflush(stdout);
        
        supprimer_cours(selected_cours_id);
        
        GtkWidget *success = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
            "Cours supprimé avec succès !");
        gtk_dialog_run(GTK_DIALOG(success));
        gtk_widget_destroy(success);
        
        GtkWidget *main_window = lookup_widget(GTK_WIDGET(button), "cour_sportif_acceuil");
        if (main_window == NULL) {
            main_window = lookup_widget(GTK_WIDGET(button), "cour_sportif_accueil");
        }
        
        if (main_window != NULL) {
            GtkWidget *treeview = lookup_widget(main_window, "treeview1");
            if (treeview != NULL) {
                afficher_cours_treeview(treeview);
                printf("DEBUG: Treeview refreshed after deletion\n");
            }
        }
        
        strcpy(selected_cours_id, "");
    } else {
        printf("DEBUG: Deletion cancelled by user\n");
    }
    
    printf("*** on_button4_clicked FINISHED ***\n\n");
    fflush(stdout);
}

// ============================================================
// ADDITIONAL BUTTONS
// ============================================================
void on_button45_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "cour_enregistrer");
    gtk_widget_hide(window);
}

void on_button_ajouter_clicked(GtkButton *button, gpointer user_data)
{
    course_on_button2_clicked(button, user_data);
}

void on_button_modifier_clicked(GtkButton *button, gpointer user_data)
{
    course_on_button3_clicked(button, user_data);
}

void on_button_supprimer_clicked(GtkButton *button, gpointer user_data)
{
    on_button4_clicked(button, user_data);
}

void on_button46_clicked(GtkButton *button, gpointer user_data)
{
    printf("********** BUTTON 46 CLICKED **********\n");
    fflush(stdout);
    on_button36_clicked(button, user_data);
}

void on_button47_clicked(GtkButton *button, gpointer user_data)
{
    on_button37_clicked(button, user_data);
}

void on_button49_clicked(GtkButton *button, gpointer user_data)
{
    on_button39_clicked(button, user_data);
}

void on_button50_clicked(GtkButton *button, gpointer user_data)
{
    on_button40_clicked(button, user_data);
}

// ============================================================
// INSCRIPTION WINDOW CALLBACKS (placeholders)
// ============================================================
// PAR CECI :
// ============================================
// GESTION DES INSCRIPTIONS AUX COURS
// ============================================


// Fonction pour ajouter une inscription
int ajouter_inscription_cours(InscriptionCours ic) {
    FILE *f = fopen(FICHIER_INSCRIPTIONS, "a");
    
    if (f == NULL) {
        perror("Erreur d'ouverture du fichier inscrit_cours.txt");
        return 0;
    }
    
    fprintf(f, "%s|%s|%s|%s|%s\n",
            ic.nom,
            ic.prenom,
            ic.type_cours,
            ic.niveau,
            ic.inscription_auto);
    
    fclose(f);
    printf("✓ Inscription ajoutée: %s %s - Type: %s - Niveau: %s\n", 
           ic.prenom, ic.nom, ic.type_cours, ic.niveau);
    
    return 1;
}

// CALLBACK BOUTON S'INSCRIRE (button51)
void on_button51_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget *window = lookup_widget(widget, "inscrit_cour");
    
    if (!window) {
        g_warning("Fenêtre inscrit_cour non trouvée");
        return;
    }
    
    // Récupérer les widgets
    GtkWidget *entry_nom = lookup_widget(window, "entry47");
    GtkWidget *entry_prenom = lookup_widget(window, "entry48");
    GtkWidget *combobox_type = lookup_widget(window, "comboboxentry9");
    GtkWidget *radio_debutant = lookup_widget(window, "radiobutton10");
    GtkWidget *radio_intermediaire = lookup_widget(window, "radiobutton11");
    GtkWidget *radio_expert = lookup_widget(window, "radiobutton12");
    GtkWidget *check_auto = lookup_widget(window, "checkbutton1");
    
    if (!entry_nom || !entry_prenom || !combobox_type) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur: widgets non trouvés!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    // Récupérer les données
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const char *prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
    
    // Récupérer le type de cours
    gchar *type_cours = NULL;
    GtkWidget *entry_combo = gtk_bin_get_child(GTK_BIN(combobox_type));
    if (entry_combo && GTK_IS_ENTRY(entry_combo)) {
        type_cours = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_combo)));
    }
    
    if (!type_cours || strlen(type_cours) == 0) {
        if (type_cours) g_free(type_cours);
        type_cours = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_type));
    }
    
    // Déterminer le niveau
    const char *niveau = "debutant";
    if (radio_intermediaire && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_intermediaire))) {
        niveau = "intermediaire";
    } else if (radio_expert && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_expert))) {
        niveau = "expert";
    }
    
    // Inscription auto
    const char *inscription_auto = "non";
    if (check_auto && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_auto))) {
        inscription_auto = "oui";
    }
    
    // Validation
    if (strlen(nom) == 0 || strlen(prenom) == 0 || !type_cours || strlen(type_cours) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez remplir tous les champs!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        if (type_cours) g_free(type_cours);
        return;
    }
    
    // Créer la structure
    InscriptionCours ic;
    strncpy(ic.nom, nom, sizeof(ic.nom) - 1);
    ic.nom[sizeof(ic.nom) - 1] = '\0';
    strncpy(ic.prenom, prenom, sizeof(ic.prenom) - 1);
    ic.prenom[sizeof(ic.prenom) - 1] = '\0';
    strncpy(ic.type_cours, type_cours, sizeof(ic.type_cours) - 1);
    ic.type_cours[sizeof(ic.type_cours) - 1] = '\0';
    strncpy(ic.niveau, niveau, sizeof(ic.niveau) - 1);
    ic.niveau[sizeof(ic.niveau) - 1] = '\0';
    strncpy(ic.inscription_auto, inscription_auto, sizeof(ic.inscription_auto) - 1);
    ic.inscription_auto[sizeof(ic.inscription_auto) - 1] = '\0';
    
    // Ajouter l'inscription
    if (ajouter_inscription_cours(ic)) {
        gchar *message = g_strdup_printf(
            "✓ Inscription réussie!\n\n"
            "Nom: %s %s\n"
            "Type de cours: %s\n"
            "Niveau: %s\n"
            "Inscription auto: %s",
            ic.prenom, ic.nom, ic.type_cours, ic.niveau, ic.inscription_auto
        );
        
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "%s", message);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_free(message);
        
        // Vider les champs
        gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
        gtk_entry_set_text(GTK_ENTRY(entry_prenom), "");
        if (entry_combo && GTK_IS_ENTRY(entry_combo)) {
            gtk_entry_set_text(GTK_ENTRY(entry_combo), "");
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_type), -1);
        if (radio_debutant) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_debutant), TRUE);
        }
        if (check_auto) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_auto), FALSE);
        }
        
        gtk_widget_hide(window);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur lors de l'inscription!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    
    if (type_cours) g_free(type_cours);
}

// CALLBACK BOUTON ANNULER (button52)
void on_button52_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget *window = lookup_widget(widget, "inscrit_cour");
    
    if (window) {
        GtkWidget *entry_nom = lookup_widget(window, "entry47");
        GtkWidget *entry_prenom = lookup_widget(window, "entry48");
        GtkWidget *combobox_type = lookup_widget(window, "comboboxentry9");
        GtkWidget *radio_debutant = lookup_widget(window, "radiobutton10");
        GtkWidget *check_auto = lookup_widget(window, "checkbutton1");
        
        if (entry_nom) gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
        if (entry_prenom) gtk_entry_set_text(GTK_ENTRY(entry_prenom), "");
        
        if (combobox_type) {
            GtkWidget *entry_combo = gtk_bin_get_child(GTK_BIN(combobox_type));
            if (entry_combo && GTK_IS_ENTRY(entry_combo)) {
                gtk_entry_set_text(GTK_ENTRY(entry_combo), "");
            }
            gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_type), -1);
        }
        
        if (radio_debutant) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_debutant), TRUE);
        }
        if (check_auto) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_auto), FALSE);
        }
        
        gtk_widget_hide(window);
    }
}


