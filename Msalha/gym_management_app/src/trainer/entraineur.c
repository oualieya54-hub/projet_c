#include "entraineur.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FICHIER_ENTRAINEURS "data/entraineurs.txt"
#define FICHIER_INSCRIPTIONS "data/inscriptions.txt"

// ============ FONCTIONS CRUD POUR LES ENTRAINEURS ============

// Ajouter un entraîneur
int ajouter_entraineur(Entraineur e) {
    FILE *f = fopen(FICHIER_ENTRAINEURS, "a");
    if (f == NULL) {
        return 0; // Échec
    }
    
    fprintf(f, "%s|%s|%s|%d/%d/%d|%s|%s|%s|%s|%s|%d/%d/%d|%s|%s\n",
            e.cin, e.nom, e.prenom,
            e.date_naissance.jour, e.date_naissance.mois, e.date_naissance.annee,
            e.sexe, e.telephone, e.email, e.adresse, e.specialite,
            e.date_fin_contrat.jour, e.date_fin_contrat.mois, e.date_fin_contrat.annee,
            e.disponibilite, e.photo);
    
    fclose(f);
    return 1; // Succès
}

// Modifier un entraîneur
int modifier_entraineur(char *cin, Entraineur e) {
    FILE *f = fopen(FICHIER_ENTRAINEURS, "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (f == NULL || temp == NULL) {
        if (f) fclose(f);
        if (temp) fclose(temp);
        return 0;
    }
    
    Entraineur e_temp;
    int trouve = 0;
    
    while (fscanf(f, "%19[^|]|%49[^|]|%49[^|]|%d/%d/%d|%9[^|]|%19[^|]|%49[^|]|%99[^|]|%49[^|]|%d/%d/%d|%19[^|]|%199[^\n]\n",
                  e_temp.cin, e_temp.nom, e_temp.prenom,
                  &e_temp.date_naissance.jour, &e_temp.date_naissance.mois, &e_temp.date_naissance.annee,
                  e_temp.sexe, e_temp.telephone, e_temp.email, e_temp.adresse, e_temp.specialite,
                  &e_temp.date_fin_contrat.jour, &e_temp.date_fin_contrat.mois, &e_temp.date_fin_contrat.annee,
                  e_temp.disponibilite, e_temp.photo) == 16) {
        
        if (strcmp(e_temp.cin, cin) == 0) {
            fprintf(temp, "%s|%s|%s|%d/%d/%d|%s|%s|%s|%s|%s|%d/%d/%d|%s|%s\n",
                    e.cin, e.nom, e.prenom,
                    e.date_naissance.jour, e.date_naissance.mois, e.date_naissance.annee,
                    e.sexe, e.telephone, e.email, e.adresse, e.specialite,
                    e.date_fin_contrat.jour, e.date_fin_contrat.mois, e.date_fin_contrat.annee,
                    e.disponibilite, e.photo);
            trouve = 1;
        } else {
            fprintf(temp, "%s|%s|%s|%d/%d/%d|%s|%s|%s|%s|%s|%d/%d/%d|%s|%s\n",
                    e_temp.cin, e_temp.nom, e_temp.prenom,
                    e_temp.date_naissance.jour, e_temp.date_naissance.mois, e_temp.date_naissance.annee,
                    e_temp.sexe, e_temp.telephone, e_temp.email, e_temp.adresse, e_temp.specialite,
                    e_temp.date_fin_contrat.jour, e_temp.date_fin_contrat.mois, e_temp.date_fin_contrat.annee,
                    e_temp.disponibilite, e_temp.photo);
        }
    }
    
    fclose(f);
    fclose(temp);
    
    remove(FICHIER_ENTRAINEURS);
    rename("temp.txt", FICHIER_ENTRAINEURS);
    
    return trouve;
}

