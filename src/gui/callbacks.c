#include "gui.h"
#include "callbacks.h"

void on_button_clicked(GtkButton *button, gpointer user_data)
{
    // g_print("Hello, GTK & Glade!\n");
    // GtkWidget *dialog = gtk_message_dialog_new(NULL,
    //                                            GTK_DIALOG_MODAL,
    //                                            GTK_MESSAGE_INFO,
    //                                            GTK_BUTTONS_OK,
    //                                            "Hello, GTK & Glade!");
    // gtk_dialog_run(GTK_DIALOG(dialog));
    // gtk_widget_destroy(dialog);
    printf("Dang\n");
}

void on_ldt_clicked(GtkButton *button, gpointer user_data)
{
    printf("LDT\n");
}

void on_opened_book(GtkButton *button, gpointer user_data)
{
    printf("HEHEHE");
    GtkWidget *searchBook = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_dialog_run(GTK_DIALOG(searchBook));
    gtk_widget_destroy(searchBook);
}