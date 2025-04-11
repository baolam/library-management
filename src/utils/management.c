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

void read_bucket_content(Node *root, int beginingKey, int nums, void (*callback)(FILE *f, long package_size))
{
    Node *n = findLeaf(root, beginingKey);
    if (n == NULL)
    {
        callback(NULL, 0);
        return;
    }

    /// Edge case
    int startSearch, i;
    for (startSearch = 0; startSearch < n->num_keys; startSearch++)
    {
        if (n->keys[startSearch] < beginingKey)
            continue;
        else
            break;
    }

    while (n != NULL && nums > 0)
    {
        for (i = startSearch; i < n->num_keys && nums > 0; i++)
        {
            Record *record = (Record *)n->pointers[i];
            if (!record->deleted)
            {
                read_content_from_record(record, callback);
                nums--;
            }
        }
        startSearch = 0;
        n = n->pointers[ORDER - 1];
    }
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
        callback(DELETE_FAILED);
        return;
    }

    Record *record = find(root, key);
    if (record->deleted)
    {
        callback(DELETE_EXISTED);
        return;
    }

    record->deleted = true;
    callback(DELETE_SUCCESS);
}

Node *add_content(Node *root, int key, char content_file[MAX_FILE_NAME_LENGTH], void *content, size_t size, void (*callback)(int id, int code, long offset, long length))
{
    FILE *f = fopen(content_file, "rb+");
    if (f == NULL)
    {
        f = fopen(content_file, "wb+");
        if (f == NULL)
        {
            callback(key, ADD_CONTENT_FAILED, 0, 0);
            return root;
        }
    }

    fseek(f, 0, SEEK_END);
    long offset = ftell(f);
    fwrite(content, size, 1, f);
    fclose(f);

    Node *temp = insert(root, key, content_file, offset, size);

    callback(key, ADD_CONTENT_SUCCESS, offset, size);

    return temp;
}

void *read_content_from_record_return(Record *record)
{
    FILE *f = fopen(record->_from, "rb");
    if (f == NULL)
        return NULL;
    void *buffer = malloc(record->length);
    if (!buffer)
        return NULL;
    fseek(f, record->offset, SEEK_SET);
    if (fread(buffer, record->length, 1, f) != 1)
    {
        free(buffer);
        fclose(f);
        return NULL;
    }
    fclose(f);
    return buffer;
}