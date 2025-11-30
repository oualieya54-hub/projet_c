#include <stdio.h>
#include <string.h>
#include "salle.h" 

int generer_nouvel_id(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 1;

    int max_id = 0;
    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id;
        if (sscanf(ligne, "%d;", &id) == 1) { // Lire uniquement le premier champ (id)
            if (id > max_id) max_id = id;
        }
    }
    fclose(f);
    return max_id + 1;
}

// -------------------- AJOUTER --------------------
int   ajouter_salle(char *filename, SalleSport s)
{
    FILE *f = fopen(filename, "a");

    if (f != NULL)                  
    {

            fprintf(f, "%d;%s;%s;%s;%s;%s;%s;%s;%.2f;%d;%d;%s\n",
    s.id_salle,
    s.nom, s.adresse, s.telephone, s.email,
    s.mode_paiement, s.heure_debut, s.heure_fin,
    s.tarif, s.capacite, s.espace_restauration, s.type_abonnement
);


        fclose(f); 
        return 1;   
    }
    return 0;       
}

// Fonction pour remplir le ctree
void afficher_salles_interface(GtkWidget *treeview, const char *filename)
{
    printf("Début affichage_salles_interface (TreeView)\n");
    
    GtkListStore *store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    // Créer le modèle avec 11 colonnes
    store = gtk_list_store_new(
        12, G_TYPE_INT,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING
    );
    
    // Vérifier si les colonnes existent déjà
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    int nb_colonnes = g_list_length(columns);
    g_list_free(columns);
    
    if (nb_colonnes == 0)
    {
        printf("Création des colonnes...\n");
        
      
        const char *titres[] = {
    "ID", "Nom", "Adresse", "Téléphone", "Email", 
    "Heure Début", "Heure Fin", "Tarif",
    "Mode Paiement", "Capacité", "Espace Resto", "Type Abonnement"
};
        for (int i = 0; i < 12; i++)
        {
            renderer = gtk_cell_renderer_text_new();
            column = gtk_tree_view_column_new_with_attributes(
                titres[i], renderer, "text", i, NULL
            );
            gtk_tree_view_column_set_resizable(column, TRUE);
            gtk_tree_view_column_set_min_width(column, 80);
            gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        }
    }
    
    // Lire le fichier
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        printf(" Impossible d’ouvrir %s\n", filename);
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
        return;
    }
    
    char ligne[512];
    int line_count = 0;
    
    while (fgets(ligne, sizeof(ligne), f))
    {
        line_count++;
        SalleSport s;
        
     int nb = sscanf(ligne,
    "%d;%49[^;];%99[^;];%19[^;];%49[^;];%29[^;];%9[^;];%9[^;];%f;%d;%d;%19[^\n]",
    &s.id_salle, s.nom, s.adresse, s.telephone, s.email,
    s.mode_paiement, s.heure_debut, s.heure_fin,
    &s.tarif, &s.capacite, &s.espace_restauration, s.type_abonnement
);
        if (nb != 12)
        {
            printf("⚠️  Ligne %d ignorée (format incorrect : %d champs lus)\n", line_count, nb);
            continue;
        }
        
        // Formatage
        char buf_tarif[20];
        char buf_cap[10];
        char buf_eres[10];
        
        sprintf(buf_tarif, "%.2f DT", s.tarif);
        sprintf(buf_cap, "%d", s.capacite);
        sprintf(buf_eres, "%s", s.espace_restauration ? "Oui" : "Non");
        
        // Ajout dans le modèle
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(
            store, &iter,
            0, s.id_salle,
            1, s.nom,
            2, s.adresse,
            3, s.telephone,
            4, s.email,
            5, s.heure_debut,
            6, s.heure_fin,
            7, buf_tarif,
            8, s.mode_paiement,
            9, buf_cap,
            10, buf_eres,
            11, s.type_abonnement,
            -1
        );
    }
    
    fclose(f);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
    
    printf("Fin affichage_salles_interface\n");
}

