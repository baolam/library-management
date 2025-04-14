#include "gui/events/callbacks.h"
#include "gui/events/book_treeview.h"
#include "gui/gui.h"
#include "gui/ui_loader.h"
#include "books.h"

int currKey = 1;

void on_book_add_clicked(GtkWidget *widget, gpointer user_data)
{
    GtkBuilder *builder = GTK_BUILDER(user_data); // Ép kiểu user_data thành GtkBuilder
    GtkWidget *add_window = GTK_WIDGET(gtk_builder_get_object(builder, "book_entry"));
    if (add_window)
    {
        gtk_widget_show_all(add_window); // Hiện cửa sổ
    }
    else
    {
        g_warning("Không tìm thấy cửa sổ 'add_window'!");
    }
}

