#ifndef SEARCH_TOGGLE_H
#define SEARCH_TOGGLE_H

#include "gui/constant.h"
#include <gtk/gtk.h>

void on_book_toggle_mode_toggled(GtkToggleButton *toggle, gpointer user_data);
void on_reader_toggle_mode_toggled(GtkToggleButton *toggle, gpointer user_data);

#endif
