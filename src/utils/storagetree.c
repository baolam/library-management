#include "storagetree.h"

void serializeFileInfor(msgpack_packer *pk, FileInfor *infor)
{
    msgpack_pack_array(pk, 4);
    msgpack_pack_int(pk, infor->deleted);
    msgpack_pack_str(pk, strlen(infor->filename));
    msgpack_pack_str_body(pk, infor->filename, strlen(infor->filename));
    msgpack_pack_long(pk, infor->offset);
    msgpack_pack_long(pk, infor->length);
}

void serializeStorageInfor(msgpack_packer *pk, StorageInfor *infor)
{
    msgpack_pack_array(pk, 2);
    msgpack_pack_int(pk, infor->searchKey);
    serializeFileInfor(pk, &infor->infor);
}

void serializeBPlusNode(msgpack_packer *pk, BPlusNode *node)
{
    msgpack_pack_array(pk, 6);
    msgpack_pack_int(pk, node->isLeaf);
    msgpack_pack_int(pk, node->numKeys);
    msgpack_pack_int(pk, node->limit);

    msgpack_pack_array(pk, node->limit - 1);
    for (int i = 0; i < node->limit; i++)
    {
        msgpack_pack_int(pk, node->keys[i]);
    }

    msgpack_pack_array(pk, node->limit);
    for (int i = 0; i < node->numKeys; i++)
    {
        serializeStorageInfor(pk, &node->infors[i]);
    }

    if (!node->isLeaf)
    {
        for (int i = 0; i <= node->limit; i++)
        {
            serializeBPlusNode(pk, node->children[i]);
        }
    }
    else
    {
        msgpack_pack_nil(pk);
    }
}

void saveBPlusTree(const char *fileName, BPlusTree *tree)
{
    msgpack_sbuffer sbuf;
    msgpack_packer pk;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_array(&pk, 2);
    msgpack_pack_int(&pk, tree->t);
    serializeBPlusNode(&pk, tree->root);
}

FileInfor deserializeFileInfor(msgpack_object *obj)
{
    FileInfor infor;
    msgpack_object_array array = obj->via.array;
    infor.deleted = array.ptr[0].via.boolean;
    infor.filename = my_strndup(array.ptr[1].via.str.ptr, array.ptr[1].via.str.size);
    infor.offset = array.ptr[2].via.i64;
    infor.length = array.ptr[3].via.i64;
    return infor;
}

StorageInfor deserializeStorageInfor(msgpack_object *obj)
{
    StorageInfor infor;
    msgpack_object_array array = obj->via.array;
    infor.searchKey = array.ptr[0].via.i64;
    infor.infor = deserializeFileInfor(&array.ptr[1]);
    return infor;
}

BPlusNode *deserializeBPlusNode(msgpack_object *obj)
{
    if (obj->type != MSGPACK_OBJECT_ARRAY)
        return NULL;

    BPlusNode *node = malloc(sizeof(BPlusNode));
    msgpack_object_array array = obj->via.array;
    node->isLeaf = array.ptr[0].via.boolean;
    node->numKeys = array.ptr[1].via.i64;
    node->limit = array.ptr[2].via.i64;

    node->keys = malloc((node->limit - 1) * sizeof(int));
    for (int i = 0; i < node->numKeys; i++)
    {
        node->keys[i] = array.ptr[3].via.array.ptr[i].via.i64;
    }

    node->infors = malloc(node->limit * sizeof(StorageInfor));
    for (int i = 0; i < node->numKeys; i++)
    {
        node->infors[i] = deserializeStorageInfor(&array.ptr[4].via.array.ptr[i]);
    }

    if (!node->isLeaf)
    {
        node->children = malloc(node->limit * sizeof(BPlusNode *));
        for (int i = 0; i <= node->numKeys; i++)
        {
            node->children[i] = deserializeBPlusNode(&array.ptr[5].via.array.ptr[i]);
        }
    }
    else
    {
        node->children = NULL;
    }
    return node;
}

BPlusTree *loadBPlusTree(msgpack_object *obj)
{
    if (obj->type != MSGPACK_OBJECT_ARRAY)
        return NULL;

    BPlusTree *tree = malloc(sizeof(BPlusTree));
    tree->t = obj->via.array.ptr[0].via.i64;
    tree->root = deserializeBPlusNode(&obj->via.array.ptr[1]);
    return tree;
}

BPlusTree *loadBPlusTreeFromFile(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(filesize);
    if (!buffer)
    {
        perror("Memory allocation error");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, filesize, file);
    fclose(file);

    msgpack_unpacked result;
    msgpack_unpacked_init(&result);

    BPlusTree *tree = NULL;
    if (msgpack_unpack_next(&result, buffer, filesize, NULL))
    {
        tree = deserialize_tree(&result.data);
    }

    msgpack_unpacked_destroy(&result);
    free(buffer);

    return tree;
}