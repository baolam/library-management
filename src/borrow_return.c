#include "borrow_return.h"
#include "books.h"
#include "utils/bplustreev2.h"
#include "reader.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEBUG_MODE 1

char borrow_return_content_file[MAX_FILE_NAME_LENGTH] = "borrow_return.bin";
char borrow_return_management_file[MAX_FILE_NAME_LENGTH] = "borrow_return_management.bin";

Node *borrow_return_management = NULL;

void add_borrow_record(BorrowReturn *b)
{
    if (b == NULL || b->totalBooks <= 0 || b->totalBooks > 100)
    {
        printf("Error: Invalid borrow record.\n");
        return;
    }

    printf("Checking reader_management=%p\n", reader_management);
    Record *reader_record = find(reader_management, b->readerId);
    if (reader_record == NULL || reader_record->deleted)
    {
        printf("Error: Reader ID %d does not exist.\n", b->readerId);
        return;
    }

    printf("Checking book_management=%p\n", book_management);
    for (int i = 0; i < b->totalBooks; i++)
    {
        Book *book = search_book(b->bookIds[i]);
        if (book == NULL)
        {
            printf("Error: Book ID %d does not exist.\n", b->bookIds[i]);
            return;
        }
        if (book->stock < b->quantities[i])
        {
            printf("Error: Not enough stock for Book ID %d. Available: %d, Requested: %d.\n",
                   b->bookIds[i], book->stock, b->quantities[i]);
            return;
        }
        book->stock -= b->quantities[i];
        update_book_direct(book);
    }

    printf("Opening file %s\n", borrow_return_content_file);
    FILE *f = fopen(borrow_return_content_file, "ab");
    if (f == NULL)
    {
        printf("Error: Cannot open borrow file: %s\n", strerror(errno));
        return;
    }
    fclose(f);

    b->status = 0;

    printf("Adding to borrow_return_management=%p\n", borrow_return_management);
    Node *new_tree = add_content(
        borrow_return_management,
        b->readerId,
        borrow_return_content_file,
        b,
        sizeof(BorrowReturn),
        NULL);

    if (new_tree == NULL)
    {
        printf("Error: Failed to add borrow record.\n");
    }
    else
    {
        borrow_return_management = new_tree;
        printf("Borrow record added successfully.\n");
    }
}

// Các hàm khác giữ nguyên
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
        printf("No borrow record found.\n");
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
        printf("No borrow record found.\n");
        return;
    }

    BorrowReturn b;
    FILE *f = fopen(record->_from, "rb");
    if (f == NULL)
    {
        printf("Error: Cannot open file.\n");
        return;
    }
    fseek(f, record->offset, SEEK_SET);
    fread(&b, sizeof(BorrowReturn), 1, f);
    fclose(f);

    int total = 0;
    for (int i = 0; i < b.totalBooks; i++)
    {
        total += b.quantities[i];
    }

    printf("Reader %d has borrowed a total of %d books.\n", readerId, total);
}

void return_books(int readerId)
{
    Record *record = find(borrow_return_management, readerId);
    if (record == NULL || record->deleted)
    {
        printf("No borrow record found.\n");
        return;
    }

    BorrowReturn b;
    FILE *f = fopen(record->_from, "rb+");
    if (f == NULL)
    {
        printf("Error: Cannot open file.\n");
        return;
    }
    fseek(f, record->offset, SEEK_SET);
    fread(&b, sizeof(BorrowReturn), 1, f);

    if (b.status == 1)
    {
        printf("Books already returned.\n");
        fclose(f);
        return;
    }

    restore_books_to_stock(&b);

    printf("Was the return on time? (1 = Yes, 0 = No): ");
    int temp;
    scanf("%d", &temp);
    b.onTime = temp ? true : false;

    while (getchar() != '\n')
        ;

    if (!b.onTime)
    {
        int fee_per_book = 5000;
        int total = 0;
        for (int i = 0; i < b.totalBooks; i++)
        {
            total += b.quantities[i] * fee_per_book;
        }
        printf("Late return! Total fee: %d VND\n", total);
    }
    else
    {
        printf("Books returned on time. No late fee.\n");
    }

    b.status = 1;
    fseek(f, record->offset, SEEK_SET);
    fwrite(&b, sizeof(BorrowReturn), 1, f);
    fclose(f);

    printf("Return processed successfully.\n");
}

void restore_books_to_stock(BorrowReturn *b)
{
    for (int i = 0; i < b->totalBooks; i++)
    {
        Book *book = search_book(b->bookIds[i]);
        if (book != NULL)
        {
            book->stock += b->quantities[i];
            update_book_direct(book);
            printf("Restored %d copies of Book ID %d.\n", b->quantities[i], b->bookIds[i]);
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

bool check_book_in_borrow(int bookId)
{
    FILE *f = fopen(borrow_return_content_file, "rb");
    if (f == NULL)
        return false;

    BorrowReturn b;
    while (fread(&b, sizeof(BorrowReturn), 1, f))
    {
        if (b.status == 0)
        {
            for (int i = 0; i < b.totalBooks; i++)
            {
                if (b.bookIds[i] == bookId)
                {
                    fclose(f);
                    return true;
                }
            }
        }
    }
    fclose(f);
    return false;
}

void save_borrow_return_management()
{
    saveTree(borrow_return_management, borrow_return_management_file);
}

void load_borrow_return_management()
{
    borrow_return_management = loadTree(borrow_return_management_file);
}