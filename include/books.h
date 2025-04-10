#ifndef BOOKS_H
#define BOOKS_H
#define FOLDER "books"

#define MAX_TITLE 50
#define MAX_AUTHOR 50
#define MAX_GENRE_NO 100

// #include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "management.h"
#include "trie.h"
#include "bplustreev2.h"

typedef struct
{
    int bookId;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char genre[MAX_GENRE_NO];
    int publicationYear;
    int stock;
} Book;

extern char *titles[];
extern char *authors[];
extern char *genres[];

extern char book_management_file[MAX_FILE_NAME_LENGTH];
extern char book_content_file[MAX_FILE_NAME_LENGTH];

extern Node *book_management;

Book generate_book(int id);
void show_book(Book book);

/// Lưu trữ vào file, load dữ liệu ra từ file
void save_book_management();
void load_book_management();

/// Thêm, Sửa, Xoá, ...
void add_book_stochastic(int total);
void search_book_by_id(int id);

/// CRUD
void create_book(Book *book);
void read_book(Book book);

#endif