#ifndef READERS_H
#define READERS_H

#define USER_FOLDER "readers"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "management.h"
#include "trie.h"
#include "bplustreev2.h"

// Struct đại diện cho người đọc
typedef struct
{
    char readerId[20];    // ID của người đọc
    char fullName[100];   // Tên đầy đủ
    char phoneNumber[15]; // Số điện thoại
    char address[100];    // Địa chỉ
} Readers;

// Biến toàn cục quản lý file và cây B+
extern char reader_management_file[MAX_FILE_NAME_LENGTH];
extern char reader_name_management_file[MAX_FILE_NAME_LENGTH];
extern char reader_content_file[MAX_FILE_NAME_LENGTH];

extern Node *reader_management;
extern TrieNode *reader_trie;

// ===== CRUD operations =====
void show_reader(Readers reader);         // Hiển thị thông tin người đọc
void add_reader(Readers *reader);         // Thêm người đọc
void search_reader_by_id(const char *id); // Tìm người đọc theo ID
void update_reader(Readers *reader);      // Cập nhật người đọc
void delete_reader(const char *id);       // Xóa người đọc

// ===== Persistence =====
void save_reader_management(); // Lưu cây
void load_reader_management(); // Đọc cây

#endif // READERS_H
