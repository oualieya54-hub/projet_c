#ifndef SALLE_CALLBACKS_H
#define SALLE_CALLBACKS_H

#include <gtk/gtk.h>

void salle_on_entry36_changed(GtkEditable *editable, gpointer user_data);
void salle_on_button32_clicked(GtkButton *button, gpointer user_data);
void salle_on_button34_clicked(GtkButton *button, gpointer user_data);
void salle_on_button33_clicked(GtkButton *button, gpointer user_data);
void salle_on_inscri_salle_clicked(GtkButton *button, gpointer user_data);
void salle_on_button49_clicked(GtkButton *button, gpointer user_data);
void salle_on_button35_clicked(GtkButton *button, gpointer user_data);
void salle_on_button36_clicked(GtkButton *button, gpointer user_data);
void salle_on_esp__ce1_activate(GtkMenuItem *menuitem, gpointer user_data);
void salle_on_ch__que1_activate(GtkMenuItem *menuitem, gpointer user_data);
void salle_on_cartebancaire1_activate(GtkMenuItem *menuitem, gpointer user_data);
void salle_on_virement1_activate(GtkMenuItem *menuitem, gpointer user_data);
void salle_on_button45_clicked(GtkButton *button, gpointer user_data);
void salle_on_button46_clicked(GtkButton *button, gpointer user_data);
void salle_on_esp__ce2_activate(GtkMenuItem *menuitem, gpointer user_data);
void salle_on_ch__que2_activate(GtkMenuItem *menuitem, gpointer user_data);
void salle_on_cartebancaire2_activate(GtkMenuItem *menuitem, gpointer user_data);
void salle_on_virement2_activate(GtkMenuItem *menuitem, gpointer user_data);
void salle_remplir_listes_inscription_salle(GtkWidget *window);


#endif /* SALLE_CALLBACKS_H */
