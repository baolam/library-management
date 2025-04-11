#include "bplustreev2.h"

Record *makeRecord(char _from[MAX_FILE_NAME_LENGTH], long offset, long length)
{
    Record *record = (Record *)malloc(sizeof(Record));
    if (record == NULL)
    {
        perror("Record creation.");
        exit(EXIT_FAILURE);
    }
    else
    {
        strncpy(record->_from, _from, MAX_FILE_NAME_LENGTH);
        record->offset = offset;
        record->length = length;
        record->deleted = false;
    }
    return record;
}

Node *makeNode(void)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
    {
        perror("Node creation.");
        exit(EXIT_FAILURE);
    }

    node->keys = malloc((ORDER - 1) * sizeof(int));
    if (node->keys == NULL)
    {
        perror("Node keys allocation.");
        exit(EXIT_FAILURE);
    }

    node->pointers = malloc(ORDER * sizeof(void *));
    if (node->pointers == NULL)
    {
        perror("New node pointers array.");
        exit(EXIT_FAILURE);
    }

    node->is_leaf = false;
    node->num_keys = 0;
    node->parent = NULL;
    node->next = NULL;

    return node;
}

Node *makeLeaf(void)
{
    Node *leaf = makeNode();
    leaf->is_leaf = true;
    return leaf;
}

Node *leftMost(Node *root)
{
    Node *temp = root;
    while (temp != NULL && !temp->is_leaf)
    {
        temp = temp->pointers[0];
    }
    return temp;
}

Node *findLeaf(Node *root, int key)
{
    if (root == NULL)
    {
        return NULL;
    }

    int i = 0;
    Node *c = root;
    while (!c->is_leaf)
    {
        i = 0;
        while (i < c->num_keys)
        {
            if (key >= c->keys[i])
                i++;
            else
                break;
        }
        c = (Node *)c->pointers[i];
    }
    return c;
}

Record *find(Node *root, int key)
{
    if (root == NULL)
        return NULL;

    int i = 0;
    Node *leaf = findLeaf(root, key);
    for (i = 0; i < leaf->num_keys; i++)
    {
        if (leaf->keys[i] == key)
            break;
    }
    if (i == leaf->num_keys)
        return NULL;
    return (Record *)leaf->pointers[i];
}

Node *startNewTree(int key, Record *record)
{
    Node *root = makeLeaf();
    root->keys[0] = key;
    root->pointers[0] = record;
    root->pointers[ORDER - 1] = NULL;
    root->parent = NULL;
    root->num_keys++;
    return root;
}

Node *insertIntoLeaf(Node *leaf, int key, Record *record)
{
    int i, insertion_point;

    insertion_point = 0;
    while (insertion_point < leaf->num_keys && leaf->keys[insertion_point] < key)
    {
        insertion_point++;
    }

    for (i = leaf->num_keys; i > insertion_point; i--)
    {
        leaf->keys[i] = leaf->keys[i - 1];
        leaf->pointers[i] = leaf->pointers[i - 1];
    }

    leaf->keys[insertion_point] = key;
    leaf->pointers[insertion_point] = record;
    leaf->num_keys++;

    return leaf;
}

int cut(int length)
{
    if (length % 2 == 0)
    {
        return length / 2;
    }
    return length / 2 + 1;
}

int getLeftIndex(Node *parent, Node *left)
{
    int left_index = 0;
    while (left_index <= parent->num_keys && parent->pointers[left_index] != left)
        left_index++;
    return left_index;
}

Node *insertIntoNewRoot(Node *left, int key, Node *right)
{
    Node *root = makeNode();
    root->keys[0] = key;
    root->pointers[0] = left;
    root->pointers[1] = right;
    root->num_keys++;
    root->parent = NULL;

    left->parent = root;
    right->parent = root;

    return root;
}

