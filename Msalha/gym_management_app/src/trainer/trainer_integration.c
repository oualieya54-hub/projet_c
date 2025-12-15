#include "trainer_integration.h"
#include "trainer_interface.h"
#include "../common/support.h"
#include "entraineur.h"
#include "trainer_callbacks.h"

extern GtkWidget *g_window_main;
extern GtkWidget *g_treeview1;
extern GtkWidget *g_window_inscri;


// Callback to reset globals when window is destroyed
static void on_trainer_window_destroy(GtkWidget *widget, gpointer user_data) {
    g_window_main = NULL;
    g_treeview1 = NULL;
}

void open_trainer_window(void) {
    GtkWidget *window_gestion;

    printf("DEBUG: open_trainer_window called, g_window_main = %p\n", g_window_main);

    // If window already exists, just show it and refresh
    if (g_window_main && GTK_IS_WIDGET(g_window_main)) {
        printf("DEBUG: Window already exists, presenting and refreshing\n");
        gtk_window_present(GTK_WINDOW(g_window_main));
        populate_treeview1();
        return;
    }

    printf("DEBUG: Creating new window\n");
    window_gestion = create_Gestion_des_entraineur();
    if (!window_gestion) {
        g_warning("Failed to create trainer management window (Gestion_des_entraineur).");
        return;
    }

    g_window_main = window_gestion;
    g_treeview1 = lookup_widget(window_gestion, "treeview1");
    printf("DEBUG: After lookup_widget, g_treeview1 = %p\n", g_treeview1);

    if (!g_treeview1) {
        g_warning("Could not find treeview 'treeview1' in trainer window.");
    }

    // Connect destroy handler to reset globals
    g_signal_connect(window_gestion, "destroy", G_CALLBACK(on_trainer_window_destroy), NULL);

    // Populate the treeview immediately
    printf("DEBUG: About to populate treeview\n");
    populate_treeview1();

    gtk_widget_show(window_gestion);
}

void open_trainer_course_registration_window(void) {
    GtkWidget *window_inscription = create_Inscription____un_cours_();
    if (window_inscription) {
        g_window_inscri = window_inscription;
        gtk_widget_show(window_inscription);
    } else {
        g_warning("Failed to create trainer course registration window (Inscription____un_cours_).");
    }
}