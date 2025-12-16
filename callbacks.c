#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include <gtk/gtk.h>
GtkWidget *g_win_admin = NULL;

char g_id_to_modify[30] = "";

/* prototypes */
static void show_info(GtkWindow *parent, const char *msg);
static void show_error(GtkWindow *parent, const char *msg);

/* implementations */
static void show_info(GtkWindow *parent, const char *msg) {
    GtkWidget *d = gtk_message_dialog_new(parent, GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", msg);
    gtk_dialog_run(GTK_DIALOG(d));
    gtk_widget_destroy(d);
}

static void show_error(GtkWindow *parent, const char *msg) {
    GtkWidget *d = gtk_message_dialog_new(parent, GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", msg);
    gtk_dialog_run(GTK_DIALOG(d));
    gtk_widget_destroy(d);
}

GtkWidget *global_admin_window = NULL;

// ==================== FILE OPERATIONS ====================

char* generate_equipment_id(void) {
    static char id[20];
    static int counter = 0;
   
    if (counter == 0) {
        FILE *f = fopen("equipements.txt", "r");
        if (f != NULL) {
            char line[512];
            while (fgets(line, sizeof(line), f)) {
                counter++;
            }
            fclose(f);
        }
    }
   
    counter++;
    sprintf(id, "EQ%05d", counter);
    return id;
}

int save_equipment(Equipement eq) {
    FILE *f = fopen("equipements.txt", "a");
    if (f == NULL) {
        g_print("Error: Cannot open file for writing\n");
        return 0;
    }
   
    fprintf(f, "%s|%s|%s|%s|%s|%d|%d\n",
            eq.id, eq.nom, eq.type, eq.quantite, eq.date_ajout, eq.etat, eq.garantie);
   
    fclose(f);
    return 1;
}

int load_all_equipment(Equipement equipements[], int max_size) {
    FILE *f = fopen("equipements.txt", "r");
    if (f == NULL) {
        return 0;
    }
   
    char line[512];
    int count = 0;
   
    while (fgets(line, sizeof(line), f) && count < max_size) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d",
                   equipements[count].id,
                   equipements[count].nom,
                   equipements[count].type,
                   equipements[count].quantite,
                   equipements[count].date_ajout,
                   &equipements[count].etat,
                   &equipements[count].garantie) == 7) {
            count++;
        }
    }
   
    fclose(f);
    return count;
}

int delete_equipment_by_id(const char *id) {
    Equipement equipements[1000];
    int count = load_all_equipment(equipements, 1000);
   
    FILE *f = fopen("equipements.txt", "w");
    if (f == NULL) {
        return 0;
    }
   
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(equipements[i].id, id) != 0) {
            fprintf(f, "%s|%s|%s|%s|%s|%d|%d\n",
                    equipements[i].id,
                    equipements[i].nom,
                    equipements[i].type,
                    equipements[i].quantite,
                    equipements[i].date_ajout,
                    equipements[i].etat,
                    equipements[i].garantie);
        } else {
            found = 1;
        }
    }
   
    fclose(f);
    return found;
}

int modify_equipment(Equipement eq) {
    Equipement equipements[1000];
    int count = load_all_equipment(equipements, 1000);
   
    FILE *f = fopen("equipements.txt", "w");
    if (f == NULL) {
        return 0;
    }
   
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(equipements[i].id, eq.id) == 0) {
            fprintf(f, "%s|%s|%s|%s|%s|%d|%d\n",
                    eq.id, eq.nom, eq.type, eq.quantite, eq.date_ajout, eq.etat, eq.garantie);
            found = 1;
        } else {
            fprintf(f, "%s|%s|%s|%s|%s|%d|%d\n",
                    equipements[i].id,
                    equipements[i].nom,
                    equipements[i].type,
                    equipements[i].quantite,
                    equipements[i].date_ajout,
                    equipements[i].etat,
                    equipements[i].garantie);
        }
    }
   
    fclose(f);
    return found;
}

