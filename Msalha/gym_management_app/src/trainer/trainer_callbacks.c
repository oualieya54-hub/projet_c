#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>

#include "trainer_callbacks.h"
#include "trainer_interface.h"
#include "../common/support.h"
#include "entraineur.h"

// ============ VARIABLES GLOBALES POUR LES DONNÉES EN ATTENTE ============
static Entraineur g_entraineur_pending;
static Inscription_Cours g_inscription_pending;
static char g_cin_pending[20];
static GtkWidget *g_window_ajout = NULL;
static GtkWidget *g_window_modif = NULL;
GtkWidget *g_window_inscri = NULL;
static GtkWidget *g_window_confirm_ajout = NULL;
static GtkWidget *g_window_confirm_modif = NULL;
static GtkWidget *g_window_confirm_inscri = NULL;
static GtkWidget *g_window_confirm_supp = NULL;
GtkWidget *g_window_main = NULL;
GtkWidget *g_treeview1 = NULL;

// ============ FONCTION POUR REMPLIR LE TREEVIEW ============
void populate_treeview1(void) {
    printf("DEBUG: populate_treeview1 called, g_treeview1 = %p\n", g_treeview1);

    if (!g_treeview1) {
        printf("DEBUG: g_treeview1 is NULL, returning early!\n");
        return;
    }

    // Check if columns already exist for THIS treeview (not using static variable)
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(g_treeview1));
    gboolean columns_exist = (g_list_length(columns) > 0);
    printf("DEBUG: columns_exist = %d, column count = %d\n", columns_exist, g_list_length(columns));
    g_list_free(columns);

    if (!columns_exist)
    {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("CIN", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(g_treeview1), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(g_treeview1), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Prénom", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(g_treeview1), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Spécialité", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(g_treeview1), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Téléphone", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(g_treeview1), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Email", renderer, "text", 5, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(g_treeview1), column);

        gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(g_treeview1), TRUE);
    }

    int nb = 0;
    Entraineur *entraineurs = afficher_tous_entraineurs(&nb);
    printf("DEBUG: Loaded %d trainers\n", nb);

    GtkListStore *store = gtk_list_store_new(6,
                                               G_TYPE_STRING,  // CIN
                                               G_TYPE_STRING,  // Nom
                                               G_TYPE_STRING,  // Prénom
                                               G_TYPE_STRING,  // Spécialité
                                               G_TYPE_STRING,  // Téléphone
                                               G_TYPE_STRING); // Email

    if (entraineurs) {
        for (int i = 0; i < nb; i++) {
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                              0, entraineurs[i].cin,
                              1, entraineurs[i].nom,
                              2, entraineurs[i].prenom,
                              3, entraineurs[i].specialite,
                              4, entraineurs[i].telephone,
                              5, entraineurs[i].email,
                              -1);
        }
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(g_treeview1), GTK_TREE_MODEL(store));
    g_object_unref(store);

    if (entraineurs) free(entraineurs);
}

// ============ AJOUTER UN ENTRAINEUR ============

