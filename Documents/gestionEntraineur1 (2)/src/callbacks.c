#include <gtk/gtk.h>
#include <string.h>
#include "salle.h"

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include <ctype.h>
#include <regex.h>



 char mode_paiement[30];
 // Au début de callbacks.c, après les includes
static char **liste_noms_salles = NULL;
static int *liste_ids_salles = NULL;
static int nb_salles_stored = 0;

static char **liste_noms_entraineurs = NULL;
static char **liste_cins_entraineurs = NULL;
static char **liste_prenoms_entraineurs = NULL;
static int nb_entraineurs_stored = 0;

void liberer_listes_inscription_salle(void) {
    for (int i = 0; i < nb_salles_stored; i++) {
        g_free(liste_noms_salles[i]);
    }
    g_free(liste_noms_salles);
    g_free(liste_ids_salles);
    liste_noms_salles = NULL;
    liste_ids_salles = NULL;
    nb_salles_stored = 0;

    for (int i = 0; i < nb_entraineurs_stored; i++) {
        g_free(liste_noms_entraineurs[i]);
        g_free(liste_cins_entraineurs[i]);
        g_free(liste_prenoms_entraineurs[i]);
    }
    g_free(liste_noms_entraineurs);
    g_free(liste_cins_entraineurs);
    g_free(liste_prenoms_entraineurs);
    liste_noms_entraineurs = NULL;
    liste_cins_entraineurs = NULL;
    liste_prenoms_entraineurs = NULL;
    nb_entraineurs_stored = 0;
}
 // ============ FONCTIONS DE VALIDATION ============

// Vérifier si une chaîne est vide ou contient uniquement des espaces
int est_vide(const char *str) {
    if (!str || strlen(str) == 0) return 1;
    for (int i = 0; str[i]; i++) {
        if (!isspace(str[i])) return 0;
    }
    return 1;
}

// Valider le format du téléphone (8 chiffres tunisien)
int valider_telephone(const char *tel) {
    if (strlen(tel) != 8) return 0;
    for (int i = 0; tel[i]; i++) {
        if (!isdigit(tel[i])) return 0;
    }
    return 1;
}

// Valider le format de l'email
int valider_email(const char *email) {
    regex_t regex;
    int resultat;
    
    // Pattern simple pour email: texte@texte.texte
    const char *pattern = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
    
    resultat = regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB);
    if (resultat != 0) return 0;
    
    resultat = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);
    
    return (resultat == 0);
}

// Valider le format de l'heure (HH:MM)
int valider_heure(const char *heure) {
    if (strlen(heure) != 5) return 0;
    if (heure[2] != ':') return 0;
    
    // Vérifier HH
    if (!isdigit(heure[0]) || !isdigit(heure[1])) return 0;
    int h = (heure[0] - '0') * 10 + (heure[1] - '0');
    if (h < 0 || h > 23) return 0;
    
    // Vérifier MM
    if (!isdigit(heure[3]) || !isdigit(heure[4])) return 0;
    int m = (heure[3] - '0') * 10 + (heure[4] - '0');
    if (m < 0 || m > 59) return 0;
    
    return 1;
}

// Comparer deux heures (retourne 1 si h1 < h2)
int heure_avant(const char *h1, const char *h2) {
    int h1_h = (h1[0] - '0') * 10 + (h1[1] - '0');
    int h1_m = (h1[3] - '0') * 10 + (h1[4] - '0');
    int h2_h = (h2[0] - '0') * 10 + (h2[1] - '0');
    int h2_m = (h2[3] - '0') * 10 + (h2[4] - '0');
    
    if (h1_h < h2_h) return 1;
    if (h1_h == h2_h && h1_m < h2_m) return 1;
    return 0;
}

