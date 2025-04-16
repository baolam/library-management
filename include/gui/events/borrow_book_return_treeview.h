#ifndef BORROW_BOOK_RETURN_TREEVIEW_H
#define BORROW_BOOK_RETURN_TREEVIEW_H

#include "gui/constant.h"

GtkListStore *reference_borrow_book_return_store();

void show_oneborrowbook_to_layout(GtkListStore *store, Book book, int status, int totals);
void show_borrowbook_to_layout(BorrowReturn *borrow, int size);
void load_borrowbook_to_layout();

#endif