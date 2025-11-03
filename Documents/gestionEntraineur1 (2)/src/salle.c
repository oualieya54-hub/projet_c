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
void chercher_salle_avec_ecriture(char *filename, char *nom_recherche, char *fichier_resultat)
{
    SalleSport s;
    int trouve = 0;
    FILE *f = fopen(filename, "r");          // Fichier d'origine
    FILE *fout = fopen(fichier_resultat, "w"); // Fichier de résultat (on écrase le contenu)

    if (f == NULL || fout == NULL)
    {
        printf("Erreur d'ouverture de fichier.\n");
        return;
    }

    while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%f;%d;%d;%d\n",
                  s.nom, s.adresse, s.telephone, s.email,
                  s.mode_paiement, s.heure_debut, s.heure_fin,
                  &s.tarif, &s.capacite, &s.espace_restauration, &s.type_abonnement) != EOF)
    {
        if (strcmp(s.nom, nom_recherche) == 0)
        {
            trouve = 1;

            // Écriture des informations trouvées dans le fichier résultat
            fprintf(fout, "Nom: %s\nAdresse: %s\nTéléphone: %s\nEmail: %s\n", 
                    s.nom, s.adresse, s.telephone, s.email);
            fprintf(fout, "Paiement: %s\nHeure début: %s\nHeure fin: %s\n", 
                    s.mode_paiement, s.heure_debut, s.heure_fin);
            fprintf(fout, "Tarif: %.2f\nCapacité: %d\nEspace resto: %d\nType abo: %d\n", 
                    s.tarif, s.capacite, s.espace_restauration, s.type_abonnement);
            break; // on arrête après avoir trouvé
        }
    }

    fclose(f);
    fclose(fout);

    if (trouve)
        printf("Salle trouvée et enregistrée dans '%s'.\n", fichier_resultat);
    else
        printf("Salle non trouvée.\n");
}


void statistiques_salles(char *filename)
{
    SalleSport s;
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return;
    }

    int nb_salles = 0;
    int nb_resto = 0;
    int abonnement[4] = {0, 0, 0, 0}; // 0=Mensuel, 1=Trimestriel, 2=Annuel, 3=Accès libre

    while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%f;%d;%d;%d\n",
                  s.nom, s.adresse, s.telephone, s.email,
                  s.mode_paiement, s.heure_debut, s.heure_fin,
                  &s.tarif, &s.capacite, &s.espace_restauration, &s.type_abonnement) != EOF)
    {
        nb_salles++;
        if (s.espace_restauration) nb_resto++;
        if (s.type_abonnement >= 0 && s.type_abonnement <= 3)
            abonnement[s.type_abonnement]++;
    }

    fclose(f);

    printf("Statistiques des salles :\n");
    printf("Nombre total de salles : %d\n", nb_salles);
    printf("Nombre de salles avec espace restauration : %d\n", nb_resto);
    printf("Nombre de salles par type d'abonnement :\n");
    printf("  Mensuel       : %d\n", abonnement[0]);
    printf("  Trimestriel   : %d\n", abonnement[1]);
    printf("  Annuel        : %d\n", abonnement[2]);
    printf("  Accès libre   : %d\n", abonnement[3]);
}