Node *insertIntoNode(Node *root, Node *n, int left_index, int key, Node *right)
{
    int i;

    for (i = root->num_keys; i > left_index; i--)
    {
        n->pointers[i + 1] = n->pointers[i];
        n->keys[i] = n->keys[i - 1];
    }

    n->pointers[left_index + 1] = right;
    n->keys[left_index] = key;
    n->num_keys++;

    return root;
}

Node *insertIntoNodeAfterSplitting(Node *root, Node *old_node, int left_index, int key, Node *right)
{
    int i, j, split, k_prime;
    Node *new_node, *child;
    int *temp_keys;
    Node **temp_pointers;

    temp_pointers = malloc((ORDER + 1) * sizeof(Node *));
    if (temp_pointers == NULL)
    {
        printf("Co null\n");
        perror("Temp_pointers null!");
        exit(EXIT_FAILURE);
    }

    temp_keys = malloc(ORDER * sizeof(int));
    if (temp_keys == NULL)
    {
        printf("Co null\n");
        perror("Temp error!");
        exit(EXIT_FAILURE);
    }

    for (i = 0, j = 0; i < old_node->num_keys + 1; i++, j++)
    {
        if (j == left_index + 1)
            j++;
        temp_pointers[j] = old_node->pointers[i];
    }

    for (i = 0, j = 0; i < old_node->num_keys; i++, j++)
    {
        if (j == left_index)
            j++;
        temp_keys[j] = old_node->keys[i];
    }

    temp_pointers[left_index + 1] = right;
    temp_keys[left_index] = key;

    split = cut(ORDER);
    new_node = makeNode();

    old_node->num_keys = 0;
    for (i = 0; i < split - 1; i++)
    {
        old_node->pointers[i] = temp_pointers[i];
        old_node->keys[i] = temp_keys[i];
        old_node->num_keys++;
    }

    old_node->pointers[i] = temp_pointers[i];
    k_prime = temp_keys[split - 1];

    for (++i, j = 0; i < ORDER; i++, j++)
    {
        new_node->pointers[j] = temp_pointers[i];
        new_node->keys[j] = temp_keys[i];
        new_node->num_keys++;
    }

    new_node->pointers[j] = temp_pointers[i];

    free(temp_pointers);
    free(temp_keys);

    new_node->parent = old_node->parent;
    for (i = 0; i <= new_node->num_keys; i++)
    {
        child = new_node->pointers[i];
        child->parent = new_node;
    }

    return insertIntoParent(root, old_node, k_prime, new_node);
}

Node *insertIntoParent(Node *root, Node *left, int key, Node *right)
{
    int left_index;
    Node *parent;

    parent = left->parent;

    if (parent == NULL)
        return insertIntoNewRoot(left, key, right);

    left_index = getLeftIndex(parent, left);

    if (parent->num_keys < ORDER - 1)
        return insertIntoNode(root, parent, left_index, key, right);

    return insertIntoNodeAfterSplitting(root, parent, left_index, key, right);
}

