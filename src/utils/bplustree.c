#include "bplustree.h"

BPlusTreeNode *create_node(bool is_leaf)
{
    BPlusTreeNode *node = (BPlusTreeNode *)malloc(sizeof(BPlusTreeNode));
    node->is_leaf = is_leaf;
    node->num_keys = 0;
    for (int i = 0; i < ORDER; i++)
    {
        node->children[i] = -1;
    }
    return node;
}

BPlusTree *create_tree(const char *folder, const char *filename)
{
    BPlusTree *tree = (BPlusTree *)malloc(sizeof(BPlusTree));
    tree->root = create_node(true);
    strcpy(tree->folder, folder);
    strcpy(*(tree->filename), filename);
    return tree;
}

void save_node(FILE *fp, BPlusTreeNode *node, long position)
{
    fseek(fp, position, SEEK_SET);
    fwrite(node, sizeof(BPlusTreeNode), 1, fp);
}

void save_tree(BPlusTree *tree)
{
    const char *filepath = get_file_path(tree->folder, *(tree->filename));
    FILE *fp = fopen(filepath, "wb");
    save_node(fp, tree->root, 0);
    fclose(fp);
}

void load_node(FILE *fp, BPlusTreeNode *node, long position)
{
    fseek(fp, position, SEEK_SET);
    fread(node, sizeof(BPlusTreeNode), 1, fp);
}

BPlusTree *load_tree(const char *folder, const char *filename)
{
    const char *filepath = get_file_path(folder, filename);

    FILE *fp = fopen(filepath, "rb");

    BPlusTree *tree = create_tree(folder, filename);
    load_node(fp, tree->root, 0);

    fclose(fp);
    return tree;
}
