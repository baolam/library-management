/**
 * @file reader.h
 * @brief File quản lí người đọc
 * @author Bùi Hải Đăng
 * @version 1.0
 *
 * File chứa những đoạn chương trình, hàm gọi sẽ được dùng cho viết giao diện, phục vụ hiển thị.
 * Cũng như debug thuật toán
 */

#ifndef READERS_H
#define READERS_H

/**
 * @brief Họ và tên
 */
#define MAX_FULLNAME 100

/**
 * @brief Lưu trữ số điện thoại
 */
#define MAX_PHONENUMBER 15

/**
 * @brief Địa chỉ
 */
#define MAX_ADDRESS 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/management.h"
#include "utils/trie.h"
#include "utils/bplustreev2.h"
#include "books.h"

/**
 * @struct Readers
 * @brief Cấu trúc quản lí của một độc giả
 */
typedef struct Readers
{
    /**
     * @brief Id của độc giả
     * Xem như khoá tìm kiếm (khoá chính) dùng cho mục đích quản lí
     */
    int readerId;

    /**
     * @brief Họ và tên
     */
    char fullName[MAX_FULLNAME];

    /**
     * @brief Số điện thoại
     */
    char phoneNumber[MAX_PHONENUMBER];

    /**
     * @brief Địa chỉ
     */
    char address[MAX_ADDRESS];
} Readers;

/**
 * @brief FIle quản lí độc giả (thuần là file quản lí)
 */
extern char reader_management_file[MAX_FILE_NAME_LENGTH];

/**
 * @brief File quản lí tên kết hợp Id (cây Trie)
 */
extern char reader_name_management_file[MAX_FILE_NAME_LENGTH];

/**
 * @brief File nội dung lưu trữ dữ liệu
 */
extern char reader_content_file[MAX_FILE_NAME_LENGTH];

/**
 * @brief Đối tượng quản lí chính
 */
extern Node *reader_management;

/**
 * @brief Đối tượng quản lí phụ, phục vụ cho thao tác tìm kiếm gợi ý
 */
extern TrieNode *reader_trie;

// ===== CRUD operations =====

/**
 * @brief Hiển thị độc giả ra ngoài màn hình Console
 * @param reader đối tượng độc giả
 */
void show_reader(Readers reader);

/**
 * @brief Thêm độc giả vào quản lí
 * @param reader đối tượng độc giả
 */
void add_reader(Readers *reader);

/**
 * @brief Tìm kiếm độc giả dựa theo Id
 * @param id là Id độc giả
 */
void search_reader_by_id(int id);

/**
 * @brief Tìm kiếm độc giả dựa theo tên
 *
 * Do độc giả có thể trùng tên nên kết quả tìm kiếm là danh sách các độc giả có tên bắt đầu
 * Cụ thể hơn có hai mức khái niệm ở đây:
 * + Mức 1: là dựa vào Prefix, trả về được các tên độc giả bắt đầu bằng Prefix chỉ định
 * + Mức 2: Vào sâu một tên, có thể xảy ra trường hợp trùng tên --> Yêu cầu lưu quản lí tên trùng luôn
 * Kết quả trả về là hợp của hai kết quả trên, lấy theo số lượng được yêu cầu
 *
 * @param prefix tiền tố tên bắt đầu ví dụ như Nguyen, Ng, Ba, Bui
 * @param maxNumbers là số lượng kết quả tối đa được trả vể
 */
void search_reader_by_name(const char *prefix, int maxNumbers);

/**
 * @brief Hàm cập nhật thông tin độc giả
 *
 * Hàm này thao tác ở màn hình Console
 * @param reader là đối tượng độc giả muốn Update, về chi tiết hơn có thể chỉ cần mỗi Id, (độc giả thiếu thông tin)
 */
void update_reader(Readers *reader);

/**
 * @brief Xoá độc giả
 *
 * Bản chất của thao tác xoá này là xoá để khỏi quản lí thui, dữ liệu độc giả nếu tiến hành
 * lưu trữ vẫn còn tồn tại trong file nội dung
 *
 * @param id là Id độc giả muốn xoá
 */
void delete_reader(int id);

// ===== Nhóm hàm hỗ trợ tương tác giao diện ======

/**
 * @brief Hàm này sẽ trả về đối tượng Reader ứng với Id được chỉ định
 *
 * Là một hàm thao tác trực tiếp trả về Reader, không cần phải thông
 * qua gọi Callback
 *
 * @return Đối tượng Reader, NULL nếu đọc không được
 */
Readers *search_reader(int id);

/**
 * @brief Hàm này sẽ trả về danh sách các độc giả theo sau Id được chỉ định một
 * lượng bao nhiêu đó
 *
 * Kết quả trả về của hàm này là một mảng các Readers
 * @note Dùng xong nhớ xoá đi nhe (gọi hàm free)
 *
 * @param beginingKey là Id bắt đầu
 * @param quanities là số lượng muốn tìm kiếm tối đa
 * @param actualReaders là kích thước thực tế
 *
 * @note Việc tồn tại hai tham số trên (quanities, actualReaders) là do có thể vị trí bắt đầu gần cuối, kết quả có thể
 * không trùng lặp giữa cả hai
 * @return Mảng các độc giả
 */
Readers *retrieve_bucket_readers(int beginingKey, int quanities, int *actualReaders);

// ===== Persistence =====

/**
 * @brief Hỗ trợ cho thao tác quản lí
 *
 * Gọi hàm này để gán Trie khỏi NULL
 */
void preparate_reader();

/**
 * @brief Lưu trữ thông tin
 *
 * Kết quả lưu trữ sẽ tạo ra 3 file theo định nghĩa
 */
void save_reader_management();

/**
 * @brief Tải dữ liệu đã lưu trữ
 *
 * Dữ liệu tải là dữ liệu quản lí. File nội dung khi nào cần các thao tác thêm/sửa/xoá
 * mới tiến hành kết nối đến
 */
void load_reader_management();

#endif // READERS_H