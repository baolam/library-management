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

int system_date = 0;
int system_current_year = 0;

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

    system_current_year = local->tm_year + 1900;
    system_date = local->tm_yday + 1;
}

void add_borrow_record(BorrowReturn *b)
{
    if (b == NULL || b->totalBooks < 0 || b->totalBooks >= MAX_BORROWED_BOOKS)
        return;

    Record *reader_record = find(reader_management, b->readerId);
    if (reader_record == NULL || reader_record->deleted)
        return;

    for (int i = 0; i < b->totalBooks; i++)
    {
        Book *book = search_book(b->infors[i].bookId);
        if (book == NULL)
            return;

        if (book->stock < b->infors[i].quantity)
        {
            printf("Error: Not enough stock for Book ID %d. Available: %d, Requested: %d.\n",
                   b->infors[i].bookId, book->stock, b->infors[i].quantity);
            return;
        }

        book->stock -= b->infors[i].quantity;
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
        printf("  - Book ID: %d | Quantity: %d | Status : %s | Ontime : %s \n", b.infors[i].bookId, b.infors[i].quantity, b.infors[i].status == ON_BORROWING ? "Borrowing" : "Returned", b.infors[i].onTime ? "On Time" : "Late");
    }
    printf("Date : %d, Year : %d \n", b.infors[0].date, b.infors[0].current_year);
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

BorrowReturn *retrieve_bucket_borrows(int beginingKey, int quanities, int *actualBorrows)
{
    int storage_pos = 0;

    BorrowReturn *borrows = (BorrowReturn *)malloc(quanities * sizeof(BorrowReturn));
    if (borrows == NULL)
    {
        *actualBorrows = 0;
        return NULL;
    }

    Node *n = findLeaf(borrow_return_management, beginingKey);
    if (n == NULL)
    {
        *actualBorrows = 0;
        return NULL;
    }

    int startSearch = getStartSearch(n, beginingKey);
    int i;

    while (n != NULL && quanities > 0)
    {
        for (i = startSearch; i < n->num_keys && quanities > 0; i++)
        {
            if (!exist_record(borrow_return_management, n->keys[i]))
                continue;

            Record *record = (Record *)n->pointers[i];
            BorrowReturn *borrow = (BorrowReturn *)read_content_from_record_return(record);

            if (borrow == NULL)
                continue;

            borrows[storage_pos] = *borrow;
            storage_pos++;
            quanities--;
        }

        startSearch = 0;
        n = n->pointers[ORDER - 1];
    }

    *actualBorrows = storage_pos;
    return borrows;
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

int getPosition(BookBorrowInfor infor[], int size, int searchId)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (searchId == infor[i].bookId)
            return i;
    }
    return -1;
}

void update_borrow_infor(BorrowReturn *borrow_return)
{
    Record *record = find(borrow_return_management, borrow_return->readerId);
    if (record == NULL || record->deleted)
    {
        printf("Da xoa, co loi, khong the ghi nhan Borrow!\n");
        return;
    }

    int status = update_content_without_callback(record, borrow_return);
    if (status == UPDATE_SUCCESS)
    {
        printf("Cap nhat thanh cong!\n");
    }
    else
    {
        printf("Cap nhat that bai \n");
    }
}

bool exist_bookid_bookborrow(BorrowReturn *b, int bookId)
{
    int i;
    for (i = 0; i < b->totalBooks; i++)
    {
        if (b->infors[i].bookId == bookId)
            return true;
    }
    return false;
}

bool add_bookborrow(BorrowReturn *b, int bookId, int quanities)
{
    bool existed = getPosition(b->infors, b->totalBooks, bookId) != -1;
    if (existed)
    {
        printf("Da ton tai Book Id \n");
        return false;
    }

    BookBorrowInfor infor;

    infor.bookId = bookId;
    infor.quantity = quanities;
    infor.onTime = false;
    infor.status = ON_BORROWING;
    infor.date = system_date;
    infor.current_year = system_current_year;

    b->infors[b->totalBooks] = infor;
    b->totalBooks++;

    Book *effected_book = search_book(bookId);
    effected_book->stock -= quanities;

    update_book_direct(effected_book);
    update_borrow_infor(b);

    return true;
}

void delete_bookborrow(BorrowReturn *b, int bookId)
{
}

int return_books(int readerId, int bookId)
{
    int late_fees = 0;

    Record *record = find(borrow_return_management, readerId);
    if (record == NULL || record->deleted)
        return 0;

    BorrowReturn b;
    FILE *f = fopen(record->_from, "rb+");
    if (f == NULL)
        return 0;

    fseek(f, record->offset, SEEK_SET);
    fread(&b, sizeof(BorrowReturn), 1, f);

    // show_borow(b);

    int index = getPosition(b.infors, b.totalBooks, bookId);
    if (index == -1)
        return 0;
    if (b.infors[index].status != ON_BORROWING)
        return 0;

    restore_books_to_stock(&b);

    int day = calculate_day_difference(b.infors[index].date, b.infors[index].current_year);
    b.infors[index].onTime = day <= OVER_DATE ? true : false;

    if (!b.infors[index].onTime)
    {
        int total = 0;
        for (int i = 0; i < b.totalBooks; i++)
        {
            total += b.infors[i].quantity * LATE_FEE;
        }
        printf("Late return! Total fee: %d VND\n", total);
        late_fees = total;
    }
    else
    {
        printf("Books returned on time. No late fee.\n");
    }

    b.infors[index].status = BORROWED;

    fseek(f, record->offset, SEEK_SET);
    fwrite(&b, sizeof(BorrowReturn), 1, f);
    fclose(f);

    // show_borow(b);

    printf("Return processed successfully.\n");
    return late_fees;
}

// Cập nhật tồn kho sách
void restore_books_to_stock(BorrowReturn *b)
{
    for (int i = 0; i < b->totalBooks; i++)
    {
        Book *book = search_book(b->infors[i].bookId);
        if (book != NULL)
        {
            book->stock += b->infors[i].quantity;
            update_book_direct(book);
            printf("Restored %d copies of Book ID %d.\n", b->infors[i].quantity, b->infors[i].bookId);
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
        Book *book = search_book(b->infors[i].bookId);
        if (!book || book->stock < b->infors[i].quantity)
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
    int index = getPosition(borrow->infors, borrow->totalBooks, bookId);

    if (borrow->infors[index].status == BORROWED)
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
