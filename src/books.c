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

void delete_book_callback(int code)
{
    if (code == DELETE_SUCCESS)
    {
        printf("Successfully deleted!\n");
    }
    else if (code == DELETE_EXISTED)
    {
        printf("Reader was already deleted.\n");
    }
    else
    {
        printf("Failed to delete reader.\n");
    }
}

void delete_book(int id)
{
    soft_delete(book_management, id, delete_book_callback);
}

void update_book_callback(FILE *f, long package)
{
    Book book;
    fread(&book, sizeof(Book), 1, f);
    printf("Current information: \n");
    show_book();

    printf("Enter title:\n");
    fgets(book.title, sizeof(book.title), stdin);
    book.title[strcspn(book.title, "\n")] = 0;

    printf("Enter author:\n");
    fgets(book.author, sizeof(book.author), stdin);
    book.author[strcspn(book.author, "\n")] = 0;

    printf("Enter genre:\n");
    fgets(book.genre, sizeof(book.genre), stdin);
    book.genre[strcspn(book.genre, "\n")] = 0;

    printf("Enter publication year:\n");
    scanf("%d", &book.publication_year);

    printf("Enter stock:\n");
    scanf("%d", &book.stock);

    fwrite(&book, sizeof(Book), 1, f);
    printf("Update successfully!\n");
}

void update_book(Book *book)
{
    Record *record = find(book_management, book->bookId);
    if (record == NULL || record->deleted)
    {
        printf("Reader not found for update.\n");
        return;
    }

    update_content_from_record(record, );
}