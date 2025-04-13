#ifndef BORROW_RETURN_H
#define BORROW_RETURN_H

#include <stdio.h>
#include <stdlib.h>

#include "books.h"
#include "reader.h"
#include "bplustreev2.h"

// Cấu trúc BorrowReturn
typedef struct BorrowReturn
{
    int readerId;
    int totalBooks;
    int bookIds[100];
    int quantities[100];
    int status; // 0 = đang mượn, 1 = đã trả
    int onTime; // 1 = đúng hạn, 0 = trễ
    int date; // phong
    int current_year;
} BorrowReturn;

// Biến toàn cục
extern Node *borrow_return_management;

// Khai báo file
extern char borrow_return_content_file[MAX_FILE_NAME_LENGTH];
extern char borrow_return_management_file[MAX_FILE_NAME_LENGTH];
extern int date; // phong
extern int current_year;

// Hàm xử lý
void add_borrow_record(BorrowReturn *b, int day, int month, int year);
void show_borrow_record(FILE *f, long size);
void search_borrow_record_by_reader(int readerId);
void delete_borrow_record(int readerId);
void stat_total_books_by_reader(int readerId);
void return_books(int readerId);

// Các hàm phụ
void restore_books_to_stock(BorrowReturn *b);
void update_book_direct(Book *book);
void update_date(int day, int month, int year); // Phong
int calculate_day_difference(int borrow_date, int borrow_year);
void load_borrow_return_management();

#endif
