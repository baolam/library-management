#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "bplustree.h"

void readContent(const char *filename, FileInfor infor, void (*callback)(FILE *f, long length));
void writeContent(const char *filename, FileInfor infor, char *content);

#endif