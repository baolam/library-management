#include "gui/gui.h"
#include "gui/callbacks.h"

void on_button_clicked(GtkWidget *widget, gpointer user_data)
{
    GtkNotebook *notebook = GTK_NOTEBOOK(user_data);

    if (!notebook)
    {
        g_print("LỖI: Notebook NULL!\n");
        return;
    }

    // Lấy ID của button từ Glade
    const gchar *button_name = gtk_buildable_get_name(GTK_BUILDABLE(widget));
    g_print("Button clicked: %s\n", button_name);

    // Kiểm tra button nào được bấm và chuyển tab
    if (g_strcmp0(button_name, "home-button") == 0)
    {
        gtk_notebook_set_current_page(notebook, 0);
    }
    else if (g_strcmp0(button_name, "manage-book") == 0)
    {
        gtk_notebook_set_current_page(notebook, 1);
    }
    else if (g_strcmp0(button_name, "manage-reader") == 0)
    {
        gtk_notebook_set_current_page(notebook, 2);
    }
    else if (g_strcmp0(button_name, "borrow-return") == 0)
    {
        gtk_notebook_set_current_page(notebook, 3);
    }
    else if (g_strcmp0(button_name, "summary") == 0)
    {
        gtk_notebook_set_current_page(notebook, 4);
    }
    else if (g_strcmp0(button_name, "manual") == 0)
    {
        gtk_notebook_set_current_page(notebook, 5);
    }
    else
    {
        g_print("Không tìm thấy tab tương ứng!\n");
    }
}