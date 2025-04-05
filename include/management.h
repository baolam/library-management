#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "bplustree.h"

void readContent(const char *filename, FileInfor infor, void (*callback)(FILE *f, long length));
void writeContent(const char *filename, FileInfor infor, void (*callback)(FILE *f, long length));

BPlusTree *readBPlusTree(const char *filename);
void writeBPlusTree(const char *filename, BPlusTree *tree);

#endif