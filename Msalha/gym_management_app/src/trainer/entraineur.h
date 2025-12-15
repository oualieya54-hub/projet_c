#ifndef ENTRAINEUR_H
#define ENTRAINEUR_H


// Structure pour la date
typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

// Structure pour l'entraîneur
typedef struct {
    char cin[20];
    char nom[50];
    char prenom[50];
    Date date_naissance;
    char sexe[10];        // "Homme" ou "Femme"
    char telephone[20];
    char email[50];
    char adresse[100];
    char specialite[50];
    Date date_fin_contrat;
    char disponibilite[20];
    char photo[200];      // Chemin vers la photo
} Entraineur;

// Structure pour l'inscription à un cours
typedef struct {
    char cin_entraineur[20];
    char nom_entraineur[50];
    char prenom_entraineur[50];
    char specialite[50];
    char nom_cours[50];
    Date date_cours;
    char type_cours[20];  // "Mixte" ou "Non Mixte"
    char public[20];      // "Enfants" ou "Adulte"
} Inscription_Cours;

// Fonctions CRUD pour les entraîneurs
int ajouter_entraineur(Entraineur e);
int modifier_entraineur(char *cin, Entraineur e);
int supprimer_entraineur(char *cin);
Entraineur* rechercher_entraineur(char *cin, int *trouve);
Entraineur* afficher_tous_entraineurs(int *nb);

// Fonctions pour l'inscription aux cours
int inscrire_cours(Inscription_Cours ic);
Inscription_Cours* afficher_inscriptions(int *nb);
int verifier_cin_existe(char *cin);


#endif






