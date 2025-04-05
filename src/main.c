#include "gui.h"
#include "bplustreev2.h"

int main()
{
    Node *root = NULL;

    int id;
    char filename[MAX_FILE_NAME_LENGTH] = "haha.txt";
    for (id = 1; id <= 100; id++)
    {
        // printf("OKOK\n");
        Record *record = makeRecord(filename, 0, 1);
        root = insert(root, id, record);
    }

    // printTree(root);

    Record *findRecord = find(root, 5);
    if (findRecord != NULL)
    {
        printf("Existed!\n");
        printf("Information _from %s, offset %ld, length %ld\n", findRecord->_from, findRecord->offset, findRecord->length);
    }

    return 0;
}