#ifndef SALLE_H
#define SALLE_H
#include <gtk/gtk.h>


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
    int espace_restauration; // 1 = oui, 0 = non
     char type_abonnement[100];
    
} SalleSport;


int ajouter_salle(char *filename, SalleSport s);
void afficher_salles_interface(GtkWidget *ctree, const char *filename);
int supprimer_salle(const char *filename, int id);

SalleSport chercher_salle(char *filename, char *nom);
int modifier_salle(char *filename, const int id, SalleSport nouv);
SalleSport chercher_salle_par_id(const char *filename, int id_recherche);


//void chercher_salle_avec_ecriture(char *filename, char *nom_recherche, char *fichier_resultat);
//void statistiques_salles(char *filename);*/
#endif // SALLE_H

