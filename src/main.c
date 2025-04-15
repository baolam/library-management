#include "gui/gui.h"
#include "books.h"
#include "reader.h"
#include "statistic.h"

int main(int argc, char *argv[])
{
    load_book_management();
    load_reader_management();

    calc_statistic_book(book_management);
    printf("Total books : %d\n", total_books);

    init_gui(argc, argv);
    return 0;
}