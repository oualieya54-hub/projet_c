#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#include "member_callbacks.h"
#include "member_interface.h"
#include "../common/support.h"
#include "member.h"

/* Static variables to hold data between windows */
static Membre temp_membre;
static char cin_to_delete[20];
static char cin_to_modify[20];

/* Global pointer to the main treeview to allow refreshing from anywhere */
GtkWidget *main_treeview = NULL;

/* Helper to center windows */
void show_centered(GtkWidget *window) {
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_show(window);
}

/* Custom Callbacks for Confirmation Windows (Dynamic Connection) */
void show_error_message(const char *msg, GtkWidget *parent) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK,
                                               "%s", msg);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/* Add Confirmation */
void on_conf_add_yes(GtkButton *button, gpointer user_data) {
    GtkWidget *window6 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *window2 = GTK_WIDGET(user_data);

    ajouter_membre(temp_membre);
    
    /* Refresh TreeView */
    if(main_treeview) afficher_membre(GTK_TREE_VIEW(main_treeview));
    
    gtk_widget_destroy(window6);
    if (window2) gtk_widget_destroy(window2);
}

void on_conf_add_no(GtkButton *button, gpointer user_data) {
    GtkWidget *window6 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *window2 = GTK_WIDGET(user_data);
    
    gtk_widget_destroy(window6);
    if (window2) gtk_widget_destroy(window2);
}

/* Delete Confirmation */
void on_conf_del_yes(GtkButton *button, gpointer user_data) {
    GtkWidget *window4 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    supprimer_membre(cin_to_delete);
    
    /* Refresh TreeView */
    if(main_treeview) afficher_membre(GTK_TREE_VIEW(main_treeview));
    
    gtk_widget_destroy(window4);
}

void on_conf_del_no(GtkButton *button, gpointer user_data) {
    GtkWidget *window4 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(window4);
}

/* Modify Confirmation */
void on_conf_mod_yes(GtkButton *button, gpointer user_data) {
    GtkWidget *window5 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *window3 = GTK_WIDGET(user_data);
    
    modifier_membre(cin_to_modify, temp_membre);
    
    /* Refresh TreeView */
    if(main_treeview) afficher_membre(GTK_TREE_VIEW(main_treeview));
    
    gtk_widget_destroy(window5);
    if (window3) gtk_widget_destroy(window3);
}

void on_conf_mod_no(GtkButton *button, gpointer user_data) {
    GtkWidget *window5 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(window5);
}

/* --------------------------------------------------------- */

void
member_on_treeview_row_activated       (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
    /* Update global pointer just in case */
    main_treeview = GTK_WIDGET(treeview);
    
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        char *cin;
        gtk_tree_model_get(model, &iter, 2, &cin, -1); // Assuming col 2 is CIN
        // Optional: Pre-fill search/delete inputs on main window?
        // Let's assume user wants to copy CIN to search entry (gm5)
        GtkWidget *win1 = gtk_widget_get_toplevel(GTK_WIDGET(treeview));
        GtkWidget *entry_search = lookup_widget(win1, "gm5");
        if(cin && entry_search) gtk_entry_set_text(GTK_ENTRY(entry_search), cin);
        
        if(cin) g_free(cin);
    }
}


/* MAIN WINDOW BUTTONS */

/* Button Delete (gm12) */
void
member_on_gm12_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window4 = create_window4();
    
    /* Connect Yes/No signals dynamically */
    GtkWidget *btn_oui = lookup_widget(window4, "vasup2");
    GtkWidget *btn_non = lookup_widget(window4, "vasup3");
    
    GtkWidget *win1 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *entry_search = lookup_widget(win1, "gm5");
    const char *cin = gtk_entry_get_text(GTK_ENTRY(entry_search));
    strcpy(cin_to_delete, cin);
    
    g_signal_connect(btn_oui, "clicked", G_CALLBACK(on_conf_del_yes), NULL);
    g_signal_connect(btn_non, "clicked", G_CALLBACK(on_conf_del_no), NULL);
    
    show_centered(window4);
}


