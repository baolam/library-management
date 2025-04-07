#ifndef BOOKS_H
#define BOOKS_H
#define FOLDER "books"

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "management.h"
#include "bplustreev2.h"

typedef struct
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

extern char management_file[MAX_FILE_NAME_LENGTH];
extern char content_file[MAX_FILE_NAME_LENGTH];

extern Node *book_management;

Book generate_book(int id);
void show_book(Book book);

/// Lưu trữ vào file, load dữ liệu ra từ file
void save_book_management();
void load_book_management();

/// Thêm, Sửa, Xoá, ...
void add_book_stochastic(int total);
void search_book_by_id(int id);

#endif