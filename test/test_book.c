#include "utils/fake.h"
#include "books.h"

int main()
{
    // preparate_book();

    // int totals = 250;
    // int id;
    // for (id = 1; id <= totals; id++)
    // {
    //     Book book = generate_book(id);
    //     add_book(&book);
    // }
    // save_book_management();

    load_book_management();

    char prefix[50] = "The";
    int size = 0;
    Book *books = search_book_by_title_direct("The", &size, 15);
    for (int i = 0; i < size; i++)
    {
        show_book(books[i]);
    }
    //search_book_by_title("The", 15);
    return 0;
}