void on_btn_ajouter_confirmer_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window;

    // Récupérer la fenêtre
    window = lookup_widget(GTK_WIDGET(button), "Ajouter_un_entraineur");
    g_window_ajout = window;

    // Récupérer les données des entries
    GtkWidget *entry_cin = lookup_widget(GTK_WIDGET(button), "entryA6");
    GtkWidget *entry_nom = lookup_widget(GTK_WIDGET(button), "entryA1");
    GtkWidget *entry_prenom = lookup_widget(GTK_WIDGET(button), "entryA2");
    GtkWidget *entry_telephone = lookup_widget(GTK_WIDGET(button), "entryA3");
    GtkWidget *entry_email = lookup_widget(GTK_WIDGET(button), "entryA4");
    GtkWidget *entry_adresse = lookup_widget(GTK_WIDGET(button), "entryA5");

    const gchar *cin = gtk_entry_get_text(GTK_ENTRY(entry_cin));
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const gchar *prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
    const gchar *telephone = gtk_entry_get_text(GTK_ENTRY(entry_telephone));
    const gchar *email = gtk_entry_get_text(GTK_ENTRY(entry_email));
    const gchar *adresse = gtk_entry_get_text(GTK_ENTRY(entry_adresse));

    // Validation des champs obligatoires
    if (strlen(cin) == 0 || strlen(nom) == 0 || strlen(prenom) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Veuillez remplir les champs obligatoires (CIN, Nom, Prénom)");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Copier dans la structure globale
    strncpy(g_entraineur_pending.cin, cin, sizeof(g_entraineur_pending.cin) - 1);
    strncpy(g_entraineur_pending.nom, nom, sizeof(g_entraineur_pending.nom) - 1);
    strncpy(g_entraineur_pending.prenom, prenom, sizeof(g_entraineur_pending.prenom) - 1);
    strncpy(g_entraineur_pending.telephone, telephone, sizeof(g_entraineur_pending.telephone) - 1);
    strncpy(g_entraineur_pending.email, email, sizeof(g_entraineur_pending.email) - 1);
    strncpy(g_entraineur_pending.adresse, adresse, sizeof(g_entraineur_pending.adresse) - 1);

    // Sexe (RadioButton)
    GtkWidget *radio_homme = lookup_widget(GTK_WIDGET(button), "radiobutton1A");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_homme)))
        strcpy(g_entraineur_pending.sexe, "Homme");
    else
        strcpy(g_entraineur_pending.sexe, "Femme");

    // Date de naissance (Calendar)
    GtkWidget *calendar = lookup_widget(GTK_WIDGET(button), "calendar1A");
    guint jour, mois, annee;
    gtk_calendar_get_date(GTK_CALENDAR(calendar), &annee, &mois, &jour);
    g_entraineur_pending.date_naissance.jour = jour;
    g_entraineur_pending.date_naissance.mois = mois + 1;
    g_entraineur_pending.date_naissance.annee = annee;

    // Date de fin de contrat (SpinButtons)
    GtkWidget *spin_jour = lookup_widget(GTK_WIDGET(button), "spinbutton1A");
    GtkWidget *spin_mois = lookup_widget(GTK_WIDGET(button), "spinbutton2A");
    GtkWidget *spin_annee = lookup_widget(GTK_WIDGET(button), "spinbutton3A");

    g_entraineur_pending.date_fin_contrat.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_jour));
    g_entraineur_pending.date_fin_contrat.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_mois));
    g_entraineur_pending.date_fin_contrat.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_annee));

    // Spécialité (ComboBox)
    GtkWidget *combo_specialite = lookup_widget(GTK_WIDGET(button), "comboboxentryA");
    gchar *specialite = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_specialite));
    if (specialite != NULL) {
        strncpy(g_entraineur_pending.specialite, specialite, sizeof(g_entraineur_pending.specialite) - 1);
        g_free(specialite);
    } else {
        strcpy(g_entraineur_pending.specialite, "");
    }

    // Disponibilité (ComboBox)
    GtkWidget *combo_dispo = lookup_widget(GTK_WIDGET(button), "comboboxentry2A");
    gchar *dispo = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_dispo));
    if (dispo != NULL) {
        strncpy(g_entraineur_pending.disponibilite, dispo, sizeof(g_entraineur_pending.disponibilite) - 1);
        g_free(dispo);
    } else {
        strcpy(g_entraineur_pending.disponibilite, "");
    }

    strcpy(g_entraineur_pending.photo, "(Aucun)");

    // Afficher le dialogue de confirmation
    g_window_confirm_ajout = create_ajout_();
    gtk_widget_show(g_window_confirm_ajout);
}

void on_btn_ajouter_annuler_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "Ajouter_un_entraineur");
    gtk_widget_hide(window);
}


// ============ MODIFIER UN ENTRAINEUR ============

