#include "books.h"

char book_management_file[MAX_FILE_NAME_LENGTH] = "book_management.bin";
char book_trie[MAX_FILE_NAME_LENGTH] = "book_trie.bin";
char book_content_file[MAX_FILE_NAME_LENGTH] = "book.bin";

Node *book_management = NULL;
TrieNode *book_trie = NULL;

void show_book(Book book)
{
    printf("Title: %s\n", book.title);
    printf("Author: %s\n", book.author);
    printf("Publisher: %s\n", book.publisher);
    printf("Year: %d\n", book.publicationYear);
    printf("Stock: %d\n", book.stock);
}

void add_book_callback(int id, int code, long offset, long length)
{
    if (code == ADD_CONTENT_SUCCESS)
    {
        printf("Successfully added!\n");
    }
    else
    {
        printf("Failed to add reader.\n");
    }
}

void add_book(Book *book)
{
    if (exist_record(book_management, book->bookId))
    {
        return;
    }

    Node *new_tree = add_content(
        book_management,
        book->bookId,
        book_content_file,
        book,
        sizeof(Book),
        add_book_callback);

    if (new_tree != NULL)
    {
        book_management = new_tree;
    }
}