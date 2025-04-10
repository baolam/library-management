#ifndef MANAGEMENT_H
#define MANAGEMENT

#define DELETE_SUCCESS 2
#define DELETE_EXISTED 1
#define DELETE_FAILED 0

#define ADD_CONTENT_FAILED 0
#define ADD_CONTENT_SUCCESS 1

#include "bplustreev2.h"
#include <stdbool.h>
#include <windows.h>

bool exist_record(Node *root, int key);

/// @brief thêm dữ liệu vào quản lí (thêm vào file nội dung (content_file), trả về Node quản lí (B+ Tree))
/// @param root
/// @param key
/// @param content_file
/// @param content
/// @param size
/// @param callback
/// @return Node (B+ Tree Node)
Node *add_content(Node *root, int key, char content_file[MAX_FILE_NAME_LENGTH], void *content, size_t size, void (*callback)(int id, int code, long offset, long length));

/// @brief Đọc dữ liệu từ file quản lí
/// @param root
/// @param key (id được lưu trữ tương ứng với dữ liệu (id))
/// @param callback
void read_content(Node *root, int key, void (*callback)(FILE *f, long package_size));

/// @brief đọc dữ liệu từ bản ghi có sẵn (Record)
/// @param record
/// @param callback
void read_content_from_record(Record *record, void (*callback)(FILE *f, long package_size));

void *read_content_from_record_return(Record *record, char content_file[MAX_FILE_NAME_LENGTH], size_t *size);
/// @brief Đọc nums số lượng bản ghi
/// @param root
/// @param beginingKey
/// @param nums
/// @param callback
void read_bucket_content(Node *root, int beginingKey, int nums, void (*callback)(FILE *f, long package_size));

/// @brief Hàm cập nhật dữ liệu
/// @param root
/// @param key
/// @param callback
void update_content(Node *root, int key, void (*callback)(FILE *f, long package_size));

/// @brief Hàm cập nhật dữ liệu, có sẵn Record
/// @param record
/// @param callback
void update_content_from_record(Record *record, void (*callback)(FILE *f, long package_size));

/// @brief Hàm xoá dữ liệu (xoá mềm, bản chất là xoá xử lí)
/// @param root
/// @param key
/// @param callback
void soft_delete(Node *root, int key, void (*callback)(int code));

#endif