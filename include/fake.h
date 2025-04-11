#ifndef FAKE_H
#define FAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "books.h"
#include "reader.h"

#define TOTAL_GENRE 12

extern char *titles[];
extern char *authors[];
extern char *genres[TOTAL_GENRE];

Book generate_book(int id);
Readers generate_reader(int id);

#endif