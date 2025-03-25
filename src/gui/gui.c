#include "../include/callbacks.h"
#include "book_manager.h"

void init_book_manager_gui(int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *borrow_button, *return_button, *entry_book_name;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("ui/book_manager.ui");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Mainwindow"));

    // Lấy các widget cần dùng
    borrow_button = GTK_WIDGET(gtk_builder_get_object(builder, "borrow_button"));
    return_button = GTK_WIDGET(gtk_builder_get_object(builder, "return_button"));
    entry_book_name = GTK_WIDGET(gtk_builder_get_object(builder, "entry_book_name"));

    // Gán sự kiện cho nút
    g_signal_connect(borrow_button, "clicked", G_CALLBACK(on_borrow_button_clicked), entry_book_name);
    g_signal_connect(return_button, "clicked", G_CALLBACK(on_return_button_clicked), entry_book_name);

    gtk_widget_show_all(window);
    gtk_main();
}
