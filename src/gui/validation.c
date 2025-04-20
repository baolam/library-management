#include "gui/validation.h"

gboolean is_numeric(const gchar *str)
{
    if (str == NULL || *str == '\0') {
        return FALSE; 
    }

    for (gint i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return FALSE;
        }
    }

    return TRUE;
}

void show_warning() {
    GtkWindow *parent_window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkWidget *dialog = gtk_message_dialog_new(
        parent_window,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Dữ liệu nhập không hợp lệ!\n Vui lòng nhập lại.");

    gtk_window_set_title(GTK_WINDOW(dialog), "Thông báo");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

