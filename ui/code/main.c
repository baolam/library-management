#include "ui_loader.h"
#include "callbacks.h"
#include "book_treeview.h"
#include "search_toggle.h"
#include "search.h"

GtkBuilder *global_builder = NULL;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    global_builder = load_ui("UI.glade");  // Load builder toàn cục
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(global_builder, "window"));

    load_books_from_file(global_builder);  // Dùng builder toàn cục

    // Kết nối tín hiệu từ Glade
    gtk_builder_connect_signals(global_builder, NULL);  // Tự động kết nối

    popup_menu_init(global_builder);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}