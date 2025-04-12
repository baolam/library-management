#include "statistic.h"

short counter_genre[TOTAL_GENRE];
int total_books = 0;
int deleted_books = 0;

int find_position(char genre[MAX_GENRE_NO])
{
    int num;
    int check;

    for (num = 0; num < TOTAL_GENRE; num++)
    {
        check = strcmp(genre, genres[num]);

        if (check == 0)
        {
            return num;
        }
    }

    return -1;
}

void set_up()
{
    int num;

    for (num = 0; num < TOTAL_GENRE; num++)
    {
        counter_genre[num] = 0;
    }
}

void calc_statistic_book(Node *book_management)
{
    Node *book_genre = leftMost(book_management);
    Record *infor;
    Book *book;

    int num;
    int position;

    while (book_genre != NULL)
    {
        for (num = 0; num < book_genre->num_keys; num++)
        {
            infor = (Record *)book_genre->pointers[num];

            if (infor->deleted)
            {
                deleted_books++;
                continue;
            }

            book = (Book *)read_content_from_record_return(infor);
            position = find_position(book->genre);

            total_books++;
            counter_genre[position]++;
        }

        book_genre = book_genre->pointers[ORDER - 1];
    }
}