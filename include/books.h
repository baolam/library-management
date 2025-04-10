#ifndef BOOKS_H
#define BOOKS_H
#define FOLDER "books"

#define MAX_TITLE 50
#define MAX_AUTHOR 50
#define MAX_GENRE_NO 100

#include <gtk/gtk.h>
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

extern char book_management_file[MAX_FILE_NAME_LENGTH];
extern char book_trie[MAX_FILE_NAME_LENGTH];
extern char book_content_file[MAX_FILE_NAME_LENGTH];

extern Node *book_management;
extern TrieNode *book_trie;

// ===================== CRUD operations ========
void show_book(Book book);
void add_book(Book *book);
void delete_book(int id);
void update_reader(Book *book);
void search_book_by_id(int id);
const search_book_by_title(const char *title, int maxNumbers);

// ====== Supporting for Reader management =====
Book *search_book(int id);

// ==== Persistence ===
void save_book_management();
void load_book_management();

#endif