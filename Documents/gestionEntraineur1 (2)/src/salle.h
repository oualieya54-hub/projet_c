#ifndef SALLE_H
#define SALLE_H



typedef struct {
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
    int type_abonnement; // 0=Mensuel, 1=Trimestriel, 2=Annuel, 3=Accès libre
    
} SalleSport;

int ajouter_salle(char *filename, SalleSport s);
int modifier_salle(char *filename, char *nom, SalleSport nouv);
int supprimer_salle(char *filename, char *nom);
SalleSport chercher_salle(char *filename, char *nom);

#endif // SALLE_H

