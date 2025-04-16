#include "statistic.h"
#include "utils/fake.h"
#include <stdio.h>

int main()
{
    load_reader_management();
    load_book_management();
    load_borrow_return_management();

    if (!book_management)
    {
        printf("book_management is NULL!\n");
        return 1;
    }

    // if (!borrow_return_management)
    // {
    //     printf("borrow_return_management is NULL!\n");
    //     return 1;
    // }

    set_up();
    calc_statistic_book(book_management);

    /// Do chưa có dữ liệu Borrow Return nên sinh giả
    int totalBorrows = 10;
    for (int i = 1; i <= totalBorrows; i++)
    {
        BorrowReturn b = generate_borrow_return(i);
        add_borrow_record(&b);
        show_borow(b);
    }

    save_borrow_return_management();

    auto_update_time();
    calc_statistic_borrowed_books(borrow_return_management); // Gọi thống kê borrow

    int total_genres = 0;
    for (int num = 0; num < TOTAL_GENRE; num++)
    {
        if (counter_genre[num] != 0)
        {
            total_genres++;
        }
    }

    printf("Number of genres: %d\n", total_genres);
    printf("Number of books: %d\n", total_books);
    printf("Number of deleted books: %d\n", deleted_books);
    printf("Number of book in borrow: %d\n", total_borrowed_books);
    printf("Number of books currently borrowed: %d\n", total_borrowed_books);
    printf("Number of overdue borrowings: %d\n", total_late_books);

    return 0;
}