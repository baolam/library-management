#include "borrow_return.h"

// Biến toàn cục
char borrow_return_content_file[MAX_FILE_NAME_LENGTH] = "borrow_return.bin";
char borrow_return_management_file[MAX_FILE_NAME_LENGTH] = "borrow_return_management.bin";
Node *borrow_return_management = NULL;
int date = 0; // Phong
int current_year = 0;

// ------------------- borrow / return -------------------
void add_borrow_record(BorrowReturn *b, int day, int month, int year)
{
    date = 0; // reset trước khi tính lại
    update_date(day, month, year);
    b->date = date;
    b->current_year = current_year;

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

    //  Trả sách
    restore_books_to_stock(&b);
    
    int days = calculate_day_difference(b.date, b.current_year);
    b.onTime = days <= 14 ? 1 : 0;

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

    b.status = 1;
    fseek(f, record->offset, SEEK_SET);
    fwrite(&b, sizeof(BorrowReturn), 1, f);
    fclose(f);
    printf("Borrow record updated to 'returned'.\n");
}

//  Cập nhật tồn kho sách
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

// Phong
void update_date(int day, int month, int year) 
{
    for(int i = 1; i < month; i++){
        if(i > 12) break;
        if((i % 2 == 0 && i < 8) || (i % 2 != 0 && i > 8)){
            if(i == 2){
                if(i % 4 == 0 && i % 100 != 0){
                    date += 29;
                    continue;
                }
                date += 28;
                continue;
            }
            date += 30;
            continue;
        }
        date += 31;
    }
    date += day;
    current_year = year;
}

int calculate_day_difference(int borrow_date, int borrow_year)
{
    int year_diff = current_year - borrow_year;
    return year_diff * 365 + (date - borrow_date);
}
void load_borrow_return_management()
{
    borrow_return_management = loadTree(borrow_return_management_file);
    if (borrow_return_management == NULL)
    {
        printf("Failed to load B+ Tree management for borrow/return!\n");
    }
    else
    {
        printf("Load B+ Tree management for borrow/return successfully!\n");
    }
}