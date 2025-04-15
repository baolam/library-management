#include "gui/constant.h"

GtkBuilder *builder = NULL;
GtkWidget *book_menu = NULL;
GtkWidget *reader_menu = NULL;

gint book_chosen_id = -1;
int book_chosen_action = BOOK_NO_ACTION;

gint reader_chosen_id = -1;
int reader_chosen_action = READER_NO_ACTION;