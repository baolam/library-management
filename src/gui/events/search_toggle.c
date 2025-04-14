#include "gui/events/search_toggle.h"

void on_toggle_mode_toggled(GtkToggleButton *toggle, gpointer user_data)
{
    if (gtk_toggle_button_get_active(toggle))
    {
        gtk_button_set_label(GTK_BUTTON(toggle), "Tìm kiếm theo ID");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), 1);
    }
    else
    {
        gtk_button_set_label(GTK_BUTTON(toggle), "Tìm theo theo tên");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), 0);
    }
}
