#include <gtk/gtk.h>
#include "callbacks.h"
#include "interface.h"

int main(int argc, char *argv[]) {
    GtkWidget *window;
   
    gtk_init(&argc, &argv);
   
    window = create_main_window();
   
    if (window) {
        g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);
        gtk_widget_show_all(window);
        gtk_main();
    }
   
    return 0;
}