void on_btn_modifier_confirmer_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "Modifier_un_entraineur");
    g_window_modif = window;

    GtkWidget *entry_cin = lookup_widget(GTK_WIDGET(button), "entry12");
    const gchar *cin_original = gtk_entry_get_text(GTK_ENTRY(entry_cin));

    if (strlen(cin_original) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Veuillez saisir un CIN");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    int trouve;
    Entraineur *e_ancien = rechercher_entraineur((char*)cin_original, &trouve);
    if (!trouve) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Entraîneur introuvable avec ce CIN");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    free(e_ancien);

    // Stocker le CIN pour la modification
    strncpy(g_cin_pending, cin_original, sizeof(g_cin_pending) - 1);

    strncpy(g_entraineur_pending.cin, cin_original, sizeof(g_entraineur_pending.cin) - 1);
    strncpy(g_entraineur_pending.nom, gtk_entry_get_text(GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entry8"))), sizeof(g_entraineur_pending.nom) - 1);
    strncpy(g_entraineur_pending.prenom, gtk_entry_get_text(GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entry9"))), sizeof(g_entraineur_pending.prenom) - 1);
    strncpy(g_entraineur_pending.telephone, gtk_entry_get_text(GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entry13"))), sizeof(g_entraineur_pending.telephone) - 1);
    strncpy(g_entraineur_pending.email, gtk_entry_get_text(GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entry10"))), sizeof(g_entraineur_pending.email) - 1);
    strncpy(g_entraineur_pending.adresse, gtk_entry_get_text(GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entry11"))), sizeof(g_entraineur_pending.adresse) - 1);

    GtkWidget *radio_homme = lookup_widget(GTK_WIDGET(button), "radiobutton3");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_homme)))
        strcpy(g_entraineur_pending.sexe, "Homme");
    else
        strcpy(g_entraineur_pending.sexe, "Femme");

    GtkWidget *calendar = lookup_widget(GTK_WIDGET(button), "calendar2");
    guint jour, mois, annee;
    gtk_calendar_get_date(GTK_CALENDAR(calendar), &annee, &mois, &jour);
    g_entraineur_pending.date_naissance.jour = jour;
    g_entraineur_pending.date_naissance.mois = mois + 1;
    g_entraineur_pending.date_naissance.annee = annee;

    g_entraineur_pending.date_fin_contrat.jour = gtk_spin_button_get_value_as_int(
        GTK_SPIN_BUTTON(lookup_widget(GTK_WIDGET(button), "spinbutton4")));
    g_entraineur_pending.date_fin_contrat.mois = gtk_spin_button_get_value_as_int(
        GTK_SPIN_BUTTON(lookup_widget(GTK_WIDGET(button), "spinbutton5")));
    g_entraineur_pending.date_fin_contrat.annee = gtk_spin_button_get_value_as_int(
        GTK_SPIN_BUTTON(lookup_widget(GTK_WIDGET(button), "spinbutton6")));

    GtkWidget *combo_specialite = lookup_widget(GTK_WIDGET(button), "comboboxentry6");
    gchar *specialite = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_specialite));
    if (specialite != NULL) {
        strncpy(g_entraineur_pending.specialite, specialite, sizeof(g_entraineur_pending.specialite) - 1);
        g_free(specialite);
    } else {
        strcpy(g_entraineur_pending.specialite, "");
    }

    GtkWidget *combo_dispo = lookup_widget(GTK_WIDGET(button), "comboboxentry5");
    gchar *dispo = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_dispo));
    if (dispo != NULL) {
        strncpy(g_entraineur_pending.disponibilite, dispo, sizeof(g_entraineur_pending.disponibilite) - 1);
        g_free(dispo);
    } else {
        strcpy(g_entraineur_pending.disponibilite, "");
    }

    strcpy(g_entraineur_pending.photo, "(Aucun)");

    // Afficher le dialogue de confirmation
    g_window_confirm_modif = create_modif_();
    gtk_widget_show(g_window_confirm_modif);
}

void on_btn_modifier_annuler_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "Modifier_un_entraineur");
    gtk_widget_hide(window);
}



// ============ SUPPRIMER UN ENTRAINEUR ============

void on_btn_supprimer_confirmer_clicked(GtkButton *button, gpointer user_data)
{
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *cin;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(g_treeview1));

    if (gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gtk_tree_model_get(model, &iter, 0, &cin, -1); // 0 is the CIN column

        // Stocker le CIN pour la suppression
        strncpy(g_cin_pending, cin, sizeof(g_cin_pending) - 1);
        g_free(cin);

        // Essayer de trouver la fenêtre principale et treeview
        GtkWidget *main_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
        if (GTK_IS_WINDOW(main_window)) {
            g_window_main = main_window;
            g_treeview1 = lookup_widget(main_window, "treeview1");
        }

        // Afficher le dialogue de confirmation
        g_window_confirm_supp = create_supp_();
        gtk_widget_show(g_window_confirm_supp);
    }
    else
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Veuillez sélectionner un entraîneur à supprimer.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// ============ INSCRIPTION À UN COURS ============

void populate_treeview2i(gchar *day, gchar *period)
{
    GtkWidget *treeview;
    GtkListStore *store;
    GtkTreeIter iter;

    treeview = lookup_widget(g_window_inscri, "treeview2i");
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));

    if (store == NULL)
    {
        store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    }

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 0, day, 1, period, -1);
}