Equipement* find_equipment_by_id(const char *id) {
    static Equipement eq;
    Equipement equipements[1000];
    int count = load_all_equipment(equipements, 1000);
   
    for (int i = 0; i < count; i++) {
        if (strcmp(equipements[i].id, id) == 0) {
            eq = equipements[i];
            return &eq;
        }
    }
   
    return NULL;
}

// ==================== TREEVIEW FUNCTIONS ====================

void setup_treeview(GtkWidget *treeview) {
    GtkListStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
   
    store = gtk_list_store_new(7,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);
   
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
   
    // Remove existing columns
    while (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) != NULL) {
        GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
        if (columns) {
            column = GTK_TREE_VIEW_COLUMN(columns->data);
            gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), column);
            g_list_free(columns);
        } else {
            break;
        }
    }
   
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
   
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
   
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 2, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
   
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Quantité", renderer, "text", 3, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
   
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Date", renderer, "text", 4, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
   
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("État", renderer, "text", 5, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
   
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Garantie", renderer, "text", 6, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
   
    g_signal_connect(treeview, "row-activated", G_CALLBACK(on_treeview_row_activated), NULL);
}

void add_equipment_to_treeview(GtkWidget *treeview, Equipement eq) {
    GtkListStore *store;
    GtkTreeIter iter;
   
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
   
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                      0, eq.id,
                      1, eq.nom,
                      2, eq.type,
                      3, eq.quantite,
                      4, eq.date_ajout,
                      5, eq.etat ? "Neuf" : "Ancien",
                      6, eq.garantie ? "Avec" : "Sans",
                      -1);
}

void populate_treeview(GtkWidget *treeview) {
    Equipement equipements[1000];
    int count = load_all_equipment(equipements, 1000);
   
    clear_treeview(treeview);
   
    for (int i = 0; i < count; i++) {
        add_equipment_to_treeview(treeview, equipements[i]);
    }
}

void clear_treeview(GtkWidget *treeview) {
    GtkListStore *store;
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    gtk_list_store_clear(store);
}

void refresh_treeview(void) {
    if (global_admin_window != NULL) {
        GtkWidget *treeview = lookup_widget(global_admin_window, "treeview_equipements");
        if (treeview) {
            populate_treeview(treeview);
        }
    }
}

void on_treeview_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data) {
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *id;
   
    model = gtk_tree_view_get_model(treeview);
   
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(model, &iter, 0, &id, -1);
        g_print("Selected equipment ID: %s\n", id);
        g_free(id);
    }
}


void on_window_destroy(GtkWidget *widget, gpointer user_data) {
    gtk_main_quit();
}
void on_button_continuer_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *radio_admin;
    GtkWidget *main_win;
    GtkWidget *notebook;
   
    printf("=== BUTTON CLICKED ===\n");
    fflush(stdout);
   
    radio_admin = lookup_widget(GTK_WIDGET(button), "radiobutton_admin");
    printf("radio_admin: %p\n", radio_admin);
    
    main_win = create_Gestion_des_equipements_();
    GtkWidget *win_admin =  create_Gestion_des_equipements_();
 gtk_widget_show(win_admin);
g_win_admin = win_admin;   
    
   
    printf("main_win created: %p\n", main_win);
   
    if (!main_win) {
        printf("ERROR: Cannot create main window!\n");
        return;
    }
   
    notebook = lookup_widget(main_win, "notebook1");
    printf("notebook: %p\n", notebook);
   
    if (!notebook) {
        printf("ERROR: Cannot find notebook\n");
        return;
    }
   
    if (radio_admin && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_admin))) {
        printf("Admin mode selected\n");
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
        global_admin_window = main_win;
        GtkWidget *treeview = lookup_widget(main_win, "treeview_equipements");
        if (treeview) {
            setup_treeview(treeview);
            populate_treeview(treeview);
        }
    } else {
        printf("Trainer mode selected\n");
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
    }
   
    GtkWidget *first_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_hide(first_window);
    gtk_widget_show_all(main_win);
    printf("Windows switched!\n");
}


// ==================== ADMINISTRATOR CALLBACKS ====================

