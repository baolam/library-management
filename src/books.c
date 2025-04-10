#include "books.h"

char book_management_file[MAX_FILE_NAME_LENGTH] = "data/book/book_management.bin";
char book_trie_management[MAX_FILE_NAME_LENGTH] = "data/book/book_trie.bin";
char book_content_file[MAX_FILE_NAME_LENGTH] = "data/book/book.bin";

Node *book_management = NULL;
TrieNode *book_trie = NULL;

void show_book(Book book)
{
    printf("===== Book Infor ===== \n");
    printf("Id : %d\n", book.bookId);
    printf("Title: %s\n", book.title);
    printf("Author: %s\n", book.author);
    printf("Genre: %s\n", book.genre);
    printf("Year: %d\n", book.publicationYear);
    printf("Stock: %d\n", book.stock);
}

void add_book_callback(int id, int code, long offset, long length)
{
    if (code == ADD_CONTENT_SUCCESS)
    {
        printf("Id : %d, Successfully added!\n", id);
    }
    else
    {
        printf("Id : %d, Failed to add reader.\n", id);
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

    insertIntoTrie(book_trie, book->title, book->bookId);
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
    show_book(book);

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
    scanf("%d", &book.publicationYear);

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

    update_content_from_record(record, update_book_callback);
}

void show_book_record(FILE *f, long size)
{
    Book book;
    fread(&book, sizeof(Book), 1, f);
    show_book(book);
}

void search_book_by_id(int id)
{
    Record *record = find(book_management, id);
    if (record == NULL || record->deleted)
    {
        printf("Reader with ID %d not found.\n", id);
        return;
    }

    read_content_from_record(record, show_book_record);
}

void search_book_by_title(const char *prefix, int maxNumbers)
{
    int recommendSize = 0;
    char *recommend[maxNumbers];

    recommendPrefix(book_trie, (char *)prefix, maxNumbers, recommend, &recommendSize);
    for (int i = 0; i < recommendSize; i++)
    {
        char *name = recommend[i];
        TrieNode *temp = searchWord(book_trie, name);
        if (temp != NULL)
        {
            for (int j = 0; j < temp->numIds; j++)
            {
                search_book_by_id(temp->ids[j]);
            }
        }
    }
}

Book *search_book(int id)
{
    Record *record = find(book_management, id);
    if (record == NULL || record->deleted)
    {
        return NULL;
    }

    Book *book = (Book *)read_content_from_record_return(record, book_content_file, sizeof(Book));
    return book;
}

void preparate_book()
{
    if (book_trie == NULL)
    {
        book_trie = makeTrieNode();
    }
}

void save_book_management()
{
    saveTree(book_management, book_management_file);
    saveTrieTree(book_trie_management, book_trie);
}

void getInfor(int *total, int *deleted)
{
}

void load_book_management()
{
    book_management = loadTree(book_management_file);
    if (book_management == NULL)
    {
        printf("Failed to load B+ Tree management!\n");
    }
    else
    {
        printf("Load B+ Tree management successfully!\n");
    }
    book_trie = loadTrieTree(book_trie_management);
    if (book_trie == NULL)
    {
        printf("Failed to load Trie management!\n");
    }
    else
    {
        printf("Load Trie management successfully!\n");
    }
}