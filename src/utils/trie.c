#include "trie.h"

// Convert character to index (a-z and ')
int charToIndex(char ch)
{
    if (ch == ' ')
        return 26;
    return ch - 'a';
}

// void hashWord(mpz_t result, char *word)
// {
//     mpz_set_ui(result, 0);
//     for (int i = 0; i < strlen(word); i++)
//     {
//         mpz_mul_ui(result, result, BASE);
//         mpz_add_ui(result, result, charToIndex(word[i]));
//     }
// }

// Create a new Trie node
TrieNode *makeTrieNode(void)
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node)
    {
        node->isEndOfWord = false;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            node->children[i] = NULL;
        }
        // mpz_init(node->id);
        node->id = -1;
    }
    return node;
}

// Search for a prefix in the trie
TrieNode *searchPrefix(TrieNode *root, char *prefix)
{
    TrieNode *current = root;
    for (int i = 0; prefix[i] != '\0'; i++)
    {
        int index = charToIndex(prefix[i]);
        if (!current->children[index])
        {
            return NULL;
        }
        current = current->children[index];
    }
    return current;
}

// Check if prefix exists
bool existPrefix(TrieNode *root, char *prefix)
{
    return searchPrefix(root, prefix) != NULL;
}

// Check if word exists
bool existWord(TrieNode *root, char *word)
{
    TrieNode *node = searchPrefix(root, word);
    return node != NULL && node->isEndOfWord;
}

void toLowerCase(char *word)
{
    int i;
    for (i = 0; i < strlen(word); i++)
        word[i] = tolower(word[i]);
}

// Insert word into trie
void insertIntoTrie(TrieNode *root, char *word, int id)
{
    /// Make sure lowercase
    char lower[MAX_CHAR_LENGTH];
    strcpy(lower, word);
    toLowerCase(lower);

    TrieNode *current = root;
    for (int i = 0; lower[i] != '\0'; i++)
    {
        int index = charToIndex(lower[i]);
        if (!current->children[index])
        {
            current->children[index] = makeTrieNode();
        }
        current = current->children[index];
    }

    current->isEndOfWord = true;
    // hashWord(current->id, word);
    current->id = id;
}

// Helper function to remove word recursively
static bool removeHelper(TrieNode *node, char *word, int depth)
{
    if (!node)
        return false;

    if (word[depth] == '\0')
    {
        if (node->isEndOfWord)
        {
            node->isEndOfWord = false;
            for (int i = 0; i < ALPHABET_SIZE; i++)
            {
                if (node->children[i])
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    int index = charToIndex(word[depth]);
    if (removeHelper(node->children[index], word, depth + 1))
    {
        free(node->children[index]);
        node->children[index] = NULL;

        if (!node->isEndOfWord)
        {
            for (int i = 0; i < ALPHABET_SIZE; i++)
            {
                if (node->children[i])
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

// Remove word from trie
void removeWord(TrieNode *root, char *word)
{
    removeHelper(root, word, 0);
}

// Helper function to find all words with given prefix
static void findWords(TrieNode *node, char *prefix, char *buffer, int depth,
                      char **recommend, int *count, int maxRecommend)
{
    if (*count >= maxRecommend)
        return;

    if (node->isEndOfWord)
    {
        buffer[depth] = '\0';
        recommend[*count] = (char *)malloc(strlen(prefix) + depth + 1);
        strcpy(recommend[*count], prefix);
        strcat(recommend[*count], buffer);
        (*count)++;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i])
        {
            buffer[depth] = (i == 26) ? ' ' : 'a' + i;
            findWords(node->children[i], prefix, buffer, depth + 1,
                      recommend, count, maxRecommend);
        }
    }
}

// Recommend words with given prefix
void recommendPrefix(TrieNode *root, char *prefix, int maxRecommend,
                     char **recommend, int *recommendSize)
{
    *recommendSize = 0;
    if (!root || !prefix)
        return;

    TrieNode *node = searchPrefix(root, prefix);
    if (!node)
        return;

    char buffer[MAX_CHAR_LENGTH];
    findWords(node, prefix, buffer, 0, recommend, recommendSize, maxRecommend);
}

// Free entire trie structure recursively
void freeTrie(TrieNode *root)
{
    if (!root)
        return;

    // Recursively free all children
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i])
        {
            freeTrie(root->children[i]);
        }
    }

    // Free current node
    free(root);
}

// Save trie to file (binary format)
void saveTree(char *filename, TrieNode *root)
{
    if (!root)
        return;

    FILE *file = fopen(filename, "wb");
    if (!file)
        return;

    // Write node data
    fwrite(&root->isEndOfWord, sizeof(bool), 1, file);
    fwrite(&root->id, sizeof(int), 1, file);

    // Write children markers and recurse
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        bool hasChild = (root->children[i] != NULL);
        fwrite(&hasChild, sizeof(bool), 1, file);

        if (hasChild)
        {
            saveTree(filename, root->children[i]);
        }
    }

    fclose(file);
}

// Helper function for loading trie
static TrieNode *loadTreeHelper(FILE *file)
{
    if (!file)
        return NULL;

    TrieNode *root = makeTrieNode();
    if (!root)
        return NULL;

    // Read node data
    if (fread(&root->isEndOfWord, sizeof(bool), 1, file) != 1)
    {
        freeTrie(root);
        return NULL;
    }
    if (fread(&root->id, sizeof(int), 1, file) != 1)
    {
        freeTrie(root);
        return NULL;
    }

    // Read and reconstruct children
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        bool hasChild;
        if (fread(&hasChild, sizeof(bool), 1, file) != 1)
        {
            freeTrie(root);
            return NULL;
        }

        if (hasChild)
        {
            root->children[i] = loadTreeHelper(file);
            if (!root->children[i])
            {
                freeTrie(root);
                return NULL;
            }
        }
    }

    return root;
}

// Load trie from file (binary format)
TrieNode *loadTree(char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
        return NULL;

    TrieNode *root = loadTreeHelper(file);
    fclose(file);
    return root;
}
