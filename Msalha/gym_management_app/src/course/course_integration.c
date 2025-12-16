#include "course_integration.h"
#include "course_interface.h"
#include "../common/support.h"
#include "gcour.h"
#include "course_callbacks.h"


void open_course_window(void) {
    GtkWidget *cour_sportif_acceuil;

    cour_sportif_acceuil = create_cour_sportif_acceuil();
    if (!cour_sportif_acceuil) {
        g_warning("Failed to create course management window (cour_sportif_acceuil).");
        return;
    }

    GtkWidget *treeview = lookup_widget(cour_sportif_acceuil, "treeview1");
    if (treeview) {
        afficher_cours_treeview(treeview);
    } else {
        g_warning("Could not find treeview 'treeview1' in course window.");
    }

    gtk_widget_show(cour_sportif_acceuil);
}

void open_course_registration_window(void) {
    GtkWidget *inscrit_cour = create_inscrit_cour();
    if(inscrit_cour) {
        gtk_widget_show(inscrit_cour);
    } else {
        g_warning("Failed to create course registration window (inscrit_cour).");
    }
}