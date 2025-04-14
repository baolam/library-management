#include "statistic.h"

short counter_genre[TOTAL_GENRE];
int total_books = 0;
int deleted_books = 0;
int total_borrowed_books = 0;
int total_late_books = 0;
OverdueBorrower overdue_list[MAX_OVERDUE];
int overdue_count = 0;

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

void calc_statistic_borrowed_books(Node *borrow_return_management)
{
    Node *borrow_node = leftMost(borrow_return_management);
    Record *infor;
    BorrowReturn *borrow;

    while (borrow_node != NULL)
    {
        int num = 0;
        for (; num < borrow_node->num_keys; num++)
        {
            infor = (Record *)borrow_node->pointers[num];

            if (infor->deleted)
            {
                continue;
            }

            borrow = (BorrowReturn *)read_content_from_record_return(infor);
            
            if (borrow->status == ON_BORROWING) // đang mượn
            {
                for (int i = 0; i < borrow->totalBooks; i++)
                {
                    total_borrowed_books += borrow->quantities[i];
                }

                int diff = calculate_day_difference(borrow->date, borrow->current_year);
                if (diff > 14)
                {
                    total_late_books++;
                }
            }
        }

        borrow_node = borrow_node->pointers[ORDER - 1];
    }
}

void collect_late_borrowers(Node *borrow_return_management)
{
    overdue_count = 0;
    Node *book_node = leftMost(borrow_return_management);
    Record *infor;
    BorrowReturn *borrow;

    while (book_node != NULL)
    {
        for (int i = 0; i < book_node->num_keys; i++)
        {
            infor = (Record *)book_node->pointers[i];
            if (infor->deleted)
                continue;

            borrow = (BorrowReturn *)read_content_from_record_return(infor);
            if (borrow->status == 0)
            {
                int diff = calculate_day_difference(borrow->date, borrow->current_year);
                if (diff > 14 && overdue_count < MAX_OVERDUE)
                {
                    overdue_list[overdue_count].readerId = borrow->readerId;
                    overdue_list[overdue_count].days_borrowed = diff;
                    overdue_count++;
                }
            }
        }
        book_node = book_node->pointers[ORDER - 1];
    }
}

void list_late_borrowers()
{
    printf("List of overdue borrowers:\n");
    for (int i = 0; i < overdue_count; i++)
    {
        printf("- Reader ID: %d | Days borrowed: %d\n",
               overdue_list[i].readerId,
               overdue_list[i].days_borrowed);
    }
}