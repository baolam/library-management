#ifndef BOOK_BORROW_ENTRY_H
#define BOOK_BORROW_ENTRY_H

#include "gui/constant.h"

typedef struct BookBorrowEntry
{
    int bookId;
    int left;
    int quantity;
} BookBorrowEntry;

BookBorrowEntry get_book_borrow_input();
void show_book_borrow_to_entry(BookBorrowEntry book_borrow_entry);
void clear_book_borrow_to_entry();

void on_book_borrow_confirmed_clicked(GtkButton *button, GtkBuilder *_builder);

#endif