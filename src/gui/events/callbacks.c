#include "gui/events/callbacks.h"

void open_book_entry_layout()
{
<<<<<<< HEAD
    builder = load_ui("ui/UI.glade");
=======
>>>>>>> e1a7e6a40cfb92d244d1ed2dcee7c0820c9dbbf5
    GtkWidget *add_window = GTK_WIDGET(gtk_builder_get_object(builder, "book_entry"));
    if (add_window)
    {
        gtk_widget_show_all(add_window);
    }
    else
    {
        g_warning("Không tìm thấy cửa sổ 'add_window'!");
    }
}

<<<<<<< HEAD
void on_reader_add_clicked(GtkWidget *widget, gpointer user_data)
{
    builder = load_ui("ui/UI.glade");
    GtkWidget *add_window = GTK_WIDGET(gtk_builder_get_object(builder, "reader_entry"));
    if (add_window)
    {
        gtk_widget_show_all(add_window);
    }
    else
    {
        g_warning("Không tìm thấy cửa sổ 'add_window'!");
    }
=======
void on_book_add_clicked(GtkWidget *widget, gpointer user_data)
{
    /// Tiến hành nhớ trạng thái
    chosen_action = BOOK_ADD_STATUS;
    open_book_entry_layout();
>>>>>>> e1a7e6a40cfb92d244d1ed2dcee7c0820c9dbbf5
}
