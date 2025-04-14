#include "gui/events/callbacks.h"
#include "gui/events/book_treeview.h"
#include "gui/gui.h"
#include "gui/ui_loader.h"
#include "books.h"

void on_book_add_clicked(GtkWidget *widget, gpointer user_data)
{
    builder = load_ui("ui/UI.glade");
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
}
