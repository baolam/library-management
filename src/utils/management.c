#include "management.h"

bool exist_record(Node *root, int key)
{
    Record *record = find(root, key);
    if (record == NULL)
        return false;
    return !record->deleted;
}

void __ext_rw_utilize(Record *record, void (*callback)(FILE *f, long package_size))
{
    FILE *f = fopen(record->_from, "r");
    if (f == NULL)
    {
        callback(NULL, 0);
        return;
    }

    fseek(f, record->offset, SEEK_SET);
    callback(f, record->length);
    fclose(f);
}

void __ext_read_write(Node *root, int key, void (*callback)(FILE *f, long package_size))
{
    if (!exist_record(root, key))
    {
        callback(NULL, 0);
        return;
    }

    Record *record = find(root, key);
    if (record == NULL)
    {
        callback(NULL, 0);
        return;
    }

    __ext_rw_utilize(record, callback);
}

void read_content(Node *root, int key, void (*callback)(FILE *f, long package_size))
{
    __ext_read_write(root, key, callback);
}

void read_content_from_record(Record *record, void (*callback)(FILE *f, long package_size))
{
    __ext_rw_utilize(record, callback);
}

void update_content(Node *root, int key, void (*callback)(FILE *f, long package_size))
{
    __ext_read_write(root, key, callback);
}

void update_content_from_record(Record *record, void (*callback)(FILE *f, long package_size))
{
    __ext_rw_utilize(record, callback);
}

void soft_delete(Node *root, int key, void (*callback)(int code))
{
    if (!exist_record(root, key))
    {
        callback(0);
        return;
    }

    Record *record = find(root, key);
    if (record->deleted)
    {
        callback(1);
        return;
    }

    record->deleted = true;
    callback(2);
}