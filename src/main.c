#include "gui.h"
#include "bplustree.h"

int main(int argc, char *argv[])
{
    // init_gui(argc, argv);
    BTree *root = create_tree(MAX_ORDER);
    if (!root)
    {
        printf("Failed to create B+ Tree");
        return 0;
    }

    for (int key = 1; key <= 9; key++)
    {
        StorageData data = create_storage_data(key, "data.txt", 100, 100);
        insertToTree(root, data);
    }

    printf("Insert data to tree successfully!\n");

    // print_tree(root);

    for (int key = 1; key <= 20; key++)
    {
        StorageData *data = search_storage_data(root, key);
        printf("Key %d\n", key);
        if (data)
        {
            printf("Found data.\n");
            printf("Filename %s\n", data->position.filename);
            printf("Offset %ld, Length %d\n", data->position.offset, data->position.length);
        }
        else
        {
            printf("Not found!\n");
        }
        printf("\n");
    }

    return 0;
}
