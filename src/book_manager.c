#include <stdio.h>
#include <string.h>
#include "books.h"

// Định nghĩa file chứa danh sách sách
#define BOOKS_FILE "books.dat"

// Hàm mượn sách
int borrow_book(const char *book_name)
{
    FILE *file = fopen(BOOKS_FILE, "r+");
    if (!file)
    {
        printf("Không thể mở file %s\n", BOOKS_FILE);
        return 0;
    }

    Book book;
    while (fread(&book, sizeof(Book), 1, file))
    {
        if (strcmp(book.title, book_name) == 0 && book.stock > 0)
        {
            book.stock--; // Giảm số lượng sách
            fseek(file, -(long)sizeof(Book), SEEK_CUR);

            fwrite(&book, sizeof(Book), 1, file);
            fclose(file);
            return 1; // Mượn thành công
        }
    }

    fclose(file);
    return 0; // Mượn thất bại
}

// Hàm trả sách
int return_book(const char *book_name)
{
    FILE *file = fopen(BOOKS_FILE, "r+");
    if (!file)
    {
        printf("Không thể mở file %s\n", BOOKS_FILE);
        return 0;
    }

    Book book;
    while (fread(&book, sizeof(Book), 1, file))
    {
        if (strcmp(book.title, book_name) == 0)
        {
            book.stock++; // Tăng số lượng sách
            fseek(file, -(long)sizeof(Book), SEEK_CUR);

            fwrite(&book, sizeof(Book), 1, file);
            fclose(file);
            return 1; // Trả thành công
        }
    }

    fclose(file);
    return 0; // Trả thất bại
}
