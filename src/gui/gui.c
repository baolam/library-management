#include "gui/gui.h"

void init_gui(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = load_ui("ui/UI.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    load_book_to_layout(1);
    load_reader_to_layout(1);
    load_borrow_to_layout(1);
    load_statistic_to_layout();
    load_statistic_late_fess();

    gtk_builder_connect_signals(builder, NULL);

    popup_book_menu_init(builder);
    popup_reader_menu_init(builder);
    popup_borrow_menu_init(builder);
    popup_book_borrow_menu_init(builder);

    gtk_widget_show_all(window);

    gtk_main();
}
