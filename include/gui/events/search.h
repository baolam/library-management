#ifndef SEARCH_H
#define SEARCH_H

#include <gtk/gtk.h>
#include <string.h>
#include "gui/constant.h"
#include "gui/events/book_treeview.h"
#include "gui/events/reader_treeview.h"
#include "utils/fake.h"
#include "books.h"
#include "reader.h"


void on_search_entry_changed(GtkSearchEntry *entry, gpointer user_data);
void on_search_entry_changed1(GtkSearchEntry *entry, gpointer user_data);

#endif