Node *insertIntoLeafAfterSplitting(Node *root, Node *leaf, int key, Record *record)
{
    Node *new_leaf;
    int *temp_keys;
    void **temp_pointers;
    int insertion_index, split, new_key, i, j;

    new_leaf = makeLeaf();

    temp_keys = malloc(ORDER * sizeof(int));
    if (temp_keys == NULL)
    {
        perror("Tempoarry keys array.");
        exit(EXIT_FAILURE);
    }

    temp_pointers = malloc(ORDER * sizeof(void *));
    if (temp_pointers == NULL)
    {
        perror("Temporary pointers array.");
        exit(EXIT_FAILURE);
    }

    insertion_index = 0;
    while (insertion_index < ORDER - 1 && leaf->keys[insertion_index] < key)
        insertion_index++;

    for (i = 0, j = 0; i < leaf->num_keys; i++, j++)
    {
        if (j == insertion_index)
            j++;
        temp_keys[j] = leaf->keys[i];
        temp_pointers[j] = leaf->pointers[i];
    }

    temp_keys[insertion_index] = key;
    temp_pointers[insertion_index] = record;

    leaf->num_keys = 0;

    split = cut(ORDER - 1);
    for (i = 0; i < split; i++)
    {
        leaf->pointers[i] = temp_pointers[i];
        leaf->keys[i] = temp_keys[i];
        leaf->num_keys++;
    }

    for (i = split, j = 0; i < ORDER; i++, j++)
    {
        new_leaf->pointers[j] = temp_pointers[i];
        new_leaf->keys[j] = temp_keys[i];
        new_leaf->num_keys++;
    }

    free(temp_pointers);
    free(temp_keys);

    new_leaf->pointers[ORDER - 1] = leaf->pointers[ORDER - 1];
    leaf->pointers[ORDER - 1] = new_leaf;

    for (i = leaf->num_keys; i < ORDER - 1; i++)
        leaf->pointers[i] = NULL;
    for (i = new_leaf->num_keys; i < ORDER - 1; i++)
        new_leaf->pointers[i] = NULL;

    new_leaf->parent = leaf->parent;
    new_key = new_leaf->keys[0];

    return insertIntoParent(root, leaf, new_key, new_leaf);
}

Node *insert(Node *root, int key, char _from[MAX_FILE_NAME_LENGTH], long offset, long length)
{
    Record *record_pointer = NULL;
    Node *leaf = NULL;

    record_pointer = find(root, key);

    if (record_pointer != NULL)
    {
        if (record_pointer->deleted)
            return root;
        strncpy(record_pointer->_from, _from, MAX_FILE_NAME_LENGTH);
        record_pointer->offset = offset;
        record_pointer->length = length;
        return root;
    }

    record_pointer = makeRecord(_from, offset, length);

    if (root == NULL)
    {
        // printf("Loi day a\n");
        return startNewTree(key, record_pointer);
    }

    leaf = findLeaf(root, key);
    if (leaf == NULL)
    {
        perror("NULL\n");
        exit(EXIT_FAILURE);
    }

    if (leaf->num_keys < ORDER - 1)
    {
        leaf = insertIntoLeaf(leaf, key, record_pointer);
        return root;
    }

    return insertIntoLeafAfterSplitting(root, leaf, key, record_pointer);
}

#ifdef PRINT_TREE

Node *queue = NULL;

int pathToLeaves(Node *const root, Node *child)
{
    int length = 0;
    Node *c = child;
    while (c != root)
    {
        c = c->parent;
        length++;
    }
    return length;
}

void enqueue(Node *new_node)
{
    Node *c;
    if (queue == NULL)
    {
        queue = new_node;
        queue->next = NULL;
    }
    else
    {
        c = queue;
        while (c->next != NULL)
        {
            c = c->next;
        }
        c->next = new_node;
        new_node->next = NULL;
    }
}

Node *dequeue()
{
    Node *n = queue;
    queue = queue->next;
    n->next = NULL;
    return n;
}

void printTree(Node *const root)
{
    Node *n = NULL;
    int i = 0;
    int rank = 0;
    int new_rank = 0;

    if (root == NULL)
    {
        printf("Empty tree \n");
        return;
    }

    queue = NULL;
    enqueue(root);
    while (queue != NULL)
    {
        n = dequeue();
        if (n->parent != NULL && n == n->parent->pointers[0])
        {
            new_rank = pathToLeaves(root, n);
            if (new_rank != rank)
            {
                rank = new_rank;
                printf("\n");
            }
        }
        for (i = 0; i < n->num_keys; i++)
        {
            printf("%d ", n->keys[i]);
        }
        if (!n->is_leaf)
        {
            for (int i = 0; i <= n->num_keys; i++)
                enqueue(n->pointers[i]);
        }
        printf("| ");
    }
    printf("\n");
}

#endif

