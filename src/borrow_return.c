#include "borrow_return.h"
#include "books.h"
#include "utils/bplustreev2.h"
#include "reader.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEBUG_MODE 1

char borrow_return_content_file[MAX_FILE_NAME_LENGTH] = "data/borrow/borrow_return.bin";
char borrow_return_management_file[MAX_FILE_NAME_LENGTH] = "data/borrow/borrow_return_management.bin";

Node *borrow_return_management = NULL;

int date = 0; // Phong
int current_year = 0;

time_t now;
struct tm *local;

void add_borrow_callback(int id, int code, long offset, long length)
{
    if (code == ADD_CONTENT_FAILED)
    {
        printf("Id : %d, failed to add!\n", id);
    }
    else
    {
        printf("Id : %d, added!\n", id);
    }
}

void auto_update_time()
{
    time(&now);
    local = localtime(&now);

    current_year = local->tm_year + 1900;
    date = local->tm_yday + 1;
}

void add_borrow_record(BorrowReturn *b)
{
    if (b == NULL || b->totalBooks <= 0 || b->totalBooks >= MAX_BORROWED_BOOKS)
        return;

    Record *reader_record = find(reader_management, b->readerId);
    if (reader_record == NULL || reader_record->deleted)
        return;

    for (int i = 0; i < b->totalBooks; i++)
    {
        Book *book = search_book(b->bookIds[i]);
        if (book == NULL)
            return;

        if (book->stock < b->quantities[i])
        {
            printf("Error: Not enough stock for Book ID %d. Available: %d, Requested: %d.\n",
                   b->bookIds[i], book->stock, b->quantities[i]);
            return;
        }
        book->stock -= b->quantities[i];
        update_book_direct(book);
    }

    FILE *f = fopen(borrow_return_content_file, "ab");
    if (f == NULL)
        return;

    fclose(f);

    borrow_return_management = add_content(
        borrow_return_management,
        b->readerId,
        borrow_return_content_file,
        b,
        sizeof(BorrowReturn),
        add_borrow_callback);
}

// Các hàm khác giữ nguyên
void show_borrow_record(FILE *f, long size)
{
    BorrowReturn b;
    fread(&b, sizeof(BorrowReturn), 1, f);
    show_borow(b);
}

void show_borow(BorrowReturn b)
{
    printf("Reader ID: %d\n", b.readerId);
    printf("Borrowed books:\n");
    for (int i = 0; i < b.totalBooks; i++)
    {
        printf("  - Book ID: %d | Quantity: %d | Status : %s\n", b.bookIds[i], b.quantities[i], b.status[i] == ON_BORROWING ? "Borrowing" : "Returned");
    }
    printf("Date : %d, Year : %d \n", b.date, b.current_year);
    printf("\n");
}

void search_borrow_record_by_reader(int readerId)
{
    Record *record = find(borrow_return_management, readerId);
    if (record == NULL || record->deleted)
        return;
    read_content_from_record(record, show_borrow_record);
}

BorrowReturn *search_borrow_by_reader(int readerId)
{
    Record *record = find(borrow_return_management, readerId);
    if (record == NULL || record->deleted)
        return NULL;
    BorrowReturn *result = (BorrowReturn *)read_content_from_record_return(record);
    return result;
}

void delete_borrow_record_callback(int code)
{
    if (code == DELETE_FAILED)
    {
        printf("Failed to delete!\n");
    }
    else if (code == DELETE_EXISTED)
    {
        printf("Has deleted before!\n");
    }
    else
    {
        printf("Delete successfully!\n");
    }
}

void delete_borrow_record(int readerId)
{
    soft_delete(borrow_return_management, readerId, delete_borrow_record_callback);
}

int getPosition(int bookId[], int size, int searchId)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (searchId == bookId[i])
            return i;
    }
    return -1;
}

