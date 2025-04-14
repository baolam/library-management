#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

#include "gui/constant.h"
#include "ui_loader.h"

#include "events/callbacks.h"
#include "events/book_treeview.h"
#include "events/books_entry.h"
#include "events/search_toggle.h"
#include "events/choose_option.h"
#include "events/search.h"

void init_gui(int argc, char *argv[]);

#endif
