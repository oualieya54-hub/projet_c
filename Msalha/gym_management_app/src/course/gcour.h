#ifndef GCOUR_H_INCLUDED
#define GCOUR_H_INCLUDED

typedef struct {
    char id[30];
    char nom[50];
    int jour;
    int mois;
    int annee;
    float heure_depart;
    char type_cour[50];
    int nb_personnes_max;     
    int mixte;
    int nbPlacesOccupees;     
} Cours;

typedef struct {
    char idMembre[20];
    char nomMembre[50];
    char idCours[20];
    char nomCours[50];
    int jour, mois, annee;
} Inscription;


void ajouter_cours(Cours c);
void modifier_cours(Cours c);
void supprimer_cours(char id[]);
Cours chercher_cours(char id[]);
void afficher_cours();


void ajouter_inscription(Inscription I);
void afficher_inscriptions();

void incrementer_places_cours(char idCours[]);
void decrementer_places_cours(char idCours[]);

int cours_est_complet(Cours c);

#endif

