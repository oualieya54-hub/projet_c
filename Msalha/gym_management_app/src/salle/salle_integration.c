#include "salle_integration.h"
#include "salle_interface.h"
#include "salle_callbacks.h"
#include "salle.h"
#include "../common/support.h"

void salle_open_add_window(void)
{
    GtkWidget *window_ajout = create_AjoutSalle();
    gtk_widget_show(window_ajout);
}

void salle_open_modify_window(GtkTreeModel *model, GtkTreeIter *iter)
{
    int id_salle;
    gtk_tree_model_get(model, iter, 0, &id_salle, -1);

    SalleSport s = chercher_salle_par_id("data/salle.txt", id_salle);
    if (s.id_salle == -1) {
        return;
    }

    GtkWidget *window_modif = create_MOdiffSalle();
    g_object_set_data(G_OBJECT(window_modif), "id_salle", GINT_TO_POINTER(id_salle));

    GtkWidget *entry56 = lookup_widget(window_modif, "entry56");
    GtkWidget *entry57 = lookup_widget(window_modif, "entry57");
    GtkWidget *entry58 = lookup_widget(window_modif, "entry58");
    GtkWidget *entry59 = lookup_widget(window_modif, "entry59");
    GtkWidget *entry60 = lookup_widget(window_modif, "entry60");
    GtkWidget *entry61 = lookup_widget(window_modif, "entry61");
    GtkWidget *spinbutton21 = lookup_widget(window_modif, "spinbutton21");
    GtkWidget *spinbutton22 = lookup_widget(window_modif, "spinbutton22");

    gtk_entry_set_text(GTK_ENTRY(entry56), s.nom);
    gtk_entry_set_text(GTK_ENTRY(entry57), s.adresse);
    gtk_entry_set_text(GTK_ENTRY(entry58), s.telephone);
    gtk_entry_set_text(GTK_ENTRY(entry59), s.email);
    gtk_entry_set_text(GTK_ENTRY(entry60), s.heure_debut);
    gtk_entry_set_text(GTK_ENTRY(entry61), s.heure_fin);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton21), s.capacite);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton22), s.tarif);

    GtkWidget *optionmenu2 = lookup_widget(window_modif, "optionmenu2");
    if(optionmenu2)
    {
        if(strcmp(s.mode_paiement, "Espece") == 0)
            gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu2), 0);
        else if(strcmp(s.mode_paiement, "Cheque") == 0)
            gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu2), 1);
        else if(strcmp(s.mode_paiement, "Carte Bancaire") == 0)
            gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu2), 2);
        else if(strcmp(s.mode_paiement, "Virement") == 0)
            gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu2), 3);
    }

    GtkWidget *radiobutton11 = lookup_widget(window_modif, "radiobutton11");
    GtkWidget *radiobutton10 = lookup_widget(window_modif, "radiobutton10");
    if (s.espace_restauration) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton11), TRUE);
    else gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton10), TRUE);

    GtkWidget *cb_mensuel = lookup_widget(window_modif, "checkbutton5");
    GtkWidget *cb_trimestriel = lookup_widget(window_modif, "checkbutton6");
    GtkWidget *cb_annuel = lookup_widget(window_modif, "checkbutton7");
    GtkWidget *cb_libre = lookup_widget(window_modif, "checkbutton8");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_mensuel), strstr(s.type_abonnement, "Mensuel") != NULL);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_trimestriel), strstr(s.type_abonnement, "Trimestriel") != NULL);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_annuel), strstr(s.type_abonnement, "Annuel") != NULL);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_libre), strstr(s.type_abonnement, "Accès libre") != NULL);

    gtk_widget_show(window_modif);
}
