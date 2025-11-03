#include <stdio.h>
#include <string.h>
#include "salle.h" 


// -------------------- AJOUTER --------------------
int ajouter_salle(char *filename, SalleSport s)
{
    FILE *f = fopen(filename, "a");  
    if (f != NULL)                  
    {

       fprintf(f, "%s;%s;%s;%s;%s;%s;%s;%.2f;%d;%d;%d\n",
                s.nom, s.adresse, s.telephone, s.email,
                s.mode_paiement, s.heure_debut, s.heure_fin,
                s.tarif, s.capacite, s.espace_restauration, s.type_abonnement);
        fclose(f); 
        return 1;   
    }
    return 0;       
}


// -------------------- MODIFIER --------------------
int modifier_salle(char *filename, char *nom, SalleSport nouv)
{
    SalleSport s;
    int tr = 0;
    FILE *f = fopen(filename, "r");      
    FILE *f2 = fopen("temp.txt", "w");   
    if (f != NULL && f2 != NULL)
    {
        while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%f;%d;%d;%d\n",
                      s.nom, s.adresse, s.telephone, s.email,
                      s.mode_paiement, s.heure_debut, s.heure_fin,
                      &s.tarif, &s.capacite, &s.espace_restauration, &s.type_abonnement) != EOF) 
        {
            if (strcmp(s.nom, nom) == 0)  
            {
                
                 fprintf(f2, "%s;%s;%s;%s;%s;%s;%s;%.2f;%d;%d;%d\n",
                        nouv.nom, nouv.adresse, nouv.telephone, nouv.email,
                        nouv.mode_paiement, nouv.heure_debut, nouv.heure_fin,
                        nouv.tarif, nouv.capacite, nouv.espace_restauration, nouv.type_abonnement);
                tr = 1;
            }
            else
            {
               
                fprintf(f2, "%s;%s;%s;%s;%s;%s;%s;%.2f;%d;%d;%d\n",
                        s.nom, s.adresse, s.telephone, s.email,
                        s.mode_paiement, s.heure_debut, s.heure_fin,
                        s.tarif, s.capacite, s.espace_restauration, s.type_abonnement);
            }
        }
    }

    if (f) fclose(f);
    if (f2) fclose(f2);

    remove(filename);            
    rename("temp.txt", filename); 

    return tr; 
}


// -------------------- SUPPRIMER --------------------
int supprimer_salle(char *filename, char *nom)
{
    SalleSport s;
    int tr = 0;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f != NULL && f2 != NULL)
    {
        while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%f;%d;%d;%d\n",
                      s.nom, s.adresse, s.telephone, s.email,
                      s.mode_paiement, s.heure_debut, s.heure_fin,
                      &s.tarif, &s.capacite, &s.espace_restauration, &s.type_abonnement) != EOF)
        {
            if (strcmp(s.nom, nom) == 0)
                tr = 1;  
            else
                fprintf(f2, "%s;%s;%s;%s;%s;%s;%s;%.2f;%d;%d;%d\n",
                        s.nom, s.adresse, s.telephone, s.email,
                        s.mode_paiement, s.heure_debut, s.heure_fin,
                        s.tarif, s.capacite, s.espace_restauration, s.type_abonnement);
        }
    }

    if (f) fclose(f);
    if (f2) fclose(f2);

    remove(filename);
    rename("temp.txt", filename);
    return tr; 
}

// -------------------- CHERCHER --------------------
SalleSport chercher_salle(char *filename, char *nom)
{
    SalleSport s;
    int tr = 0;
    FILE *f = fopen(filename, "r");

    if (f != NULL)
    {
        while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%f;%d;%d;%d\n",
                      s.nom, s.adresse, s.telephone, s.email,
                      s.mode_paiement, s.heure_debut, s.heure_fin,
                      &s.tarif, &s.capacite, &s.espace_restauration, &s.type_abonnement) != EOF)
        {
            if (strcmp(s.nom, nom) == 0)
            {
                tr = 1;
                break;
            }
        }
    }

    if (f) fclose(f);

    if (!tr)
        strcpy(s.nom, "introuvable");

    return s;
}

void afficher_salles(char *filename)
{
    SalleSport s;
    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        printf("❌ Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    printf("\n===== Liste des salles =====\n");

    while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%f;%d;%d;%d\n",
                  s.nom, s.adresse, s.telephone, s.email,
                  s.mode_paiement, s.heure_debut, s.heure_fin,
                  &s.tarif, &s.capacite, &s.espace_restauration, &s.type_abonnement) != EOF)
    {
        printf("Nom : %s\n", s.nom);
        printf("Adresse : %s\n", s.adresse);
        printf("Téléphone : %s\n", s.telephone);
        printf("Email : %s\n", s.email);
        printf("Mode de paiement : %s\n", s.mode_paiement);
        printf("Heure d'ouverture : %s\n", s.heure_debut);
        printf("Heure de fermeture : %s\n", s.heure_fin);
        printf("Tarif : %.2f\n", s.tarif);
        printf("Capacité : %d\n", s.capacite);
        printf("Espace restauration : %d\n", s.espace_restauration);
        printf("Type d'abonnement : %d\n", s.type_abonnement);
        printf("-------------------------------------------\n");
    }

    fclose(f);
}