/* Button Search (gm8) - "Recherccher" */
void
member_on_gm8_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *win1 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *entry = lookup_widget(win1, "gm5");
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    
    /* Capture treeview pointer for later use if it wasn't captured */
    if (!main_treeview) {
        main_treeview = lookup_widget(win1, "gm9");
    }
    
    Membre *m = rechercher_membre((char*)text);
    if (m) {
        GtkWidget *win8 = create_window8();
        
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(win8, "entry1")), m->nom);
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(win8, "entry2")), m->prenom);
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(win8, "entry3")), m->cin);
        
        char date[20]; sprintf(date, "%d/%d/%d", m->jour, m->mois, m->annee);
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(win8, "entry4")), date);
        
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(win8, "entry5")), m->email);
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(win8, "entry6")), m->num);
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(win8, "entry7")), m->ville);
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(win8, "entry8")), m->abonnement);
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(win8, "entry9")), m->sport);
        
        show_centered(win8);
    } else {
        GtkWidget *win9 = create_window9();
        show_centered(win9);
    }
}


/* Button Modify (gm10) - Opens Window 3 */
void
member_on_gm10_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *win1 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *entry_cin = lookup_widget(win1, "gm5");
    const char *cin = gtk_entry_get_text(GTK_ENTRY(entry_cin));
    
    GtkWidget *window3 = create_window3();
    
    if (strlen(cin) > 0) {
        Membre *m = rechercher_membre((char*)cin);
        if (m) {
            strcpy(cin_to_modify, m->cin);
            
            gtk_entry_set_text(GTK_ENTRY(lookup_widget(window3, "modm6")), m->nom);
            gtk_entry_set_text(GTK_ENTRY(lookup_widget(window3, "modm7")), m->prenom);
            
            GtkWidget *cin_entry = lookup_widget(window3, "modm8");
            gtk_entry_set_text(GTK_ENTRY(cin_entry), m->cin);
            gtk_editable_set_editable(GTK_EDITABLE(cin_entry), FALSE); /* Make CIN unmodifiable */
            gtk_widget_set_sensitive(cin_entry, FALSE); /* Optional: visual indication */
            
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(window3, "modm9")), m->annee);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(window3, "modm10")), m->mois);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(window3, "modm11")), m->jour);
            
            gtk_entry_set_text(GTK_ENTRY(lookup_widget(window3, "modm17")), m->email);
            gtk_entry_set_text(GTK_ENTRY(lookup_widget(window3, "modm15")), m->ville);
            gtk_entry_set_text(GTK_ENTRY(lookup_widget(window3, "modm13")), m->num);
            
            if (strcmp(m->sexe, "Homme") == 0)
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(window3, "modm20")), TRUE);
            else
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(window3, "modm21")), TRUE);
        }
    }
    
    show_centered(window3);
}


/* Button Add (gm11) - Opens Window 2 */
void
member_on_gm11_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window2 = create_window2();
    show_centered(window2);
}


/* ---------------------------------------------------------------- */

/* WINDOW 2 (ADD) LOGIC */
void
member_on_ajm29_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *win2 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    strcpy(temp_membre.nom, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win2, "ajm6"))));
    strcpy(temp_membre.prenom, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win2, "ajm7"))));
    
    const char *cin_input = gtk_entry_get_text(GTK_ENTRY(lookup_widget(win2, "ajm8")));
    
    /* CIN Validation: Must be 8 digits */
    if (strlen(cin_input) != 8) {
        show_error_message("Le CIN doit contenir exactement 8 chiffres.", win2);
        return;
    }
    for (int i = 0; i < 8; i++) {
        if (cin_input[i] < '0' || cin_input[i] > '9') {
            show_error_message("Le CIN doit contenir uniquement des chiffres.", win2);
            return;
        }
    }
    
    strcpy(temp_membre.cin, cin_input);
    
    temp_membre.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(lookup_widget(win2, "ajm9")));
    temp_membre.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(lookup_widget(win2, "ajm10")));
    temp_membre.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(lookup_widget(win2, "ajm11")));
    
    strcpy(temp_membre.email, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win2, "ajm17"))));
    strcpy(temp_membre.ville, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win2, "ajm15"))));
    strcpy(temp_membre.num, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win2, "ajm13"))));
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win2, "ajm20"))))
        strcpy(temp_membre.sexe, "Homme");
    else
        strcpy(temp_membre.sexe, "Femme");
       
    gchar *abo = gtk_combo_box_get_active_text(GTK_COMBO_BOX(lookup_widget(win2, "ajm19")));
    if (abo) strcpy(temp_membre.abonnement, abo);
    
    temp_membre.sport[0] = '\0';
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win2, "ajm26")))) strcat(temp_membre.sport, "Musculation ");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win2, "ajm25")))) strcat(temp_membre.sport, "Dance ");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win2, "ajm24")))) strcat(temp_membre.sport, "Aerobic ");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win2, "ajm23")))) strcat(temp_membre.sport, "Box ");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win2, "ajm27")))) strcat(temp_membre.sport, "Gym ");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win2, "ajm28")))) strcat(temp_membre.sport, "Natation ");
    
    GtkWidget *win6 = create_window6();
    
    GtkWidget *btn_oui = lookup_widget(win6, "valam2");
    GtkWidget *btn_non = lookup_widget(win6, "valajm3");
    
    g_signal_connect(btn_oui, "clicked", G_CALLBACK(on_conf_add_yes), win2);
    g_signal_connect(btn_non, "clicked", G_CALLBACK(on_conf_add_no), win2);
    
    show_centered(win6);
}