// Helper function to write a node to file
static void writeNode(FILE *fp, Node *node)
{
    if (!node)
        return;

    // Write node metadata
    fwrite(&node->is_leaf, sizeof(bool), 1, fp);
    fwrite(&node->num_keys, sizeof(int), 1, fp);

    // Write keys
    fwrite(node->keys, sizeof(int), node->num_keys, fp);

    if (node->is_leaf)
    {
        // Write records for leaf nodes
        for (int i = 0; i < node->num_keys; i++)
        {
            Record *record = (Record *)node->pointers[i];
            fwrite(record, sizeof(Record), 1, fp);
        }
        // Write next leaf pointer
        bool has_next = (node->pointers[ORDER - 1] != NULL);
        fwrite(&has_next, sizeof(bool), 1, fp);
        if (has_next)
        {
            writeNode(fp, (Node *)node->pointers[ORDER - 1]);
        }
    }
    else
    {
        // Write child pointers for internal nodes
        for (int i = 0; i <= node->num_keys; i++)
        {
            Node *child = (Node *)node->pointers[i];
            writeNode(fp, child);
        }
    }
}

void saveTree(Node *root, char *filename)
{
    if (root == NULL || !filename)
    {
        perror("No root or no filename !");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }

    // Write tree structure
    writeNode(fp, root);
    fclose(fp);
}

// Helper function to read a node from file
static Node *readNode(FILE *fp)
{
    Node *node = makeNode();
    if (!node)
        return NULL;

    // Read node metadata
    fread(&node->is_leaf, sizeof(bool), 1, fp);
    fread(&node->num_keys, sizeof(int), 1, fp);

    // Allocate memory for keys
    node->keys = (int *)malloc(ORDER * sizeof(int));
    if (!node->keys)
    {
        free(node);
        return NULL;
    }

    // Read keys
    fread(node->keys, sizeof(int), node->num_keys, fp);

    if (node->is_leaf)
    {
        // Read records for leaf nodes
        node->pointers = (void **)malloc(ORDER * sizeof(Record *));
        if (!node->pointers)
        {
            free(node->keys);
            free(node);
            return NULL;
        }

        for (int i = 0; i < node->num_keys; i++)
        {
            Record *record = (Record *)malloc(sizeof(Record));
            if (!record)
            {
                // Clean up allocated memory
                for (int j = 0; j < i; j++)
                {
                    free((Record *)node->pointers[j]);
                }
                free(node->keys);
                free(node->pointers);
                free(node);
                return NULL;
            }
            fread(record, sizeof(Record), 1, fp);
            node->pointers[i] = record;
        }

        // Read next leaf pointer if exists
        bool has_next = false;
        fread(&has_next, sizeof(bool), 1, fp);
        if (has_next)
        {
            Node *next_leaf = readNode(fp);
            if (!next_leaf)
            {
                // Clean up allocated memory
                for (int j = 0; j < node->num_keys; j++)
                {
                    free((Record *)node->pointers[j]);
                }
                free(node->keys);
                free(node->pointers);
                free(node);
                return NULL;
            }
            node->pointers[ORDER - 1] = next_leaf;
        }
        else
        {
            node->pointers[ORDER - 1] = NULL;
        }
    }
    else
    {
        // Read child pointers for internal nodes
        node->pointers = (void **)malloc((ORDER + 1) * sizeof(Node *));
        if (!node->pointers)
        {
            free(node->keys);
            free(node);
            return NULL;
        }

        for (int i = 0; i <= node->num_keys; i++)
        {
            Node *child = readNode(fp);
            if (!child)
            {
                // Clean up allocated memory
                for (int j = 0; j < i; j++)
                {
                    free((Node *)node->pointers[j]);
                }
                free(node->keys);
                free(node->pointers);
                free(node);
                return NULL;
            }
            node->pointers[i] = child;
            child->parent = node;
        }
    }

    return node;
}

Node *loadTree(char *filename)
{
    if (!filename)
        return NULL;

    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        perror("Failed to open file for reading");
        return NULL;
    }

    Node *root = readNode(fp);
    fclose(fp);
    return root;
}
