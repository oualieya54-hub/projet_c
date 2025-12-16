#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

/* ================= STRUCT ================= */
typedef struct {
    char id[20];
    char nom[100];
    char type[50];
    char quantite[20];
    char date_ajout[30];
    int etat;       // 0 ancien, 1 neuf
    int garantie;   // 0 sans, 1 avec
} Equipement;

/* ================= MAIN ================= */
void on_window_destroy(GtkWidget *widget, gpointer user_data);
void on_button_continuer_clicked(GtkButton *button, gpointer user_data);

/* ================= ADMIN ================= */
void on_button_chercher_clicked(GtkButton *button, gpointer user_data);
void on_button_ajouter_clicked(GtkButton *button, gpointer user_data);
void on_button_modif_clicked(GtkButton *button, gpointer user_data);
void on_button_supprimer_clicked(GtkButton *button, gpointer user_data);
void on_button5_clicked(GtkButton *button, gpointer user_data);

/* ================= ADD ================= */
void on_button_ajouter1_clicked(GtkButton *button, gpointer user_data);
void on_button_annulerlajout_clicked(GtkButton *button, gpointer user_data);

/* ================= MODIFY ================= */
void on_button_modifier1_clicked(GtkButton *button, gpointer user_data);
void on_button_annulermodif_clicked(GtkButton *button, gpointer user_data);

/* ================= DELETE ================= */
void on_button_supprimer1_clicked(GtkButton *button, gpointer user_data);
void on_button_annulersupprimer_clicked(GtkButton *button, gpointer user_data);
void on_button_recherchersupprimer_clicked(GtkButton *button, gpointer user_data);

/* ================= RESERVATION ================= */
void on_button_continuer2_clicked(GtkButton *button, gpointer user_data);
void on_button_reserver_clicked(GtkButton *button, gpointer user_data);
void on_button_annulerreserver_clicked(GtkButton *button, gpointer user_data);

/* ======= WRAPPERS OBLIGATOIRES (POUR interface.c) ======= */
void on_button_modif1_clicked(GtkButton *button, gpointer user_data);
void on_button_annuler_modif_clicked(GtkButton *button, gpointer user_data);
void on_button_chercher1_clicked(GtkButton *button, gpointer user_data);
void on_button_sup_sup_clicked(GtkButton *button, gpointer user_data);
void on_button_annuler_sup_clicked(GtkButton *button, gpointer user_data);
void on_button_res__rver_clicked(GtkButton *button, gpointer user_data);
void on_button_annuler_reservation_clicked(GtkButton *button, gpointer user_data);

/* ================= TREEVIEW ================= */
void setup_treeview(GtkWidget *treeview);
void populate_treeview(GtkWidget *treeview);
void add_equipment_to_treeview(GtkWidget *treeview, Equipement eq);
void clear_treeview(GtkWidget *treeview);
void refresh_treeview(void);
void on_treeview_row_activated(GtkTreeView *, GtkTreePath *, GtkTreeViewColumn *, gpointer);

/* ================= FILE ================= */
int save_equipment(Equipement eq);
int load_all_equipment(Equipement equipements[], int max);
int delete_equipment_by_id(const char *id);
int modify_equipment(Equipement eq);
Equipement* find_equipment_by_id(const char *id);
char* generate_equipment_id(void);

/* ================= GLOBAL ================= */
extern GtkWidget *global_admin_window;

#endif