/* WINDOW 3 (MODIFY) LOGIC */
void
member_on_modm29_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *win3 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    strcpy(temp_membre.nom, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win3, "modm6"))));
    strcpy(temp_membre.prenom, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win3, "modm7"))));
    strcpy(temp_membre.cin, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win3, "modm8"))));
    
    temp_membre.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(lookup_widget(win3, "modm9")));
    temp_membre.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(lookup_widget(win3, "modm10")));
    temp_membre.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(lookup_widget(win3, "modm11")));
    
    strcpy(temp_membre.email, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win3, "modm17"))));
    strcpy(temp_membre.ville, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win3, "modm15"))));
    strcpy(temp_membre.num, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win3, "modm13"))));
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win3, "modm20"))))
        strcpy(temp_membre.sexe, "Homme");
    else
        strcpy(temp_membre.sexe, "Femme");
        
    gchar *abo = gtk_combo_box_get_active_text(GTK_COMBO_BOX(lookup_widget(win3, "modm19")));
    if (abo) strcpy(temp_membre.abonnement, abo);
    
    temp_membre.sport[0] = '\0';
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win3, "modm26")))) strcat(temp_membre.sport, "Musculation ");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win3, "modm25")))) strcat(temp_membre.sport, "Dance ");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win3, "modm28")))) strcat(temp_membre.sport, "Natation ");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win3, "modm26")))) strcat(temp_membre.sport, "Musculation ");
    
    GtkWidget *win5 = create_window5();
    
    GtkWidget *btn_oui = lookup_widget(win5, "vamodm2");
    GtkWidget *btn_non = lookup_widget(win5, "vamodm3");
    
    g_signal_connect(btn_oui, "clicked", G_CALLBACK(on_conf_mod_yes), win3);
    g_signal_connect(btn_non, "clicked", G_CALLBACK(on_conf_mod_no), win3);
    
    show_centered(win5);
}


/* Validating View (Window 8) */
void
member_on_valideraffichage_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *win8 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(win8);
}


/* Error Window OK (Window 9) */
void
member_on_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *win9 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(win9);
}

/* Private Coach Signal (called from main.c manually connected signal) */
void
member_on_button3_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *win7 = create_window7();
    
    /* Connect Validate button in Win7 (cp12) */
    GtkWidget *btn_valider = lookup_widget(win7, "cp12");
    g_signal_connect(btn_valider, "clicked", G_CALLBACK(member_on_cp12_clicked), NULL);
    
    show_centered(win7);
}

void member_on_cp12_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *win7 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    CoachDemande cm;
    
    strcpy(cm.nom, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win7, "cp5"))));
    strcpy(cm.prenom, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win7, "cp6"))));
    strcpy(cm.cin, gtk_entry_get_text(GTK_ENTRY(lookup_widget(win7, "cp7"))));
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(win7, "cp8"))))
        strcpy(cm.sexe, "Homme");
    else
        strcpy(cm.sexe, "Femme");
        
    gchar *coach = gtk_combo_box_get_active_text(GTK_COMBO_BOX(lookup_widget(win7, "cp11")));
    if (coach) strcpy(cm.nom_coach, coach);
    
    demander_coach(cm);
    
    gtk_widget_destroy(win7);
}