void on_button_chercher_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry;
    GtkWidget *treeview;
    const gchar *search_text;
    Equipement equipements[1000];
    int count;
   
    entry = lookup_widget(GTK_WIDGET(button), "entry_chercher");
    treeview = lookup_widget(GTK_WIDGET(button), "treeview_equipements");
   
    if (!entry || !treeview) {
        g_print("Error: Cannot find search entry or treeview\n");
        return;
    }
   
    search_text = gtk_entry_get_text(GTK_ENTRY(entry));
   
    if (strlen(search_text) == 0) {
        populate_treeview(treeview);
        return;
    }
   
    count = load_all_equipment(equipements, 1000);
    clear_treeview(treeview);
   
    for (int i = 0; i < count; i++) {
        if (strstr(equipements[i].nom, search_text) != NULL ||
            strstr(equipements[i].type, search_text) != NULL ||
            strstr(equipements[i].id, search_text) != NULL) {
            add_equipment_to_treeview(treeview, equipements[i]);
        }
    }
}

void on_button_ajouter_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *add_window = create_window_d_ajout();
    gtk_widget_show(add_window);
}

void on_button_modif_clicked(GtkButton *button, gpointer user_data) {
     GtkWidget *win_admin = lookup_widget(GTK_WIDGET(button), "Gestion_des_equipements_");

    GtkWidget *entryId = lookup_widget(GTK_WIDGET(button), "entry_id");
    if (!entryId) {
        show_error(GTK_WINDOW(win_admin), "entry_id introuvable !");
        return;
    }

    const char *id_txt = gtk_entry_get_text(GTK_ENTRY(entryId));
    if (!id_txt || strlen(id_txt) == 0 || atoi(id_txt) <= 0) {
        show_error(GTK_WINDOW(win_admin), "Saisis un ID valide avant de modifier !");
        return;
    }

    /* ✅ stocker l'ID pour l'utiliser dans modifier1 */
    strncpy(g_id_to_modify, id_txt, sizeof(g_id_to_modify)-1);
    g_id_to_modify[sizeof(g_id_to_modify)-1] = '\0';

    /* هنا تفتح نافذة التعديل (كودك أنت) */
    GtkWidget *w = create_window_de_modification();
    gtk_widget_show(w);
    GtkWidget *treeview;
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *id;
   
    treeview = lookup_widget(GTK_WIDGET(button), "treeview_equipements");
    if (!treeview) {
        g_print("Error: Cannot find treeview\n");
        return;
    }
   
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
   
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &id, -1);
       
        Equipement *eq = find_equipment_by_id(id);
        if (eq) {
            GtkWidget *modify_window = create_window_de_modification();
           
            g_object_set_data_full(G_OBJECT(modify_window), "equipment_id", g_strdup(id), g_free);
           
            gtk_widget_show(modify_window);
        }
        g_free(id);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez sélectionner un équipement à modifier");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_button_supprimer_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *treeview;
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *id;
   
    treeview = lookup_widget(GTK_WIDGET(button), "treeview_equipements");
    if (!treeview) return;
   
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
   
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &id, -1);
       
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_QUESTION,
                                                   GTK_BUTTONS_YES_NO,
                                                   "Voulez-vous vraiment supprimer cet équipement?");
       
        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES) {
            if (delete_equipment_by_id(id)) {
                refresh_treeview();
                g_print("Equipment deleted successfully\n");
            }
        }
       
        gtk_widget_destroy(dialog);
        g_free(id);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez sélectionner un équipement à supprimer");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_button5_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry = lookup_widget(GTK_WIDGET(button), "entry_chercher");
    if (entry) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    }
    refresh_treeview();
}

// ==================== ADD EQUIPMENT ====================

