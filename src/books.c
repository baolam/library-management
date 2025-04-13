#include "books.h"
#include "borrow_return.h"
#include <stdio.h>
#include <string.h>

char book_management_file[MAX_FILE_NAME_LENGTH] = "data/book/book_management.bin";
char book_trie_management[MAX_FILE_NAME_LENGTH] = "data/book/book_trie.bin";
char book_content_file[MAX_FILE_NAME_LENGTH] = "data/book/book.bin";

Node *book_management = NULL;
TrieNode *book_trie = NULL;

void show_book(Book book)
{
    printf("===== Book Info ===== \n");
    printf("Id: %d\n", book.bookId);
    printf("Title: %s\n", book.title);
    printf("Author: %s\n", book.author);
    printf("Genre: %s\n", book.genre);
    printf("Year: %d\n", book.publicationYear);
    printf("Stock: %d\n", book.stock);
    printf("-------------------------\n");
}

void add_book_callback(int id, int code, long offset, long length)
{
    if (code == ADD_CONTENT_SUCCESS)
    {
        printf("Id: %d, Successfully added!\n", id);
    }
    else
    {
        printf("Id: %d, Failed to add book.\n", id);
    }
}

void add_book(Book *book)
{
    if (book == NULL)
    {
        printf("Error: Book pointer is NULL.\n");
        return;
    }

    if (exist_record(book_management, book->bookId))
    {
        printf("Book ID %d already exists!\n", book->bookId);
        return;
    }

    // Đảm bảo file tồn tại
    FILE *f = fopen(book_content_file, "ab");
    if (f == NULL)
    {
        printf("Error: Cannot open book content file %s\n", book_content_file);
        return;
    }
    fclose(f);

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
    else
    {
        printf("Error: Failed to add book to B+ tree.\n");
        return;
    }

    // Kiểm tra và khởi tạo book_trie nếu cần
    if (book_trie == NULL)
    {
        book_trie = makeTrieNode();
        if (book_trie == NULL)
        {
            printf("Error: Failed to initialize book trie.\n");
            return;
        }
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
        printf("Book was already deleted.\n");
    }
    else
    {
        printf("Failed to delete book.\n");
    }
}

void delete_book(int id)
{
    // Kiểm tra xem sách có đang được mượn không
    // (Cần hàm tìm tất cả bản ghi mượn chứa bookId, hiện tại chưa có)
    // Giả sử bạn thêm hàm `check_book_in_borrow` trong borrow_return.c
    if (check_book_in_borrow(id))
    {
        printf("Error: Book ID %d is currently borrowed. Cannot delete.\n", id);
        return;
    }

    soft_delete(book_management, id, delete_book_callback);
}

void update_book_callback(FILE *f, long package)
{
    if (f == NULL)
    {
        printf("Error: Cannot read book record for update.\n");
        return;
    }

    Book book;
    fread(&book, sizeof(Book), 1, f);
    printf("Current information: \n");
    show_book(book);

    // Xóa bộ đệm nhập liệu trước khi đọc chuỗi
    while (getchar() != '\n')
        ; // Xóa bộ đệm

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

    // Ghi đè thông tin mới vào file
    fseek(f, -sizeof(Book), SEEK_CUR); // Quay lại vị trí bắt đầu của bản ghi
    fwrite(&book, sizeof(Book), 1, f);
    printf("Update successfully!\n");
}

void update_book(int id)
{
    Record *record = find(book_management, id);
    if (record == NULL || record->deleted)
    {
        printf("Book not found for update.\n");
        return;
    }

    update_content_from_record(record, update_book_callback);
}

void show_book_record(FILE *f, long size)
{
    if (f == NULL)
    {
        printf("Error: Cannot read book record.\n");
        return;
    }

    Book book;
    fread(&book, sizeof(Book), 1, f);
    show_book(book);
}

void search_book_by_id(int id)
{
    Record *record = find(book_management, id);
    if (record == NULL || record->deleted)
    {
        printf("Book with ID %d not found.\n", id);
        return;
    }

    read_content_from_record(record, show_book_record);
}

void search_book_by_title(const char *prefix, int maxNumbers)
{
    if (book_trie == NULL)
    {
        printf("Error: Book trie is not initialized.\n");
        return;
    }

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

Book *search_book(int id)
{
    Record *record = find(book_management, id);
    if (record == NULL || record->deleted)
    {
        return NULL;
    }

    Book *book = (Book *)read_content_from_record_return(record);
    return book;
}

void prepare_book()
{
    if (book_trie == NULL)
    {
        book_trie = makeTrieNode();
        if (book_trie == NULL)
        {
            printf("Error: Failed to initialize book trie.\n");
            return;
        }
    }

    // Đảm bảo file tồn tại
    FILE *file = fopen(book_management_file, "a");
    if (file == NULL)
    {
        printf("Error: Cannot create book management file %s\n", book_management_file);
        return;
    }
    fclose(file);

    file = fopen(book_trie_management, "a");
    if (file == NULL)
    {
        printf("Error: Cannot create book trie management file %s\n", book_trie_management);
        return;
    }
    fclose(file);

    file = fopen(book_content_file, "a");
    if (file == NULL)
    {
        printf("Error: Cannot create book content file %s\n", book_content_file);
        return;
    }
    fclose(file);
}

void save_book_management()
{
    saveTree(book_management, book_management_file);
    saveTrieTree(book_trie_management, book_trie);
}

void getInfor(int *total, int *deleted)
{
    // TODO: Triển khai nếu cần
}

bool load_book_management()
{
    book_management = loadTree(book_management_file);
    if (book_management == NULL)
    {
        printf("Failed to load B+ Tree management!\n");
        // Tiếp tục thử tải book_trie ngay cả khi book_management thất bại
    }
    else
    {
        printf("Load B+ Tree management successfully!\n");
    }

    book_trie = loadTrieTree(book_trie_management);
    if (book_trie == NULL)
    {
        printf("Failed to load Trie management!\n");
        // Nếu cả hai đều thất bại, trả về false
        if (book_management == NULL)
        {
            return false;
        }
    }
    else
    {
        printf("Load Trie management successfully!\n");
    }

    // Trả về true nếu ít nhất một trong hai thành công
    return (book_management != NULL || book_trie != NULL);
}