// Vérifier si le nom existe déjà
int nom_existe(const char *nom) {
    FILE *f = fopen("salle.txt", "r");
    if (!f) return 0;
    
    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f)) {
        SalleSport s;
        int nb = sscanf(ligne, 
            "%d;%49[^;];%99[^;];%19[^;];%49[^;];%29[^;];%9[^;];%9[^;];%f;%d;%d;%19[^\n]",
            &s.id_salle, s.nom, s.adresse, s.telephone, s.email,
            s.mode_paiement, s.heure_debut, s.heure_fin,
            &s.tarif, &s.capacite, &s.espace_restauration, s.type_abonnement);
        
        if (nb == 12 && strcasecmp(s.nom, nom) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// Afficher un message d'erreur
void afficher_erreur(GtkWidget *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        "%s", message
    );
    gtk_window_set_title(GTK_WINDOW(dialog), " Erreur de validation");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
void on_button35_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window_ajout = lookup_widget(button, "AjoutSalle");

    // ========== RÉCUPÉRATION DES CHAMPS ==========
    char nom[50], adresse[100], telephone[20], email[50];
    char h_debut[10], h_fin[10];

    strcpy(nom, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_ajout, "entry37"))));
    strcpy(adresse, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_ajout, "entry38"))));
    strcpy(telephone, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_ajout, "entry39"))));
    strcpy(email, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_ajout, "entry40"))));
    strcpy(h_debut, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_ajout, "entry41"))));
    strcpy(h_fin, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_ajout, "entry43"))));

    int capacite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(lookup_widget(window_ajout, "spinbutton1")));
    float tarif = gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(lookup_widget(window_ajout, "spinbutton2")));

    // ========== VALIDATION DES CHAMPS OBLIGATOIRES ==========
    
    // 1. Nom
    if (est_vide(nom)) {
        afficher_erreur(window_ajout, " Le nom de la salle est obligatoire !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry37"));
        return;
    }
    if (strlen(nom) < 3) {
        afficher_erreur(window_ajout, " Le nom doit contenir au moins 3 caractères !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry37"));
        return;
    }
    if (nom_existe(nom)) {
        afficher_erreur(window_ajout, " Une salle avec ce nom existe déjà !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry37"));
        return;
    }

    // 2. Adresse
    if (est_vide(adresse)) {
        afficher_erreur(window_ajout, " L'adresse est obligatoire !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry38"));
        return;
    }
    if (strlen(adresse) < 4) {
        afficher_erreur(window_ajout, " L'adresse doit contenir au moins 4 caractères !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry38"));
        return;
    }

    // 3. Téléphone
    if (est_vide(telephone)) {
        afficher_erreur(window_ajout, " Le numéro de téléphone est obligatoire !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry39"));
        return;
    }
    if (!valider_telephone(telephone)) {
        afficher_erreur(window_ajout, " Le téléphone doit contenir exactement 8 chiffres !\nExemple: 20123456");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry39"));
        return;
    }

    // 4. Email
    if (est_vide(email)) {
        afficher_erreur(window_ajout, " L'email est obligatoire !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry40"));
        return;
    }
    if (!valider_email(email)) {
        afficher_erreur(window_ajout, " Format d'email invalide !\nExemple: contact@salle.com");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry40"));
        return;
    }

    // 5. Heure de début
    if (est_vide(h_debut)) {
        afficher_erreur(window_ajout, " L'heure de début est obligatoire !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry41"));
        return;
    }
    if (!valider_heure(h_debut)) {
        afficher_erreur(window_ajout, " Format d'heure de début invalide !\nFormat attendu: HH:MM (exemple: 08:00)");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry41"));
        return;
    }

    // 6. Heure de fin
    if (est_vide(h_fin)) {
        afficher_erreur(window_ajout, " L'heure de fin est obligatoire !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry43"));
        return;
    }
    if (!valider_heure(h_fin)) {
        afficher_erreur(window_ajout, " Format d'heure de fin invalide !\nFormat attendu: HH:MM (exemple: 22:00)");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry43"));
        return;
    }
    if (!heure_avant(h_debut, h_fin)) {
        afficher_erreur(window_ajout, " L'heure de fin doit être après l'heure de début !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "entry43"));
        return;
    }

    // 7. Mode de paiement
    if (strlen(mode_paiement) == 0) {
        afficher_erreur(window_ajout, " Veuillez sélectionner un mode de paiement !");
        return;
    }
    // 8. Tarif
    if (tarif <= 0.0) {
        afficher_erreur(window_ajout, " Le tarif doit être supérieur à 0 !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "spinbutton2"));
        return;
    }
 if (tarif < 10.0) {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window_ajout),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION,
            GTK_BUTTONS_YES_NO,
            "⚠️ Le tarif semble faible (%.2f DT).\nVoulez-vous continuer ?",
            tarif
        );
        gtk_window_set_title(GTK_WINDOW(dialog), "Confirmation");
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        if (response != GTK_RESPONSE_YES) {
            gtk_widget_grab_focus(lookup_widget(window_ajout, "spinbutton2"));
            return;
        }
    }
    // 9. Capacité
    if (capacite <= 0) {
        afficher_erreur(window_ajout, " La capacité doit être supérieure à 0 !");
        gtk_widget_grab_focus(lookup_widget(window_ajout, "spinbutton1"));
        return;
    }
    if (capacite < 10) {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window_ajout),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION,
            GTK_BUTTONS_YES_NO,
            "⚠️ La capacité semble faible (%d personnes).\nVoulez-vous continuer ?",
            capacite
        );
        gtk_window_set_title(GTK_WINDOW(dialog), "Confirmation");
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        if (response != GTK_RESPONSE_YES) {
            gtk_widget_grab_focus(lookup_widget(window_ajout, "spinbutton1"));
            return;
        }
    }


    // 10. Type d'abonnement
