#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#define MAX_ORDER 4
#define WARNING
#define PRINT_TREE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef WARNING
// printf("Make sure, there's enough memory for creating data!");
#endif

typedef struct
{
    bool deleted;
    char *filename;
    long offset;
    long length;
} FileInfor;

typedef struct
{
    int searchKey;
    FileInfor infor;
} StorageInfor;

typedef struct BPlusNode
{
    bool isLeaf;
    int numKeys, limit;
    StorageInfor *infors;
    int *keys;
    struct BPlusNode *next;
    struct BPlusNode **children;
} BPlusNode;

typedef struct
{
    BPlusNode *root;
    int t;
} BPlusTree;

/// @brief hàm xây dựng thông tin lưu trữ file, dùng để quản lí của B+ Tree
/// @param filename
/// @param offset
/// @param length
/// @return
FileInfor buildFileInfor(char *filename, long offset, long length);

/// @brief hàm dựng thông tin đc lưu trữ ở cây (với node lá), gồm thêm khoá tìm kiếm
/// @param searchKey
/// @param infor
/// @return
StorageInfor buildStorageInfor(int searchKey, FileInfor fileInfor);

/// @brief tạo node
/// @param isLeaf
/// @param t số lượng phần tử tối đa của 1 node (t-1 keys, t children)
/// @return
BPlusNode *buildNode(bool isLeaf, int t);

/// @brief tạo cây
/// @param t
/// @return
BPlusTree *buildTree(int t);

/// @brief
/// @param tree
/// @param bKey (khoá tìm kiếm của cây)
/// @param inforKey (khoá tìm kiếm thông tin)
/// @return
StorageInfor *retrieveKey(BPlusTree *tree, int bKey, int inforKey);

/// @brief tương tự hàm retrieveKey, nhưng trả về true/false
/// @param tree
/// @param bKey
/// @param searchKey
/// @return
bool existKey(BPlusTree *tree, int bKey, int searchKey);

/// @brief thêm dữ liệu vào cây
/// @param tree
/// @param key
/// @param infor
void insertInfor(BPlusTree *tree, int key, StorageInfor infor);

/// @brief xoá dữ liệu khỏi cây (soft-delete, thay đổi biến)
/// @param tree
/// @param key
void deleteInfor(BPlusTree *tree, int bKey, int searchKey);

#ifdef PRINT_TREE
void printTree(BPlusTree *tree);
void printNode(BPlusNode *node, int level);
#endif

#endif