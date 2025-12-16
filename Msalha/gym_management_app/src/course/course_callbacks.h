void on_button36_clicked(GtkButton *button, gpointer user_data);
void on_button37_clicked(GtkButton *button, gpointer user_data);
void on_button39_clicked(GtkButton *button, gpointer user_data);
void on_button40_clicked(GtkButton *button, gpointer user_data);
void on_supprimer_cour_clicked(GtkButton *button, gpointer user_data);
void on_button44_clicked(GtkButton *button, gpointer user_data);
void on_button1_clicked(GtkButton *button, gpointer user_data);
void on_button2_clicked(GtkButton *button, gpointer user_data);
void on_button3_clicked(GtkButton *button, gpointer user_data);
void on_button4_clicked(GtkButton *button, gpointer user_data);
void on_button45_clicked(GtkButton *button, gpointer user_data);
void on_button_ajouter_clicked(GtkButton *button, gpointer user_data);
void on_button_modifier_clicked(GtkButton *button, gpointer user_data);
void on_button46_clicked(GtkButton *button, gpointer user_data);
void on_button47_clicked(GtkButton *button, gpointer user_data);
void on_button49_clicked(GtkButton *button, gpointer user_data);
void on_button50_clicked(GtkButton *button, gpointer user_data);
void afficher_cours_treeview(GtkWidget *treeview);
void on_treeview1_row_activated(GtkTreeView *treeview, GtkTreePath *path,
                                GtkTreeViewColumn *column, gpointer user_data);

void
on_treeview1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button46_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button47_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button49_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button50_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data);
gboolean on_cour_sportif_acceuil_focus_in_event(GtkWidget *widget, GdkEventFocus *event, gpointer user_data);

gboolean
on_cour_sportif_acceuil_focus_in_event (GtkWidget       *widget,
                                        GdkEventFocus   *event,
                                        gpointer         user_data);
void on_button51_clicked(GtkWidget *widget, gpointer data);
void on_button52_clicked(GtkWidget *widget, gpointer data);
void afficher_fenetre_inscription(GtkBuilder *builder);


