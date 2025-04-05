#include "bplustree.h"

FileInfor buildFileInfor(char filename[MAX_FILE_NAME], long offset, long length)
{
    FileInfor infor;
    strcpy(infor.filename, filename);
    infor.offset = offset;
    infor.length = length;
    infor.deleted = false;
    return infor;
}

StorageInfor buildStorageInfor(int searchKey, FileInfor fileInfor)
{
    StorageInfor sInfor;
    sInfor.searchKey = searchKey;
    sInfor.infor = fileInfor;
    return sInfor;
}

BPlusNode *buildNode(bool isLeaf, int t)
{
    BPlusNode *node = (BPlusNode *)malloc(sizeof(BPlusNode));
    node->isLeaf = isLeaf;
    node->numKeys = 0;
    node->limit = t;
    node->keys = (int *)malloc((t - 1) * sizeof(int));
    node->infors = (StorageInfor *)malloc((t * sizeof(StorageInfor)));
    node->children = (BPlusNode **)malloc((t * sizeof(BPlusNode)));
    node->next = NULL;
    return node;
}

BPlusTree *buildTree(int t)
{
    BPlusTree *tree = (BPlusTree *)malloc(sizeof(BPlusTree));
    tree->root = NULL;
    tree->t = t;
    return tree;
}

BPlusNode *findLeaf(BPlusNode *node, int key)
{
    BPlusNode *current = node;
    while (!current->isLeaf)
    {
        int i = 0;
        while (i < current->numKeys && key > current->keys[i])
        {
            i++;
        }
        current = current->children[i];
    }
    return current;
}

StorageInfor *retrieveKey(BPlusTree *tree, int bKey, int inforKey)
{
    if (tree->root->numKeys == 0)
    {
        return NULL;
    }

    BPlusNode *leaf = findLeaf(tree->root, bKey);
#ifdef DEBUG
    printf("retrieveKey: bKey = %d, inforKey = %d\n",
           bKey, inforKey);
    if (leaf != NULL)
    {
        printf("Key: %d\n", leaf->numKeys);
        for (int i = 0; i < leaf->numKeys; i++)
        {
            printf("Infor : %d\n", leaf->infors[i].searchKey);
        }
    }
    else
    {
        printf("No key found!\n");
    }
#endif
    for (int i = 0; i < leaf->numKeys; i++)
    {
        if (leaf->infors[i].searchKey == inforKey)
        {
            return &leaf->infors[i];
        }
    }

    return NULL;
}

bool existKey(BPlusTree *tree, int bKey, int inforKey)
{
    StorageInfor *result = retrieveKey(tree, bKey, inforKey);
    if (!result)
    {
        return false;
    }
    return !result->infor.deleted;
}

void splitChild(BPlusNode *parent, int index, BPlusNode *child)
{
    BPlusNode *newChild = buildNode(child->isLeaf, child->limit);

    int mid = (child->limit - 1) / 2;
    newChild->numKeys = (child->limit - 1) - mid - (child->isLeaf ? 0 : 1);
    child->numKeys = mid;

    //// Phần xử lí tách node cùng cấp
    if (child->isLeaf)
    {
        for (int i = 0; i < newChild->numKeys; i++)
        {
            newChild->keys[i] = child->keys[i + mid];
        }
        for (int i = 0; i <= newChild->numKeys; i++)
        {
            newChild->infors[i] = child->infors[i + mid];
        }
        newChild->next = child->next;
        child->next = newChild;
    }
    else
    {
        for (int i = 0; i < newChild->numKeys; i++)
        {
            newChild->keys[i] = child->keys[i + mid];
        }
        for (int i = 0; i <= newChild->numKeys; i++)
        {
            newChild->children[i] = child->children[i + mid];
        }
        newChild->children[newChild->numKeys] = child->children[child->limit - 1];
    }

    //// Phần xử lí tách node khác cấp
    for (int i = parent->numKeys; i > index; i--)
    {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = newChild;
    for (int i = parent->numKeys - 1; i >= index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }
    parent->keys[index] = child->keys[mid];
    parent->numKeys++;
}

void insertNonFull(BPlusNode *node, int key, StorageInfor infor)
{
    /// Tìm vị trí để insert
    if (node->isLeaf)
    {
        int i = node->numKeys - 1;
        while (i >= 0 && node->keys[i] > key)
        {
            i--;
        }

        node->numKeys++;
        for (int j = node->numKeys; j > i + 1; j--)
        {
            node->keys[j] = node->keys[j - 1];
            node->infors[j] = node->infors[j - 1];
        }

        node->keys[i + 1] = key;
        node->infors[i + 1] = infor;
    }
    else
    {
        /// Chỉ xảy ra trường hợp đệ quy tìm để chèn
        int i = 0;
        while (i < node->numKeys && key >= node->keys[i])
        {
            i++;
        }
        if (node->children[i]->numKeys == node->children[i]->limit - 1)
        {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i])
            {
                i++;
            }
        }
        insertNonFull(node->children[i], key, infor);
    }
}

void insertInfor(BPlusTree *tree, int key, StorageInfor infor)
{
    if (!tree->root)
    {
        tree->root = buildNode(true, tree->t);
        tree->root->numKeys = 1;
        tree->root->infors[0] = infor;
        tree->root->keys[0] = key;
        return;
    }

    if (tree->root->numKeys == tree->t - 1)
    {
        /// Trường hợp Full
        BPlusNode *newRoot = buildNode(false, tree->t);
        newRoot->children[0] = tree->root;
        splitChild(newRoot, 0, tree->root);
        insertNonFull(newRoot, key, infor);
        tree->root = newRoot;
    }
    else
    {
        insertNonFull(tree->root, key, infor);
    }
}

void deleteInfor(BPlusTree *tree, int bKey, int searchKey)
{
    StorageInfor *sInfor = retrieveKey(tree, bKey, searchKey);
    if (sInfor != NULL)
    {
        sInfor->infor.deleted = true;
    }
}

#ifdef PRINT_TREE
void printNode(BPlusNode *node, int level)
{
    if (!node)
    {
        return;
    }

    for (int i = 0; i < level; i++)
    {
        printf(" ");
    }

    if (node->isLeaf)
    {
        printf("Leaf Node (Keys : %d) [ ", node->numKeys);
        for (int i = 0; i < node->numKeys; i++)
        {
            printf("%d", node->keys[i]);
            if (i != node->numKeys - 1)
            {
                printf(", ");
            }
        }
        printf("]\n");
        for (int i = 0; i < node->numKeys; i++)
        {
            for (int j = 0; j < level; j++)
            {
                printf(" ");
            }
            printf("  SearchKey: %d, Filename: %s, Offset: %ld, Length: %ld, Delete? : %d\n",
                   node->infors[i].searchKey,
                   node->infors[i]
                       .infor.filename,
                   node->infors[i].infor.offset,
                   node->infors[i].infor.length,
                   node->infors[i].infor.deleted);
        }
    }
    else
    {
        printf("Internal Node (Keys: %d) [ ", node->numKeys);
        for (int i = 0; i < node->numKeys; i++)
        {
            printf("%d", node->keys[i]);
            if (i != node->numKeys - 1)
            {
                printf(", ");
            }
        }
        printf("]\n");
        for (int i = 0; i <= node->numKeys; i++)
        {
            printNode(node->children[i], level + 2);
        }
    }
}

void printTree(BPlusTree *tree)
{
    printf("B+ Tree Data Structure\n");
    printNode(tree->root, 0);
    printf("\n");
    printf("----------------------------\n");
}

#endif