void on_button_ajouter1_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_id, *combo_type, *spin_quantite;
    GtkWidget *radio_neuf, *check_garantie;
    GtkWidget *spin_jour, *spin_mois, *spin_ans;
    Equipement eq;
   
    entry_id = lookup_widget(GTK_WIDGET(button), "entry_id_ajouter");
    combo_type = lookup_widget(GTK_WIDGET(button), "combobox_type");
    spin_quantite = lookup_widget(GTK_WIDGET(button), "spinbutton_quantit___ajout__");
    radio_neuf = lookup_widget(GTK_WIDGET(button), "radiobutton_neuf");
    check_garantie = lookup_widget(GTK_WIDGET(button), "checkbutton_avecgarantie");
    spin_jour = lookup_widget(GTK_WIDGET(button), "spinbutton_jour");
    spin_mois = lookup_widget(GTK_WIDGET(button), "spinbutton_mois");
    spin_ans = lookup_widget(GTK_WIDGET(button), "spinbutton_ans");
   
    if (!entry_id || !combo_type || !spin_quantite) {
        g_print("Error: Cannot find form fields\n");
        return;
    }
   
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entry_id));
   
    if (strlen(id) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez remplir tous les champs");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
   
    strcpy(eq.id, id);
    strcpy(eq.nom, id);
   
    gchar *type_text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_type));
    if (type_text) {
        strcpy(eq.type, type_text);
        g_free(type_text);
    } else {
        strcpy(eq.type, "Non spécifié");
    }
   
    sprintf(eq.quantite, "%d", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_quantite)));
   
    int jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_jour));
    int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_mois));
    int ans = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_ans));
    sprintf(eq.date_ajout, "%02d/%02d/%d", jour, mois, ans);
   
    eq.etat = radio_neuf ? gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_neuf)) : 0;
    eq.garantie = check_garantie ? gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_garantie)) : 0;
   
    if (save_equipment(eq)) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Équipement ajouté avec succès!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
       
        refresh_treeview();
       
        GtkWidget *window = lookup_widget(GTK_WIDGET(button), "window_d_ajout");
        if (window) gtk_widget_destroy(window);
    }
}

void on_button_annulerlajout_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "window_d_ajout");
    if (window) gtk_widget_destroy(window);
}

