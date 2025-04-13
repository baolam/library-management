/**
 * @file readers.h
 * @brief Quản lý người đọc trong hệ thống thư viện.
 *
 * Chứa định nghĩa struct Readers, các hàm thêm, tìm kiếm, cập nhật,
 * và xóa người đọc. Dữ liệu được lưu bằng file và quản lý bởi B+ Tree và Trie.
 */

#ifndef READERS_H
#define READERS_H

#define USER_FOLDER "readers"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <books.h>
#include "management.h"
#include "trie.h"
#include "bplustreev2.h"
#include "borrow_return.h"

/**
 * @brief Struct đại diện cho người đọc trong hệ thống.
 */
typedef struct
{
    int readerId;         ///< Mã người đọc (duy nhất)
    char fullName[100];   ///< Họ tên đầy đủ
    char phoneNumber[15]; ///< Số điện thoại liên hệ
    char address[100];    ///< Địa chỉ
} Readers;

// Biến toàn cục quản lý file và cây
extern char reader_management_file[MAX_FILE_NAME_LENGTH];      ///< Tệp chứa offset quản lý theo ID
extern char reader_name_management_file[MAX_FILE_NAME_LENGTH]; ///< Tệp chứa offset quản lý theo tên
extern char reader_content_file[MAX_FILE_NAME_LENGTH];         ///< Tệp lưu dữ liệu người đọc

extern Node *reader_management; ///< Cây B+ quản lý người đọc theo ID
extern TrieNode *reader_trie;   ///< Trie để tìm kiếm người đọc theo tên

// ===== CRUD operations =====

/**
 * @brief Hiển thị thông tin một người đọc.
 */
void show_reader(Readers reader);

/**
 * @brief Thêm người đọc mới.
 */
void add_reader(Readers *reader);

/**
 * @brief Tìm người đọc theo ID.
 */
void search_reader_by_id(int id);

/**
 * @brief Tìm người đọc theo tên (gợi ý nhiều kết quả).
 * @param name Tên cần tìm.
 * @param maxNumbers Số lượng kết quả tối đa.
 */
void search_reader_by_name(const char *name, int maxNumbers);

/**
 * @brief Cập nhật thông tin người đọc.
 */
void update_reader(Readers *reader);

/**
 * @brief Xóa người đọc theo ID.
 */
void delete_reader(int id);

// ===== Persistence =====

/**
 * @brief Chuẩn bị các file và cây quản lý người đọc.
 */
void prepare_reader();

/**
 * @brief Lưu cây quản lý người đọc vào file.
 */
void save_reader_management();

/**
 * @brief Tải cây quản lý người đọc từ file.
 */
bool load_reader_management();

#endif // READERS_H