void on_btn_inscrire_confirmer_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "Inscription____un_cours_");
    g_window_inscri = window;

    GtkWidget *entry_cin = lookup_widget(GTK_WIDGET(button), "entry16");
    const gchar *cin = gtk_entry_get_text(GTK_ENTRY(entry_cin));

    if (strlen(cin) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Veuillez saisir un CIN");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    if (!verifier_cin_existe((char*)cin)) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Ce CIN n'existe pas dans la base de données");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    strncpy(g_inscription_pending.cin_entraineur, cin, sizeof(g_inscription_pending.cin_entraineur) - 1);
    strncpy(g_inscription_pending.nom_entraineur, gtk_entry_get_text(GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entry14"))), sizeof(g_inscription_pending.nom_entraineur) - 1);
    strncpy(g_inscription_pending.prenom_entraineur, gtk_entry_get_text(GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entry15"))), sizeof(g_inscription_pending.prenom_entraineur) - 1);

    GtkWidget *combo_specialite = lookup_widget(GTK_WIDGET(button), "comboboxentryi1");
    gchar *specialite = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_specialite));
    if (specialite != NULL) {
        strncpy(g_inscription_pending.specialite, specialite, sizeof(g_inscription_pending.specialite) - 1);
        g_free(specialite);
    } else {
        strcpy(g_inscription_pending.specialite, "");
    }

    GtkWidget *combo_cours = lookup_widget(GTK_WIDGET(button), "comboboxentryi2");
    gchar *cours = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_cours));
    if (cours != NULL) {
        strncpy(g_inscription_pending.nom_cours, cours, sizeof(g_inscription_pending.nom_cours) - 1);
        g_free(cours);
    } else {
        strcpy(g_inscription_pending.nom_cours, "");
    }

    GtkWidget *radio_mixte = lookup_widget(GTK_WIDGET(button), "radiobutton1i");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_mixte)))
        strcpy(g_inscription_pending.type_cours, "Mixte");
    else
        strcpy(g_inscription_pending.type_cours, "Non Mixte");

    GtkWidget *radio_enfants = lookup_widget(GTK_WIDGET(button), "radiobuttoni3");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_enfants)))
        strcpy(g_inscription_pending.public, "Enfants");
    else
        strcpy(g_inscription_pending.public, "Adulte");

    g_inscription_pending.date_cours.jour = 1;
    g_inscription_pending.date_cours.mois = 1;
    g_inscription_pending.date_cours.annee = 2025;



    // Afficher le dialogue de confirmation
    g_window_confirm_inscri = create_inscri_();
    gtk_widget_show(g_window_confirm_inscri);
}

void on_btn_inscrire_annuler_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "Inscription____un_cours_");
    gtk_widget_hide(window);
}



// ============ RECHERCHER UN ENTRAINEUR ============

