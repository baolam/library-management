/**
 * @file borrow_return.h
 * @brief Quản lý việc mượn và trả sách trong hệ thống thư viện.
 *
 * Bao gồm các hàm xử lý mượn sách, trả sách, thống kê và khôi phục tồn kho.
 */

#ifndef BORROW_RETURN_H
#define BORROW_RETURN_H
#define MAX_BORROWED_BOOKS 100
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "books.h"
#include "reader.h"
#include "bplustreev2.h"

/**
 * @brief Struct đại diện cho một bản ghi mượn-trả sách.
 */
typedef struct BorrowReturn
{
    int readerId;                       ///< ID người đọc
    int totalBooks;                     ///< Tổng số sách mượn
    int bookIds[MAX_BORROWED_BOOKS];    ///< Danh sách các ID sách đã mượn
    int quantities[MAX_BORROWED_BOOKS]; ///< Số lượng tương ứng với mỗi sách
    int status;                         ///< Trạng thái: 0 = đang mượn, 1 = đã trả
    bool onTime;                        ///< Trả đúng hạn (1) hoặc trễ (0)
} BorrowReturn;

// Biến toàn cục quản lý dữ liệu
extern Node *borrow_return_management; ///< Cây B+ quản lý mượn-trả theo ID

// Tên file lưu dữ liệu
extern char borrow_return_content_file[MAX_FILE_NAME_LENGTH];    ///< Tệp chứa nội dung bản ghi
extern char borrow_return_management_file[MAX_FILE_NAME_LENGTH]; ///< Tệp chứa offset

/**
 * @brief Thêm bản ghi mượn sách.
 */
void add_borrow_record(BorrowReturn *b);

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
/**
 * @brief Kiểm tra xem một cuốn sách có đang được mượn hay không.
 * @param bookId ID của cuốn sách cần kiểm tra.
 * @return true nếu sách đang được mượn, false nếu không.
 */
bool check_book_in_borrow(int bookId);

#endif // BORROW_RETURN_H
