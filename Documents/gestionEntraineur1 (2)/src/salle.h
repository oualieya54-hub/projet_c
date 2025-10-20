#ifndef SALLE_H
#define SALLE_H

#include <gtk/gtk.h>

typedef struct {
    char nom[50];
    char adresse[100];
    char telephone[20];
    char email[50];
    char mode_paiement[30];
    char heure_debut[10];
    char heure_fin[10];
    int tarif;
    int capacite;
    int espace_restauration; // 1 = oui, 0 = non
} SalleSport;

#endif // SALLE_H

