#ifndef CHOOSE_OPTION_H
#define CHOOSE_OPTION_H

#include "gui/constant.h"

void popup_menu_init(GtkBuilder *builder);
void popup_menu_show(GtkTreeView *treeview, GdkEventButton *event);

void on_book_treeview_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
gboolean on_book_treeview_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data);

void on_book_edit(GtkMenuItem *item, gpointer user_data);
void on_book_delete(GtkMenuItem *item, gpointer user_data);

#endif
