#ifndef READERS_ENTRY_H
#define READERS_ENTRY_H

#include "gui/constant.h"

Readers get_reader_input();

void show_reader_to_entry(Readers r);
void clear_reader_to_entry();

void on_reader_button_confirm_clicked(GtkButton *button, GtkBuilder *_builder);

#endif