void on_btn_rechercher_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *entry_recherche = lookup_widget(GTK_WIDGET(button), "entryG1");
    const gchar *cin = gtk_entry_get_text(GTK_ENTRY(entry_recherche));
    
    if (strlen(cin) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Veuillez saisir un CIN à rechercher");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    int trouve;
    Entraineur *e = rechercher_entraineur((char*)cin, &trouve);
    
    if (trouve) {
        gchar *message = g_strdup_printf(
            "Entraîneur trouvé :\n\n"
            "CIN : %s\nNom : %s\nPrénom : %s\nSexe : %s\n"
            "Date de naissance : %02d/%02d/%d\n"
            "Téléphone : %s\nEmail : %s\nAdresse : %s\n"
            "Spécialité : %s\nDisponibilité : %s\n"
            "Date fin contrat : %02d/%02d/%d",
            e->cin, e->nom, e->prenom, e->sexe,
            e->date_naissance.jour, e->date_naissance.mois, e->date_naissance.annee,
            e->telephone, e->email, e->adresse,
            e->specialite, e->disponibilite,
            e->date_fin_contrat.jour, e->date_fin_contrat.mois, e->date_fin_contrat.annee
        );
        
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "%s", message);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        g_free(message);
        free(e);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Aucun entraîneur trouvé avec ce CIN");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// ============ MAPPING DES NOMS GLADE VERS NOS FONCTIONS ============

void on_buttonA1_clicked(GtkButton *button, gpointer user_data) {
    on_btn_ajouter_annuler_clicked(button, user_data);
}

void on_buttonA2_clicked(GtkButton *button, gpointer user_data) {
    on_btn_ajouter_confirmer_clicked(button, user_data);
}

void on_buttonM1_clicked(GtkButton *button, gpointer user_data) {
    on_btn_modifier_annuler_clicked(button, user_data);
}

void on_buttonM2_clicked(GtkButton *button, gpointer user_data) {
    on_btn_modifier_confirmer_clicked(button, user_data);
}

void on_buttoni1_clicked(GtkButton *button, gpointer user_data) {
    on_btn_inscrire_annuler_clicked(button, user_data);
}

void on_buttoni2_clicked(GtkButton *button, gpointer user_data) {
    on_btn_inscrire_confirmer_clicked(button, user_data);
}

void on_buttonG1_clicked(GtkButton *button, gpointer user_data) {
    on_btn_rechercher_clicked(button, user_data);
}

void on_buttonG2_clicked(GtkButton *button, gpointer user_data) {
    on_btn_supprimer_confirmer_clicked(button, user_data);
}

void on_buttonG3_clicked(GtkButton *button, gpointer user_data) {
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *cin;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(g_treeview1));

    if (gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gtk_tree_model_get(model, &iter, 0, &cin, -1); // 0 is the CIN column

        int trouve;
        Entraineur *e = rechercher_entraineur(cin, &trouve);
        g_free(cin);

        if (trouve && e)
        {
            GtkWidget *window = create_Modifier_un_entraineur();
            g_window_modif = window;

            // Populate the fields
            GtkWidget *entry_cin = lookup_widget(window, "entry12");
            GtkWidget *entry_nom = lookup_widget(window, "entry8");
            GtkWidget *entry_prenom = lookup_widget(window, "entry9");
            GtkWidget *entry_telephone = lookup_widget(window, "entry13");
            GtkWidget *entry_email = lookup_widget(window, "entry10");
            GtkWidget *entry_adresse = lookup_widget(window, "entry11");
            GtkWidget *radio_homme = lookup_widget(window, "radiobutton3");
            GtkWidget *radio_femme = lookup_widget(window, "radiobutton4");
            GtkWidget *calendar = lookup_widget(window, "calendar2");
            GtkWidget *spin_jour = lookup_widget(window, "spinbutton4");
            GtkWidget *spin_mois = lookup_widget(window, "spinbutton5");
            GtkWidget *spin_annee = lookup_widget(window, "spinbutton6");
            GtkWidget *combo_specialite = lookup_widget(window, "comboboxentry6");
            GtkWidget *combo_dispo = lookup_widget(window, "comboboxentry5");

            gtk_entry_set_text(GTK_ENTRY(entry_cin), e->cin);
            gtk_editable_set_editable(GTK_EDITABLE(entry_cin), FALSE); // Make CIN read-only
            
            gtk_entry_set_text(GTK_ENTRY(entry_nom), e->nom);
            gtk_entry_set_text(GTK_ENTRY(entry_prenom), e->prenom);
            gtk_entry_set_text(GTK_ENTRY(entry_telephone), e->telephone);
            gtk_entry_set_text(GTK_ENTRY(entry_email), e->email);
            gtk_entry_set_text(GTK_ENTRY(entry_adresse), e->adresse);

            if (strcmp(e->sexe, "Homme") == 0) {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_homme), TRUE);
            } else {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_femme), TRUE);
            }

            gtk_calendar_select_month(GTK_CALENDAR(calendar), e->date_naissance.mois - 1, e->date_naissance.annee);
            gtk_calendar_select_day(GTK_CALENDAR(calendar), e->date_naissance.jour);

            gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_jour), e->date_fin_contrat.jour);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_mois), e->date_fin_contrat.mois);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_annee), e->date_fin_contrat.annee);
            
            gtk_entry_set_text(GTK_ENTRY(gtk_bin_get_child(GTK_BIN(combo_specialite))), e->specialite);
            gtk_entry_set_text(GTK_ENTRY(gtk_bin_get_child(GTK_BIN(combo_dispo))), e->disponibilite);

            gtk_widget_show(window);
            free(e);
        }
    }
    else
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Veuillez sélectionner un entraîneur à modifier.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_buttonG4_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = create_Ajouter_un_entraineur();
    g_window_ajout = window;

    // Stocker la fenêtre principale et treeview
    GtkWidget *main_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    if (GTK_IS_WINDOW(main_window)) {
        g_window_main = main_window;
        g_treeview1 = lookup_widget(main_window, "treeview1");
    }

    gtk_widget_show(window);
}

