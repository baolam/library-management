#ifndef POPUP_MENU_H
#define POPUP_MENU_H

#include <gtk/gtk.h>

void connect_signals(GtkBuilder *builder);
gboolean on_treeview_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data);

#endif