char type_abonnement[100] = ""; // Commence vide
int au_moins_un = 0;

if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(window_ajout, "checkbutton1")))) {
    if (au_moins_un) strcat(type_abonnement, ", ");
    strcat(type_abonnement, "Mensuel");
    au_moins_un = 1;
}
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(window_ajout, "checkbutton2")))) {
    if (au_moins_un) strcat(type_abonnement, ", ");
    strcat(type_abonnement, "Trimestriel");
    au_moins_un = 1;
}
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(window_ajout, "checkbutton3")))) {
    if (au_moins_un) strcat(type_abonnement, ", ");
    strcat(type_abonnement, "Annuel");
    au_moins_un = 1;
}
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(window_ajout, "checkbutton4")))) {
    if (au_moins_un) strcat(type_abonnement, ", ");
    strcat(type_abonnement, "Accès libre");
    au_moins_un = 1;
}

if (!au_moins_un) {
    afficher_erreur(window_ajout, " Veuillez sélectionner au moins un type d'abonnement !");
    return;
}

    // ========== VALIDATION RÉUSSIE - CRÉATION DE LA SALLE ==========
    
    int espace_restauration = gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(lookup_widget(window_ajout, "radiobutton1"))
    );

    SalleSport s;
    s.id_salle = generer_nouvel_id("salle.txt");
    strcpy(s.nom, nom);
    strcpy(s.adresse, adresse);
    strcpy(s.telephone, telephone);
    strcpy(s.email, email);
    strcpy(s.heure_debut, h_debut);
    strcpy(s.heure_fin, h_fin);
    strcpy(s.mode_paiement, mode_paiement);
    s.capacite = capacite;
    s.tarif = tarif;
    s.espace_restauration = espace_restauration;
    strcpy(s.type_abonnement, type_abonnement);

    if (ajouter_salle("salle.txt", s))
    {
        // Message de succès
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window_ajout),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Salle '%s' ajoutée avec succès !\nID: %d",
            s.nom, s.id_salle
        );
        gtk_window_set_title(GTK_WINDOW(dialog), "Succès");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // ============ CHERCHER LA FENÊTRE GESTION EXISTANTE ============
        GtkWidget *window_gestion = NULL;
        GList *toplevels = gtk_window_list_toplevels();
        
        printf("🔍 Recherche de la fenêtre GestionSalle...\n");
        
        for (GList *l = toplevels; l; l = l->next)
        {
            GtkWidget *w = GTK_WIDGET(l->data);
            if (GTK_IS_WINDOW(w) && w != window_ajout)
            {
                const char *name = gtk_widget_get_name(w);
                printf("  Fenêtre trouvée: %s\n", name ? name : "NULL");
                
                if (name && strcmp(name, "GestionSalle") == 0)
                {
                    window_gestion = w;
                    printf("  ✓ GestionSalle trouvée !\n");
                    break;
                }
            }
        }
        g_list_free(toplevels);

        // ============ FERMER LA FENÊTRE D'AJOUT ============
        gtk_widget_destroy(window_ajout);

        // ============ GÉRER LA FENÊTRE GESTION ============
        if (window_gestion)
        {
            // Si elle existe, rafraîchir le TreeView
            printf("📝 Rafraîchissement de la fenêtre existante...\n");
            GtkWidget *treeview = lookup_widget(window_gestion, "treeview3");
            if (treeview)
            {
                afficher_salles_interface(treeview, "salle.txt");
                printf("✓ TreeView rafraîchi\n");
            }
            
            // S'assurer qu'elle est visible et au premier plan
            gtk_widget_show(window_gestion);
            gtk_window_present(GTK_WINDOW(window_gestion));
        }
        else
        {
            // Si elle n'existe pas, la créer
            printf("🆕 Création d'une nouvelle fenêtre GestionSalle...\n");
            window_gestion = create_GestionSalle();
            
            if (window_gestion)
            {
                GtkWidget *treeview = lookup_widget(window_gestion, "treeview3");
                if (treeview)
                {
                    afficher_salles_interface(treeview, "salle.txt");
                }
                gtk_widget_show(window_gestion);
                printf("✓ Fenêtre GestionSalle créée et affichée\n");
            }
            else
            {
                printf("❌ Erreur : impossible de créer la fenêtre GestionSalle\n");
            }
        }
    }
    else
    {
        afficher_erreur(window_ajout, " Erreur lors de l'ajout de la salle !");
    }
}


void
on_button36_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window = lookup_widget(button, "AjoutSalle");
    gtk_widget_destroy(window);
}


void
on_esp__ce1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
 strcpy(mode_paiement, "Espece");
}


void
on_ch__que1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
   strcpy(mode_paiement, "Cheque");
}


