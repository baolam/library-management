#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "gui/constant.h"

void open_book_entry_layout();
void open_reader_entry_layout();
void open_borrow_book_layout();
void open_book_borrow_entry_layout();

gboolean on_window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean on_main_window_delete_event(GtkWidget *widge, GdkEvent *event, gpointer user_data);

void on_book_add_clicked(GtkWidget *widget, gpointer user_data);
void on_reader_add_clicked(GtkWidget *widget, gpointer user_data);
void on_borrow_add_clicked(GtkWidget *widget, gpointer user_data);
void on_book_borrow_add_clicked(GtkWidget *widget, gpointer user_data);

void on_to_index_clicked(GtkButton *button, gpointer user_data);
void on_to_main_ui_clicked(GtkButton *button, gpointer user_data);
void on_to_credits_clicked(GtkButton *button, gpointer user_data);

#endif
