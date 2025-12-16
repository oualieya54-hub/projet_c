#include "member_integration.h"
#include "member_interface.h"
#include "../common/support.h"
#include "member.h"
#include "member_callbacks.h"

/*
 * This is the extern global variable declared in member_callbacks.c
 * It's needed to link the treeview widget created here with the
 * callback functions that might need to refresh it.
 */
extern GtkWidget *main_treeview;

void open_member_window(void) {
    GtkWidget *window1;

    window1 = create_window1();
    if (!window1) {
        g_warning("Failed to create member management window (window1).");
        return;
    }
    
    gtk_widget_show(window1);

    GtkWidget *treeview = lookup_widget(window1, "gm9");
    if (treeview) {
        main_treeview = treeview; 
        afficher_membre(GTK_TREE_VIEW(treeview));
    } else {
        g_warning("Could not find treeview 'gm9' in member window.");
    }

    GtkWidget *btn_coach = lookup_widget(window1, "button3");
    if (btn_coach) {
        g_signal_connect(btn_coach, "clicked", G_CALLBACK(member_on_button3_clicked), NULL);
    } else {
        g_warning("Could not find 'button3' in member window.");
    }
}

void open_private_coach_window(void) {
    GtkWidget *window7 = create_window7();
    if (window7) {
        gtk_widget_show(window7);
    } else {
        g_warning("Failed to create private coach window (window7).");
    }
}

void open_subscriptions_window(void) {
    GtkWidget *window8 = create_window8();
    if (window8) {
        gtk_widget_show(window8);
    } else {
        g_warning("Failed to create subscriptions window (window8).");
    }
}