#include "gui/gui.h"

void init_gui(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkBuilder *builder = load_ui("ui/UI.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    load_book_to_layout(builder, 1);
    gtk_builder_connect_signals(builder, NULL);

    popup_menu_init(builder);
    gtk_widget_show_all(window);

    gtk_main();
}