void
on_cartebancaire1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    strcpy(mode_paiement, "Carte Bancaire");
}


void
on_virement1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
 strcpy(mode_paiement, "Virement");
}




// ==================== SUPPRESSION ====================

// Structure pour passer les données au callback idle
typedef struct {
    GtkWidget *treeview;
    char nom_supprime[50];
} RefreshData;

// Fonction callback qui sera appelée quand GTK est idle
static gboolean rafraichir_treeview_idle(gpointer data)
{
    RefreshData *refresh = (RefreshData *)data;
    
    printf("Rafraichissement du TreeView en idle...\n");
    afficher_salles_interface(refresh->treeview, "salle.txt");
    printf("TreeView rafraichi\n");
    
    // Libérer la mémoire
    g_free(refresh);
    
    // Retourner FALSE pour ne pas rappeler cette fonction
    return FALSE;
}

void on_button34_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window_gestion = lookup_widget(button, "GestionSalle");
    GtkWidget *treeview = lookup_widget(window_gestion, "treeview3");
    
    // Obtenir la sélection
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    // Vérifier qu'une ligne est sélectionnée
    if (!gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window_gestion),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Veuillez sélectionner une salle à supprimer !"
        );
        gtk_window_set_title(GTK_WINDOW(dialog), "Aucune sélection");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    // Récupérer l'ID de la salle sélectionnée (colonne 0)
    int id_salle;
    gtk_tree_model_get(model, &iter, 0, &id_salle, -1);
    
    printf("Salle sélectionnée avec ID : %d\n", id_salle);
    
    // ====== Dialogue de confirmation ======
    GtkWidget *dialog_confirm = gtk_message_dialog_new(
        GTK_WINDOW(window_gestion),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "Êtes-vous sûr de vouloir supprimer la salle avec l'ID :\n\n%d ?",
        id_salle
    );
    gtk_window_set_title(GTK_WINDOW(dialog_confirm), "Confirmation de suppression");
    
    // Attendre la réponse
    int response = gtk_dialog_run(GTK_DIALOG(dialog_confirm));
    gtk_widget_destroy(dialog_confirm);
    
    if (response == GTK_RESPONSE_YES)
    {
        printf("Confirmation reçue, suppression en cours...\n");
        
        int resultat = supprimer_salle("salle.txt", id_salle);
        
        if (resultat == 1)
        {
            printf("Suppression réussie de la salle ID %d\n", id_salle);
            
            // Rafraîchir le TreeView via g_idle_add (meilleure pratique)
            RefreshData *refresh = g_malloc(sizeof(RefreshData));
            refresh->treeview = treeview;
            // On ne stocke plus le nom, mais on pourrait stocker l'ID si besoin
            // Pour ce rafraîchissement, on recharge tout → pas besoin de plus
            
            g_idle_add(rafraichir_treeview_idle, refresh);
            printf("Rafraîchissement planifié\n");
        }
        else
        {
            printf("Échec de la suppression\n");
            
            GtkWidget *dialog_error = gtk_message_dialog_new(
                GTK_WINDOW(window_gestion),
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Erreur lors de la suppression de la salle !"
            );
            gtk_dialog_run(GTK_DIALOG(dialog_error));
            gtk_widget_destroy(dialog_error);
        }
    }
    else
    {
        printf("Suppression annulée par l'utilisateur\n");
    }
    
    printf("Fin de on_button34_clicked\n");
}
// ==================== BOUTON AJOUTER dans GestionSalle ====================
void on_button32_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window_ajout = create_AjoutSalle();
    gtk_widget_show(window_ajout);
}


