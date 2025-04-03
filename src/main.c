#include "gui.h"
#include "bplustree.h"
#include "bookinfor.h"
#include "management.h"
#include "stdbool.h"

const char *output_file = "hello_world.bin";

void handleBook(FILE *f, long length)
{
    Book book;
    fread(&book, sizeof(Book), 1, f);
    show_book(book);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    FileInfor file_infor;
    file_infor.offset = 1120;
    file_infor.length = 1;

    readContent(output_file, file_infor, handleBook);

    return 0;
}
