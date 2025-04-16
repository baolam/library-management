#ifndef SEARCH_H
#define SEARCH_H

#include "gui/constant.h"

void on_book_search_entry_changed(GtkSearchEntry *entry, gpointer user_data);
void on_reader_search_entry_changed(GtkSearchEntry *entry, gpointer user_data);
void on_borrow_search_entry_changed(GtkSearchEntry *entry, gpointer user_data);

#endif
