#include "statistic.h"

int main(){
    load_book_management();
    set_up();
    calc_statistic_book(book_management);

    int total_genres = 0;

    for(int num = 0; num < TOTAL_GENRE; num++){
        if(counter_genre[num] != 0){
            total_genres++;
        }
    }

    printf("Number of genres: %d\n", total_genres);
    printf("Number of books: %d\n", total_books);
    printf("Number of deleted books: %d\n", deleted_books);
}