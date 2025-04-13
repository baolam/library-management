#include "utils/management.h"

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

int getStartSearch(Node *n, int beginingKey)
{
    int startSearch;
    for (startSearch = 0; startSearch < n->num_keys; startSearch++)
    {
        if (n->keys[startSearch] < beginingKey)
            continue;
        else
            break;
    }
    return startSearch;
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
    int i;
    int startSearch = getStartSearch(n, beginingKey);

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

// void *read_bucket_content_return(Node *root, int beginingKey, int *actualSize, int nums, size_t package_size)
// {
//     Node *n = findLeaf(root, beginingKey);
//     if (n == NULL)
//     {
//         *actualSize = 0;
//         return NULL;
//     }

//     int startSearch = getStartSearch(n, beginingKey);
//     int storage_pos = 0;
//     int i;

//     void *desired_result = (void *)malloc(package_size * nums);
//     if (desired_result == NULL)
//     {
//         *actualSize = 0;
//         free(desired_result);
//         return NULL;
//     }

//     while (n != NULL && nums > 0)
//     {
//         for (i = startSearch; i < n->num_keys && nums > 0; i++)
//         {
//             Record *record = (Record *)n->pointers[i];
//             void *expected = read_content_from_record_return(record);
//             if (expected == NULL)
//                 continue;
//             desired_result[storage_pos] = expected;
//             storage_pos++;
//         }
//     }

//     *actualSize = storage_pos;
//     return desired_result;
// }

void update_content(Node *root, int key, void (*callback)(FILE *f, long package_size))
{
    __ext_read_write(root, key, callback);
}

void update_content_from_record(Record *record, void (*callback)(FILE *f, long package_size))
{
    __ext_rw_utilize(record, callback);
}

int update_content_without_callback(Record *record, void *content)
{
    FILE *f = fopen(record->_from, "r+");
    if (f == NULL)
        return UPDATE_FILE_FAILED;
    fseek(f, record->offset, SEEK_SET);
    fwrite(content, 1, record->length, f);
    fclose(f);
    return UPDATE_SUCCESS;
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
    if (record == NULL || record->deleted)
        return NULL;

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