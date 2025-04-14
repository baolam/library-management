#ifndef SEARCH_H
#define SEARCH_H

#include <gtk/gtk.h>
#include <string.h>
#include "gui/constant.h"
#include "gui/events/book_treeview.h"
#include "utils/fake.h"
#include "books.h"

void on_search_entry_changed(GtkSearchEntry *entry, gpointer user_data);

#endif
