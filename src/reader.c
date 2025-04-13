#include "reader.h"
#include "borrow_return.h"
#include <errno.h> // Thêm để lấy mã lỗi hệ thống
#define TEST_READER
#define DEBUG_MODE 0

char reader_management_file[MAX_FILE_NAME_LENGTH] = "data/reader/reader_management.bin";
char reader_name_management_file[MAX_FILE_NAME_LENGTH] = "data/reader/reader_name_management.bin";
char reader_content_file[MAX_FILE_NAME_LENGTH] = "data/reader/reader.bin";

Node *reader_management = NULL;
TrieNode *reader_trie = NULL;

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
    if (f == NULL)
    {
        printf("Error: Cannot read reader record.\n");
        return;
    }
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
    if (reader_trie == NULL)
    {
        printf("Error: Reader trie is not initialized.\n");
        return;
    }

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
#if DEBUG_MODE
    if (code == ADD_CONTENT_SUCCESS)
    {
        printf("[DEBUG] Successfully added reader ID %d at offset %ld\n", id, offset);
    }
    else
    {
        printf("[DEBUG] Failed to add reader ID %d. Error code: %d\n", id, code);
    }
#endif
    // Luôn in thông báo cho người dùng
    if (code == ADD_CONTENT_SUCCESS)
    {
        printf("Successfully added reader ID %d!\n", id);
    }
    else
    {
        printf("Failed to add reader ID %d.\n", id);
    }
}

void add_reader(Readers *r)
{
    if (r == NULL)
    {
        printf("[DEBUG] Error: Reader pointer is NULL\n");
        return;
    }

    int key = r->readerId;
#if DEBUG_MODE
    printf("[DEBUG] Adding reader ID = %d\n", key);
#endif

    if (exist_record(reader_management, key))
    {
        printf("Reader ID already exists!\n");
        return;
    }

#if DEBUG_MODE
    printf("[DEBUG] Checking file %s\n", reader_content_file);
#endif
    FILE *f = fopen(reader_content_file, "ab");
    if (f == NULL)
    {
#if DEBUG_MODE
        printf("[DEBUG] Error: Cannot open file %s (errno: %d)\n", reader_content_file, errno);
#endif
        printf("Error: Cannot open reader content file.\n");
        return;
    }
#if DEBUG_MODE
    printf("[DEBUG] File %s opened successfully\n", reader_content_file);
#endif
    fclose(f);

#if DEBUG_MODE
    if (reader_management == NULL)
    {
        printf("[DEBUG] reader_management is NULL\n");
    }
    else
    {
        printf("[DEBUG] reader_management is initialized\n");
    }
#endif

#if DEBUG_MODE
    printf("[DEBUG] Calling add_content for reader ID %d\n", key);
#endif
    Node *new_tree = add_content(
        reader_management,
        key,
        reader_content_file,
        r,
        sizeof(Readers),
        add_reader_callback);

    if (new_tree == NULL)
    {
#if DEBUG_MODE
        printf("[DEBUG] Failed to add reader to B+ tree\n");
#endif
    }
    else
    {
        reader_management = new_tree;
#if DEBUG_MODE
        printf("[DEBUG] Successfully updated B+ tree\n");
        print_tree_keys(reader_management);
#endif
    }

    // Kiểm tra và khởi tạo reader_trie nếu cần
    if (reader_trie == NULL)
    {
#if DEBUG_MODE
        printf("[DEBUG] reader_trie is NULL, initializing...\n");
#endif
        reader_trie = makeTrieNode();
        if (reader_trie == NULL)
        {
            printf("Error: Failed to initialize reader trie.\n");
            return;
        }
    }
#if DEBUG_MODE
    printf("[DEBUG] Inserting %s into trie\n", r->fullName);
#endif
    insertIntoTrie(reader_trie, r->fullName, key);
}

void update_reader_callback(FILE *f, long size)
{
    if (f == NULL)
    {
        printf("Error: Cannot read reader record for update.\n");
        return;
    }

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

    printf("Enter new address: ");
    fgets(r.address, sizeof(r.address), stdin);
    r.address[strcspn(r.address, "\n")] = 0;

    // Ghi đè thông tin mới vào file
    fseek(f, -sizeof(Readers), SEEK_CUR); // Quay lại vị trí bắt đầu của bản ghi
    fwrite(&r, sizeof(Readers), 1, f);

    printf("Update successful!\n");
}

void update_reader(Readers *reader)
{
    if (reader == NULL)
    {
        printf("Error: Reader pointer is NULL.\n");
        return;
    }

    int key = reader->readerId;
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
    // Kiểm tra xem người đọc có đang mượn sách không
    Record *borrow_record = find(borrow_return_management, id);
    if (borrow_record != NULL && !borrow_record->deleted)
    {
        BorrowReturn b;
        FILE *f = fopen(borrow_record->_from, "rb");
        if (f == NULL)
        {
            printf("Error: Cannot open borrow record file %s\n", borrow_record->_from);
            return;
        }
        fseek(f, borrow_record->offset, SEEK_SET);
        if (fread(&b, sizeof(BorrowReturn), 1, f) != 1)
        {
            printf("Error: Failed to read borrow record for Reader ID %d\n", id);
            fclose(f);
            return;
        }
        fclose(f);
        if (b.status == 0) // Đang mượn
        {
            printf("Error: Reader ID %d is currently borrowing books. Cannot delete.\n", id);
            return;
        }
    }

    soft_delete(reader_management, id, delete_reader_callback);
}

// ------------------- Save / Load Tree -------------------

void prepare_reader()
{
    if (reader_trie == NULL)
    {
        reader_trie = makeTrieNode();
        if (reader_trie == NULL)
        {
            printf("Error: Failed to initialize reader trie.\n");
        }
    }

    // Đảm bảo file tồn tại
    FILE *file = fopen(reader_management_file, "a");
    if (file == NULL)
    {
        printf("Error: Cannot create reader management file %s\n", reader_management_file);
        return;
    }
    fclose(file);

    file = fopen(reader_name_management_file, "a");
    if (file == NULL)
    {
        printf("Error: Cannot create reader name management file %s\n", reader_name_management_file);
        return;
    }
    fclose(file);

    file = fopen(reader_content_file, "a");
    if (file == NULL)
    {
        printf("Error: Cannot create reader content file %s\n", reader_content_file);
        return;
    }
    fclose(file);
}

void save_reader_management()
{
    saveTree(reader_management, reader_management_file);
    saveTrieTree(reader_name_management_file, reader_trie);
}

bool load_reader_management()
{
    FILE *file = fopen(reader_management_file, "r");
    if (file == NULL)
    {
#if DEBUG_MODE
        printf("No existing tree found. Starting new.\n");
#endif
        reader_management = NULL;
        return false;
    }
    fclose(file);

    reader_management = loadTree(reader_management_file);
    if (reader_management == NULL)
    {
#if DEBUG_MODE
        printf("Failed to load tree. Initializing new.\n");
#endif
        return false;
    }

    file = fopen(reader_name_management_file, "r");
    if (file == NULL)
    {
#if DEBUG_MODE
        printf("No existing tree found. Starting new.\n");
#endif
        reader_trie = NULL;
        return false;
    }
    fclose(file);

    reader_trie = loadTrieTree(reader_name_management_file);
    if (reader_trie == NULL)
    {
#if DEBUG_MODE
        printf("Failed to load tree. Initializing new.\n");
#endif
        return false;
    }

    return true;
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