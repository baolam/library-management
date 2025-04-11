#include "reader.h"

#define TEST_READER
#define DEBUG_MODE 0
char reader_management_file[MAX_FILE_NAME_LENGTH] = "data/reader/reader_management.bin";
char reader_name_management_file[MAX_FILE_NAME_LENGTH] = "data/reader/reader_name_management.bin";
char reader_content_file[MAX_FILE_NAME_LENGTH] = "data/reader/reader.bin";
char borrow_return_content_file[] = "borrow_return.bin";
char borrow_return_management_file[] = "borrow_return_management.bin";
Node *borrow_return_management = NULL; // B+ Tree gốc
Node *reader_management = NULL;
TrieNode *reader_trie = NULL;
void restore_books_to_stock(BorrowReturn *b);
void print_tree_keys(Node *root);

// ------------------- CRUD FUNCTIONS -------------------

void show_reader(Readers reader)
{
    printf("ID: %d\n", reader.readerId);
    printf("Name: %s\n", reader.fullName);
    printf("Phone: %s\n", reader.phoneNumber);
    printf("Address: %s\n", reader.address);
    printf("-------------------------\n");
}

void show_reader_record(FILE *f, long size)
{
    Readers r;
    fread(&r, sizeof(Readers), 1, f);
    show_reader(r);
}

void search_reader_by_id(int id)
{
#if DEBUG_MODE
    printf("[DEBUG] Searching for ID = %d\n", id);
#endif
    Record *record = find(reader_management, id);
    if (record == NULL || record->deleted)
    {
        printf("Reader with ID %d not found.\n", id);
        return;
    }

    read_content_from_record(record, show_reader_record);
}

void search_reader_by_name(const char *name, int maxNumbers)
{
    int recommend_size = 0;
    char *recommend[maxNumbers];

    recommendPrefix(reader_trie, (char *)name, maxNumbers, recommend, &recommend_size);
    for (int i = 0; i < recommend_size; ++i)
    {
        char *name = recommend[i];
        TrieNode *temp = searchWord(reader_trie, name);
        if (temp != NULL)
        {
            for (int j = 0; temp->numIds && maxNumbers > 0; ++j)
            {
                if (exist_record(reader_management, temp->ids[j]))
                {
                    maxNumbers--;
                    search_reader_by_id(temp->ids[j]);
                }
            }
            if (maxNumbers == 0)
                break;
        }
    }
}

void add_reader_callback(int id, int code, long offset, long length)
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

void add_reader(Readers *r)
{
    int key = r->readerId;
#if DEBUG_MODE
    printf("[DEBUG] Adding reader ID = %d\n", key);
#endif

    if (exist_record(reader_management, key))
    {
        printf("Reader ID already exists!\n");
        return;
    }

    Node *new_tree = add_content(
        reader_management,
        key,
        reader_content_file,
        r,
        sizeof(Readers),
        add_reader_callback);

    if (new_tree != NULL)
    {
        reader_management = new_tree;
#if DEBUG_MODE
        print_tree_keys(reader_management);
#endif
    }

    insertIntoTrie(reader_trie, r->fullName, key);
}

void update_reader_callback(FILE *f, long size)
{
    Readers r;
    fread(&r, sizeof(Readers), 1, f);
    printf("Current information:\n");
    show_reader(r);

    printf("Enter new name: ");
    fgets(r.fullName, sizeof(r.fullName), stdin);
    r.fullName[strcspn(r.fullName, "\n")] = 0;

    printf("Enter new phone number: ");
    fgets(r.phoneNumber, sizeof(r.phoneNumber), stdin);
    r.phoneNumber[strcspn(r.phoneNumber, "\n")] = 0;

    printf("Enter new strcspnaddress: ");
    fgets(r.address, sizeof(r.address), stdin);
    r.address[strcspn(r.address, "\n")] = 0;

    // fseek(f, 0, SEEK_SET);
    fwrite(&r, sizeof(Readers), 1, f);

    printf("Update successful;y!\n");
}

void update_reader(Readers *reader)
{
    int key = (reader->readerId);
    Record *record = find(reader_management, key);
    if (record == NULL || record->deleted)
    {
        printf("Reader not found for update.\n");
        return;
    }

    update_content_from_record(record, update_reader_callback);
}

void delete_reader_callback(int code)
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

void delete_reader(int id)
{

    soft_delete(reader_management, id, delete_reader_callback);
}
// ------------------- borrow / return -------------------
void add_borrow_record(BorrowReturn *b)
{
    b->status = 0; // trạng thái mặc định: đang mượn
    int key = b->readerId;
    Node *new_tree = add_content(
        borrow_return_management,
        key,
        borrow_return_content_file,
        b,
        sizeof(BorrowReturn),
        NULL);
    if (new_tree != NULL)
    {
        borrow_return_management = new_tree;
    }
}

