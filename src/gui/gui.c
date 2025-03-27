#include "gui.h"
#include "callbacks.h"

void init_gui(int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("ui/borrow-book.ui");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "Mainwindow"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_main();
}
