#include "statistic.h"
#include <stdio.h>

int main(){
    load_borrow_return_management();
    load_book_management();

    if (!book_management) {
        printf("book_management is NULL!\n");
        return 1;
    }
    if (!borrow_return_management) {
        printf("borrow_return_management is NULL!\n");
        return 1;
    }

    set_up();
    calc_statistic_book(book_management);
    calc_statistic_borrowed_books(borrow_return_management); // Gọi thống kê borrow

    int day, month,year;

    printf("Day: ");
    scanf("%d", &day);
    printf("Month: ");
    scanf("%d", &month);
    printf("Year: ");
    scanf("%d", &year);

    update_date(day, month, year);

    int total_genres = 0;
    for (int num = 0; num < TOTAL_GENRE; num++) {
        if (counter_genre[num] != 0) {
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