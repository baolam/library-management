#ifndef BOOKS_ENTRY_H
#define BOOKS_ENTRY_H

#include "gui/constant.h"

Book get_book_input();
void show_book_to_entry(Book book);

void on_book_button_confirm_clicked(GtkButton *button, gpointer user_data);

#endif