// -------------------- SUPPRIMER --------------------

int supprimer_salle(const char *filename, int id)
{
    if (id <= 0) {
        printf("ID invalide pour la suppression.\n");
        return 0;
    }

    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Impossible d'ouvrir le fichier %s en lecture.\n", filename);
        return 0;
    }

    FILE *f2 = fopen("temp.txt", "w");
    if (!f2) {
        printf("Impossible de créer le fichier temporaire.\n");
        fclose(f);
        return 0;
    }

    char ligne[512];
    int trouve = 0;

    while (fgets(ligne, sizeof(ligne), f)) {
        // Supprimer le saut de ligne éventuel pour une comparaison propre
        ligne[strcspn(ligne, "\n")] = '\0';

        int current_id;
        // On lit uniquement l'ID au début de la ligne (avant le premier ';')
        if (sscanf(ligne, "%d;", &current_id) == 1) {
            if (current_id == id) {
                // Ne PAS écrire cette ligne dans temp.txt → suppression
                trouve = 1;
                continue;
            }
        }
        // Réécrire la ligne inchangée
        fprintf(f2, "%s\n", ligne);
    }

    fclose(f);
    fclose(f2);

    if (trouve) {
        // Remplacer l'ancien fichier par le nouveau
        if (remove(filename) != 0) {
            printf("Erreur lors de la suppression de l'ancien fichier.\n");
            return 0;
        }
        if (rename("temp.txt", filename) != 0) {
            printf("Erreur lors du renommage du fichier temporaire.\n");
            return 0;
        }
        printf("Salle avec ID %d supprimée avec succès.\n", id);
        return 1;
    } else {
        // Aucune salle trouvée → supprimer le fichier temporaire
        remove("temp.txt");
        printf("Aucune salle trouvée avec l'ID %d.\n", id);
        return 0;
    }
}


// -------------------- CHERCHER --------------------
SalleSport chercher_salle(char *filename, char *nom)
{
    SalleSport s;
    int tr = 0;
    FILE *f = fopen(filename, "r");

    // Initialiser la structure
    s.id_salle = -1;
    strcpy(s.nom, "introuvable");
    strcpy(s.adresse, "");
    strcpy(s.telephone, "");
    strcpy(s.email, "");
    strcpy(s.mode_paiement, "");
    strcpy(s.heure_debut, "");
    strcpy(s.heure_fin, "");
    s.tarif = 0.0;
    s.capacite = 0;
    s.espace_restauration = 0;
    strcpy(s.type_abonnement, "");

    if (f != NULL)
    {
        char ligne[512];
        
        printf("Recherche de: '%s'\n", nom);
        
        while (fgets(ligne, sizeof(ligne), f) != NULL)
        {
            SalleSport temp;
            // CORRECTION : Lecture avec ID en premier
            int nb = sscanf(ligne, 
                "%d;%49[^;];%99[^;];%19[^;];%49[^;];%29[^;];%9[^;];%9[^;];%f;%d;%d;%19[^\n]",
                &temp.id_salle, temp.nom, temp.adresse, temp.telephone, temp.email,
                temp.mode_paiement, temp.heure_debut, temp.heure_fin,
                &temp.tarif, &temp.capacite, &temp.espace_restauration, temp.type_abonnement);
            
            if (nb != 12) continue;
            
            printf("   Comparaison avec: '%s'\n", temp.nom);
            
            // Comparaison insensible à la casse et trim des espaces
            char nom_trim[50], temp_nom_trim[50];
            
            // Copier et supprimer les espaces de début/fin
            strcpy(nom_trim, nom);
            strcpy(temp_nom_trim, temp.nom);
            
            // Trim le nom recherché
            char *start = nom_trim;
            while (*start == ' ' || *start == '\t' || *start == '\n') start++;
            char *end = start + strlen(start) - 1;
            while (end > start && (*end == ' ' || *end == '\t' || *end == '\n')) end--;
            *(end + 1) = '\0';
            memmove(nom_trim, start, strlen(start) + 1);
            
            // Trim le nom du fichier
            start = temp_nom_trim;
            while (*start == ' ' || *start == '\t' || *start == '\n') start++;
            end = start + strlen(start) - 1;
            while (end > start && (*end == ' ' || *end == '\t' || *end == '\n')) end--;
            *(end + 1) = '\0';
            memmove(temp_nom_trim, start, strlen(start) + 1);
            
            if (strcasecmp(temp_nom_trim, nom_trim) == 0)
            {
                s = temp;
                tr = 1;
                printf("✓ Salle '%s' trouvée\n", temp.nom);
                break;
            }
        }
        fclose(f);
    }

    if (!tr)
        printf("✗ Salle '%s' introuvable\n", nom);

    return s;
}

