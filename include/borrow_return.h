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
 * @brief Tiền phải trả cho sách quá hạn
 */
#define LATE_FEE 5000

/**
 * @brief Quá thời gian, dùng để tính phí
 */
#define OVER_DATE 14

/**
 * @brief Mã trạng thái đang mượn sách
 */
#define ON_BORROWING 1

/**
 * @brief mã trạng thái đang không được mượn
 */
#define NOT_BORROWING 0

/**
 * @brief trả về kết quả mượn sách thành công
 */
#define BORROW_SUCCESS 0

/**
 * @brief trả về kết quả mượn sách thất bại(mượn quá số lượng cho phép)
 */
#define BORROW_FAILED 1

/**
 * @brief trả về kết quả mượn sách thất bại(không đủ sách trong kho)
 */
#define BORROW_FAILED_NOT_ENOUGH_BOOK 2

/**
 * @brief Mã trạng thái đã trả sách
 */
#define BORROWED 2

/**
 * @brief không tìm thấy bản ghi -> lỗi
 */
#define NOT_FOUND 1

/**
 * @brief Đã trả rồi -> không cho trả nữa
 */
#define ALREADY_RETURNED 2

/**
 * @brief trả sách thành công
 */
#define RETURN_SUCCESS 0

/**
 * @brief nếu sách đang nằm trong danh sách mượn
 */
#define ALREADY_IN_LIST_BORROWED true

/**
 * @brief  nếu sách không bị ai mượn hoặc đã trả.
 */
#define NOT_IN_LIST_BORROWED false

/**
 * @brief Không có bản ghi nào
 */

#define NO_RECORD -1;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
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
    int status[MAX_BORROWED_BOOKS];  // 0 = đang mượn, 1 = đã trả
    bool onTime[MAX_BORROWED_BOOKS]; // 1 = đúng hạn, 0 = trễ
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
extern time_t now;
extern struct tm *local;

/**
 * @brief Thêm bản ghi mượn sách.
 */
void add_borrow_record(BorrowReturn *b);

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
 * @brief Tìm bản ghi mượn sách theo ID người đọc
 * @return Kết quả bản ghi dữ liệu
 */
BorrowReturn *search_borrow_by_reader(int readerId);

/**
 * @brief Hàm trả về một tập hợp các bản ghi mượn theo sau một Id tìm kiếm
 *
 * Dùng hàm này khi cần hứng chính xác một mảng các lần mượn. Được dùng cho
 * nhóm thao tác giao diện
 * @note Dùng xong nhớ free để tránh lãng phí bộ nhớ
 *
 * @param beginingKey là vị trí bắt đầu đọc đầu tiên
 * @param quanities là số lượng sách muốn lấy sau khi kiếm Id đầu tiên
 * @param actualBorrows là số lượng mượn thực tế
 *
 * @return là mảng chứa các lần mượn tìm được
 */
BorrowReturn *retrieve_bucket_borrows(int beginingKey, int quanities, int *actualBorrows);

/**
 * @brief Xóa bản ghi mượn theo ID người đọc.
 */
void delete_borrow_record(int readerId);

/**
 * @brief Xử lý việc trả sách (cập nhật trạng thái, phạt nếu trễ).
 */
void return_books(int readerId, int bookId);

/**
 * @brief Khôi phục lại số lượng sách vào kho (khi trả).
 */
void restore_books_to_stock(BorrowReturn *b);

/**
 * @brief Cập nhật trực tiếp dữ liệu sách vào file.
 */
void update_book_direct(Book *book);

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

/********************************************************/
/**
 * ================================
 *  Các hàm hỗ trợ giao diện (GTK)
 * ================================
 */

/**
 * @brief Mượn sách có trả về kết quả để giao diện xử lý.
 * @param b Con trỏ đến bản ghi mượn sách.
 * @return 0: thành công, 1: quá số lượng cho phép, 2: sách không đủ trong kho.
 */
int gui_add_borrow_record(BorrowReturn *b);

/**
 * @brief Trả sách có trả về kết quả để giao diện xử lý.
 * @param readerId ID người đọc.
 * @param bookId ID sách.
 * @return 0: thành công, 1: không có bản ghi, 2: đã trả rồi.
 */
int gui_return_books(int readerId, int bookId);

/**
 * @brief Kiểm tra một cuốn sách có đang được mượn hay không.
 * @param bookId ID của cuốn sách.
 * @return true nếu sách đang được mượn, false nếu không.
 */
bool gui_is_book_borrowed(int bookId);

/**
 * @brief Thống kê tổng số sách người đọc đã mượn.
 * @param readerId ID người đọc.
 * @return Số sách đã mượn, hoặc -1 nếu không có bản ghi.
 */
int gui_stat_total_books_by_reader(int readerId);

void auto_update_time();

#endif // BORROW_RETURN_H