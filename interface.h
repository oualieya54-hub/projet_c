#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>

/* =====================================================
   Fenêtre d’accueil / choix (admin / trainer)
   ===================================================== */
GtkWidget* create_main_window(void);

/* =====================================================
   Fenêtre principale : Gestion des équipements
   (contient le notebook admin / trainer)
   ===================================================== */
GtkWidget* create_Gestion_des_equipements_(void);

/* =====================================================
   Fenêtres ADMIN (CRUD équipements)
   ===================================================== */

/* Ajout */
GtkWidget* create_window_d_ajout(void);

/* Modification */
GtkWidget* create_window_de_modification(void);

/* Suppression */
GtkWidget* create_window_de_suppresion(void);

/* =====================================================
   Fenêtre de réservation (trainer)
   ===================================================== */
GtkWidget* create_window_de_reservation(void);

#endif /* INTERFACE_H */
