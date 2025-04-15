#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/bplustreev2.h"

#define TEST_FILE "test_bplustree.bin"

void test_save_load()
{
    Node *root = NULL;

    // Insert 10 keys and records
    root = insert(root, 10, "source1", 100, 10);
    root = insert(root, 20, "source2", 200, 20);
    root = insert(root, 5, "source3", 50, 5);
    root = insert(root, 15, "source4", 150, 15);
    root = insert(root, 25, "source5", 250, 25);
    root = insert(root, 30, "source6", 300, 30);
    root = insert(root, 35, "source7", 350, 35);
    root = insert(root, 40, "source8", 400, 40);
    root = insert(root, 45, "source9", 450, 45);
    root = insert(root, 50, "source10", 500, 50);

    printf("Original tree:\n");
#ifdef PRINT_TREE
    printTree(root);
#endif

    // Save the tree
    saveTree(root, TEST_FILE);

    // Load the tree
    Node *loaded_root = loadTree(TEST_FILE);

    printf("Loaded tree:\n");
#ifdef PRINT_TREE
    printTree(loaded_root);
#endif

    // Test search on loaded tree
    int keys_to_test[] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55};
    for (int i = 0; i < sizeof(keys_to_test) / sizeof(int); i++)
    {
        int key = keys_to_test[i];
        Record *rec = find(loaded_root, key);
        if (rec != NULL)
        {
            printf("Found key %d: source=%s, offset=%ld, length=%ld\n", key, rec->_from, rec->offset, rec->length);
        }
        else
        {
            printf("Key %d not found\n", key);
        }
    }
}

void test_leaf_node_traversal()
{
    Node *root = NULL;

    // Insert 10 keys and records
    root = insert(root, 10, "source1", 100, 10);
    root = insert(root, 20, "source2", 200, 20);
    root = insert(root, 5, "source3", 50, 5);
    root = insert(root, 15, "source4", 150, 15);
    root = insert(root, 25, "source5", 250, 25);
    root = insert(root, 30, "source6", 300, 30);
    root = insert(root, 35, "source7", 350, 35);
    root = insert(root, 40, "source8", 400, 40);
    root = insert(root, 45, "source9", 450, 45);
    root = insert(root, 50, "source10", 500, 50);

    // Save the tree
    saveTree(root, TEST_FILE);

    // Load the tree
    Node *loaded_root = loadTree(TEST_FILE);

    printf("Traverse leaf nodes after loading:\n");

    // Find leftmost leaf node
    Node *leaf = leftMost(loaded_root);

    // Traverse leaf nodes using pointers[ORDER - 1]
    while (leaf != NULL)
    {
        printf("Leaf node keys: ");
        for (int i = 0; i < leaf->num_keys; i++)
        {
            Record *rec = (Record *)leaf->pointers[i];
            printf("%d(source=%s, offset=%ld, length=%ld) ", leaf->keys[i], rec->_from, rec->offset, rec->length);
        }
        printf("\n");
        leaf = (Node *)leaf->pointers[ORDER - 1];
    }
}

int main()
{
    test_save_load();
    test_leaf_node_traversal();
    return 0;
}
