#ifndef MANAGEMENT_H
#define MANAGEMENT

#include "bplustreev2.h"
#include <stdbool.h>

bool exist_record(Node *root, int key);

void read_content(Node *root, int key, void (*callback)(FILE *f, long package_size));
void read_content_from_record(Record *record, void (*callback)(FILE *f, long package_size));
void read_bucket_content(Node *root, int beginingKey, int nums, void (*callback)(FILE *f, long package_size));

void update_content(Node *root, int key, void (*callback)(FILE *f, long package_size));
void update_content_from_record(Record *record, void (*callback)(FILE *f, long package_size));

void soft_delete(Node *root, int key, void (*callback)(int code));

#endif