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
 
 #include <string.h>
 
 /// Tổng số sách hiện có (chưa bị xóa)
 extern int total_books;
 
 /// Tổng số sách đã bị xóa (được đánh dấu là deleted)
 extern int deleted_books;
 
 /// Bộ đếm số sách theo từng thể loại, chỉ số tương ứng với mảng genres
 extern short counter_genre[TOTAL_GENRE];
 
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
 
 #endif
 