// Supprimer un entraîneur
int supprimer_entraineur(char *cin) {
    FILE *f = fopen(FICHIER_ENTRAINEURS, "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (f == NULL || temp == NULL) {
        if (f) fclose(f);
        if (temp) fclose(temp);
        return 0;
    }
    
    Entraineur e;
    int trouve = 0;
    
    while (fscanf(f, "%19[^|]|%49[^|]|%49[^|]|%d/%d/%d|%9[^|]|%19[^|]|%49[^|]|%99[^|]|%49[^|]|%d/%d/%d|%19[^|]|%199[^\n]\n",
                  e.cin, e.nom, e.prenom,
                  &e.date_naissance.jour, &e.date_naissance.mois, &e.date_naissance.annee,
                  e.sexe, e.telephone, e.email, e.adresse, e.specialite,
                  &e.date_fin_contrat.jour, &e.date_fin_contrat.mois, &e.date_fin_contrat.annee,
                  e.disponibilite, e.photo) != EOF) {
        
        if (strcmp(e.cin, cin) != 0) {
            fprintf(temp, "%s|%s|%s|%d/%d/%d|%s|%s|%s|%s|%s|%d/%d/%d|%s|%s\n",
                    e.cin, e.nom, e.prenom,
                    e.date_naissance.jour, e.date_naissance.mois, e.date_naissance.annee,
                    e.sexe, e.telephone, e.email, e.adresse, e.specialite,
                    e.date_fin_contrat.jour, e.date_fin_contrat.mois, e.date_fin_contrat.annee,
                    e.disponibilite, e.photo);
        } else {
            trouve = 1;
        }
    }
    
    fclose(f);
    fclose(temp);
    
    remove(FICHIER_ENTRAINEURS);
    rename("temp.txt", FICHIER_ENTRAINEURS);
    
    return trouve;
}

// Rechercher un entraîneur par CIN
Entraineur* rechercher_entraineur(char *cin, int *trouve) {
    FILE *f = fopen(FICHIER_ENTRAINEURS, "r");
    if (f == NULL) {
        *trouve = 0;
        return NULL;
    }
    
    Entraineur *e = (Entraineur*)malloc(sizeof(Entraineur));
    *trouve = 0;
    
    while (fscanf(f, "%19[^|]|%49[^|]|%49[^|]|%d/%d/%d|%9[^|]|%19[^|]|%49[^|]|%99[^|]|%49[^|]|%d/%d/%d|%19[^|]|%199[^\n]\n",
                  e->cin, e->nom, e->prenom,
                  &e->date_naissance.jour, &e->date_naissance.mois, &e->date_naissance.annee,
                  e->sexe, e->telephone, e->email, e->adresse, e->specialite,
                  &e->date_fin_contrat.jour, &e->date_fin_contrat.mois, &e->date_fin_contrat.annee,
                  e->disponibilite, e->photo) != EOF) {
        
        if (strcmp(e->cin, cin) == 0) {
            *trouve = 1;
            fclose(f);
            return e;
        }
    }
    
    fclose(f);
    free(e);
    return NULL;
}

