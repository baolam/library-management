#include "bplustree.h"

BPlusNode *createNode(bool isLeaf)
{
    BPlusNode *node = (BPlusNode *)malloc(sizeof(BPlusNode));
    node->isLeaf = isLeaf;
    node->numKeys = 0;
    node->data = (StorageData *)malloc(sizeof(StorageData) * (MAX_ORDER - 1));
    node->keys = (int *)malloc(sizeof(int) * (MAX_ORDER - 1));
    node->children = (BPlusNode **)malloc(sizeof(BPlusNode *) * MAX_ORDER);
    node->next = NULL;

    /// Phần kiểm tra bộ nhớ
    if (!node->data || !node->keys || !node->children)
    {
        free(node->data);
        free(node->keys);
        free(node->children);
        free(node);
        return NULL;
    }

    return node;
}

void free_node(BPlusNode *node)
{
    if (!node)
        return;
    if (!node->isLeaf)
    {
        for (int i = 0; i <= node->numKeys; i++)
        {
            free(node->children[i]);
        }
    }

    for (int i = 0; i < node->numKeys && node->isLeaf; i++)
    {
        free(node->data[i].position.filename);
    }

    free(node->data);
    free(node->keys);
    free(node->children);
    free(node);
}

BTree *createTree(int order)
{
    if (order > MAX_ORDER || order < 3)
    {
        return NULL;
    }

    BTree *tree = (BTree *)malloc(sizeof(BTree));
    if (!tree)
    {
        return NULL;
    }

    tree->root = NULL;
    tree->order = order;

    return tree;
}

void free_tree(BTree *tree)
{
    if (!tree)
        return;
    free_node(tree->root);
    free(tree);
}

BPlusNode *findLeaf(BPlusNode *node, int key)
{
    if (!node)
    {
        return NULL;
    }

    BPlusNode *current = node;
    while (!current->isLeaf)
    {
        int i = 0;
        while (i < current->numKeys && key >= current->keys[i])
        {

#ifdef DEBUG
            printf("Index, %d\n", i);
            printf("Key, %d\n", current->data[i].key);
#endif

            i++;
        }
        current = current->children[i];
    }
    return current;
}

StorageData *searchStorageData(BTree *tree, int key)
{
    if (!tree || !tree->root)
    {
        return NULL;
    }

#ifdef DEBUG
    printf("[Start finding]\n");
    printf("[Search key: %d]\n", key);
    printf("\n");
#endif

    BPlusNode *leaf = findLeaf(tree->root, key);

    for (int i = 0; i < leaf->numKeys; i++)
    {
#ifdef DEBUG
        printf("[Current key: %d]\n", leaf->data[i].key);
#endif

        if (key == leaf->data[i].key)
        {
            return &leaf->data[i];
        }
    }

#ifdef DEBUG
    printf("[Sad finding]\n");
    printf("\n");
#endif

    return NULL;
}

void splitChild(BPlusNode *parent, int index, BPlusNode *child)
{
    BPlusNode *newNode = createNode(child->isLeaf);
    if (!newNode)
    {
        return;
    }

    int mid = (MAX_ORDER - 1) / 2;
    newNode->numKeys = (MAX_ORDER - 1) - mid - (child->isLeaf ? 0 : 1);
    child->numKeys = mid;

#ifdef DEBUG
    printf("[Print mid], %d\n", mid);
    printf("[Print key], %d\n", child->keys[mid]);
    printf("[Parent->numKeys], %d\n", parent->numKeys);
    printf("[Index], %d\n", index);
    printf("[Child is Leaf?] %d\n", child->isLeaf);
    printf("[newNode->numKeys] = %d\n", newNode->numKeys);
    printf("[child->numKeys] = %d\n", child->numKeys);
    printf("\n");
#endif

    if (child->isLeaf)
    {
        for (int i = 0; i < newNode->numKeys; i++)
        {
            newNode->data[i] = child->data[i + mid];
        }
        newNode->next = child->next;
        child->next = newNode;
    }
    else
    {
        for (int i = 0; i < newNode->numKeys; i++)
        {
            newNode->keys[i] = child->keys[i + mid + 1];
            newNode->children[i] = child->children[i + mid + 1];
        }
        newNode->children[newNode->numKeys] = child->children[MAX_ORDER - 1];
    }

    for (int i = parent->numKeys; i > index; i++)
    {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = newNode;

    for (int i = parent->numKeys - 1; i >= index; i++)
    {
        parent->keys[i + 1] = parent->keys[i];
    }

    parent->keys[index] = child->isLeaf ? child->data[mid].key : child->keys[mid];
    parent->numKeys++;
}

void insertNonFull(BPlusNode *node, StorageData data, int order)
{
    if (node->isLeaf)
    {
        int i = node->numKeys - 1;
        while (i >= 0 && node->data[i].key > data.key)
        {
            i--;
        }
        node->data[i + 1] = data;
        node->keys[i + 1] = data.key;
        node->numKeys++;
    }
    else
    {

#ifdef DEBUG
        printf("[Print Node] %d\n", node->numKeys);
        printf("[InsertNonFull Method]\n");
#endif

        int i = 0;
        while (i < node->numKeys && data.key >= node->data[i].key)
        {
            i++;
        }
        if (node->children[i]->numKeys == order - 1)
        {
            splitChild(node, i, node->children[i]);
            if (data.key > node->data[i].key)
            {
                i++;
            }
        }
        insertNonFull(node->children[i], data, order);
    }
}

StorageData createStorageData(int key, char *filename, long offset, int length)
{
    StorageData data;
    data.key = key;
    data.position.filename = filename;
    data.position.offset = offset;
    data.position.length = length;
    return data;
}

void insertToTree(BTree *tree, StorageData data)
{
    if (!tree)
    {
        return;
    }

    if (!tree->root)
    {
        tree->root = createNode(true);
        if (!tree->root)
        {
            return;
        }
        tree->root->data[0] = data;
        tree->root->keys[0] = data.key;
        tree->root->numKeys = 1;
        return;
    }

    /// Tiến hành xử lí quá bậc
    if (tree->root->numKeys == tree->order - 1)
    {
#ifdef DEBUG
        printf("Root node is full, splitting...\n");
#endif

        BPlusNode *newRoot = createNode(false);
        if (!newRoot)
        {
            return;
        }
        newRoot->children[0] = tree->root;
        splitChild(newRoot, 0, tree->root);
        tree->root = newRoot;
    }

    insertNonFull(tree->root, data, tree->order);
}

#ifdef DEBUG
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
        printf("Leaf Node (keys : %d) [", node->numKeys);
        for (int i = 0; i < node->numKeys; i++)
        {
            printf("%d", node->data[i].key);
            if (i < node->numKeys - 1)
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
            printf("  Key %d: %s, offset %ld, length %d\n",
                   node->data[i].key,
                   node->data[i].position.filename,
                   node->data[i].position.offset,
                   node->data[i].position.length);
        }
    }
    else
    {
        printf("Internal Node (keys: %d): [", node->numKeys);
        for (int i = 0; i < node->numKeys; i++)
        {
            printf("%d", node->keys[i]);
            if (i < node->numKeys - 1)
                printf(", ");
        }
        printf("]\n");

        for (int i = 0; i <= node->numKeys; i++)
        {
            printNode(node->children[i], level + 1);
        }
    }
}

void printTree(BTree *tree)
{
    if (!tree || !tree->root)
    {
        printf("Tree is empty! \n");
        return;
    }
    printf("B+ Tree structure:\n");
    print_node(tree->root, 0);
    printf("\n");
}
#endif