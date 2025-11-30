#include <gtk/gtk.h>
#include "interface.h"
#include "support.h"

int main(int argc, char *argv[])
{
    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = create_AjoutSalle();
    gtk_widget_show(window);

    gtk_main();
    return 0;
}

