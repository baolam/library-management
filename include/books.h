/**
 * @file books.h
 * @brief Lưu trữ những hàm sẽ được dùng cho quản lí sách
 * @authors Nguyễn Huy Phong, Nguyễn Đức Bảo Lâm
 * @version 1.0
 *
 * Một số hàm dùng để quản lí sách
 */

#ifndef BOOKS_H
#define BOOKS_H
// #define FOLDER "books"

#define MAX_TITLE 50     ///< Độ dài tối đa của tiêu đề
#define MAX_AUTHOR 50    ///< Độ dài tối đa của tác giả
#define MAX_GENRE_NO 100 ///< Độ dài tối đa của thể loại sách

// #include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils/management.h"
#include "utils/trie.h"
#include "utils/bplustreev2.h"

/**
 * @struct Book
 * @brief Cấu trúc dữ liệu cho một cuốn sách
 *
 * Cấu trúc này chứa bookId, title, author, genre, publicationYear, stock.
 * Việc sử dụng cấu trúc này giúp dễ dàng thêm, xóa, tìm kiếm sách.
 * Ngoài ra dưới ảnh hưởng của cách quản lí dữ liệu bằng B+ Tree, việc đọc và viết dữ liệu vào
 * file cũng sẽ dễ dàng hơn.
 *
 */
typedef struct
{
    int bookId;               ///< Mã sách, cũng là khóa chính, dùng như khoá tìm kiếm cho quản lí
    char title[MAX_TITLE];    ///< tiêu đề của sách, không quá 50 kí tự
    char author[MAX_AUTHOR];  ///< Tác giả của sách, không quá 50 kí tự
    char genre[MAX_GENRE_NO]; ///< Thể loại sách, không quá 100 kí tự
    int publicationYear;      ///< Năm xuất bản
    int stock;                ///< Số lượng sách
} Book;

extern char book_management_file[MAX_FILE_NAME_LENGTH]; ///< File quản lí sách
extern char book_trie_management[MAX_FILE_NAME_LENGTH]; ///< File quản lí cây Trie, dùng hỗ trợ cho tìm kiếm bằng tên tác giả
extern char book_content_file[MAX_FILE_NAME_LENGTH];    ///< File chứa nội dung sách, file quản lí chính

extern Node *book_management; ///< Cây B+ Tree quản lí sách
extern TrieNode *book_trie;   ///< Cây trie quản lí author

/// Các hàm thao tác với Book

/***
 * @brief Hàm hiển thị thông tin quyển sách.
 * Gọi hàm này để in ra thông tin ra ngoài màn hình console,
 * Dùng cho trường hợp Debug, muốn kiểm tra sự hoạt động của các hàm
 * khác.
 *
 * @param book Quyển sách cần hiển thị
 */
void show_book(Book book);

/***
 * @brief Thêm một quyển sách mới
 *
 * Việc thêm một quyển sách mới gồm thêm vào cây B+ Tree, cây Trie đối với Author.
 * Nguyên tắc hoạt động:
 * Nếu bản ghi đã tồn tại (đảm bảo Id phải mới) --> không thêm
 * Nếu chưa tồn tại tiến hành thêm.
 *
 * @param book Quyển sách cần thêm
 */
void add_book(Book *book);

/**
 * @brief Xóa một quyển sách
 *
 * Việc xoá một quyển sách bao gồm việc xoá bản ghi trong cây B+ Tree, cây Trie đối với Author.
 * Do tính chất dữ liệu lưu trữ trong file nội dung là liên tục
 * nên việc xoá trực tiếp khỏi file nội dung là không nên do sẽ ghi chuyển dịch
 * lưu trữ.
 *
 * Phương thức xoá được dùng là soft_delete. Chỉ là xoá khỏi file quản lí,
 * Dữ liệu vẫn tồn tại trong file nội dung
 *
 * @param id Id của quyển sách cần xoá
 */
void delete_book(int id);

/**
 * @brief Cập nhật thông tin của một quyển sách
 *
 * Cập nhật thông tin quyển sách, bản chất là cập nhật file nội dung
 * Do tính chất khoá Id là cố định và bất biến một khi được tạo nên không cần
 * thao tác ở cây B+ tree (thao tác cập nhật, chỉ tận dụng thao tác tìm kiếm)
 *
 * Phương thức này được dùng và hiển thị ra console.
 * @param id Id của quyển sách cần cập nhật
 */
