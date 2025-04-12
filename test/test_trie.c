#include <stdio.h>
#include <stdlib.h>
#include "utils/trie.h"

void test_trie_operations()
{
    printf("Creating trie...\n");
    TrieNode *root = makeTrieNode();

    printf("Inserting words...\n");
    insertIntoTrie(root, "hello", 1);
    insertIntoTrie(root, "world", 2);
    insertIntoTrie(root, "hello", 3); // Duplicate word, different ID
    insertIntoTrie(root, "foo", 4);
    insertIntoTrie(root, "bar", 5);
    insertIntoTrie(root, "hello", 6); // Duplicate word, different ID

    printf("Checking word existence...\n");
    printf("'hello' exists: %d\n", existWord(root, "hello"));
    printf("'world' exists: %d\n", existWord(root, "world"));
    printf("'test' exists: %d\n", existWord(root, "test"));

    printf("Saving trie to file...\n");
    saveTrieTree("test/trie.dat", root);

    printf("Freeing trie...\n");
    freeTrie(root);

    printf("Loading trie from file...\n");
    root = loadTrieTree("test/trie.dat");

    printf("Checking loaded trie...\n");

    printf("1. Checking if root exists...\n");
    if (!root)
    {
        printf("ERROR: Loaded root is NULL!\n");
        return;
    }

    printf("2. Checking 'hello'...\n");
    bool helloExists = existWord(root, "hello");
    printf("'hello' exists: %d\n", helloExists);

    printf("3. Checking 'world'...\n");
    bool worldExists = existWord(root, "world");
    printf("'world' exists: %d\n", worldExists);

    printf("4. Checking 'test' (should not exist)...\n");
    bool testExists = existWord(root, "test");
    printf("'test' exists: %d\n", testExists);

    int recommendIndex = 0;
    char *recommends[5];

    printf("5. Getting recommendations for 'hello'...\n");
    recommendPrefix(root, "he", 5, recommends, &recommendIndex);
    for (int id = 0; id < recommendIndex; id++)
    {
        char *tmp = recommends[id];
        printf("Recommendation %d: %s\n", id + 1, tmp);
        TrieNode *result = searchWord(root, tmp);
        for (int j = 0; j < result->numIds; j++)
        {
            printf("Key: %d, ", result->ids[j]);
        }
        printf("\n");
    }

    printf("Freeing trie again...\n");
    if (root)
    {
        freeTrie(root);
    }
}

int main()
{
    test_trie_operations();
    return 0;
}