// ==================== MODIFY EQUIPMENT ====================
/*
void on_button_modifier1_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *entry_nom, *entry_type, *spin_quantite;
    GtkWidget *radio_neuf;
    const gchar *id;
    Equipement eq;
   
    window = lookup_widget(GTK_WIDGET(button), "window_de_modification");
    id = g_object_get_data(G_OBJECT(window), "equipment_id");
   
    if (!id) {
        g_print("Error: No equipment ID found\n");
        return;
    }
   
    entry_nom = lookup_widget(GTK_WIDGET(button), "entry_nom_modifier");
    entry_type = lookup_widget(GTK_WIDGET(button), "entry_type_modifier");
    spin_quantite = lookup_widget(GTK_WIDGET(button), "spinbutton_quantite_modifier");
    radio_neuf = lookup_widget(GTK_WIDGET(button), "radiobutton_neuf_modif");
   
    Equipement *old_eq = find_equipment_by_id(id);
    if (old_eq) {
        strcpy(eq.id, old_eq->id);
        strcpy(eq.nom, gtk_entry_get_text(GTK_ENTRY(entry_nom)));
        strcpy(eq.type, gtk_entry_get_text(GTK_ENTRY(entry_type)));
        sprintf(eq.quantite, "%d", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_quantite)));
        strcpy(eq.date_ajout, old_eq->date_ajout);
        eq.etat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_neuf)) ? 1 : 0;
        eq.garantie = old_eq->garantie;
       
        if (modify_equipment(eq)) {
            GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_INFO,
                                                       GTK_BUTTONS_OK,
                                                       "Équipement modifié avec succès!");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
           
            refresh_treeview();
            gtk_widget_destroy(window);
        }
    }
}

void on_button_modifier1_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *e_nom  = lookup_widget(GTK_WIDGET(button), "entrynom_equip_modif");
    GtkWidget *cb_type = lookup_widget(GTK_WIDGET(button), "combobox_type_modif");
    GtkWidget *spin   = lookup_widget(GTK_WIDGET(button), "spinbutton_quantite_modif");

    if(!e_nom || !cb_type || !spin){
        g_print("Erreur lookup_widget: e_nom=%p cb_type=%p spin=%p\n", e_nom, cb_type, spin);
        return;
    }

    const char *nom = gtk_entry_get_text(GTK_ENTRY(e_nom));

    gchar *type = gtk_combo_box_get_active_text(GTK_COMBO_BOX(cb_type)); // GTK2 OK
    int quantite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin));

    g_print("MODIF -> nom=%s type=%s quantite=%d\n", nom, type ? type : "(null)", quantite);

    if(type) g_free(type);
}

void on_button_modifier1_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *win = lookup_widget(GTK_WIDGET(button), "window_de_modification");

    GtkWidget *entryNom = lookup_widget(GTK_WIDGET(button), "entrynom_equip_modif");
    GtkWidget *comboType = lookup_widget(GTK_WIDGET(button), "combobox_type_modif");
    GtkWidget *spinQte = lookup_widget(GTK_WIDGET(button), "spinbutton_quantite_modif");

    if(!entryNom || !comboType || !spinQte) {
        show_error(GTK_WINDOW(win), "Erreur: widgets introuvables (noms Glade incorrects).");
        return;
    }

    const char *nom = gtk_entry_get_text(GTK_ENTRY(entryNom));
    gchar *type = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboType));
    int qte = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinQte));

    int id = /* IMPORTANT: get selected equipment ID here (unique) ;

    if(id <= 0) { show_error(GTK_WINDOW(win), "Choisir un équipement (ID) avant de modifier."); g_free(type); return; }

    int ok = modify_equipment(id, nom, type, qte);
    g_free(type);

    if(ok) {
        show_info(GTK_WINDOW(win), "✅ Modification effectuée avec succès !");
        // refresh treeview
        GtkWidget *tree = lookup_widget(GTK_WIDGET(button), "treeview_equipements");
        // afficher_equipements(tree);  // your refresh function
    } else {
        show_error(GTK_WINDOW(win), "❌ Aucun équipement trouvé avec cet ID.");
    }
}
*/
/*void on_button_modifier1_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *win = lookup_widget(GTK_WIDGET(button), "window_de_modification");
    if (!win) return;

    // ⚠️ Mets ici les VRAIS noms Glade
    GtkWidget *entryId   = lookup_widget(win, "entryid_equip_modif");
    GtkWidget *entryNom  = lookup_widget(win, "entrynom_equip_modif");
    GtkWidget *wType     = lookup_widget(win, "entrytype_equip_modif");          // ou combobox
    GtkWidget *spinQte   = lookup_widget(win, "spinbutton_quantite_modif");

    if(!entryId || !entryNom || !wType || !spinQte) {
        show_error(GTK_WINDOW(win), "Erreur: widgets introuvables (noms Glade incorrects).");
        return;
    }

    const char *id_txt  = gtk_entry_get_text(GTK_ENTRY(entryId));
    const char *nom_txt = gtk_entry_get_text(GTK_ENTRY(entryNom));

    if(!id_txt || strlen(id_txt) == 0) {
        show_error(GTK_WINDOW(win), "ID vide !");
        return;
    }

    int id_int = atoi(id_txt);
    if(id_int <= 0) {
        show_error(GTK_WINDOW(win), "ID invalide !");
        return;
    }

    // ---- construire Equipement eq ----
    Equipement eq;
    memset(&eq, 0, sizeof(eq));

    // si eq.id est char[]
    strncpy(eq.id, id_txt, sizeof(eq.id)-1);

    // nom
    if(!nom_txt || strlen(nom_txt) == 0) {
        show_error(GTK_WINDOW(win), "Nom vide !");
        return;
    }
    strncpy(eq.nom, nom_txt, sizeof(eq.nom)-1);

    // type : Entry OU ComboBox
    if (GTK_IS_ENTRY(wType)) {
        const char *type_txt = gtk_entry_get_text(GTK_ENTRY(wType));
        if(!type_txt || strlen(type_txt) == 0) {
            show_error(GTK_WINDOW(win), "Type vide !");
            return;
        }
        strncpy(eq.type, type_txt, sizeof(eq.type)-1);
    } 
    else if (GTK_IS_COMBO_BOX(wType)) {
        gchar *type_txt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(wType));
        if(!type_txt) {
            show_error(GTK_WINDOW(win), "Choisis un type !");
            return;
        }
        strncpy(eq.type, type_txt, sizeof(eq.type)-1);
        g_free(type_txt);
    } 
    else {
        show_error(GTK_WINDOW(win), "Widget Type n'est ni Entry ni ComboBox !");
        return;
    }

    // quantite (si eq.quantite est char[])
    int qte_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinQte));
    if(qte_int < 0) {
        show_error(GTK_WINDOW(win), "Quantité invalide !");
        return;
    }
    snprintf(eq.quantite, sizeof(eq.quantite), "%d", qte_int);

    // ---- appel modification ----
    int ok = modify_equipment(eq);

    if(ok)
        show_info(GTK_WINDOW(win), "✅ Modification effectuée avec succès !");
    else
        show_error(GTK_WINDOW(win), "❌ ID non trouvé, aucune modification.");
}
/*
void on_button_modifier1_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *win = lookup_widget(GTK_WIDGET(button), "window_de_modification");

    GtkWidget *entryId   = lookup_widget(GTK_WIDGET(button), "entry_id_modifier");
    GtkWidget *entryNom  = lookup_widget(GTK_WIDGET(button), "entry_nom_modifier");
    GtkWidget *entryType = lookup_widget(GTK_WIDGET(button), "entry_type_modifier");
    GtkWidget *spinQte   = lookup_widget(GTK_WIDGET(button), "spinbutton_quantite_modifier");

    if(!entryId || !entryNom || !entryType || !spinQte) {
        show_error(GTK_WINDOW(win), "Erreur: widgets introuvables (noms Glade incorrects).");
        return;
    }

    Equipement eq;

    strcpy(eq.id, gtk_entry_get_text(GTK_ENTRY(entryId)));

sprintf(eq.quantite, "%d",
        gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinQte)));

    if(eq.id <= 0) {
        show_error(GTK_WINDOW(win), "ID invalide !");
        return;
    }

    int ok = modify_equipment(eq);

    if(ok)
        show_info(GTK_WINDOW(win), "✅ Modification effectuée avec succès !");
    else
        show_error(GTK_WINDOW(win), "❌ ID non trouvé, aucune modification.");
}
*/
/*
void on_button_modifier1_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *win = lookup_widget(GTK_WIDGET(button), "window_de_modification");
    if(!win) return;

    // ✅ NOMS EXACTS D’APRÈS TON CODE GLADE
    GtkWidget *entryNom  = lookup_widget(win, "entrynom_equip_modif");
    GtkWidget *comboType = lookup_widget(win, "combobox_type_modif");
    GtkWidget *spinQte   = lookup_widget(win, "spinbutton_quantite_modif");

    if(!entryNom || !comboType || !spinQte) {
        show_error(GTK_WINDOW(win), "Erreur: widgets introuvables (vérifie les noms dans Glade).");
        return;
    }

    const char *nom_txt = gtk_entry_get_text(GTK_ENTRY(entryNom));
    if(!nom_txt || strlen(nom_txt) == 0) {
        show_error(GTK_WINDOW(win), "Nom vide !");
        return;
    }

    gchar *type_txt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboType));
    if(!type_txt) {
        show_error(GTK_WINDOW(win), "Choisis un type !");
        return;
    }

    int qte_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinQte));
    if(qte_int < 0) {
        g_free(type_txt);
        show_error(GTK_WINDOW(win), "Quantité invalide !");
        return;
    }

    Equipement eq;
    strncpy(eq.type,type_buf, sizeof(eq.type)-1);

    // ⚠️ Ici on modifie selon LE NOM (pas idéal si noms dupliqués)
    // Mieux: modifier par ID depuis treeview (je te donne ça si tu veux)
    strncpy(eq.nom, nom_txt, sizeof(eq.nom)-1);
    strncpy(eq.type, type_txt, sizeof(eq.type)-1);
    snprintf(eq.quantite, sizeof(eq.quantite), "%d", qte_int);

    g_free(type_txt);

    int ok = modify_equipment(eq);

    if(ok)
        show_info(GTK_WINDOW(win), "✅ Modification effectuée avec succès !");
    else
        show_error(GTK_WINDOW(win), "❌ Aucune modification (équipement non trouvé).");
}
*/
void on_button_modifier1_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *win = lookup_widget(GTK_WIDGET(button), "window_de_modification");

    GtkWidget *entryNom  = lookup_widget(GTK_WIDGET(button), "entrynom_equip_modif");
    GtkWidget *comboType = lookup_widget(GTK_WIDGET(button), "combobox_type_modif");
    GtkWidget *spinQte   = lookup_widget(GTK_WIDGET(button), "spinbutton_quantite_modif");

    if (!win || !entryNom || !comboType || !spinQte) {
        show_error(GTK_WINDOW(win), "Erreur: widgets de modification introuvables !");
        return;
    }

    /* ✅ ID depuis la fenêtre admin (entry_id) */
    if (!global_admin_window) {
        show_error(GTK_WINDOW(win), "Erreur: fenêtre admin introuvable !");
        return;
    }

    GtkWidget *entryIdMain = lookup_widget(global_admin_window, "entry_id");
    if (!entryIdMain) {
        show_error(GTK_WINDOW(win), "Erreur: widget entry_id introuvable !");
        return;
    }

    const char *id_txt = gtk_entry_get_text(GTK_ENTRY(entryIdMain));
    if (!id_txt || strlen(id_txt) == 0 || atoi(id_txt) <= 0) {
        show_error(GTK_WINDOW(win), "Saisis l'ID (entry_id) avant de modifier !");
        return;
    }

    /* Nom */
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entryNom));
    if (!nom || strlen(nom) == 0) {
        show_error(GTK_WINDOW(win), "Nom invalide !");
        return;
    }

    /* Quantité */
    int qte = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinQte));
    if (qte <= 0) {
        show_error(GTK_WINDOW(win), "Quantité invalide !");
        return;
    }

    /* Type : récupérer depuis GtkComboBox (pas ComboBoxText) */
    char type_buf[100] = "";
    GtkTreeIter iter;

    if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(comboType), &iter)) {
        GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(comboType));
        gchar *type_txt = NULL;

        gtk_tree_model_get(model, &iter, 0, &type_txt, -1);
        if (type_txt) {
            strncpy(type_buf, type_txt, sizeof(type_buf) - 1);
            type_buf[sizeof(type_buf) - 1] = '\0';
            g_free(type_txt);
        }
    } else {
        show_error(GTK_WINDOW(win), "Choisis un type !");
        return;
    }

    /* Construire Equipement */
    Equipement eq;
    memset(&eq, 0, sizeof(eq));

    strncpy(eq.id, id_txt, sizeof(eq.id) - 1);
    eq.id[sizeof(eq.id) - 1] = '\0';

    strncpy(eq.nom, nom, sizeof(eq.nom) - 1);
    eq.nom[sizeof(eq.nom) - 1] = '\0';

    strncpy(eq.type, type_buf, sizeof(eq.type) - 1);
    eq.type[sizeof(eq.type) - 1] = '\0';

    snprintf(eq.quantite, sizeof(eq.quantite), "%d", qte);

    /* ✅ Modifier + refresh table */
    int ok = modify_equipment(eq);

    if (ok) {
        show_info(GTK_WINDOW(win), "✅ Modification effectuée avec succès !");
        refresh_treeview();              /* ✅ met à jour le tableau */
    } else {
        show_error(GTK_WINDOW(win), "❌ ID non trouvé, aucune modification.");
    }
}
void on_button_annulermodif_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "window_de_modification");
    if (window) gtk_widget_destroy(window);
}

