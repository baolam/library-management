#ifndef READER_TREEVIEW_H
#define READER_TREEVIEW_H

#include "gui/constant.h"

GtkListStore *reference_store_reader();

void show_onereader_to_layout(GtkListStore *store, Readers reader);
void show_reader_to_layout(Readers *reader, int size);
void load_reader_to_layout(int beginingKey);

#endif
