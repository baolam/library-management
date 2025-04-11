#ifndef STATISTIC_H
#define STATISTIC_H

#include "bplustreev2.h"
#include "fake.h"
#include "books.h"
#include "management.h"

#include <string.h>

extern short counter_genre[TOTAL_GENRE];
extern int total_books;
extern int deleted_books;

void set_up();
void calc_statistic_book(Node *book_management);

#endif