// ==================== DELETE EQUIPMENT ====================

void on_button_supprimer1_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_recherche;
    const gchar *search_text;
   
    entry_recherche = lookup_widget(GTK_WIDGET(button), "entry_recherche_supprimer");
   
    if (!entry_recherche) {
        g_print("Error: Cannot find search entry\n");
        return;
    }
   
    search_text = gtk_entry_get_text(GTK_ENTRY(entry_recherche));
   
    if (strlen(search_text) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez entrer un ID d'équipement");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
   
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "Voulez-vous vraiment supprimer cet équipement?");
   
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES) {
        if (delete_equipment_by_id(search_text)) {
            refresh_treeview();
           
            GtkWidget *success = gtk_message_dialog_new(NULL,
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_INFO,
                                                       GTK_BUTTONS_OK,
                                                       "Équipement supprimé avec succès!");
            gtk_dialog_run(GTK_DIALOG(success));
            gtk_widget_destroy(success);
           
            GtkWidget *window = lookup_widget(GTK_WIDGET(button), "window de suppresion");
            if (window) gtk_widget_destroy(window);
        }
    }
   
    gtk_widget_destroy(dialog);
}

void on_button_annulersupprimer_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "window de suppresion");
    if (window) gtk_widget_destroy(window);
}

void on_button_recherchersupprimer_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_recherche;
    const gchar *search_text;
   
    entry_recherche = lookup_widget(GTK_WIDGET(button), "entry_recherche_supprimer");
   
    if (!entry_recherche) return;
   
    search_text = gtk_entry_get_text(GTK_ENTRY(entry_recherche));
   
    Equipement *eq = find_equipment_by_id(search_text);
   
    if (eq) {
        g_print("Found equipment: %s - %s\n", eq->id, eq->nom);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Équipement non trouvé!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// ==================== RESERVE EQUIPMENT ====================

void on_button_continuer2_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_nom_equip, *entry_cours;
    GtkWidget *combo_nom_equip;
    const gchar *nom_equip, *cours;
   
    entry_nom_equip = lookup_widget(GTK_WIDGET(button), "entrynom_reserver");
    entry_cours = lookup_widget(GTK_WIDGET(button), "entry4");
    combo_nom_equip = lookup_widget(GTK_WIDGET(button), "combobox_nom_equip");
   
    if (!entry_nom_equip || !entry_cours) {
        g_print("Error: Cannot find form fields\n");
        return;
    }
   
    nom_equip = gtk_entry_get_text(GTK_ENTRY(entry_nom_equip));
    cours = gtk_entry_get_text(GTK_ENTRY(entry_cours));
   
    if (strlen(nom_equip) == 0 || strlen(cours) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez remplir tous les champs");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
   
    FILE *f = fopen("reservations.txt", "a");
    if (f != NULL) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(f, "%s|%s|%02d/%02d/%d\n", nom_equip, cours, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
        fclose(f);
       
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Réservation effectuée avec succès!");
gtk_dialog_run(GTK_DIALOG(dialog));
gtk_widget_destroy(dialog);
}
}
void on_button_reserver_clicked(GtkButton *button, gpointer user_data) {
GtkWidget *entry_entraineur, *entry_equipement;
const gchar *entraineur, *equipement;
entry_entraineur = lookup_widget(GTK_WIDGET(button), "entry_nom_entraineur");
entry_equipement = lookup_widget(GTK_WIDGET(button), "entry_nom_equipement_reserver");

if (!entry_entraineur || !entry_equipement) {
    g_print("Error: Cannot find form fields\n");
    return;
}

entraineur = gtk_entry_get_text(GTK_ENTRY(entry_entraineur));
equipement = gtk_entry_get_text(GTK_ENTRY(entry_equipement));

if (strlen(entraineur) == 0 || strlen(equipement) == 0) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_WARNING,
                                               GTK_BUTTONS_OK,
                                               "Veuillez remplir tous les champs");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
}

FILE *f = fopen("reservations.txt", "a");
if (f != NULL) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(f, "%s|%s|%02d/%02d/%d\n", entraineur, equipement, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    fclose(f);
   
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Réservation effectuée avec succès!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
   
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "window de reservation");
    if (window) gtk_widget_destroy(window);
}
}
void on_button_annulerreserver_clicked(GtkButton *button, gpointer user_data) {
GtkWidget *window = lookup_widget(GTK_WIDGET(button), "window de reservation");
if (window) gtk_widget_destroy(window);
}

