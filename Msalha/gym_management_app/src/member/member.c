#include "member.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* File paths */
#define MEMBER_FILE "data/member.txt"
#define COACH_FILE "data/coach.txt"

/* Add a member to member.txt */
int ajouter_membre(Membre m) {
    FILE *f = fopen(MEMBER_FILE, "a");
    if (f == NULL) return 0;
    
    // Format: nom;prenom;cin;j;m;a;sexe;email;num;ville;abo;sport
    fprintf(f, "%s;%s;%s;%d;%d;%d;%s;%s;%s;%s;%s;%s\n", 
            m.nom, m.prenom, m.cin, 
            m.jour, m.mois, m.annee, 
            m.sexe, m.email, m.num, 
            m.ville, m.abonnement, m.sport);
            
    fclose(f);
    return 1;
}

/* Supprimer member by CIN */
int supprimer_membre(char *cin) {
    Membre m;
    FILE *f = fopen(MEMBER_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    int found = 0;
    char line[1024];

    if (f == NULL || temp == NULL) {
        if (f) fclose(f);
        if (temp) fclose(temp);
        return 0;
    }

    while (fgets(line, sizeof(line), f) != NULL) {
        m.sport[0] = '\0'; // Init sport to empty in case it's missing
        int res = sscanf(line, "%[^;];%[^;];%[^;];%d;%d;%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]", 
                  m.nom, m.prenom, m.cin, 
                  &m.jour, &m.mois, &m.annee, 
                  m.sexe, m.email, m.num, 
                  m.ville, m.abonnement, m.sport);
        
        if (res >= 11) { // Allow 11 (missing sport) or 12
            if (strcmp(m.cin, cin) != 0) {
                fprintf(temp, "%s", line);
            } else {
                found = 1;
            }
        } else {
            // If the line is malformed, we keep it to be safe
            fprintf(temp, "%s", line);
        }
    }

    fclose(f);
    fclose(temp);
    remove(MEMBER_FILE);
    rename("temp.txt", MEMBER_FILE);
    return found;
}

/* Modify member logic */
/* Modify member logic */
int modifier_membre(char *cin_original, Membre new_m) {
    Membre m;
    FILE *f = fopen(MEMBER_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    int found = 0;
    char line[1024];

    if (f == NULL || temp == NULL) {
        if (f) fclose(f);
        if (temp) fclose(temp);
        return 0;
    }

    while (fgets(line, sizeof(line), f) != NULL) {
        // Check if this line is the one we want to modify
        m.sport[0] = '\0'; 
        int res = sscanf(line, "%[^;];%[^;];%[^;];%d;%d;%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]", 
                  m.nom, m.prenom, m.cin, 
                  &m.jour, &m.mois, &m.annee, 
                  m.sexe, m.email, m.num, 
                  m.ville, m.abonnement, m.sport);
        
        if (res >= 11) {
            if (strcmp(m.cin, cin_original) == 0) {
                // Determine newline character if present in original line or default to \n
                fprintf(temp, "%s;%s;%s;%d;%d;%d;%s;%s;%s;%s;%s;%s\n", 
                        new_m.nom, new_m.prenom, new_m.cin, 
                        new_m.jour, new_m.mois, new_m.annee, 
                        new_m.sexe, new_m.email, new_m.num, 
                        new_m.ville, new_m.abonnement, new_m.sport);
                found = 1;
            } else {
                fprintf(temp, "%s", line);
            }
        } else {
             fprintf(temp, "%s", line);
        }
    }

    fclose(f);
    fclose(temp);
    remove(MEMBER_FILE);
    rename("temp.txt", MEMBER_FILE);
    return found;
}

/* Search by CIN */
Membre *rechercher_membre(char *cin) {
    static Membre m;
    FILE *f = fopen(MEMBER_FILE, "r");
    char line[1024];

    if (f == NULL) return NULL;

    while (fgets(line, sizeof(line), f) != NULL) {
        m.sport[0] = '\0';
        int res = sscanf(line, "%[^;];%[^;];%[^;];%d;%d;%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]", 
                  m.nom, m.prenom, m.cin, 
                  &m.jour, &m.mois, &m.annee, 
                  m.sexe, m.email, m.num, 
                  m.ville, m.abonnement, m.sport);

        if (res >= 11) {
            if (strcmp(m.cin, cin) == 0) {
                fclose(f);
                return &m;
            }
        }
    }
    fclose(f);
    return NULL;
}

/* Display in TreeView */
enum {
    NOM_COL,
    PRENOM_COL,
    CIN_COL,
    DATE_COL,
    SEXE_COL,
    EMAIL_COL,
    NUM_COL,
    VILLE_COL,
    ABO_COL,
    SPORT_COL,
    N_COLS
};

void afficher_membre(GtkTreeView *liste) {
    GtkListStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    // Setup Columns if they don't exist
    if (gtk_tree_view_get_column(liste, 0) == NULL) {
        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_append_column(liste, gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", NOM_COL, NULL));
        gtk_tree_view_append_column(liste, gtk_tree_view_column_new_with_attributes("Prenom", renderer, "text", PRENOM_COL, NULL));
        gtk_tree_view_append_column(liste, gtk_tree_view_column_new_with_attributes("CIN", renderer, "text", CIN_COL, NULL));
        gtk_tree_view_append_column(liste, gtk_tree_view_column_new_with_attributes("Naissance", renderer, "text", DATE_COL, NULL));
        gtk_tree_view_append_column(liste, gtk_tree_view_column_new_with_attributes("Sexe", renderer, "text", SEXE_COL, NULL));
        gtk_tree_view_append_column(liste, gtk_tree_view_column_new_with_attributes("Email", renderer, "text", EMAIL_COL, NULL));
        gtk_tree_view_append_column(liste, gtk_tree_view_column_new_with_attributes("Tel", renderer, "text", NUM_COL, NULL));
        gtk_tree_view_append_column(liste, gtk_tree_view_column_new_with_attributes("Ville", renderer, "text", VILLE_COL, NULL));
        gtk_tree_view_append_column(liste, gtk_tree_view_column_new_with_attributes("Abonnement", renderer, "text", ABO_COL, NULL));
        gtk_tree_view_append_column(liste, gtk_tree_view_column_new_with_attributes("Sport", renderer, "text", SPORT_COL, NULL));
    }
    
    // Check if store is already attached
    store = GTK_LIST_STORE(gtk_tree_view_get_model(liste));
    if (store == NULL) {
        store = gtk_list_store_new(N_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
        gtk_tree_view_set_model(liste, GTK_TREE_MODEL(store));
        g_object_unref(store);
    } else {
        gtk_list_store_clear(store);
    }
    
    Membre m;
    FILE *f = fopen(MEMBER_FILE, "r");
    char line[1024];
    
    if (f != NULL) {
        char date_str[30];
        while (fgets(line, sizeof(line), f) != NULL) {
            m.sport[0] = '\0';
            int res = sscanf(line, "%[^;];%[^;];%[^;];%d;%d;%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]", 
                      m.nom, m.prenom, m.cin, 
                      &m.jour, &m.mois, &m.annee, 
                      m.sexe, m.email, m.num, 
                      m.ville, m.abonnement, m.sport);

            if (res >= 11) { 
                sprintf(date_str, "%d/%d/%d", m.jour, m.mois, m.annee);
                
                GtkTreeIter iter;
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter,
                                   NOM_COL, m.nom,
                                   PRENOM_COL, m.prenom,
                                   CIN_COL, m.cin,
                                   DATE_COL, date_str,
                                   SEXE_COL, m.sexe,
                                   EMAIL_COL, m.email,
                                   NUM_COL, m.num,
                                   VILLE_COL, m.ville,
                                   ABO_COL, m.abonnement,
                                   SPORT_COL, m.sport,
                                   -1);
            }
        }
        fclose(f);
    }
}

int demander_coach(CoachDemande cm) {
    FILE *f = fopen(COACH_FILE, "a");
    if (f == NULL) return 0;
    
    fprintf(f, "%s;%s;%s;%s;%s\n", cm.nom, cm.prenom, cm.cin, cm.sexe, cm.nom_coach);
    fclose(f);
    return 1;
}