// -------------------- MODIFIER --------------------
// -------------------- MODIFIER --------------------
int modifier_salle(char *filename, const int id, SalleSport nouv)
{
    SalleSport s;
    int tr = 0;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (!f || !f2) {
        if (f) fclose(f);
        if (f2) fclose(f2);
        return 0;
    }

    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f))
    {
        // Parser la ligne complète (12 champs)
      
        int nb = sscanf(ligne,
    "%d;%49[^;];%99[^;];%19[^;];%49[^;];%29[^;];%9[^;];%9[^;];%f;%d;%d;%19[^\n]",
    &s.id_salle, s.nom, s.adresse, s.telephone, s.email,
    s.mode_paiement, s.heure_debut, s.heure_fin,
    &s.tarif, &s.capacite, &s.espace_restauration, s.type_abonnement
);


        // Si le parsing échoue, réécrire la ligne telle quelle
        if (nb != 12) {
            fprintf(f2, "%s", ligne); // ligne inchangée
            continue;
        }

        if (s.id_salle == id)
        {
            // Écrire les NOUVELLES données, mais avec le MÊME ID
       fprintf(f2, "%d;%s;%s;%s;%s;%s;%s;%s;%.2f;%d;%d;%s\n",
        id,
        nouv.nom, nouv.adresse, nouv.telephone, nouv.email,
        nouv.mode_paiement, nouv.heure_debut, nouv.heure_fin,
        nouv.tarif, nouv.capacite, nouv.espace_restauration,
        nouv.type_abonnement);

            tr = 1;
        }
        else
        {
            // Réécrire l'ancienne ligne inchangée
            fprintf(f2, "%s", ligne);
        }
    }

    fclose(f);
    fclose(f2);

    if (tr)
    {
        remove(filename);
        rename("temp.txt", filename);
    }
    else
    {
        remove("temp.txt"); // Pas de salle trouvée → supprimer le temp
    }

    return tr;
}

// -------------------- CHERCHER PAR ID --------------------
SalleSport chercher_salle_par_id(const char *filename, int id_recherche)
{
    SalleSport s;
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        s.id_salle = -1; // Indique qu'aucune salle n'a été trouvée
        return s;
    }

    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f))
    {
        SalleSport temp;
        // Lecture de la ligne complète avec ID en premier
        int nb = sscanf(ligne,
            "%d;%49[^;];%99[^;];%19[^;];%49[^;];%29[^;];%9[^;];%9[^;];%f;%d;%d;%19[^"
            "\n]",
            &temp.id_salle, temp.nom, temp.adresse, temp.telephone, temp.email,
            temp.mode_paiement, temp.heure_debut, temp.heure_fin,
            &temp.tarif, &temp.capacite, &temp.espace_restauration, temp.type_abonnement
        );

        if (nb == 12 && temp.id_salle == id_recherche)
        {
            fclose(f);
            return temp; // Salle trouvée → on la renvoie
        }
    }

    fclose(f);
    // Si on arrive ici, aucune salle n’a été trouvée
    s.id_salle = -1;
    return s;
}

