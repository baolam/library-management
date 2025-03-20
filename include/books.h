#ifndef BOOKS_H
#define BOOKS_H
#define FOLDER "books"

#include <gtk/gtk.h>

typedef struct
{
    int bookId;
    char title[50];
    char author[50];
    char genre[10];
    int publicationYear;
    int stock;
} Book;

/// @brief Thêm từ khoá on trước để ám chỉ sẽ lấy thông tin
/// từ giao diện
void on_created_new_book(GtkButton *submit, gpointer user_input);
void on_update_book();
void on_delete_book();

#endif