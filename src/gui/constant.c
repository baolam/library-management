#include "gui/constant.h"

GtkBuilder *builder = NULL;
GtkWidget *book_menu = NULL;
GtkWidget *reader_menu = NULL;
GtkWidget *borrow_menu = NULL;

gint book_chosen_id = -1;
int book_chosen_action = BOOK_NO_ACTION;

gint reader_chosen_id = -1;
int reader_chosen_action = READER_NO_ACTION;

gint borrow_chosen_id = -1;
int borrow_chosen_action = BORROW_NO_ACTION;

int current_book_page = 1;
int current_reader_page = 1;
int current_borrow_page = 1;
int current_book_borrow_page = 1;

BorrowReturn *temp_borrow = NULL;