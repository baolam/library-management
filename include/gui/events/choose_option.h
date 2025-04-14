#ifndef CHOOSE_OPTION_H
#define CHOOSE_OPTION_H

#include <gtk/gtk.h>
#include "gui/constant.h"


void popup_menu_init(GtkBuilder *builder);
void popup_menu_show(GtkTreeView *treeview);

void on_treeview_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
void on_book_add(GtkMenuItem *item, gpointer user_data);
void on_book_edit(GtkMenuItem *item, gpointer user_data);
void on_book_delete(GtkMenuItem *item, gpointer user_data);

#endif