void on_entry36_changed(GtkEditable *editable, gpointer user_data)
{
    GtkWidget *entry = GTK_WIDGET(editable);
    GtkWidget *window_gestion = gtk_widget_get_toplevel(entry);
    GtkWidget *treeview = lookup_widget(window_gestion, "treeview3");
    
    const char *texte = gtk_entry_get_text(GTK_ENTRY(entry));
    
    // Si vide, afficher tout
    if (strlen(texte) == 0)
    {
        afficher_salles_interface(treeview, "salle.txt");
        return;
    }
    
    // Créer le modèle pour les résultats
    GtkListStore *store = gtk_list_store_new(12, 
        G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    
    // Ouvrir le fichier et chercher dans tous les champs
    FILE *f = fopen("salle.txt", "r");
    if (!f)
    {
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
        return;
    }
    
    char ligne[512];
    int nb_resultats = 0;
    
    // Convertir le texte de recherche en minuscules pour comparaison
    char texte_lower[100];
    strncpy(texte_lower, texte, 99);
    texte_lower[99] = '\0';
    for (int i = 0; texte_lower[i]; i++) {
        texte_lower[i] = tolower(texte_lower[i]);
    }
    
    while (fgets(ligne, sizeof(ligne), f))
    {
        SalleSport s;
        int nb = sscanf(ligne, 
            "%d;%49[^;];%99[^;];%19[^;];%49[^;];%29[^;];%9[^;];%9[^;];%f;%d;%d;%19[^\n]",
            &s.id_salle, s.nom, s.adresse, s.telephone, s.email,
            s.mode_paiement, s.heure_debut, s.heure_fin,
            &s.tarif, &s.capacite, &s.espace_restauration, s.type_abonnement);
        
        if (nb != 12) continue;
        
        // Créer une chaîne avec tous les champs pour la recherche
        char tous_champs[1000];
        char buf_id[20], buf_tarif[20], buf_cap[20], buf_eres[20];
        sprintf(buf_id, "%d", s.id_salle);
        sprintf(buf_tarif, "%.2f", s.tarif);
        sprintf(buf_cap, "%d", s.capacite);
        sprintf(buf_eres, "%s", s.espace_restauration ? "Oui" : "Non");
        
        snprintf(tous_champs, sizeof(tous_champs), "%s %s %s %s %s %s %s %s %s %s %s %s",
                 buf_id, s.nom, s.adresse, s.telephone, s.email,
                 s.mode_paiement, s.heure_debut, s.heure_fin,
                 buf_tarif, buf_cap, buf_eres, s.type_abonnement);
        
        // Convertir en minuscules
        for (int i = 0; tous_champs[i]; i++) {
            tous_champs[i] = tolower(tous_champs[i]);
        }
        
        // Vérifier si le texte recherché est présent
        if (strstr(tous_champs, texte_lower) != NULL)
        {
            GtkTreeIter iter;
            
            char buf_tarif_display[20], buf_cap_display[10], buf_eres_display[10];
            sprintf(buf_tarif_display, "%.2f DT", s.tarif);
            sprintf(buf_cap_display, "%d", s.capacite);
            sprintf(buf_eres_display, "%s", s.espace_restauration ? "Oui" : "Non");
            
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                0, s.id_salle,
                1, s.nom, 
                2, s.adresse, 
                3, s.telephone, 
                4, s.email,
                5, s.heure_debut, 
                6, s.heure_fin, 
                7, buf_tarif_display,
                8, s.mode_paiement, 
                9, buf_cap_display, 
                10, buf_eres_display,
                11, s.type_abonnement, 
                -1);
            
            nb_resultats++;
        }
    }
    
    fclose(f);
    
    // Si aucun résultat, afficher tout
    if (nb_resultats == 0)
    {
        g_object_unref(store);
        afficher_salles_interface(treeview, "salle.txt");
    }
    else
    {
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
    }
    
    printf("Recherche '%s' : %d résultat(s) trouvé(s)\n", texte, nb_resultats);
}
void on_button33_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window_gestion = lookup_widget(button, "GestionSalle");
    GtkWidget *treeview = lookup_widget(window_gestion, "treeview3");

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (!gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_gestion),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez sélectionner une salle à modifier !");
        gtk_window_set_title(GTK_WINDOW(dialog), "Aucune sélection");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Récupérer l'ID (colonne 0)
    int id_salle;
    gtk_tree_model_get(model, &iter, 0, &id_salle, -1);

    // Chercher la salle par ID (tu devras créer `chercher_salle_par_id`)
    SalleSport s = chercher_salle_par_id("salle.txt", id_salle);
    
    // Si la salle n'est pas trouvée (id non valide)
    if (s.id_salle == -1) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_gestion),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur : salle introuvable.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Ouvrir la fenêtre de modification
    GtkWidget *window_modif = create_MOdiffSalle();
    
    // Stocker l'ID pour le bouton "Modifier"
    g_object_set_data(G_OBJECT(window_modif), "id_salle", GINT_TO_POINTER(id_salle));
    
  GtkWidget *entry56 = lookup_widget(window_modif, "entry56"); // Nom
    GtkWidget *entry57 = lookup_widget(window_modif, "entry57"); // Adresse
    GtkWidget *entry58 = lookup_widget(window_modif, "entry58"); // Téléphone
    GtkWidget *entry59 = lookup_widget(window_modif, "entry59"); // Email
    GtkWidget *entry60 = lookup_widget(window_modif, "entry60"); // Heure début
    GtkWidget *entry61 = lookup_widget(window_modif, "entry61"); // Heure fin
    GtkWidget *spinbutton21 = lookup_widget(window_modif, "spinbutton21"); // Capacité
    GtkWidget *spinbutton22 = lookup_widget(window_modif, "spinbutton22"); // Tarif

    gtk_entry_set_text(GTK_ENTRY(entry56), s.nom);
    gtk_entry_set_text(GTK_ENTRY(entry57), s.adresse);
    gtk_entry_set_text(GTK_ENTRY(entry58), s.telephone);
    gtk_entry_set_text(GTK_ENTRY(entry59), s.email);
    gtk_entry_set_text(GTK_ENTRY(entry60), s.heure_debut);
    gtk_entry_set_text(GTK_ENTRY(entry61), s.heure_fin);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton21), s.capacite);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton22), s.tarif);

    // --- Mode de paiement ---
    GtkWidget *optionmenu2 = lookup_widget(window_modif, "optionmenu2");
    if(optionmenu2)
    {
        if(strcmp(s.mode_paiement, "Espece") == 0)
            gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu2), 0);
        else if(strcmp(s.mode_paiement, "Cheque") == 0)
            gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu2), 1);
        else if(strcmp(s.mode_paiement, "Carte Bancaire") == 0)
            gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu2), 2);
        else if(strcmp(s.mode_paiement, "Virement") == 0)
            gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu2), 3);
    }

    // Espace restauration
    GtkWidget *radiobutton11 = lookup_widget(window_modif, "radiobutton11");
    GtkWidget *radiobutton10 = lookup_widget(window_modif, "radiobutton10");
    if (s.espace_restauration) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton11), TRUE);
    else gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton10), TRUE);

    // Checkbuttons
    GtkWidget *cb_mensuel = lookup_widget(window_modif, "checkbutton5");
    GtkWidget *cb_trimestriel = lookup_widget(window_modif, "checkbutton6");
    GtkWidget *cb_annuel = lookup_widget(window_modif, "checkbutton7");
    GtkWidget *cb_libre = lookup_widget(window_modif, "checkbutton8");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_mensuel), strcmp(s.type_abonnement, "Mensuel") == 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_trimestriel), strcmp(s.type_abonnement, "Trimestriel") == 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_annuel), strcmp(s.type_abonnement, "Annuel") == 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_libre), strcmp(s.type_abonnement, "Accès libre") == 0);

    gtk_widget_show(window_modif);
}




