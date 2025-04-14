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
        printf("Failed to delete book.\n");
    }
}

void delete_book(int id)
{
    soft_delete(book_management, id, delete_book_callback);
    save_book_management();
}

void update_book_callback(FILE *f, long package)
{
    Book book;
    fread(&book, sizeof(Book), 1, f);
    printf("Current information: \n");
    show_book(book);

    printf("Enter title:\n");
    char title[MAX_TITLE];
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

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

    removeIdFromWord(book_trie, book.title, book.bookId);
    insertIntoTrie(book_trie, title, book.bookId);

    strcpy(book.title, title);

    fwrite(&book, sizeof(Book), 1, f);
    printf("Update successfully!\n");
}

void update_book(int id)
{
    Record *record = find(book_management, id);
    if (record == NULL || record->deleted)
    {
        printf("Reader not found for update.\n");
        return;
    }

    update_content_from_record(record, update_book_callback);
}

int update_book_from_object(Book *book)
{
    Record *record = find(book_management, book->bookId);
    if (record == NULL || record->deleted)
    {
        printf("Reader not found for update.\n");
        return UPDATE_FAILED;
    }

    // Book *old_book = read_content_from_record_return(record);
    // removeIdFromWord(book_trie, old_book->title, book->bookId);

    insertIntoTrie(book_trie, book->title, book->bookId);

    return update_content_without_callback(record, book);
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
            for (int j = 0; j < temp->numIds && maxNumbers > 0; j++)
            {
                if (exist_record(book_management, temp->ids[j]))
                {
                    search_book_by_id(temp->ids[j]);
                    maxNumbers--;
                }
            }
            if (maxNumbers == 0)
                break;
        }
    }
}

Book *search_book_by_title_direct(const char *prefix, int *actualBooks, int maxNumbers)
{
    int recommendSize = 0, storage_pos = 0;
    char *recommend[maxNumbers];

    /// Cấp phát bộ nhớ cho Book
    Book *books = (Book *)malloc(maxNumbers * sizeof(Book));
    if (books == NULL)
    {
        printf("Memory allocation failed.\n");
        *actualBooks = 0;
        return NULL;
    }

    recommendPrefix(book_trie, (char *)prefix, maxNumbers, recommend, &recommendSize);
    for (int i = 0; i < recommendSize; i++)
    {
        char *name = recommend[i];
        TrieNode *temp = searchWord(book_trie, name);
        if (temp != NULL)
        {
            for (int j = 0; j < temp->numIds && maxNumbers > 0; j++)
            {
                if (exist_record(book_management, temp->ids[j]))
                {
                    Book *book = search_book(temp->ids[j]);
                    books[storage_pos] = *book;
                    storage_pos++;
                    maxNumbers--;
                }
            }
            if (maxNumbers == 0)
                break;
        }
    }

    *actualBooks = storage_pos;
    return books;
}

Book *retrieve_bucket_books(int beginingKey, int quanities, int *actualBooks)
{
    int storage_pos = 0;

    Book *books = (Book *)malloc(quanities * sizeof(Book));
    if (books == NULL)
    {
        *actualBooks = 0;
        return NULL;
    }

    Node *n = findLeaf(book_management, beginingKey);
    if (n == NULL)
    {
        *actualBooks = 0;
        return NULL;
    }

    int startSearch = getStartSearch(n, beginingKey);
    int i;

    while (n != NULL && quanities > 0)
    {
        for (i = startSearch; i < n->num_keys && quanities > 0; i++)
        {
            if (!exist_record(book_management, n->keys[i]))
                continue;

            Record *record = (Record *)n->pointers[i];
            Book *book = (Book *)read_content_from_record_return(record);

            if (book == NULL)
                continue;

            books[storage_pos] = *book;
            storage_pos++;
            quanities--;
        }

        startSearch = 0;
        n = n->pointers[ORDER - 1];
    }

    *actualBooks = storage_pos;
    return books;
}

Book *search_book(int id)
{
    Record *record = find(book_management, id);
    Book *book = (Book *)read_content_from_record_return(record);
    return book;
}

void prepare_book()
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

void load_book_management()
{
    book_management = loadTree(book_management_file);
    if (book_management == NULL)
    {
        printf("(Book) Failed to load B+ Tree management!\n");
    }
    else
    {
        printf("(Book) Load B+ Tree management successfully!\n");
    }
    book_trie = loadTrieTree(book_trie_management);
    if (book_trie == NULL)
    {
        printf("(Book) Failed to load Trie management!\n");
    }
    else
    {
        printf("(Book) Load Trie management successfully!\n");
    }
}