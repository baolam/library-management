#ifndef STATISTIC_H
#define STATISTIC_H

#include <string.h>

#include "utils/bplustreev2.h"
#include "utils/fake.h"
#include "utils/management.h"
#include "books.h"

extern short counter_genre[TOTAL_GENRE];
extern int total_books;
extern int deleted_books;

void set_up();
void calc_statistic_book(Node *book_management);

#endif