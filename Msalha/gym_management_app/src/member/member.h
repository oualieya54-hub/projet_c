#ifndef MEMBER_H
#define MEMBER_H

#include <gtk/gtk.h>

typedef struct {
    char nom[50];
    char prenom[50];
    char cin[20];
    int jour;
    int mois;
    int annee;
    char sexe[20];
    char email[50];
    char num[20];
    char ville[20];
    char abonnement[20];
    char sport[100]; // Comma separated list of sports
} Membre;

typedef struct {
    char nom[50];
    char prenom[50];
    char cin[20];
    char sexe[20];
    char nom_coach[50];
} CoachDemande;

// CRUD Operations
int ajouter_membre(Membre m);
int modifier_membre(char *cin_original, Membre m);
int supprimer_membre(char *cin);
Membre *rechercher_membre(char *cin);

// Utility
void afficher_membre(GtkTreeView *liste);
int demander_coach(CoachDemande cm);
void vider_entrees(GtkWidget *window);

#endif