void
on_button45_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}
void on_button46_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window_modif = gtk_widget_get_toplevel(GTK_WIDGET(button));
    if (!window_modif) return;

    int id_salle = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(window_modif), "id_salle"));
    if (id_salle <= 0) return;

    // ============ RÉCUPÉRATION DES DONNÉES ============
    char nom[50], adresse[100], telephone[20], email[50];
    char h_debut[10], h_fin[10];
    char local_mode_paiement[30] = "Non défini";

    strcpy(nom, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_modif, "entry56"))));
    strcpy(adresse, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_modif, "entry57"))));
    strcpy(telephone, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_modif, "entry58"))));
    strcpy(email, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_modif, "entry59"))));
    strcpy(h_debut, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_modif, "entry60"))));
    strcpy(h_fin, gtk_entry_get_text(GTK_ENTRY(lookup_widget(window_modif, "entry61"))));

    // Mode de paiement
    GtkWidget *optionmenu2 = lookup_widget(window_modif, "optionmenu2");
    if (optionmenu2)
    {
        int index = gtk_option_menu_get_history(GTK_OPTION_MENU(optionmenu2));
        switch (index)
        {
            case 0: strcpy(local_mode_paiement, "Espece"); break;
            case 1: strcpy(local_mode_paiement, "Cheque"); break;
            case 2: strcpy(local_mode_paiement, "Carte Bancaire"); break;
            case 3: strcpy(local_mode_paiement, "Virement"); break;
        }
    }

    // Espace restauration
    int espace_restauration = gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(lookup_widget(window_modif, "radiobutton11"))
    );

    // Type abonnement
  // Type abonnement - PERMETTRE PLUSIEURS CHOIX
char type_abonnement[100] = "";
int nb_types = 0;

