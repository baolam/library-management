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
#include <books.h>
#include "management.h"
#include "trie.h"
#include "bplustreev2.h"

// Struct đại diện cho người đọc
typedef struct
{
    int readerId;                      // Đổi từ char[] sang int
    char fullName[MAX_FULLNAME];       // Tên đầy đủ
    char phoneNumber[MAX_PHONENUMBER]; // Số điện thoại
    char address[MAX_ADDRESS];         // Địa chỉ
} Readers;

// Biến toàn cục quản lý file và cây
extern char reader_management_file[MAX_FILE_NAME_LENGTH];
extern char reader_name_management_file[MAX_FILE_NAME_LENGTH];
extern char reader_content_file[MAX_FILE_NAME_LENGTH];

extern Node *reader_management;
extern TrieNode *reader_trie;

// ===== CRUD operations =====
void show_reader(Readers reader);
void add_reader(Readers *reader);
void search_reader_by_id(int id);
void search_reader_by_name(const char *prefix, int maxNumbers);
void update_reader(Readers *reader);
void delete_reader(int id);

// ===== Persistence =====
void preparate_reader();
void save_reader_management();
void load_reader_management();

#endif // READERS_H