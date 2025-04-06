#include "gui.h"
#include "bplustreev2.h"
#include "bookinfor.h"
#include "management.h"

void readBook(FILE *f, long package_size)
{
    if (f == NULL || package_size == 0)
    {
        printf("Failed to read data!\n");
        return;
    }

    Book book1;
    fread(&book1, package_size, 1, f);

    printf("Book infor:\n");
    printf("Id : %s\n", book1.bookId);
    printf("Title : %s\n", book1.title);
    printf("Author : %s\n", book1.author);
    printf("Genre : %s\n", book1.genre);
    printf("Publication year : %d\n", book1.publicationYear);
    printf("Stock : %d\n", book1.stock);
    printf("\n");
}

int main()
{
    // Node *root = NULL;
    // char filename[MAX_FILE_NAME_LENGTH] = "data.bin";
    // int id = 1;

    // FILE *f = fopen(filename, "wb");

    // for (id = 1; id <= 100; id++)
    // {
    //     Book book = generate_book(id);
    //     long offset = ftell(f);
    //     fwrite(&book, sizeof(Book), 1, f);
    //     printf("Id : %d, Offset : %ld, Length: %lld\n", id, offset, sizeof(Book));
    //     Record *record = makeRecord(filename, offset, sizeof(Book));
    //     root = insert(root, id, record);
    // }

    // fclose(f);
    // printTree(root);

    // printf("Saving\n");
    // saveTree(root, "dsa.bin");
    // printf("Saved succesfully!\n");

    Node *root = loadTree("dsa.bin");
    // printTree(root);

    read_bucket_content(root, 5, 20, readBook);
    // printf("Exist record: %d\n", exist_record(root, 5));

    return 0;
}