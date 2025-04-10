#include "books.h"

void create_book_callback(int id, int code, long offset, long length)
{
    if (code == ADD_CONTENT_SUCCESS)
    {
        printf("Book ID %d create successfully!\n", id);
    }
    else
    {
        printf("Failed to add book ID %d.\n", id);
    }
}

void create_book(Book *book)
{
    int key = book->bookId;

    if (exist_record(book_management, key))
    {
        printf("Book ID %d already exists!\n", key);
        return;
    }

    Node *new_tree = add_content(
        book_management,  // gốc cây B+
        key,              // khoá chính (bookId)
        content_file,     // file lưu nội dung sách
        book,             // con trỏ đến struct Book
        sizeof(Book),     // kích thước
        add_book_callback // callback
    );

    if (new_tree != NULL)
    {
        book_management = new_tree;
    }
}

void search_book_by_id(const char *id)
{
    int key = atoi(id);
    Record *book_found = find(book_management, key);

    if (book_found == NULL || book_found->deleted)
    {
        printf("Book with ID %s not found.\n", id);
        return;
    }

    read_content_from_record(book_found, read_book)
}

void read_book(FILE *f, long size)
{
    Book b;
    fread(&b, sizeof(Book), 1, f);
}