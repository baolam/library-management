#include "callbacks.h"

void on_button_clicked(GtkButton *button, gpointer user_data)
{
    g_print("Hello, GTK & Glade!\n");
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Hello, GTK & Glade!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
