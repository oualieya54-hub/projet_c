
#include <gtk/gtk.h>

// ============ VARIABLES GLOBALES ============
extern GtkWidget *g_window_main;
extern GtkWidget *g_treeview1;
extern GtkWidget *g_window_inscri;
extern void populate_treeview1(void);

// ============ CALLBACKS POUR AJOUTER UN ENTRAINEUR ============
void on_btn_ajouter_confirmer_clicked(GtkButton *button, gpointer user_data);
void on_btn_ajouter_annuler_clicked(GtkButton *button, gpointer user_data);


// ============ CALLBACKS POUR MODIFIER UN ENTRAINEUR ============
void on_btn_modifier_confirmer_clicked(GtkButton *button, gpointer user_data);
void on_btn_modifier_annuler_clicked(GtkButton *button, gpointer user_data);


// ============ CALLBACKS POUR SUPPRIMER UN ENTRAINEUR ============
void on_btn_supprimer_confirmer_clicked(GtkButton *button, gpointer user_data);

// ============ CALLBACKS POUR INSCRIPTION À UN COURS ============
void on_btn_inscrire_confirmer_clicked(GtkButton *button, gpointer user_data);
void on_btn_inscrire_annuler_clicked(GtkButton *button, gpointer user_data);


// ============ CALLBACK POUR RECHERCHER UN ENTRAINEUR ============
void on_btn_rechercher_clicked(GtkButton *button, gpointer user_data);

// ============ MAPPING DES NOMS GLADE (nécessaire pour Glade-2) ============
void on_buttonA1_clicked(GtkButton *button, gpointer user_data);
void on_buttonA2_clicked(GtkButton *button, gpointer user_data);
void on_buttonM1_clicked(GtkButton *button, gpointer user_data);
void on_buttonM2_clicked(GtkButton *button, gpointer user_data);
void on_buttoni1_clicked(GtkButton *button, gpointer user_data);
void on_buttoni2_clicked(GtkButton *button, gpointer user_data);
void on_buttonG1_clicked(GtkButton *button, gpointer user_data);
void on_buttonG2_clicked(GtkButton *button, gpointer user_data);
void on_buttonG3_clicked(GtkButton *button, gpointer user_data);
void on_buttonG4_clicked(GtkButton *button, gpointer user_data);
void on_buttonG5_clicked(GtkButton *button, gpointer user_data);

// ============ CALLBACKS POUR LES DIALOGUES DE CONFIRMATION ============
// Confirmation Ajouter
void on_buttona1_clicked(GtkButton *button, gpointer user_data);
void on_buttona2_clicked(GtkButton *button, gpointer user_data);

// Confirmation Modifier
void on_buttonm1_clicked(GtkButton *button, gpointer user_data);
void on_buttonm2_clicked(GtkButton *button, gpointer user_data);

// Confirmation Inscription
void on_buttonI1_clicked(GtkButton *button, gpointer user_data);
void on_buttonI2_clicked(GtkButton *button, gpointer user_data);

// Confirmation Supprimer
void on_buttons1_clicked(GtkButton *button, gpointer user_data);
void on_buttons2_clicked(GtkButton *button, gpointer user_data);