if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(window_modif, "checkbutton5")))) {
    if (nb_types > 0) strcat(type_abonnement, ", ");
    strcat(type_abonnement, "Mensuel");
    nb_types++;
}
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(window_modif, "checkbutton6")))) {
    if (nb_types > 0) strcat(type_abonnement, ", ");
    strcat(type_abonnement, "Trimestriel");
    nb_types++;
}
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(window_modif, "checkbutton7")))) {
    if (nb_types > 0) strcat(type_abonnement, ", ");
    strcat(type_abonnement, "Annuel");
    nb_types++;
}
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(window_modif, "checkbutton8")))) {
    if (nb_types > 0) strcat(type_abonnement, ", ");
    strcat(type_abonnement, "Accès libre");
    nb_types++;
}

    // Capacité et tarif
    int capacite = gtk_spin_button_get_value_as_int(
        GTK_SPIN_BUTTON(lookup_widget(window_modif, "spinbutton21"))
    );
    float tarif = gtk_spin_button_get_value_as_float(
        GTK_SPIN_BUTTON(lookup_widget(window_modif, "spinbutton22"))
    );

    // ============ TROUVER LA FENÊTRE GESTION ============
    GtkWidget *window_gestion = NULL;
    GtkWidget *treeview = NULL;
    
    printf("🔍 Recherche de la fenêtre GestionSalle...\n");
    
    GList *toplevels = gtk_window_list_toplevels();
    for (GList *l = toplevels; l; l = l->next)
    {
        GtkWidget *w = GTK_WIDGET(l->data);
        if (GTK_IS_WINDOW(w) && w != window_modif)
        {
            const char *name = gtk_widget_get_name(w);
            const char *title = gtk_window_get_title(GTK_WINDOW(w));
            
            printf("  Fenêtre trouvée: name='%s', title='%s'\n", 
                   name ? name : "NULL", 
                   title ? title : "NULL");
            
            // Essayer plusieurs méthodes de détection
            if ((name && strcmp(name, "GestionSalle") == 0) ||
                (title && strstr(title, "Gestion des salles") != NULL))
            {
                window_gestion = w;
                treeview = lookup_widget(window_gestion, "treeview3");
                printf("  ✓ GestionSalle trouvée ! TreeView: %p\n", treeview);
                break;
            }
        }
    }
    g_list_free(toplevels);
    
    if (!window_gestion)
    {
        printf("  ❌ GestionSalle NON trouvée !\n");
    }

    // ============ CRÉER LA STRUCTURE ============
    SalleSport s;
    s.id_salle = id_salle;
    strcpy(s.nom, nom);
    strcpy(s.adresse, adresse);
    strcpy(s.telephone, telephone);
    strcpy(s.email, email);
    strcpy(s.heure_debut, h_debut);
    strcpy(s.heure_fin, h_fin);
    strcpy(s.mode_paiement, local_mode_paiement);
    s.capacite = capacite;
    s.tarif = tarif;
    s.espace_restauration = espace_restauration;
    strcpy(s.type_abonnement, type_abonnement);

    // ============ MODIFIER DANS LE FICHIER ============
    printf("📝 Modification de la salle ID %d...\n", id_salle);
    int resultat = modifier_salle("salle.txt", id_salle, s);
    printf("Résultat modification: %d\n", resultat);

    // ============ FERMER LA FENÊTRE DE MODIFICATION ============
    printf("🗑️ Fermeture de la fenêtre de modification...\n");
    gtk_widget_destroy(window_modif);

    // ============ RAFRAÎCHIR LE TREEVIEW ============
    if (resultat)
    {
        printf("✓ Modification réussie dans le fichier\n");
        
        if (treeview)
        {
            printf("🔄 Rafraîchissement du TreeView...\n");
            afficher_salles_interface(treeview, "salle.txt");
            printf("✓ TreeView rafraîchi\n");
        }
        else
        {
            printf("❌ TreeView introuvable !\n");
        }
    }
    else
    {
        printf("❌ Erreur lors de la modification du fichier\n");
        g_warning("Erreur lors de la modification");
    }
}

void on_esp__ce2_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    strcpy(mode_paiement, "Espece");
}

void on_ch__que2_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    strcpy(mode_paiement, "Cheque");
}

void on_cartebancaire2_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    strcpy(mode_paiement, "Carte Bancaire");
}

void on_virement2_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    strcpy(mode_paiement, "Virement");
}






