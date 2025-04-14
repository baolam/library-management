#ifndef BOOK_TREEVIEW_H
#define BOOK_TREEVIEW_H

#include "gui/constant.h"

GtkListStore *reference_store();
void show_onebook_to_layout(GtkListStore *store, Book book);
void show_book_to_layout(Book *book, int size);
void load_book_to_layout(int beginingKey);

#endif
