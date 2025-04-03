#include "gui.h"
#include "bplustree.h"
#include "stdbool.h"

const char *filepath = "E:/library-management/fake_information/books.txt";

struct
{
    char bookId[20];
    char title[100];
    char author[100];
    char genre[50];
    int publicationYear;
    int stock;
} Book;

int main(int argc, char *argv[])
{
    FILE *f = fopen(filepath, "r");
    if (f == NULL)
    {
        printf("Error reading file!");
        return 0;
    }

    return 0;
}
