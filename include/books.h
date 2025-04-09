#ifndef BOOKS_H
#define BOOKS_H
#define FOLDER "books"

#define MAX_TITLE 50
#define MAX_AUTHOR 50
#define MAX_GENRE_NO 10
#define MAX_GENRE_LENGTH 10

#include <gtk/gtk.h>

typedef struct
{
    int bookId;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char genre[MAX_GENRE_NO][MAX_GENRE_LENGTH];
    int publicationYear;
    int stock;
} Book;

/// @brief Thêm từ khoá on trước để ám chỉ sẽ lấy thông tin
/// từ giao diện
void on_created_new_book(GtkButton *submit, gpointer user_input);
void on_update_book();
void on_delete_book();

#endif