#ifndef BPLUSTREE_H
#define BPLUSTREE_H
// #define DEBUG

#ifdef DEBUG
#include <stdio.h>
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORDER 5

typedef struct
{
    char *filename;
    long offset;
    int length;
} FilePosition;

typedef struct
{
    int key;
    FilePosition position;
} StorageData;

typedef struct BPlusNode
{
    bool isLeaf;
    int numKeys;
    StorageData *data;
    int *keys;
    struct BPlusNode **children;
    struct BPlusNode *next;
} BPlusNode;

typedef struct
{
    BPlusNode *root;
    int order;
} BTree;

BPlusNode *createNode(bool isLeaf);
BTree *createTree(int order);
StorageData *searchStorageData(BTree *tree, int key);
StorageData createStorageData(int key, char *filename, long offset, int length);
void insertToTree(BTree *tree, StorageData data);

/// Phương thức hiển thị mẫu
#ifdef DEBUG
void printNode(BPlusNode *node, int level);
void printTree(BTree *tree);
#endif

#endif