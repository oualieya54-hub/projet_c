#include <gtk/gtk.h>
#include "interface.h"
#include "support.h"

int main(int argc, char *argv[])
{
    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = create_inscritsalle();
    // ⬇️ Ajoutez cette ligne pour remplir les listes
    remplir_listes_inscription_salle(window);
    gtk_widget_show(window);

    gtk_main();
    return 0;
}