void return_books(int readerId, int bookId)
{
    Record *record = find(borrow_return_management, readerId);
    if (record == NULL || record->deleted)
        return;

    BorrowReturn b;
    FILE *f = fopen(record->_from, "rb+");
    if (f == NULL)
        return;

    fseek(f, record->offset, SEEK_SET);
    fread(&b, sizeof(BorrowReturn), 1, f);

    show_borow(b);

    int index = getPosition(b.bookIds, b.totalBooks, bookId);
    if (b.status[index] != ON_BORROWING)
        return;

    restore_books_to_stock(&b);
    int day = calculate_day_difference(b.date, b.current_year);
    b.onTime[index] = day <= OVER_DATE ? true : false;

    if (!b.onTime[index])
    {
        int total = 0;
        for (int i = 0; i < b.totalBooks; i++)
        {
            total += b.quantities[i] * LATE_FEE;
        }
        printf("Late return! Total fee: %d VND\n", total);
    }
    else
    {
        printf("Books returned on time. No late fee.\n");
    }

    b.status[index] = BORROWED;

    fseek(f, record->offset, SEEK_SET);
    fwrite(&b, sizeof(BorrowReturn), 1, f);
    fclose(f);

    show_borow(b);

    printf("Return processed successfully.\n");
}

// Cập nhật tồn kho sách
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
        return;

    FILE *f = fopen(record->_from, "rb+");
    if (f == NULL)
        return;

    fseek(f, record->offset, SEEK_SET);
    fwrite(book, sizeof(Book), 1, f);
    fclose(f);
}

bool check_book_in_borrow(int bookId)
{
    FILE *f = fopen(borrow_return_content_file, "rb");
    if (f == NULL)
        return NOT_IN_LIST_BORROWED;

    /**
     *
     * Phương thức đọc dữ liệu ở đây bị sai
     */
    // BorrowReturn b;
    // while (fread(&b, sizeof(BorrowReturn), 1, f))
    // {
    //     show_borow(b);
    //     if (b.status == ON_BORROWING)
    //     {
    //         for (int i = 0; i < b.totalBooks; i++)
    //         {
    //             if (b.bookIds[i] == bookId)
    //             {
    //                 fclose(f);
    //                 return ALREADY_IN_LIST_BORROWED;
    //             }
    //         }
    //     }
    // }

    fclose(f);
    return NOT_IN_LIST_BORROWED;
}

void save_borrow_return_management()
{
    saveTree(borrow_return_management, borrow_return_management_file);
}

void load_borrow_return_management()
{
    borrow_return_management = loadTree(borrow_return_management_file);
}
/**
 * ================================
 *  Các hàm hỗ trợ giao diện (GTK)
 * ================================
 */
int gui_add_borrow_record(BorrowReturn *b)
{
    if (b->totalBooks > MAX_BORROWED_BOOKS)
        return BORROW_FAILED;

    for (int i = 0; i < b->totalBooks; ++i)
    {
        Book *book = search_book(b->bookIds[i]);
        if (!book || book->stock < b->quantities[i])
            return BORROW_FAILED_NOT_ENOUGH_BOOK;
    }

    add_borrow_record(b);
    return BORROW_SUCCESS;
}

int gui_return_books(int readerId, int bookId)
{
    Record *record = find(borrow_return_management, readerId);
    if (!record || record->deleted)
        return NOT_FOUND;

    BorrowReturn *borrow = (BorrowReturn *)read_content_from_record_return(record);
    int index = getPosition(borrow->bookIds, borrow->totalBooks, bookId);

    if (borrow->status[index] == BORROWED)
        return ALREADY_RETURNED;

    return_books(readerId, bookId);
    return RETURN_SUCCESS;
}

bool gui_is_book_borrowed(int bookId)
{
    if (check_book_in_borrow(bookId))
    {
        return ON_BORROWING; // Sách đang được mượn
    }
    return NOT_BORROWING; // Sách không được mượn
}

int gui_stat_total_books_by_reader(int readerId)
{
    Record *record = find(borrow_return_management, readerId);
    if (record == NULL || record->deleted)
        return NO_RECORD;

    BorrowReturn *b = (BorrowReturn *)read_content_from_record_return(record);
    if (b == NULL)
        return NO_RECORD;

    return b->totalBooks;
}
