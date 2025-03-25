#include "gui.h"
#include "bplustree.h"

StorageInfor getInfor(int key)
{
    FileInfor _file = buildFileInfor("data.txt", 0, 0);
    StorageInfor infor = buildStorageInfor(key, _file);
    return infor;
}

int main(int argc, char *argv[])
{
    BPlusTree *tree = buildTree(5);

    insertInfor(tree, 10, getInfor(10));
    insertInfor(tree, 20, getInfor(20));
    insertInfor(tree, 5, getInfor(5));
    insertInfor(tree, 15, getInfor(15));
    insertInfor(tree, 25, getInfor(25));

    printTree(tree);

    bool found = existKey(tree, 10, 10);
    printf("Status: %d", found);

    deleteInfor(tree, 10, 10);

    found = existKey(tree, 10, 10);
    printf("Status: %d", found);

    return 0;
}
