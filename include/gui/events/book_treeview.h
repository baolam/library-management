#ifndef BOOK_TREEVIEW_H
#define BOOK_TREEVIEW_H

#include <gtk/gtk.h>
#include "gui/constant.h"
#include "books.h"

extern int currnent_page;

void load_book_to_layout(GtkBuilder *builder, int beginingKey);

#endif
