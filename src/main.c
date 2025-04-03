#include "gui.h"
#include "bplustree.h"
#include "bookinfor.h"
#include "stdbool.h"

const char *output_file = "hello_world.bin";

int main(int argc, char *argv[])
{
    srand(time(NULL));

    FILE *f = fopen(output_file, "rb");
    // for (int id = 1; id <= 10; id++)
    // {
    //     Book book = generate_book(id);
    //     long offset = ftell(f);
    //     size_t length = fwrite(&book, sizeof(Book), 1, f);

    //     printf("Book ID: %d | Offset: %ld | Length: %zu bytes\n", id, offset, length);
    // }

    Book book;
    fseek(f, 1120, SEEK_SET);
    fread(&book, sizeof(Book), 1, f);

    show_book(book);

    fclose(f);

    return 0;
}