// Afficher tous les entraîneurs
Entraineur* afficher_tous_entraineurs(int *nb) {
    FILE *f = fopen(FICHIER_ENTRAINEURS, "r");
    if (f == NULL) {
        *nb = 0;
        return NULL;
    }
    
    // Compter le nombre d'entraîneurs
    *nb = 0;
    Entraineur e_temp;
    while (fscanf(f, "%19[^|]|%49[^|]|%49[^|]|%d/%d/%d|%9[^|]|%19[^|]|%49[^|]|%99[^|]|%49[^|]|%d/%d/%d|%19[^|]|%199[^\n]\n",
                  e_temp.cin, e_temp.nom, e_temp.prenom,
                  &e_temp.date_naissance.jour, &e_temp.date_naissance.mois, &e_temp.date_naissance.annee,
                  e_temp.sexe, e_temp.telephone, e_temp.email, e_temp.adresse, e_temp.specialite,
                  &e_temp.date_fin_contrat.jour, &e_temp.date_fin_contrat.mois, &e_temp.date_fin_contrat.annee,
                  e_temp.disponibilite, e_temp.photo) == 16) {
        (*nb)++;
    }
    
    if (*nb == 0) {
        fclose(f);
        return NULL;
    }
    
    // Allouer la mémoire
    Entraineur *entraineurs = (Entraineur*)malloc((*nb) * sizeof(Entraineur));
    
    // Relire le fichier
    rewind(f);
    int i = 0;
    while (fscanf(f, "%19[^|]|%49[^|]|%49[^|]|%d/%d/%d|%9[^|]|%19[^|]|%49[^|]|%99[^|]|%49[^|]|%d/%d/%d|%19[^|]|%199[^\n]\n",
                  entraineurs[i].cin, entraineurs[i].nom, entraineurs[i].prenom,
                  &entraineurs[i].date_naissance.jour, &entraineurs[i].date_naissance.mois, &entraineurs[i].date_naissance.annee,
                  entraineurs[i].sexe, entraineurs[i].telephone, entraineurs[i].email, entraineurs[i].adresse, entraineurs[i].specialite,
                  &entraineurs[i].date_fin_contrat.jour, &entraineurs[i].date_fin_contrat.mois, &entraineurs[i].date_fin_contrat.annee,
                  entraineurs[i].disponibilite, entraineurs[i].photo) == 16) {
        i++;
    }
    
    fclose(f);
    return entraineurs;
}

// ============ FONCTIONS POUR L'INSCRIPTION AUX COURS ============

// Vérifier si un CIN existe
int verifier_cin_existe(char *cin) {
    int trouve;
    Entraineur *e = rechercher_entraineur(cin, &trouve);
    if (trouve) {
        free(e);
        return 1;
    }
    return 0;
}

// Inscrire un entraîneur à un cours
int inscrire_cours(Inscription_Cours ic) {
    FILE *f = fopen(FICHIER_INSCRIPTIONS, "a");
    if (f == NULL) {
        return 0;
    }
    
    fprintf(f, "%s|%s|%s|%s|%s|%d/%d/%d|%s|%s\n",
            ic.cin_entraineur, ic.nom_entraineur, ic.prenom_entraineur,
            ic.specialite, ic.nom_cours,
            ic.date_cours.jour, ic.date_cours.mois, ic.date_cours.annee,
            ic.type_cours, ic.public);
    
    fclose(f);
    return 1;
}

// Afficher toutes les inscriptions
Inscription_Cours* entraineur_afficher_inscriptions(int *nb) {
    FILE *f = fopen(FICHIER_INSCRIPTIONS, "r");
    if (f == NULL) {
        *nb = 0;
        return NULL;
    }
    
    // Compter le nombre d'inscriptions
    *nb = 0;
    Inscription_Cours ic_temp;
    while (fscanf(f, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%[^|]|%[^\n]\n",
                  ic_temp.cin_entraineur, ic_temp.nom_entraineur, ic_temp.prenom_entraineur,
                  ic_temp.specialite, ic_temp.nom_cours,
                  &ic_temp.date_cours.jour, &ic_temp.date_cours.mois, &ic_temp.date_cours.annee,
                  ic_temp.type_cours, ic_temp.public) != EOF) {
        (*nb)++;
    }
    
    if (*nb == 0) {
        fclose(f);
        return NULL;
    }
    
    // Allouer la mémoire
    Inscription_Cours *inscriptions = (Inscription_Cours*)malloc((*nb) * sizeof(Inscription_Cours));
    
    // Relire le fichier
    rewind(f);
    int i = 0;
    while (fscanf(f, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%[^|]|%[^\n]\n",
                  inscriptions[i].cin_entraineur, inscriptions[i].nom_entraineur, inscriptions[i].prenom_entraineur,
                  inscriptions[i].specialite, inscriptions[i].nom_cours,
                  &inscriptions[i].date_cours.jour, &inscriptions[i].date_cours.mois, &inscriptions[i].date_cours.annee,
                  inscriptions[i].type_cours, inscriptions[i].public) != EOF) {
        i++;
    }
    
    fclose(f);
    return inscriptions;
}














