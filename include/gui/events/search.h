#ifndef SEARCH_H
#define SEARCH_H

#include "gui/constant.h"
#include <ctype.h>

void on_book_search_entry_changed(GtkSearchEntry *entry, gpointer user_data);
void on_reader_search_entry_changed(GtkSearchEntry *entry, gpointer user_data);
void on_borrow_search_entry_changed(GtkSearchEntry *entry, gpointer user_data);
void on_entry_search_book_borrow_search_changed(GtkSearchEntry *entry, gpointer user_data);

#endif
