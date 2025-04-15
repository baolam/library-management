#include "gui/gui.h"

void init_gui(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = load_ui("ui/UI.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    load_reader_to_layout(1);
    load_book_to_layout(1);

    gtk_builder_connect_signals(builder, NULL);

    popup_book_menu_init(builder);
    popup_reader_menu_init(builder);

    gtk_widget_show_all(window);

    gtk_main();
}
