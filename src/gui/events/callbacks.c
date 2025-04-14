#include "gui/events/callbacks.h"

void open_book_entry_layout()
{
    GtkWidget *add_window = GTK_WIDGET(gtk_builder_get_object(builder, "book_entry"));
    if (add_window)
    {
        /// Tiến hành nhớ trạng thái
        chosen_action = BOOK_ADD_STATUS;
        gtk_widget_show_all(add_window);
    }
    else
    {
        g_warning("Không tìm thấy cửa sổ 'add_window'!");
    }
}

void on_book_add_clicked(GtkWidget *widget, gpointer user_data)
{
    open_book_entry_layout();
}
