#ifndef STATISTIC_SHOW_H
#define STATISTIC_SHOW_H

#include "gui/constant.h"

void load_statistic_to_layout();
void load_statistic_late_fess();
void show_onestatisticlate_to_layout(GtkListStore *store, OverdueBorrower *borrower);

void on_previous_statistics_page_clicked(GtkWidget *widget, gpointer user_data);
void on_next_statistics_page_clicked(GtkWidget *widget, gpointer user_data);

#endif