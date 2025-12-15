#include <gtk/gtk.h>


void
member_on_treeview_row_activated       (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
member_on_gm12_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
member_on_gm8_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
member_on_gm10_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
member_on_gm11_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
member_on_ajm29_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
member_on_modm29_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
member_on_valideraffichage_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
member_on_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data);

/* Added by Antigravity */
void member_on_button3_clicked(GtkButton *button, gpointer user_data);
void member_on_cp12_clicked(GtkButton *button, gpointer user_data);

void on_conf_add_yes(GtkButton *button, gpointer user_data);
void on_conf_add_no(GtkButton *button, gpointer user_data);
void on_conf_del_yes(GtkButton *button, gpointer user_data);
void on_conf_del_no(GtkButton *button, gpointer user_data);
void on_conf_mod_yes(GtkButton *button, gpointer user_data);
void on_conf_mod_no(GtkButton *button, gpointer user_data);
