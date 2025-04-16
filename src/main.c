#include "gui/gui.h"
#include "books.h"
#include "reader.h"
#include "statistic.h"

int main(int argc, char *argv[])
{
    auto_update_time();
    printf("Nam hien tai: %d\n", system_current_year);
    printf("Ngay hien tai: %d\n", system_date);

    load_book_management();
    load_reader_management();
    load_borrow_return_management();

    calc_statistic_book(book_management);
    calc_statistic_reader(reader_management);
    calc_statistic_borrowed_books(borrow_return_management);
    collect_late_borrowers(borrow_return_management);

    printf("Total books : %d\n", total_books);
    printf("Total readers : %d\n", total_readers);

    init_gui(argc, argv);
    return 0;
}