/**
 * @file statistic.h
 * @brief Quản lý thống kê sách trong hệ thống quản lý thư viện
 * @authors Nguyễn Huy Phong
 * @version 1.0
 */

 #ifndef STATISTIC_H
 #define STATISTIC_H
 
 #include "bplustreev2.h"
 #include "fake.h"
 #include "books.h"
 #include "management.h"
 #include "borrow_return.h"
 
 #include <string.h>

 #define MAX_OVERDUE 1000

 /// Cấu trúc người mượn quá hạn
 typedef struct OverdueBorrower {
    int readerId;
    int days_borrowed;
} OverdueBorrower;
 
 /// Tổng số sách hiện có (chưa bị xóa)
 extern int total_books;
 
 /// Tổng số sách đã bị xóa (được đánh dấu là deleted)
 extern int deleted_books;
 
 /// Bộ đếm số sách theo từng thể loại, chỉ số tương ứng với mảng genres
 extern short counter_genre[TOTAL_GENRE];

 /// Tổng số sách hiện đang được mượn
 extern int total_borrowed_books;

 /// Tổng số sách quá hạn
 extern int total_late_books;

 /// Danh sách người mượn quá hạn
 extern OverdueBorrower overdue_list[MAX_OVERDUE];

 /// tổng số người mượn quá hạn
 extern int overdue_count;

 /**
  * @brief Thiết lập lại dữ liệu thống kê ban đầu
  *
  * Gán tất cả các bộ đếm thể loại về 0.
  * Hàm này nên được gọi trước khi bắt đầu thống kê.
  */
 void set_up();
 
 /**
  * @brief Tính toán lại dữ liệu thống kê sách
  *
  * Duyệt toàn bộ cây B+ Tree để thống kê:
  * - Tổng số sách còn tồn tại (không bị xóa)
  * - Tổng số sách bị xóa
  * - Số lượng sách theo từng thể loại
  *
  * @param book_management Cây B+ Tree quản lý sách
  */
 void calc_statistic_book(Node *book_management);
 
 /**
  * @brief Tính toán lại dữ liệu thống kê sách đang được mượn
  * 
  * Duyệt toàn bộ cây B+ Tree để thống kê:
  * - Tổng số sách đang được mượn (chưa được trả)
  */
 void calc_statistic_borrowed_books(Node *borrow_return_management);

 /**
 * @brief Tạo danh sách người mượn quá hạn
 * 
 * xem xét nếu như cuốn sách quá hạn thì thêm vào mảng
 */
 void collect_late_borrowers(Node *borrow_return_management);

 /**
  * @brief In ra danh sách người mượn quá hạn
  */
 void list_late_borrowers();

 #endif
 