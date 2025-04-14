/**
 * @file borrow_return.h
 * @brief Quản lý việc mượn và trả sách trong hệ thống thư viện.
 *
 * Bao gồm các hàm xử lý mượn sách, trả sách, thống kê và khôi phục tồn kho.
 */

#ifndef BORROW_RETURN_H
#define BORROW_RETURN_H

/**
 * @brief Số lượng sách mượn tối đa
 */
#define MAX_BORROWED_BOOKS 100

/**
 * @brief Mã trạng thái đang mượn sách
 */
#define ON_BORROWING 0

/**
 * @brief Mã trạng thái đã trả sách
 */
#define BORROWED 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "books.h"
#include "reader.h"
#include "utils/bplustreev2.h"

/**
 * @brief Struct đại diện cho một bản ghi mượn-trả sách.
 */
typedef struct BorrowReturn
{
    int readerId;
    int totalBooks;
    int bookIds[MAX_BORROWED_BOOKS];
    int quantities[MAX_BORROWED_BOOKS];
    int status;  // 0 = đang mượn, 1 = đã trả
    bool onTime; // 1 = đúng hạn, 0 = trễ
    int date;
    int current_year;
} BorrowReturn;

// Biến toàn cục
extern Node *borrow_return_management;

// Khai báo file
extern char borrow_return_content_file[MAX_FILE_NAME_LENGTH];
extern char borrow_return_management_file[MAX_FILE_NAME_LENGTH];
extern int date;
extern int current_year;

/**
 * @brief Thêm bản ghi mượn sách.
 */
void add_borrow_record(BorrowReturn *b, int day, int month, int year);

/**
 * @brief Hàm hiển thị bản ghi
 */
void show_borow(BorrowReturn b);

/**
 * @brief Hiển thị các bản ghi mượn sách từ file.
 * @param f Con trỏ file.
 * @param size Kích thước file.
 */
void show_borrow_record(FILE *f, long size);

/**
 * @brief Tìm bản ghi mượn sách theo ID người đọc.
 */
void search_borrow_record_by_reader(int readerId);

/**
 * @brief Xóa bản ghi mượn theo ID người đọc.
 */
void delete_borrow_record(int readerId);

/**
 * @brief Thống kê tổng số sách đã mượn bởi người đọc.
 */
void stat_total_books_by_reader(int readerId);

/**
 * @brief Xử lý việc trả sách (cập nhật trạng thái, phạt nếu trễ).
 */
void return_books(int readerId);

/**
 * @brief Khôi phục lại số lượng sách vào kho (khi trả).
 */
void restore_books_to_stock(BorrowReturn *b);

/**
 * @brief Cập nhật trực tiếp dữ liệu sách vào file.
 */
void update_book_direct(Book *book);

void update_date(int day, int month, int year); // Phong
int calculate_day_difference(int borrow_date, int borrow_year);

/**
 * @brief Kiểm tra xem một cuốn sách có đang được mượn hay không.
 * @param bookId ID của cuốn sách cần kiểm tra.
 * @return true nếu sách đang được mượn, false nếu không.
 */
bool check_book_in_borrow(int bookId);

/**
 * @brief Lưu trữ việc quản lí mượn trả
 */
void save_borrow_return_management();

/**
 * @brief Tải dữ liệu quản lí mượn trả từ file
 */
void load_borrow_return_management();

#endif // BORROW_RETURN_H