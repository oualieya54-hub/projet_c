#ifndef PROJECT4_H
#define PROJECT4_H

#include <gtk/gtk.h>

// Définition de la taille maximale des chaînes pour les champs
#define MAX_STR_LEN 50

// --- 1. STRUCTURES DE DONNÉES ---

// Structure pour un équipement (basée sur la fenêtre d'ajout/modification)
typedef struct Equipement {
    char id_equipement[MAX_STR_LEN];
    char type[MAX_STR_LEN];
    char etat[MAX_STR_LEN];     // Ex: "neuf", "ancien", "réparable"
    int quantite;               // Nombre d'unités de cet équipement
    char date_ajout[MAX_STR_LEN]; // Format YYYY-MM-DD
    int garantie;               // 1 pour Oui, 0 pour Non
    struct Equipement *next;    // Pointeur pour la liste chaînée
} Equipement;

// Structure pour une réservation (basée sur la fenêtre de réservation)
typedef struct Reservation {
    char id_equipement[MAX_STR_LEN];
    char nom_entraineur[MAX_STR_LEN];
    char nom_cours[MAX_STR_LEN];
    char date_reservation[MAX_STR_LEN];
    char heure_reservation[MAX_STR_LEN];
    struct Reservation *next;
} Reservation;


// --- 2. VARIABLES GLOBALES ET BUILDER GTK ---
extern GtkBuilder *builder;
extern Equipement *liste_equipements_head; // Tête de la liste d'équipements


// --- 3. FONCTIONS DE GESTION DE MÉMOIRE (Non-BD) ---
Equipement* ajouter_equipement_en_memoire(const char *id, const char *type, const char *etat, int quantite);
int supprimer_equipement_en_memoire(const char *id);
void nettoyer_listes(); // Libère la mémoire à la fermeture de l'application


// --- 4. FONCTIONS DE GESTION GTK (Handlers de Signaux) ---
void on_continuer_login_clicked(GtkButton *button, gpointer user_data);
void on_ajouter_equipement_clicked(GtkButton *button, gpointer user_data);
void on_supprimer_equipement_clicked(GtkButton *button, gpointer user_data);
void on_reserver_clicked(GtkButton *button, gpointer user_data);

// Fonction de rafraîchissement de la GtkTreeView
void update_equipement_treeview(GtkTreeView *treeview);

#endif
