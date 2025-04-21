#include "utils/trie.h"

// Convert character to index (a-z and ')
int charToIndex(char ch)
{
    if (ch >= 'a' && ch <= 'z')
        return ch - 'a';
    else if (ch >= '0' && ch <= '9')
        return 26 + (ch - '0');
    else if (ch == ' ')
        return 36;
    return -1;
}

void toLowerCase(char *word)
{
    int i;
    for (i = 0; i < strlen(word); i++)
        word[i] = tolower(word[i]);
}

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
        node->numIds = 0;
        node->ids = NULL;
    }
    return node;
}

// Search for a prefix in the trie
TrieNode *searchPrefix(TrieNode *root, char *prefix)
{
    if (!root || !prefix)
    {
        return NULL;
    }

    char lower[MAX_CHAR_LENGTH];
    strcpy(lower, prefix);
    toLowerCase(lower);

    TrieNode *current = root;

    for (int i = 0; lower[i] != '\0'; i++)
    {
        int index = charToIndex(lower[i]);
        if (!current || !current->children[index])
        {
            return NULL;
        }
        current = current->children[index];
    }

    return current;
}

TrieNode *searchWord(TrieNode *root, char *word)
{
    TrieNode *node = searchPrefix(root, word);
    if (node == NULL)
        return NULL;
    if (!node->isEndOfWord)
        return NULL;
    return node;
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
        if (index == -1) continue;
        if (!current->children[index])
        {
            current->children[index] = makeTrieNode();
        }
        current = current->children[index];
    }

    current->isEndOfWord = true;

    // Check for duplicate ID
    for (int i = 0; i < current->numIds; i++)
    {
        if (current->ids[i] == id)
            return;
    }

    // Add new ID with dynamic array resizing
    current->numIds++;
    current->ids = realloc(current->ids, current->numIds * sizeof(int));
    if (!current->ids)
    {
        current->numIds = 0; // Reset on allocation failure
        return;
    }
    current->ids[current->numIds - 1] = id;
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

int getIndexOfKey(TrieNode *target, int id)
{
    int found = -1;
    int i;
    for (i = 0; i < target->numIds; i++)
    {
        if (target->ids[i] == id)
        {
            found = i;
            break;
        }
    }
    return found;
}

void removeIdFromWord(TrieNode *root, char *word, int id)
{
    if (!root)
        return;

    TrieNode *target = searchWord(root, word);
    if (target == NULL)
        return;

    int position = getIndexOfKey(target, id);
    if (position == -1)
        return;

    if (target->numIds == 1)
    {
        removeWord(root, word);
        return;
    }

    // printf("Dong 208. trie.c, chua viet xong ham xoa nha ::)");

    int *temp = malloc((target->numIds - 1) * sizeof(int));
    int i, j;
    for (i = 0, j = 0; i < target->numIds; i++, j++)
    {
        if (j == position)
            j++;
        temp[i] = target->ids[j];
    }

    for (i = 0; i < target->numIds - 1; i++)
    {
        target->ids[i] = temp[i];
    }
    target->numIds--;

    free(temp);
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
            if (i < 26)
                buffer[depth] = 'a' + i;
            else
                buffer[depth] = '0' + (i - 26);
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

    // Free IDs array if it exists
    if (root->ids)
    {
        free(root->ids);
    }

    // Free current node
    free(root);
}

// Save trie to file (binary format)
void saveTreeHelper(FILE *file, TrieNode *root)
{
    if (!root || !file)
        return;

    // Write node data
    if (fwrite(&root->isEndOfWord, sizeof(bool), 1, file) != 1 ||
        fwrite(&root->numIds, sizeof(int), 1, file) != 1)
    {
        return;
    }

    if (root->numIds > 0 &&
        fwrite(root->ids, sizeof(int), root->numIds, file) != root->numIds)
    {
        return;
    }

    // Write children markers and recurse
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        bool hasChild = (root->children[i] != NULL);
        if (fwrite(&hasChild, sizeof(bool), 1, file) != 1)
        {
            return;
        }

        if (hasChild)
        {
            saveTreeHelper(file, root->children[i]);
            if (ferror(file))
            {
                return;
            }
        }
    }
}

void saveTrieTree(char *filename, TrieNode *root)
{
    if (!root)
        return;

    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        return;
    }

    saveTreeHelper(file, root);

    if (ferror(file))
    {
        if (file)
            fclose(file);
        return;
    }

    fclose(file);
}

// Helper function for loading trie
static TrieNode *loadTreeHelper(FILE *file)
{
    if (!file)
    {
        return NULL;
    }

    TrieNode *root = makeTrieNode();
    if (!root)
    {
        return NULL;
    }

    // Read node data
    if (fread(&root->isEndOfWord, sizeof(bool), 1, file) != 1 ||
        fread(&root->numIds, sizeof(int), 1, file) != 1)
    {
        freeTrie(root);
        return NULL;
    }

    root->ids = NULL;
    if (root->numIds > 0)
    {
        root->ids = malloc(root->numIds * sizeof(int));
        if (!root->ids || fread(root->ids, sizeof(int), root->numIds, file) != root->numIds)
        {
            if (root->ids)
                free(root->ids);
            freeTrie(root);
            return NULL;
        }
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
TrieNode *loadTrieTree(char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        return NULL;
    }

    TrieNode *root = loadTreeHelper(file);

    if (ferror(file))
    {
        if (root)
            freeTrie(root);
        root = NULL;
    }

    fclose(file);
    return root;
}
