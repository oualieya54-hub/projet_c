#include "gcour.h"
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#define FICHIER "data/cours.txt"
#define FICHIER_INS "data/inscriptions.txt"

extern GtkBuilder *builder; 

void ajouter_cours(Cours c) {
    FILE *f = fopen(FICHIER, "a");
    if (f) {
        fprintf(f, "%s %s %d %d %d %.2f %s %d %d %d\n",
                c.id, c.nom, c.jour, c.mois, c.annee,
                c.heure_depart, c.type_cour,
                c.nb_personnes_max, c.mixte, c.nbPlacesOccupees);
        fclose(f);
    }
}

void modifier_cours(Cours c) {
    FILE *f = fopen(FICHIER, "r");
    FILE *tmp = fopen("tmp.txt", "w");
    Cours x;

    if (f && tmp) {
        while (fscanf(f, "%s %s %d %d %d %f %s %d %d %d",
                      x.id, x.nom, &x.jour, &x.mois, &x.annee,
                      &x.heure_depart, x.type_cour,
                      &x.nb_personnes_max, &x.mixte, &x.nbPlacesOccupees) != EOF) {

            if (strcmp(x.id, c.id) == 0)
                fprintf(tmp, "%s %s %d %d %d %.2f %s %d %d %d\n",
                        c.id, c.nom, c.jour, c.mois, c.annee,
                        c.heure_depart, c.type_cour,
                        c.nb_personnes_max, c.mixte, c.nbPlacesOccupees);
            else
                fprintf(tmp, "%s %s %d %d %d %.2f %s %d %d %d\n",
                        x.id, x.nom, x.jour, x.mois, x.annee,
                        x.heure_depart, x.type_cour,
                        x.nb_personnes_max, x.mixte, x.nbPlacesOccupees);
        }

        fclose(f);
        fclose(tmp);
        remove(FICHIER);
        rename("tmp.txt", FICHIER);
    }
}

void supprimer_cours(char id[]) {
    FILE *f = fopen(FICHIER, "r");
    FILE *tmp = fopen("tmp.txt", "w");
    Cours x;

    if (f && tmp) {
        while (fscanf(f, "%s %s %d %d %d %f %s %d %d %d",
                      x.id, x.nom, &x.jour, &x.mois, &x.annee,
                      &x.heure_depart, x.type_cour,
                      &x.nb_personnes_max, &x.mixte, &x.nbPlacesOccupees) != EOF) {

            if (strcmp(x.id, id) != 0)
                fprintf(tmp, "%s %s %d %d %d %.2f %s %d %d %d\n",
                        x.id, x.nom, x.jour, x.mois, x.annee,
                        x.heure_depart, x.type_cour,
                        x.nb_personnes_max, x.mixte, x.nbPlacesOccupees);
        }

        fclose(f);
        fclose(tmp);
        remove(FICHIER);
        rename("tmp.txt", FICHIER);
    }
}

Cours chercher_cours(char id[]) {
    FILE *f = fopen(FICHIER, "r");
    Cours x, vide = {"", "", 0, 0, 0, 0, "", 0, 0, 0};

    if (f) {
        while (fscanf(f, "%s %s %d %d %d %f %s %d %d %d",
                      x.id, x.nom, &x.jour, &x.mois, &x.annee,
                      &x.heure_depart, x.type_cour,
                      &x.nb_personnes_max, &x.mixte, &x.nbPlacesOccupees) != EOF) {
            if (strcmp(x.id, id) == 0) {
                fclose(f);
                return x;
            }
        }
        fclose(f);
    }
    return vide;
}

void afficher_cours() {
    FILE *f = fopen(FICHIER, "r");
    Cours x;

    if (f) {
        while (fscanf(f, "%s %s %d %d %d %f %s %d %d %d",
                      x.id, x.nom, &x.jour, &x.mois, &x.annee,
                      &x.heure_depart, x.type_cour,
                      &x.nb_personnes_max, &x.mixte, &x.nbPlacesOccupees) != EOF) {

            printf("%s %s %d/%d/%d %.2f %s | max:%d occ:%d mixte:%d\n",
                   x.id, x.nom, x.jour, x.mois, x.annee,
                   x.heure_depart, x.type_cour,
                   x.nb_personnes_max, x.nbPlacesOccupees, x.mixte);
        }
        fclose(f);
    }
}


void ajouter_inscription(Inscription I) {
    Cours c = chercher_cours(I.idCours);

    if (strlen(c.id) == 0) {
        printf("Erreur: cours introuvable.\n");
        return;
    }

    if (c.nbPlacesOccupees >= c.nb_personnes_max) {
        printf("Erreur: cours complet.\n");
        return;
    }

    c.nbPlacesOccupees++;
    modifier_cours(c);

    FILE *f = fopen(FICHIER_INS, "a");
    if (f) {
        fprintf(f, "%s;%s;%s;%s;%d;%d;%d\n",
                I.idMembre, I.nomMembre,
                I.idCours, I.nomCours,
                I.jour, I.mois, I.annee);
        fclose(f);
        printf("Inscription enregistrée.\n");
    }
}

void gcour_afficher_inscriptions() {
    FILE *f = fopen(FICHIER_INS, "r");
    Inscription I;

    if (f) {
        printf("\n--- Liste des inscriptions ---\n");

        while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%d;%d;%d\n",
                      I.idMembre, I.nomMembre,
                      I.idCours, I.nomCours,
                      &I.jour, &I.mois, &I.annee) != EOF) {

            printf("%s - %s → %s (%s) le %d/%d/%d\n",
                   I.idMembre, I.nomMembre,
                   I.nomCours, I.idCours,
                   I.jour, I.mois, I.annee);
        }
        fclose(f);
    }
}



