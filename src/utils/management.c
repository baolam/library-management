#include "management.h"

void readContent(const char *filename, FileInfor infor, void (*callback)(FILE *f, long length))
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fseek(file, infor.offset, SEEK_SET);
    callback(file, infor.length);
    fclose(file);
}

void writeContent(const char *filename, FileInfor infor, void (*callback)(FILE *f, long length))
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fseek(file, infor.offset, SEEK_SET);
    /// Callback nhớ gọi fwrite
    callback(file, infor.length);
    fclose(file);
}

void serializeNode(FILE *f, BPlusNode *node)
{
    if (!f || !node)
        return;

    // Write node metadata
    if (fwrite(&node->isLeaf, sizeof(bool), 1, f) != 1)
        return;
    if (fwrite(&node->numKeys, sizeof(int), 1, f) != 1)
        return;
    if (fwrite(&node->limit, sizeof(int), 1, f) != 1)
        return;

    // Write keys array
    if (node->numKeys > 0)
    {
        if (fwrite(node->keys, sizeof(int), node->numKeys, f) != node->numKeys)
            return;
    }

    if (node->isLeaf)
    {
        // Write leaf node information
        for (int i = 0; i < node->numKeys; i++)
        {
            if (fwrite(&node->infors[i].searchKey, sizeof(int), 1, f) != 1)
                return;
            if (fwrite(&node->infors[i].infor.deleted, sizeof(bool), 1, f) != 1)
                return;
            if (fwrite(node->infors[i].infor.filename, sizeof(char), MAX_FILE_NAME, f) != MAX_FILE_NAME)
                return;
            if (fwrite(&node->infors[i].infor.offset, sizeof(long), 1, f) != 1)
                return;
            if (fwrite(&node->infors[i].infor.length, sizeof(long), 1, f) != 1)
                return;
        }
    }
    else
    {
        // Write internal node children
        for (int i = 0; i <= node->numKeys; i++)
        {
            serializeNode(f, node->children[i]);
        }
    }
}

void writeBPlusTree(const char *filename, BPlusTree *tree)
{
    if (!filename || !tree || !tree->root)
    {
        return;
    }

    FILE *f = fopen(filename, "wb");
    if (!f)
    {
        return;
    }

    // Write tree metadata
    if (fwrite(&tree->t, sizeof(tree->t), 1, f) != 1)
    {
        fclose(f);
        return;
    }

    // Serialize the tree structure
    serializeNode(f, tree->root);

    fclose(f);
}

BPlusNode *readBPlusNode(FILE *f, int t)
{
    if (!f)
        return NULL;

    BPlusNode *node = buildNode(false, t);
    if (!node)
        return NULL;

    // Read node metadata
    if (fread(&node->isLeaf, sizeof(bool), 1, f) != 1)
        goto error;
    if (fread(&node->numKeys, sizeof(int), 1, f) != 1)
        goto error;
    if (fread(&node->limit, sizeof(int), 1, f) != 1)
        goto error;

    // Validate numKeys before reading
    if (node->numKeys < 0 || node->numKeys > node->limit)
        goto error;

    // Read keys array
    if (node->numKeys > 0)
    {
        if (fread(node->keys, sizeof(int), node->numKeys, f) != node->numKeys)
            goto error;
    }

    if (node->isLeaf)
    {
        // Read leaf node information
        for (int i = 0; i < node->numKeys; i++)
        {
            if (fread(&node->infors[i].searchKey, sizeof(int), 1, f) != 1)
                goto error;
            if (fread(&node->infors[i].infor.deleted, sizeof(bool), 1, f) != 1)
                goto error;
            if (fread(node->infors[i].infor.filename, sizeof(char), MAX_FILE_NAME, f) != MAX_FILE_NAME)
                goto error;
            if (fread(&node->infors[i].infor.offset, sizeof(long), 1, f) != 1)
                goto error;
            if (fread(&node->infors[i].infor.length, sizeof(long), 1, f) != 1)
                goto error;
        }
        node->next = NULL;
    }
    else
    {
        // Read internal node children
        for (int i = 0; i <= node->numKeys; i++)
        {
            node->children[i] = readBPlusNode(f, t);
            if (!node->children[i])
                goto error;
        }
    }

    return node;

error:
    // TODO: Add proper node cleanup here
    return NULL;
}

BPlusTree *readBPlusTree(const char *filename)
{
    if (!filename)
        return NULL;

    FILE *f = fopen(filename, "rb");
    if (!f)
        return NULL;

    BPlusTree *tree = (BPlusTree *)malloc(sizeof(BPlusTree));
    if (!tree)
    {
        fclose(f);
        return NULL;
    }

    // Read tree metadata
    if (fread(&tree->t, sizeof(int), 1, f) != 1)
    {
        free(tree);
        fclose(f);
        return NULL;
    }

    // Rebuild the tree structure
    tree->root = readBPlusNode(f, tree->t);
    if (!tree->root)
    {
        free(tree);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return tree;
}
