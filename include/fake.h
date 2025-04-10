#ifndef FAKE_H
#define FAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "books.h"
#include "reader.h"

extern char *titles[];
extern char *authors[];
extern char *genres[];

Book generate_book(int id);
Readers generate_reader(int id);

#endif