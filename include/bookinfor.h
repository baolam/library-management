#ifndef BOOKINFOR_H
#define BOOKINFOR_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

typedef struct Book
{
    char bookId[20];
    char title[100];
    char author[100];
    char genre[50];
    int publicationYear;
    int stock;
} Book;

extern char *titles[];
extern char *authors[];
extern char *genres[];

Book generate_book(int id);
void show_book(Book book);

#endif