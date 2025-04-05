#include "gui.h"
#include "bplustree.h"
#include "bookinfor.h"
#include "management.h"
#include "stdbool.h"

const char *output_file = "hello_world.bin";

void handleBook(FILE *f, long length)
{
    Book book;
    fread(&book, length, 1, f);
    show_book(book);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    BPlusTree *tree = buildTree(4);
    FILE *f = fopen(output_file, "wb");

    for (int id = 1; id <= 10; id++)
    {
        Book book = generate_book(id);
        long offset = ftell(f);
        size_t length = sizeof(Book);

        fwrite(&book, length, 1, f);

        printf("Book ID: %d | Offset: %ld | Length: %zu bytes\n", id, offset, length);
        FileInfor fileInfor = buildFileInfor((char *)output_file, offset, length);
        StorageInfor storageInfor = buildStorageInfor(id, fileInfor);
        insertInfor(tree, id, storageInfor);
    }

    printTree(tree);

    // Ensure file operations succeeded
    if (f == NULL)
    {
        printf("Error opening file %s\n", output_file);
        return 1;
    }

    // Cleanup resources
    fclose(f);

    // Verify tree operations
    StorageInfor *infor = retrieveKey(tree, 5, 5);
    if (infor != NULL)
    {
        printf("Retrieved key 5 successfully\n");
    }
    else
    {
        printf("Failed to retrieve key 5\n");
    }

    // Free tree memory
    // freeBPlusTree(tree);

    return 0;
}
