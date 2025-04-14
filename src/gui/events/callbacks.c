#include "gui/events/callbacks.h"

void open_book_entry_layout()
{
    // builder = load_ui("ui/UI.glade");
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

void on_reader_add_clicked(GtkWidget *widget, gpointer user_data)
{
    // builder = load_ui("ui/UI.glade");
    GtkWidget *add_window = GTK_WIDGET(gtk_builder_get_object(builder, "reader_entry"));
    if (add_window)
    {
        gtk_widget_show_all(add_window);
    }
    else
    {
        g_warning("Không tìm thấy cửa sổ 'add_window'!");
    }
}

void on_book_add_clicked(GtkWidget *widget, gpointer user_data)
{
    /// Tiến hành nhớ trạng thái
    chosen_action = BOOK_ADD_STATUS;
    open_book_entry_layout();
}
