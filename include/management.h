#ifndef MANAGEMENT_H
#define MANAGEMENT

#define DELETE_SUCCESS 2
#define DELETE_EXISTED 1
#define DELETE_FAILED 0

#define ADD_CONTENT_FAILED 0
#define ADD_CONTENT_SUCCESS 1

#include "bplustreev2.h"
#include <stdbool.h>
#include <windows.h>

bool exist_record(Node *root, int key);

Node *add_content(Node *root, int key, char content_file[MAX_FILE_NAME_LENGTH], void *content, size_t size, void (*callback)(int id, int code, long offset, long length));
void read_content(Node *root, int key, void (*callback)(FILE *f, long package_size));
void read_content_from_record(Record *record, void (*callback)(FILE *f, long package_size));
void read_bucket_content(Node *root, int beginingKey, int nums, void (*callback)(FILE *f, long package_size));

void update_content(Node *root, int key, void (*callback)(FILE *f, long package_size));
void update_content_from_record(Record *record, void (*callback)(FILE *f, long package_size));

void soft_delete(Node *root, int key, void (*callback)(int code));

#endif