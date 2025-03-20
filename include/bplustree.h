#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#define ORDER 4

#include <stdbool.h>
#include <string.h>
#include "files.h"

typedef struct
{
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

/// Thủ tục thao tác liên quan đến file của BPlusTree
void save_node(FILE *fp, BPlusTreeNode *node, long position);
void load_node(FILE *fp, BPlusTreeNode *node, long position);

void save_tree(BPlusTree *tree);
BPlusTree *load_tree(const char *folder, const char *filepath);

#endif