void on_buttonG5_clicked(GtkButton *button, gpointer user_data) {
    // Bouton Précédent - fermer la fenêtre principale ou revenir
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "Gestion des entraineur");
    if (window) {
        gtk_widget_hide(window);
    }
}

// ============ CALLBACKS POUR LES DIALOGUES DE CONFIRMATION ============

// --- Confirmation Ajouter ---
void on_buttona1_clicked(GtkButton *button, gpointer user_data) {
    // NON - Annuler l'ajout
    if (g_window_confirm_ajout) {
        gtk_widget_destroy(g_window_confirm_ajout);
        g_window_confirm_ajout = NULL;
    }
}

void on_buttona2_clicked(GtkButton *button, gpointer user_data) {
    // OUI - Confirmer l'ajout
    if (ajouter_entraineur(g_entraineur_pending)) {
        // Remplir le treeview avec les données mises à jour
        populate_treeview1();

        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Entraîneur ajouté avec succès !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        /* Populating treeview2i in Inscription window */
        if (g_window_inscri) {
            GtkWidget *combobox_day = lookup_widget(g_window_ajout, "comboboxentry1A");
            GtkWidget *combobox_period = lookup_widget(g_window_ajout, "comboboxentry2A");

            if (combobox_day && combobox_period) {
                gchar *day = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_day));
                gchar *period = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_period));

                if (day && period) {
                    populate_treeview2i(day, period);
                    g_free(day);
                    g_free(period);
                }
            }
        }

        // Fermer les fenêtres
        if (g_window_confirm_ajout) {
            gtk_widget_destroy(g_window_confirm_ajout);
            g_window_confirm_ajout = NULL;
        }
        if (g_window_ajout) {
            gtk_widget_hide(g_window_ajout);
        }
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Erreur lors de l'ajout de l'entraîneur");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// --- Confirmation Modifier ---
void on_buttonm1_clicked(GtkButton *button, gpointer user_data) {
    // NON - Annuler la modification
    if (g_window_confirm_modif) {
        gtk_widget_destroy(g_window_confirm_modif);
        g_window_confirm_modif = NULL;
    }
}

void on_buttonm2_clicked(GtkButton *button, gpointer user_data) {
    // OUI - Confirmer la modification
    if (modifier_entraineur(g_cin_pending, g_entraineur_pending)) {
        // Remplir le treeview avec les données mises à jour
        populate_treeview1();

        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Entraîneur modifié avec succès !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // Fermer les fenêtres
        if (g_window_confirm_modif) {
            gtk_widget_destroy(g_window_confirm_modif);
            g_window_confirm_modif = NULL;
        }
        if (g_window_modif) {
            gtk_widget_hide(g_window_modif);
        }
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Erreur lors de la modification de l'entraîneur");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// --- Confirmation Inscription ---
void on_buttonI1_clicked(GtkButton *button, gpointer user_data) {
    // NON - Annuler l'inscription
    if (g_window_confirm_inscri) {
        gtk_widget_destroy(g_window_confirm_inscri);
        g_window_confirm_inscri = NULL;
    }
}

void on_buttonI2_clicked(GtkButton *button, gpointer user_data) {
    // OUI - Confirmer l'inscription
    if (inscrire_cours(g_inscription_pending)) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Inscription réussie !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // Fermer les fenêtres
        if (g_window_confirm_inscri) {
            gtk_widget_destroy(g_window_confirm_inscri);
            g_window_confirm_inscri = NULL;
        }
        if (g_window_inscri) {
            gtk_widget_hide(g_window_inscri);
        }
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Erreur lors de l'inscription");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// --- Confirmation Supprimer ---
void on_buttons1_clicked(GtkButton *button, gpointer user_data) {
    // NON - Annuler la suppression
    if (g_window_confirm_supp) {
        gtk_widget_destroy(g_window_confirm_supp);
        g_window_confirm_supp = NULL;
    }
}

void on_buttons2_clicked(GtkButton *button, gpointer user_data) {
    // OUI - Confirmer la suppression
    if (supprimer_entraineur(g_cin_pending)) {
        // Remplir le treeview avec les données mises à jour
        populate_treeview1();

        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Entraîneur supprimé avec succès !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Entraîneur introuvable");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

    // Fermer le dialogue de confirmation
    if (g_window_confirm_supp) {
        gtk_widget_destroy(g_window_confirm_supp);
        g_window_confirm_supp = NULL;
    }
}