void update_book(int id);

/**
 * @brief Cập nhật thông tin một quyển sách, cập nhật vào file nội dung
 *
 * Gọi hàm này không cần dùng đến callback
 * @param book cuốn sách muốn cập nhật
 */
int update_book_from_object(Book *book);

/**
 * @brief Tìm kiếm sách bằng Id
 *
 * Kết quả trả về sẽ hiển thị ra màn hình console, (chưa tích hợp hiển thị
 * trên giao diện)
 *
 * @param id Id của quyển sách cần tìm
 */
void search_book_by_id(int id);

/**
 * @brief Tìm kiếm sách bằng Title
 *
 * Cung cấp vào tên Author hoặc nhóm chữ cái đầu của tên Author muốn tìm kiếm
 * Kết quả hiển thị là những quyển sách có tên bắt đầu bằng từ tìm kiếm
 *
 * @param prefix là nhóm chữ cái đầu của tên Author
 * @param maxNumbers là số lượng kết quả tối đa
 */
void search_book_by_title(const char *prefix, int maxNumbers);

/**
 * @brief Tìm kiếm sách bằng Title, có hứng kết quả trả về
 *
 * Hàm này sẽ trả về kết quả tìm kiếm số lượng sách, hứng kết quả trả về lưu trữ vào mảng.
 * Việc dùng hàm này sẽ giúp đảm bảo lợi thế khi kết nối giao diện. Giao diện có thể gặp
 * khó khăn khi tiến hành gọi callback
 *
 * @param prefix là nhóm chữ cái đầu của tên
 * @param books là số lượng sách đọc được (mảng)
 * @param actualBooks là số lượng sách thực tế (mảng)
 * @param maxNumbers là số lượng kết quả tối đa
 *
 * @return là mảng chứa các cuốn sách tìm được
 */
Book *search_book_by_title_direct(const char *prefix, int *actualBooks, int maxNumbers);

/**
 * @brief Hàm trả về một tập hợp các quyển sách theo sau một Id tìm kiếm
 *
 * Dùng hàm này khi cần hứng chính xác một mảng các quyển sách. Được dùng cho
 * nhóm thao tác giao diện
 * @note Dùng xong nhớ free để tránh lãng phí bộ nhớ
 *
 * @param beginingKey là vị trí bắt đầu đọc đầu tiên
 * @param quanities là số lượng sách muốn lấy sau khi kiếm Id đầu tiên
 * @param actualBooks là số lượng sách thực tế
 *
 * @return là mảng chứa các quyển sách tìm được
 */
Book *retrieve_bucket_books(int beginingKey, int quanities, int *actualBooks);

/// Phục vụ cho nhóm nhiệm vụ mượn, trả sách

/***
 * @brief Tìm kiếm sách và trả về sách
 *
 * Trả về bản dựng (Book) một quyển sách từ Id được cung cấp
 * Nếu không tìm thấy sách (đã bị xoá, lỗi đọc file), trả về NULL
 * Tìm thấy thì trả về bản dựng (Book) của nó
 *
 * Cách hoạt động:
 * + Lấy Record tương ứng ứng với Id kèm theo
 * + Đọc dữ liệu từ file nội dung ứng với Record
 * + Gghi nhận dữ liệu và trả về kết quả
 *
 * @param id Id của quyển sách cần tìm
 */
Book *search_book(int id);

/// Lưu trữ vào file, load dữ liệu ra từ file

/**
 * @brief Hàm hỗ trợ cho cây Trie, tìm kiếm = Author
 *
 * Bản chất chỉ là tạo node quản lí nếu cây chưa tồn tại
 */
void prepare_book();

/**
 * @brief Lưu trữ dữ liệu
 *
 * Nội dung lưu trữ gồm file quản lí B+ Tree, file quản lí cây Trie, file nội dung
 * Lưu trữ trong thư mục data/book
 *
 */
void save_book_management();

/**
 * @brief Load dữ liệu
 *
 * Lấy nội dung dữ liệu đã lưu trữ trước đó
 */
void load_book_management();

#endif