
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project4.h"

// --- Définitions des variables globales (déclarées dans project4.h) ---
GtkBuilder *builder = NULL;
Equipement *liste_equipements_head = NULL;


// ====================================================================
// A. FONCTIONS DE GESTION DE LA MÉMOIRE (Listes Chaînées)
// ====================================================================

/**
 * Alloue et ajoute un nouvel équipement au début de la liste.
 * @return Le nouveau nœud d'équipement ajouté.
 */
Equipement* ajouter_equipement_en_memoire(const char *id, const char *type, const char *etat, int quantite) {
    // 1. Allouer de la mémoire pour le nouvel équipement
    Equipement *nouvel_equipement = (Equipement *)malloc(sizeof(Equipement));
    if (nouvel_equipement == NULL) {
        perror("Erreur d'allocation mémoire pour Equipement");
        return NULL;
    }

    // 2. Remplir les champs
    strncpy(nouvel_equipement->id_equipement, id, MAX_STR_LEN - 1);
    strncpy(nouvel_equipement->type, type, MAX_STR_LEN - 1);
    strncpy(nouvel_equipement->etat, etat, MAX_STR_LEN - 1);
    nouvel_equipement->quantite = quantite;
    // Les autres champs (date, garantie) seraient remplis ici...

    // 3. Ajouter au début de la liste chaînée
    nouvel_equipement->next = liste_equipements_head;
    liste_equipements_head = nouvel_equipement;
   
    return nouvel_equipement;
}

/**
 * Nettoie et libère toute la mémoire des listes chaînées.
 */
void nettoyer_listes() {
    Equipement *current = liste_equipements_head;
    Equipement *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    liste_equipements_head = NULL;
    printf("Mémoire des équipements libérée.\n");
    // FAIRE DE MÊME POUR LA LISTE DES RÉSERVATIONS
}


// ====================================================================
// B. FONCTIONS DE GESTION GTK (Handlers de Signaux)
// ====================================================================

void on_ajouter_equipement_clicked(GtkButton *button, gpointer user_data) {
    // 1. Récupérer les widgets d'entrée
    GtkWidget *entry_id = GTK_WIDGET(gtk_builder_get_object(builder, "id_equipement_entry"));
    GtkWidget *spinbutton_quantite = GTK_WIDGET(gtk_builder_get_object(builder, "quantite_spinbutton"));
   
    // 2. Extraire les données
    const char *id = gtk_entry_get_text(GTK_ENTRY(entry_id));
    int quantite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_quantite));

    // Vérification basique
    if (strlen(id) == 0 || quantite <= 0) {
        // Afficher un dialogue d'erreur Gtk
        return;
    }

    // 3. Appeler la fonction de gestion de mémoire
    Equipement *nouvel_item = ajouter_equipement_en_memoire(id, "Poids", "neuf", quantite);

    if (nouvel_item != NULL) {
        printf("Ajout réussi: ID %s, Quantité %d\n", nouvel_item->id_equipement, nouvel_item->quantite);
        // Mettre à jour la vue (Treeview) de l'administrateur
        // update_equipement_treeview(GTK_TREE_VIEW(gtk_builder_get_object(builder, "equipement_list_treeview")));
    } else {
        printf("Échec de l'ajout.\n");
    }
}

void on_supprimer_equipement_clicked(GtkButton *button, gpointer user_data) {
    // ... Logique pour récupérer l'ID à supprimer ...
    // ... Appeler supprimer_equipement_en_memoire(id) ...
}

// ... Autres handlers ...
