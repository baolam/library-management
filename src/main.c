#include "gui.h"
#include "bplustree.h"
#include "bookinfor.h"
#include "stdbool.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    Book book1 = generate_book(1);
    show_book(book1);

    return 0;
}
