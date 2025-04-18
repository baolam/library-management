#include "gui/events/search_toggle.h"

void __utilize(GtkToggleButton *toggle)
{
    if (gtk_toggle_button_get_active(toggle))
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), 1);
        gtk_button_set_label(GTK_BUTTON(toggle), "Tìm kiếm theo tên");
    }
    else
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), 0);
        gtk_button_set_label(GTK_BUTTON(toggle), "Tìm theo theo ID");
    }
}

void on_book_toggle_mode_toggled(GtkToggleButton *toggle, gpointer user_data)
{
    __utilize(toggle);
}

void on_reader_toggle_mode_toggled(GtkToggleButton *toggle, gpointer user_data)
{
    __utilize(toggle);
}

void on_borrow_toggle_mode_toggled(GtkToggleButton *toggle, gpointer user_data)
{
    // __utilize(toggle);
    if (gtk_toggle_button_get_active(toggle))
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), 0);
        gtk_button_set_label(GTK_BUTTON(toggle), "Tìm kiếm theo ID");
    }
    else
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), 0);
        gtk_button_set_label(GTK_BUTTON(toggle), "Tìm theo theo ID");
    }
}

void on_toggle_mode_book_borrow_toggled(GtkToggleButton *toggle, gpointer user_data)
{
    __utilize(toggle);
}