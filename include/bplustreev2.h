/// Program depends on https://www.programiz.com/dsa/b-plus-tree

#ifndef BPLUSTREEV2_H
#define BPLUSTREEV2_H

#define PRINT_TREE
#define ORDER 10
#define MAX_FILE_NAME_LENGTH 128

#ifdef PRINT_TREE
#include <stdio.h>
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Record
{
    char _from[MAX_FILE_NAME_LENGTH];
    long offset;
    long length;
    bool deleted;
} Record;

typedef struct Node
{
    void **pointers;
    int *keys;
    struct Node *parent;
    bool is_leaf;
    int num_keys;
    struct Node *next;
} Node;

Record *makeRecord(char _from[MAX_FILE_NAME_LENGTH], long offset, long length);
Node *leftMost(Node *root);
Node *makeNode(void);
Node *makeLeaf(void);

/// Phương thức tìm kiếm
Node *findLeaf(Node *root, int key);
Record *find(Node *root, int key);

/// Phương thức bổ trợ
int cut(int length);
int getLeftIndex(Node *parent, Node *left);

#ifdef PRINT_TREE
extern Node *queue;

int pathToLeaves(Node *const root, Node *child);

void enqueue(Node *new_node);
Node *dequeue(void);

void printTree(Node *const root);
#endif

/// Phương thức thêm phần tử

/// @brief Khởi tạo một cây mới (bản chất là Node root) khi chưa tồn tại
/// @param key (khoá tìm kiếm)
/// @param pointer (con trỏ trỏ đến dữ liệu ghi)
/// @return Node gốc
Node *startNewTree(int key, Record *pointer);

/// @brief hỗ trợ insert, insert vào node lá
/// @param leaf (lá)
/// @param key khoá tìm kiếm
/// @param record bản ghi dữ liệu
/// @return
Node *insertIntoLeaf(Node *leaf, int key, Record *record);

/// @brief hỗ trợ insert
/// @param root
/// @param n
/// @param left_index
/// @param key
/// @param right
/// @return
Node *insertIntoNode(Node *root, Node *n, int left_index, int key, Node *right);

/// @brief hỗ trợ insert
/// @param root
/// @param old_node
/// @param left_index
/// @param key
/// @param right
/// @return
Node *insertIntoNodeAfterSplitting(Node *root, Node *old_node, int left_index, int key, Node *right);

/// @brief hỗ trợ insert
/// @param left
/// @param key
/// @param right
/// @return
Node *insertIntoNewRoot(Node *left, int key, Node *right);

/// @brief hỗ trợ insert
/// @param root
/// @param left
/// @param key
/// @param right
/// @return
Node *insertIntoParent(Node *root, Node *left, int key, Node *right);

/// @brief hỗ trợ insert
/// @param root
/// @param leaf
/// @param key
/// @param record
/// @return
Node *insertIntoLeafAfterSplitting(Node *root, Node *leaf, int key, Record *record);

/// @brief Hàm insert, thêm dữ liệu lưu trữ vào quản lí, thêm vào cây B+
/// @param root cây quản lí
/// @param key khoá dùng để tìm kiếm, trường hợp đã tồn tại, cập nhật
/// @param _from tên file nội dung lưu trữ
/// @param offset vị trí bắt đầu lưu trữ trong file
/// @param length kích thước dữ liệu
/// @return
Node *insert(Node *root, int key, char _from[MAX_FILE_NAME_LENGTH], long offset, long length);

/// Phương thức lưu trữ

/// @brief Lưu file
/// @param root cây lưu trữ
/// @param filename tên file muốn lưu trữ (đuôi .bin)
void saveTree(Node *root, char *filename);

/// @brief Load cây
/// @param filename tên file
/// @return
Node *loadTree(char *filename);

#endif