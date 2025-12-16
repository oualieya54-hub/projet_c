#include <gtk/gtk.h>
#include <stdlib.h> // For malloc and free
#include <string.h> // For strcmp

#include "common/support.h"
#include "auth.h"
#include "member/member_integration.h"
#include "trainer/trainer_integration.h"
#include "course/course_integration.h"
#include "salle/salle_interface.h"
#include "salle/salle.h"
#include "salle/salle_callbacks.h"

// Struct to hold pointers to the widgets we need in the callback
typedef struct {
    GtkWidget *entry_user;
    GtkWidget *entry_pass;
    GtkWidget *label_status;
    gboolean login_successful; // Flag to manage application lifecycle
} LoginWidgets;

// Forward declarations
void open_login_window(void);
void open_main_hub_window(const char* role, const char* username);
void on_btn_login_clicked(GtkButton *button, gpointer user_data);
void on_login_window_destroy(GtkWidget *widget, gpointer user_data);
void on_logout_button_clicked(GtkButton *button, gpointer user_data);

// Role-specific button callbacks
void on_admin_manage_members_clicked(GtkButton *button, gpointer user_data) { open_member_window(); }
void on_admin_manage_trainers_clicked(GtkButton *button, gpointer user_data) { open_trainer_window(); }
void on_admin_manage_courses_clicked(GtkButton *button, gpointer user_data) { open_course_window(); }
void on_admin_manage_gyms_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = create_GestionSalle();
    GtkWidget *treeview = lookup_widget(window, "treeview3");
    if (treeview) {
        afficher_salles_interface(treeview, "data/salle.txt");
    }
    gtk_widget_show(window);
}
void on_coach_register_course_clicked(GtkButton *button, gpointer user_data) { open_trainer_course_registration_window(); }
void on_member_request_coach_clicked(GtkButton *button, gpointer user_data) { open_private_coach_window(); }
void on_member_view_subscriptions_clicked(GtkButton *button, gpointer user_data) { open_subscriptions_window(); }
void on_member_register_course_clicked(GtkButton *button, gpointer user_data) { open_course_registration_window(); }
void on_member_register_gym_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = create_inscritsalle();
    salle_remplir_listes_inscription_salle(window);
    gtk_widget_show(window);
}


int main(int argc, char *argv[]) {
    gtk_set_locale();
    gtk_init(&argc, &argv);

    add_pixmap_directory("pixmaps");
    
    open_login_window(); // Start with the login window
    
    gtk_main();

    return 0;
}

void open_login_window(void) {
    GtkBuilder *builder;
    GtkWidget *login_window;
    GtkWidget *btn_login;
    LoginWidgets *widgets = g_malloc(sizeof(LoginWidgets));
    widgets->login_successful = FALSE;

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ui/login_window.glade", NULL);
    
    login_window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
    
    widgets->entry_user = GTK_WIDGET(gtk_builder_get_object(builder, "entry_username"));
    widgets->entry_pass = GTK_WIDGET(gtk_builder_get_object(builder, "entry_password"));
    widgets->label_status = GTK_WIDGET(gtk_builder_get_object(builder, "label_status"));
    btn_login = GTK_WIDGET(gtk_builder_get_object(builder, "btn_login"));

    g_object_unref(builder); 

    g_signal_connect(btn_login, "clicked", G_CALLBACK(on_btn_login_clicked), widgets);
    g_signal_connect(login_window, "destroy", G_CALLBACK(on_login_window_destroy), widgets);

    gtk_widget_show_all(login_window);
}

void on_login_window_destroy(GtkWidget *widget, gpointer user_data) {
    LoginWidgets *widgets = (LoginWidgets *)user_data;
    if (!widgets->login_successful) {
        gtk_main_quit();
    }
    g_free(widgets);
}

void on_btn_login_clicked(GtkButton *button, gpointer user_data) {
    LoginWidgets *widgets = (LoginWidgets *)user_data;
    
    const char *username = gtk_entry_get_text(GTK_ENTRY(widgets->entry_user));
    const char *password = gtk_entry_get_text(GTK_ENTRY(widgets->entry_pass));
    const char *role = authenticate_user(username, password);

    if (role) {
        widgets->login_successful = TRUE;
        open_main_hub_window(role, username);
        GtkWidget *login_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
        gtk_widget_destroy(login_window);
    } else {
        gtk_label_set_text(GTK_LABEL(widgets->label_status), "Invalid username or password.");
    }
}

void on_logout_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *hub_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(hub_window);
    open_login_window();
}

void open_main_hub_window(const char* role, const char* username) {
    GtkBuilder *builder = gtk_builder_new();
    GtkWidget *window;
    GtkWidget *btn_logout;
    char greeting[100];
    
    if (strcmp(role, "Admin") == 0) {
        gtk_builder_add_from_file(builder, "ui/main_window.glade", NULL);
        window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
        sprintf(greeting, "Hello, %s!", username);
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "label_hello_admin")), greeting);

        g_signal_connect(gtk_builder_get_object(builder, "btn_manage_members"), "clicked", G_CALLBACK(on_admin_manage_members_clicked), NULL);
        g_signal_connect(gtk_builder_get_object(builder, "btn_manage_trainers"), "clicked", G_CALLBACK(on_admin_manage_trainers_clicked), NULL);
        g_signal_connect(gtk_builder_get_object(builder, "btn_manage_courses"), "clicked", G_CALLBACK(on_admin_manage_courses_clicked), NULL);
        g_signal_connect(gtk_builder_get_object(builder, "btn_manage_salle"), "clicked", G_CALLBACK(on_admin_manage_gyms_clicked), NULL);

    } else if (strcmp(role, "Coach") == 0) {
        gtk_builder_add_from_file(builder, "ui/coach_hub.glade", NULL);
        window = GTK_WIDGET(gtk_builder_get_object(builder, "coach_hub_window"));
        sprintf(greeting, "Hello, %s!", username);
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "label_hello_coach")), greeting);

        g_signal_connect(gtk_builder_get_object(builder, "btn_coach_course_registration"), "clicked", G_CALLBACK(on_coach_register_course_clicked), NULL);

    } else if (strcmp(role, "Membre") == 0) {
        gtk_builder_add_from_file(builder, "ui/member_hub.glade", NULL);
        window = GTK_WIDGET(gtk_builder_get_object(builder, "member_hub_window"));
        sprintf(greeting, "Hello, %s!", username);
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "label_hello_member")), greeting);

        g_signal_connect(gtk_builder_get_object(builder, "btn_member_request_coach"), "clicked", G_CALLBACK(on_member_request_coach_clicked), NULL);
        g_signal_connect(gtk_builder_get_object(builder, "btn_member_view_subscriptions"), "clicked", G_CALLBACK(on_member_view_subscriptions_clicked), NULL);
        g_signal_connect(gtk_builder_get_object(builder, "btn_member_register_course"), "clicked", G_CALLBACK(on_member_register_course_clicked), NULL);
        g_signal_connect(gtk_builder_get_object(builder, "btn_inscrit_salle"), "clicked", G_CALLBACK(on_member_register_gym_clicked), NULL);
        
    } else {
        g_object_unref(builder);
        return;
    }

    // Connect the logout button for all roles
    btn_logout = GTK_WIDGET(gtk_builder_get_object(builder, "btn_logout"));
    g_signal_connect(btn_logout, "clicked", G_CALLBACK(on_logout_button_clicked), NULL);

    g_object_unref(builder);
    gtk_widget_show_all(window);
}

