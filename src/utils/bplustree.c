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
    // Validate input
    if (t <= 0)
        return NULL;

    // Allocate node structure
    BPlusNode *node = (BPlusNode *)malloc(sizeof(BPlusNode));
    if (!node)
        return NULL;

    // Initialize basic properties
    node->isLeaf = isLeaf;
    node->numKeys = 0;
    node->limit = t;
    node->next = NULL;

    // Allocate keys array
    node->keys = (int *)malloc(t * sizeof(int));
    if (!node->keys)
    {
        free(node);
        return NULL;
    }

    // Allocate storage info (for leaf nodes) or children (for internal nodes)
    if (isLeaf)
    {
        node->infors = (StorageInfor *)malloc(t * sizeof(StorageInfor));
        node->children = NULL;
        if (!node->infors)
        {
            free(node->keys);
            free(node);
            return NULL;
        }
    }
    else
    {
        node->infors = NULL;
        node->children = (BPlusNode **)malloc((t + 1) * sizeof(BPlusNode *));
        if (!node->children)
        {
            free(node->keys);
            free(node);
            return NULL;
        }
        // Initialize children to NULL
        for (int i = 0; i <= t; i++)
        {
            node->children[i] = NULL;
        }
    }

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
    // Validate inputs
    if (!parent || !child || index < 0 || index > parent->numKeys)
    {
        return;
    }

    // Create new child node
    BPlusNode *newChild = buildNode(child->isLeaf, child->limit);
    if (!newChild)
    {
        return; // Allocation failed
    }

    // Calculate split point with validation
    int t = child->limit;
    if (t <= 1 || child->numKeys <= 0)
    {
        free(newChild);
        return;
    }

    int mid = (t - 1) / 2;
    newChild->numKeys = t - 1 - mid;
    child->numKeys = mid;

    // Handle leaf node split
    if (child->isLeaf)
    {
        // Validate array bounds before copying
        if (mid + newChild->numKeys - 1 >= t)
        {
            free(newChild);
            return;
        }

        // Copy keys and infors to new child
        for (int i = 0; i < newChild->numKeys; i++)
        {
            if (mid + i < t)
            {
                newChild->keys[i] = child->keys[mid + i];
                newChild->infors[i] = child->infors[mid + i];
            }
        }

        // Maintain leaf chain
        newChild->next = child->next;
        child->next = newChild;
    }
    // Handle internal node split
    else
    {
        // Copy keys to new child
        for (int i = 0; i < newChild->numKeys; i++)
        {
            newChild->keys[i] = child->keys[mid + 1 + i];
        }

        // Copy children pointers
        for (int i = 0; i <= newChild->numKeys; i++)
        {
            newChild->children[i] = child->children[mid + 1 + i];
        }
    }

    // Insert new child into parent
    // Shift parent's children to make space
    for (int i = parent->numKeys; i > index; i--)
    {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = newChild;

    // Shift parent's keys and insert middle key
    for (int i = parent->numKeys - 1; i >= index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }
    parent->keys[index] = child->keys[mid];
    parent->numKeys++;
}

void insertNonFull(BPlusNode *node, int key, StorageInfor infor)
{
    // Validate inputs
    if (!node || node->numKeys >= node->limit)
    {
        return;
    }

    // Handle leaf node insertion
    if (node->isLeaf)
    {
        // Find insertion position
        int pos = node->numKeys - 1;
        while (pos >= 0 && node->keys[pos] > key)
        {
            pos--;
        }
        pos++; // Position to insert at

        // Shift elements to make space
        for (int i = node->numKeys; i > pos; i--)
        {
            node->keys[i] = node->keys[i - 1];
            node->infors[i] = node->infors[i - 1];
        }

        // Insert new key and info
        node->keys[pos] = key;
        node->infors[pos] = infor;
        node->numKeys++;
    }
    // Handle internal node insertion
    else
    {
        // Find child to insert into
        int child_idx = 0;
        while (child_idx < node->numKeys && key >= node->keys[child_idx])
        {
            child_idx++;
        }

        // Validate child pointer
        if (child_idx < 0 || child_idx > node->numKeys || !node->children[child_idx])
        {
            return;
        }

        // Check if child needs splitting
        if (node->children[child_idx]->numKeys == node->children[child_idx]->limit - 1)
        {
            splitChild(node, child_idx, node->children[child_idx]);
            // After split, check which child to insert into
            if (child_idx < node->numKeys && key > node->keys[child_idx])
            {
                child_idx++;
            }
        }

        // Validate child pointer again after potential split
        if (child_idx < 0 || child_idx > node->numKeys || !node->children[child_idx])
        {
            return;
        }

        // Recursively insert into child
        insertNonFull(node->children[child_idx], key, infor);
    }
}

void insertInfor(BPlusTree *tree, int key, StorageInfor infor)
{
    // Validate inputs
    if (!tree || tree->t <= 0)
    {
        return;
    }

    // Handle empty tree case
    if (!tree->root)
    {
        tree->root = buildNode(true, tree->t);
        if (!tree->root)
        {
            return; // Allocation failed
        }
        tree->root->keys[0] = key;
        tree->root->infors[0] = infor;
        tree->root->numKeys = 1;
        return;
    }

    // Handle root full case
    if (tree->root->numKeys == tree->t - 1)
    {
        // Create new root
        BPlusNode *newRoot = buildNode(false, tree->t);
        if (!newRoot)
        {
            return; // Allocation failed
        }

        // Set up new root and split old root
        newRoot->children[0] = tree->root;
        splitChild(newRoot, 0, tree->root);

        // Insert into appropriate child
        int child_idx = 0;
        if (key > newRoot->keys[0])
        {
            child_idx++;
        }
        insertNonFull(newRoot->children[child_idx], key, infor);

        // Update tree root
        tree->root = newRoot;
    }
    // Normal insertion case
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

static void freeNodeRecursive(BPlusNode *node)
{
    if (!node)
        return;

    // Free children recursively for internal nodes
    if (!node->isLeaf && node->children)
    {
        for (int i = 0; i <= node->numKeys; i++)
        {
            freeNodeRecursive(node->children[i]);
        }
        free(node->children);
    }

    // Free keys and infors
    if (node->keys)
        free(node->keys);
    if (node->isLeaf && node->infors)
        free(node->infors);

    free(node);
}

void freeBPlusTree(BPlusTree *tree)
{
    if (!tree)
        return;

    if (tree->root)
    {
        freeNodeRecursive(tree->root);
    }

    free(tree);
}
