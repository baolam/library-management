#include "ui_loader.h"
#include "callbacks.h"
#include "book_treeview.h"
#include "search_toggle.h"
#include "search.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkBuilder *builder = load_ui("UI.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    load_books_from_file(builder, "books.txt");
    
    connect_signals(builder);

    popup_menu_init(builder);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
