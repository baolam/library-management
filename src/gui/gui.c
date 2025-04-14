#include "gui/gui.h"
#include "gui/events/callbacks.h"
#include "gui/events/book_treeview.h"
#include "gui/events/search_toggle.h"
#include "gui/events/search.h"

GtkBuilder *builder = NULL;

void init_gui(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = load_ui("ui/UI.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    load_book_to_layout(1);
    gtk_builder_connect_signals(builder, NULL);

    popup_menu_init(builder);
    gtk_widget_show_all(window);

    gtk_main();
}
