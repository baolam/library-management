#ifndef STORAGETREE_H
#define STORAGETREE_H

#include "bplustree.h"
#include <string.h>
#include <msgpack.h>

char *my_strndup(const char *s, size_t n)
{
    char *result;
    size_t len = strnlen(s, n);
    result = (char *)malloc(len + 1);
    if (!result)
        return NULL;
    memcpy(result, s, len);
    result[len] = '\0';
    return result;
}

void serializeFileInfor(msgpack_packer *pk, FileInfor *infor);
void serializeStorageInfor(msgpack_packer *pk, StorageInfor *infor);
void serializeBPlusNode(msgpack_packer *pk, BPlusNode *node);

FileInfor deserializeFileInfor(msgpack_object *obj);
StorageInfor deserializeStorageInfor(msgpack_object *obj);
BPlusNode *deserializeBPlusNode(msgpack_object *obj);

void saveBPlusTree(const char *fileName, BPlusTree *tree);
BPlusTree *loadBPlusTree(msgpack_object *obj);
BPlusTree *loadBPlusTreeFromFile(const char *filename);
#endif