void show_borrow_record(FILE *f, long size)
{
    BorrowReturn b;
    fread(&b, sizeof(BorrowReturn), 1, f);

    printf("Reader ID: %d\n", b.readerId);
    printf("Borrowed books:\n");
    for (int i = 0; i < b.totalBooks; i++)
    {
        printf("  - Book ID: %d | Quantity: %d\n", b.bookIds[i], b.quantities[i]);
    }
    printf("Status: %s\n", b.status == 0 ? "Borrowing" : "Returned");
}
void search_borrow_record_by_reader(int readerId)
{
    Record *record = find(borrow_return_management, readerId);
    if (record == NULL || record->deleted)
    {
        printf("Not found.\n");
        return;
    }
    read_content_from_record(record, show_borrow_record);
}

void delete_borrow_record(int readerId)
{
    soft_delete(borrow_return_management, readerId, NULL);
}
void stat_total_books_by_reader(int readerId)
{
    Record *record = find(borrow_return_management, readerId);
    if (record == NULL || record->deleted)
    {
        printf("No borrow records found for reader ID %d.\n", readerId);
        return;
    }

    BorrowReturn b;
    FILE *f = fopen(record->_from, "rb");
    fseek(f, record->offset, SEEK_SET);
    fread(&b, sizeof(BorrowReturn), 1, f);
    fclose(f);

    int total = 0;
    for (int i = 0; i < b.totalBooks; i++)
    {
        total += b.quantities[i];
    }

    printf("Reader ID %d has borrowed a total of %d books.\n", readerId, total);
}
void return_books(int readerId)
{
    Record *record = find(borrow_return_management, readerId);
    if (record == NULL || record->deleted)
    {
        printf("No borrow records found for reader ID %d.\n", readerId);
        return;
    }

    BorrowReturn b;
    FILE *f = fopen(record->_from, "rb+");
    fseek(f, record->offset, SEEK_SET);
    fread(&b, sizeof(BorrowReturn), 1, f);

    if (b.status == 1)
    {
        printf("Books already returned.\n");
        fclose(f);
        return;
    }

    // ✅ Trả sách
    restore_books_to_stock(&b);
    printf("Was the return on time? (1 = Yes, 0 = No): ");
    scanf("%d", &b.onTime);
    while (getchar() != '\n')
        ;
    if (b.onTime == 0)
    {
        int fee_per_book = 5000;
        int total = 0;
        for (int i = 0; i < b.totalBooks; i++)
        {
            total += b.quantities[i] * fee_per_book;
        }
        printf("Late return! Total late fee: %d VND\n", total);
    }
    else
    {
        printf("Books returned on time. No late fee.\n");
    }
    b.status = 1;                       // ✅ đánh dấu đã trả
    fseek(f, record->offset, SEEK_SET); // quay lại và ghi đè
    fwrite(&b, sizeof(BorrowReturn), 1, f);
    fclose(f);
    printf("Borrow record updated to 'returned'.\n");
}

// ✅ Cập nhật tồn kho sách
void restore_books_to_stock(BorrowReturn *b)
{
    for (int i = 0; i < b->totalBooks; i++)
    {
        Book *book = search_book(b->bookIds[i]);
        if (book != NULL)
        {
            book->stock += b->quantities[i]; // Trả sách thì + lại stock
            update_book_direct(book);
        }
    }
}

void update_book_direct(Book *book)
{
    Record *record = find(book_management, book->bookId);
    if (record == NULL || record->deleted)
    {
        printf("Book not found for update.\n");
        return;
    }

    FILE *f = fopen(record->_from, "rb+");
    if (f == NULL)
    {
        printf("Failed to open book file for update.\n");
        return;
    }

    fseek(f, record->offset, SEEK_SET);
    fwrite(book, sizeof(Book), 1, f);
    fclose(f);
}
// ------------------- Save / Load Tree -------------------
void preparate_reader()
{
    if (reader_trie == NULL)
    {
        reader_trie = makeTrieNode();
    }
}

void save_reader_management()
{
    saveTree(reader_management, reader_management_file);
    saveTrieTree(reader_name_management_file, reader_trie);
}

void load_reader_management()
{
    FILE *file = fopen(reader_management_file, "r");
    if (file == NULL)
    {
#if DEBUG_MODE
        printf("No existing tree found. Starting new.\n");
#endif
        reader_management = NULL;
        return;
    }
    fclose(file);

    reader_management = loadTree(reader_management_file);
    if (reader_management == NULL)
    {
#if DEBUG_MODE
        printf("Failed to load tree. Initializing new.\n");
#endif
    }

    file = fopen(reader_name_management_file, "r");
    if (file == NULL)
    {
#if DEBUG_MODE
        printf("No existing tree found. Starting new.\n");
#endif
        reader_trie = NULL;
        return;
    }
    fclose(file);

    reader_trie = loadTrieTree(reader_name_management_file);
    if (reader_trie == NULL)
    {
#if DEBUG_MODE
        printf("Failed to load tree. Initializing new.\n");
#endif
    }
}

void print_tree_keys(Node *root)
{
#if DEBUG_MODE
    if (root == NULL)
    {
        printf("[DEBUG] Tree is empty.\n");
        return;
    }

    Node *current = root;
    while (!current->is_leaf)
    {
        current = (Node *)current->pointers[0];
    }

    printf("[DEBUG] Keys in B+ Tree: ");
    while (current != NULL)
    {
        for (int i = 0; i < current->num_keys; i++)
        {
            printf("%d ", current->keys[i]);
        }
        current = current->next;
    }
    printf("\n");
#endif
}
