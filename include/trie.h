#ifndef TRIE_H
#define TRIE_H

#define ALPHABET_SIZE 27
#define MAX_CHAR_LENGTH 50

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE]; // Child nodes for each alphabet character
    bool isEndOfWord;                         // Marks end of a complete word
    int numIds;                               // Number of IDs associated with this node
    int *ids;                                 // Array of IDs (max MAX_IDS) for this word
} TrieNode;

/// Hàm tiện ích
int charToIndex(char ch);
// void hashWord(mpz_t result, char *word);

TrieNode *searchPrefix(TrieNode *root, char *prefix);
TrieNode *searchWord(TrieNode *root, char *word);
TrieNode *makeTrieNode(void);

/// Một số phương thức quản lí
bool existPrefix(TrieNode *root, char *prefix);
bool existWord(TrieNode *root, char *word);

void insertIntoTrie(TrieNode *root, char *words, int id);
void removeWord(TrieNode *root, char *word);
void recommendPrefix(TrieNode *root, char *prefix, int maxRecommend, char **recommend, int *recommendSize);

void freeTrie(TrieNode *root);
void saveTree(char *filename, TrieNode *root);
TrieNode *loadTree(char *filename);

#endif