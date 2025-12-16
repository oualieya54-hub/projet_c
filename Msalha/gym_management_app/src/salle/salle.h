#ifndef SALLE_H
#define SALLE_H

#include <gtk/gtk.h>

// Structures
typedef struct {
    int id_salle;
    char nom[50];
    char adresse[100];
    char telephone[20];
    char email[50];
    char mode_paiement[30];
    char heure_debut[10];
    char heure_fin[10];
    float tarif;
    int capacite;
    int espace_restauration; // 1 pour oui, 0 pour non
    char type_abonnement[100];
} SalleSport;

typedef struct {
    int id_salle;
    char cin_entraineur[20];
    char nom_entraineur[50];
    char prenom_entraineur[50];
    char nom_salle[50];
} Inscription_Salle;

// Macros
#define FICHIER_INSCRIPTIONS_SALLE "data/inscriptions_salle.txt"
#define FICHIER_SALLE "data/salle.txt"

// Function Prototypes

// Fonctions pour les salles
int generer_nouvel_id(const char *filename);
int ajouter_salle(char* filename, SalleSport s);
int supprimer_salle(const char* filename, int id);
int modifier_salle(char *filename, const int id, SalleSport nouv);
SalleSport chercher_salle(char *filename, char* nom);
SalleSport chercher_salle_par_id(const char *filename, int id_recherche);
void afficher_salles_interface(GtkWidget *treeview, const char *filename);

// Fonctions pour les inscriptions
int verifier_inscription_existe(int id_salle, char *cin);
int inscrire_salle(Inscription_Salle is, GtkWidget *parent);
Inscription_Salle* afficher_inscriptions_salle(int *nb);
void afficher_inscriptions_treeview(GtkWidget *treeview);

// Fonctions utilitaires et statistiques
void remplir_option_menu(GtkOptionMenu *menu, char *items[], int count);
void afficher_statistiques_inscriptions(GtkWidget *parent);

#endif // SALLE_H

