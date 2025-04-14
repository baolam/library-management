#ifndef READER_TREEVIEW_H
#define READER_TREEVIEW_H

#include <gtk/gtk.h>
#include "gui/constant.h"
#include "reader.h"

extern int current_reader_page;

GtkListStore *reference_store1();
void show_onereader_to_layout(GtkListStore *store, Readers reader);
void show_reader_to_layout(Readers *reader, int size);
void load_reader_to_layout(int beginingKey);

#endif
