#ifndef BORROW_RETURN_TREEVIEW_H
#define BORROW_RETURN_TREEVIEW_H

#include "gui/constant.h"

GtkListStore *reference_borrow_return_store();

void show_oneborrow_to_layout(GtkListStore *store, BorrowReturn borrow);
void show_borrow_to_layout(BorrowReturn *borrow, int size);
void load_borrow_to_layout(int beginingKey);

#endif