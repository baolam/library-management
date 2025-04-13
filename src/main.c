#include "gui/gui.h"
#include "books.h"
#include "reader.h"

int main(int argc, char *argv[])
{
    load_book_management();
    load_reader_management();
    init_gui(argc, argv);
    return 0;
}