void on_inscri_salle_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *om_entr = lookup_widget(window, "optionmenu3");
    GtkWidget *om_salle = lookup_widget(window, "optionmenu4");
    int idx_entr = gtk_option_menu_get_history(GTK_OPTION_MENU(om_entr));
    int idx_salle = gtk_option_menu_get_history(GTK_OPTION_MENU(om_salle));

    if (idx_entr < 0 || idx_salle < 0) {
        afficher_erreur(window, "Veuillez sélectionner un entraîneur et une salle !");
        return;
    }

    // Relire les fichiers pour obtenir les données réelles
    FILE *fs = fopen("salle.txt", "r");
    SalleSport s_temp;
    int i = 0;
    while (fscanf(fs, "%d;%49[^;];%99[^;];%19[^;];%49[^;];%29[^;];%9[^;];%9[^;];%f;%d;%d;%19[^\n]",
                  &s_temp.id_salle, s_temp.nom, s_temp.adresse, s_temp.telephone, s_temp.email,
                  s_temp.mode_paiement, s_temp.heure_debut, s_temp.heure_fin,
                  &s_temp.tarif, &s_temp.capacite, &s_temp.espace_restauration, s_temp.type_abonnement) == 12) {
        if (i == idx_salle) break;
        i++;
    }
    fclose(fs);

    FILE *fe = fopen("entraineurs.txt", "r");
    char cin[20], nom[50], prenom[50];
    i = 0;
    while (fscanf(fe, "%19[^;];%49[^;];%49[^\n]", cin, nom, prenom) == 3) {
        if (i == idx_entr) break;
        i++;
    }
    fclose(fe);

    Inscription_Salle is;
    is.id_salle = s_temp.id_salle;
    strcpy(is.cin_entraineur, cin);
    strcpy(is.nom_entraineur, nom);
    strcpy(is.prenom_entraineur, prenom);
    strcpy(is.nom_salle, s_temp.nom);

   if (inscrire_salle(is, window)) {
        afficher_inscriptions_treeview(lookup_widget(window, "treeview4"));
        GtkWidget *dlg = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
            "Inscription réussie !");
        gtk_dialog_run(GTK_DIALOG(dlg));
        gtk_widget_destroy(dlg);
    }
}
// collback.c
void remplir_listes_inscription_salle(GtkWidget *window) {
    GtkWidget *om_entr = lookup_widget(window, "optionmenu3");
    GtkWidget *om_salle = lookup_widget(window, "optionmenu4");

    // Libérer les anciennes listes si elles existent
    liberer_listes_inscription_salle();

    // Charger les salles
    FILE *fs = fopen("salle.txt", "r");
    if (!fs) {
        g_warning("Impossible d'ouvrir salle.txt");
        return;
    }

    char ligne[512];
    while (fgets(ligne, sizeof(ligne), fs) && nb_salles_stored < 1000) {
        SalleSport s;
        if (sscanf(ligne,
            "%d;%49[^;];%99[^;];%19[^;];%49[^;];%29[^;];%9[^;];%9[^;];%f;%d;%d;%19[^\n]",
            &s.id_salle, s.nom, s.adresse, s.telephone, s.email,
            s.mode_paiement, s.heure_debut, s.heure_fin,
            &s.tarif, &s.capacite, &s.espace_restauration, s.type_abonnement) == 12) {

            liste_noms_salles = g_realloc(liste_noms_salles, (nb_salles_stored + 1) * sizeof(char*));
            liste_ids_salles = g_realloc(liste_ids_salles, (nb_salles_stored + 1) * sizeof(int));

            liste_noms_salles[nb_salles_stored] = g_strdup(s.nom);
            liste_ids_salles[nb_salles_stored] = s.id_salle;
            nb_salles_stored++;
        }
    }
    fclose(fs);

    // Charger les entraîneurs
    FILE *fe = fopen("entraineurs.txt", "r");
    if (!fe) {
        g_warning("Impossible d'ouvrir entraineurs.txt");
        return;
    }

    char cin[20], nom[50], prenom[50];
    while (fscanf(fe, "%19[^;];%49[^;];%49[^\n]", cin, nom, prenom) == 3 && nb_entraineurs_stored < 1000) {
        liste_noms_entraineurs = g_realloc(liste_noms_entraineurs, (nb_entraineurs_stored + 1) * sizeof(char*));
        liste_cins_entraineurs = g_realloc(liste_cins_entraineurs, (nb_entraineurs_stored + 1) * sizeof(char*));
        liste_prenoms_entraineurs = g_realloc(liste_prenoms_entraineurs, (nb_entraineurs_stored + 1) * sizeof(char*));

        liste_noms_entraineurs[nb_entraineurs_stored] = g_strdup(nom);
        liste_cins_entraineurs[nb_entraineurs_stored] = g_strdup(cin);
        liste_prenoms_entraineurs[nb_entraineurs_stored] = g_strdup(prenom);
        nb_entraineurs_stored++;
    }
    fclose(fe);

    // Remplir les menus
    char **items_salles = g_new(char*, nb_salles_stored);
    for (int i = 0; i < nb_salles_stored; i++) {
        items_salles[i] = liste_noms_salles[i];
    }
    remplir_option_menu(GTK_OPTION_MENU(om_salle), items_salles, nb_salles_stored);
    g_free(items_salles);

    char **items_entr = g_new(char*, nb_entraineurs_stored);
    for (int i = 0; i < nb_entraineurs_stored; i++) {
        items_entr[i] = g_strdup_printf("%s %s", liste_noms_entraineurs[i], liste_prenoms_entraineurs[i]);
    }
    remplir_option_menu(GTK_OPTION_MENU(om_entr), items_entr, nb_entraineurs_stored);
    for (int i = 0; i < nb_entraineurs_stored; i++) {
        g_free(items_entr[i]);
    }
    g_free(items_entr);

    // Charger les inscriptions existantes
    GtkWidget *treeview = lookup_widget(window, "treeview4");
    afficher_inscriptions_treeview(treeview);
}

void
on_button49_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    afficher_statistiques_inscriptions(window);
}

