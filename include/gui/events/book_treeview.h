#ifndef BOOK_TREEVIEW_H
#define BOOK_TREEVIEW_H

#include <gtk/gtk.h>
#include "gui/constant.h"
#include "books.h"

extern int current_book_page;

GtkListStore *reference_store();
void show_onebook_to_layout(GtkListStore *store, Book book);
void show_book_to_layout(Book *book, int size);
void load_book_to_layout(int beginingKey);

#endif
