

#include <stdio.h>
#include <string.h>
#include "salle.h"

int main (int argc, char *argv[]) { 


     SalleSport s1 = {"GymPlus", "Tunis", "20123456", "gymplus@gmail.com", "Espèces", "08:00", "22:00", 40.5, 50, 1, 0};
    SalleSport s2 = {"PowerFit", "Sousse", "20998877", "powerfit@gmail.com", "Carte", "07:00", "23:00", 55.0, 60, 0, 2};
    SalleSport s3 = {"BodyZone", "Ariana", "22112233", "bodyzone@gmail.com", "Virement", "09:00", "21:00", 30.0, 40, 1, 1};
    
    char filename[] = "salles.txt";

    printf("=== Test CRUD SalleSport ===\n\n");

   
    printf("Ajout des salles...\n");
    ajouter_salle(filename, s1);
    ajouter_salle(filename, s2);
    ajouter_salle(filename, s3);
    printf("✅ Ajout terminé.\n\n");

   
    SalleSport nouv = {"PowerFit", "Sousse", "20998877", "contact@powerfit.tn", "Carte", "06:00", "22:00", 60.0, 70, 1, 3};
    if (modifier_salle(filename, "PowerFit", nouv))
        printf("✅ Modification réussie.\n\n");
    else
        printf("❌ Erreur de modification.\n\n");

   
    SalleSport trouv = chercher_salle(filename, "BodyZone");
    if (strlen(trouv.nom) > 0)
        printf("✅ Salle trouvée : %s, %s, %.2f DT\n\n", trouv.nom, trouv.adresse, trouv.tarif);
    else
        printf("❌ Salle non trouvée.\n\n");

   
    if (supprimer_salle(filename, "GymPlus"))
        printf("✅ Suppression réussie.\n\n");
    else
        printf("❌ Salle introuvable pour suppression.\n\n");

return 0; }
