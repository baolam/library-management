#ifndef BOOKS_ENTRY_H
#define BOOKS_ENTRY_H

#include "gui/constant.h"

Book get_book_input();
void show_book_to_entry(Book book);
void clear_book_to_entry();

void on_button_confirm_clicked(GtkButton *button, GtkBuilder *_builder);

#endif