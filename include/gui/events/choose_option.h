#ifndef CHOOSE_OPTION_H
#define CHOOSE_OPTION_H

#include "gui/constant.h"

void popup_book_menu_init(GtkBuilder *_builder);
void popup_book_menu_show(GtkTreeView *treeview, GdkEventButton *event);
void popup_reader_menu_init(GtkBuilder *_builder);
void popup_reader_menu_show(GtkTreeView *treeview, GdkEventButton *event);
void popup_borrow_menu_init(GtkBuilder *_builder);
void popup_borrow_menu_show(GtkTreeView *treeview, GdkEventButton *event);

void on_book_treeview_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
gboolean on_book_treeview_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data);

void on_reader_treeview_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
gboolean on_reader_treeview_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data);

void on_borrow_treeview_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
gboolean on_borrow_treeview_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data);

void on_book_edit_activate(GtkMenuItem *item, gpointer user_data);
void on_book_delete_activate(GtkMenuItem *item, gpointer user_data);

void on_reader_edit_activate(GtkMenuItem *item, gpointer user_data);
void on_reader_delete_activate(GtkMenuItem *item, gpointer user_data);

void on_borrow_detail_activate(GtkMenuItem *item, gpointer user_data);
void on_borrow_erase_activate(GtkMenuItem *item, gpointer user_data);

#endif
