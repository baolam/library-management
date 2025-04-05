/// Program depends on https://www.programiz.com/dsa/b-plus-tree

#ifndef BPLUSTREEV2_H
#define BPLUSTREEV2_H

#define PRINT_TREE
#define ORDER 5
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
Node *startNewTree(int key, Record *pointer);
Node *insertIntoLeaf(Node *leaf, int key, Record *record);
Node *insertIntoNode(Node *root, Node *n, int left_index, int key, Node *right);
Node *insertIntoNodeAfterSplitting(Node *root, Node *old_node, int left_index, int key, Node *right);
Node *insertIntoNewRoot(Node *left, int key, Node *right);
Node *insertIntoParent(Node *root, Node *left, int key, Node *right);
Node *insertIntoLeafAfterSplitting(Node *root, Node *leaf, int key, Record *record);
Node *insert(Node *root, int key, Record *record);

/// Phương thức lưu trữ
void saveTree(Node *root, char *filename);
Node *loadTree(char *filename);

#endif