#include "choose_option.h"
#include "popup_menu.h"

void connect_signals(GtkBuilder *builder) {
    GtkWidget *treeview = GTK_WIDGET(gtk_builder_get_object(builder, "book_treeview"));
    g_signal_connect(treeview, "button-press-event", G_CALLBACK(on_treeview_button_press), NULL);
}

gboolean on_treeview_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        GtkWidget *menu = create_popup_menu(widget);
        gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent *)event);
        return TRUE;
    }
    return FALSE;
}
