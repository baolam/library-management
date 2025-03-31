#include "gui.h"
#include "callbacks.h"

GtkBuilder *builder = NULL;
GtkWidget *window;

void init_gui(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    /// Thêm từng file builder
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ui/library.glade", NULL);
    // gtk_builder_add_from_file(builder, "ui/search-book.ui", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show_all(window);
    gtk_main();

    g_object_unref(builder);
}
