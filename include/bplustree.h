#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#define ORDER 4

#include <stdbool.h>
#include <string.h>
#include "files.h"

typedef struct
{
    /// Khoá truy cập
    int keys[ORDER - 1];
    // Vị trí lưu trữ dữ liệu trong file
    long offsets[ORDER - 1];
    int children[ORDER];
    bool is_leaf;
    int num_keys;
} BPlusTreeNode;

typedef struct
{
    BPlusTreeNode *root;
    char folder[50];
    char *filename[50];
} BPlusTree;

BPlusTreeNode *create_node(bool is_leaf);
BPlusTree *create_tree(const char *folder, const char *filename);

/// @brief Lưu trữ node vào file
/// @param fp dối tượng file
/// @param node
/// @param position vị trí lưu trữ trong file (bit)
void save_node(FILE *fp, BPlusTreeNode *node, long position);

/// @brief Lấy node được lưu trữ ra khỏi file
/// @param fp
/// @param node
/// @param position
void load_node(FILE *fp, BPlusTreeNode *node, long position);

/// @brief  Lưu trữ cây
/// @param tree Cây BPlus
void save_tree(BPlusTree *tree);

/// @brief lưu trữ cây để quản lí
/// @param folder thư mục lưu trữ
/// @param filename tên file
/// @return
BPlusTree *load_tree(const char *folder, const char *filename);

#endif