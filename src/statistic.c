#include "statistic.h"

short counter_genre[TOTAL_GENRE];

int total_books = 0;
int deleted_books = 0;
int total_readers = 0;
int deleted_readers = 0;
int total_borrowed_books = 0;
int total_late_books = 0;

OverdueBorrower overdue_list[MAX_OVERDUE];
int overdue_count = 0;

int calculate_day_difference(int borrow_date, int borrow_year)
{
    int year_diff = current_year - borrow_year;
    return year_diff * 365 + (date - borrow_date);
}

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
            infor = (Record *)find(book_management, book_genre->keys[num]);
            total_books++;

            if (infor->deleted)
            {
                deleted_books++;
                continue;
            }

            book = (Book *)read_content_from_record_return(infor);
            position = find_position(book->genre);

            counter_genre[position]++;
        }

        book_genre = book_genre->pointers[ORDER - 1];
    }
}

void calc_statistic_reader(Node *reader_management)
{
    Node *reader = leftMost(book_management);
    Record *infor;

    int num;

    while (reader != NULL)
    {
        for (num = 0; num < reader->num_keys; num++)
        {
            infor = (Record *)find(book_management, reader->keys[num]);
            total_readers++;

            if (infor->deleted)
            {
                deleted_readers++;
                continue;
            }
        }

        reader = reader->pointers[ORDER - 1];
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
            infor = (Record *)find(borrow_return_management, borrow_node->keys[num]);

            if (infor->deleted)
                continue;

            borrow = (BorrowReturn *)read_content_from_record_return(infor);
            for (int i = 0; i < borrow->totalBooks; i++)
            {
                total_borrowed_books += borrow->infors[i].quantity;
                int day = calculate_day_difference(borrow->infors[i].date, borrow->infors[i].current_year);
                borrow->infors[i].onTime = day <= OVER_DATE ? true : false;
                if (!borrow->infors[i].onTime)
                    total_late_books += borrow->infors[i].quantity;
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
            infor = (Record *)find(borrow_return_management, book_node->keys[i]);
            if (infor->deleted)
                continue;

            borrow = (BorrowReturn *)read_content_from_record_return(infor);
            for (int i = 0; i < borrow->totalBooks; i++)
            {
                if (borrow->infors[i].status == ON_BORROWING)
                {
                    int diff = calculate_day_difference(borrow->infors[i].date, borrow->infors[i].current_year);
                    if (diff > OVER_DATE && overdue_count < MAX_OVERDUE)
                    {
                        overdue_list[overdue_count].readerId = borrow->readerId;
                        overdue_list[overdue_count].days_borrowed = diff;
                        overdue_count++;
                        break;
                    }
                }
            }
        }

        book_node = book_node->pointers[ORDER - 1];
    }
}

int stat_total_books_from_object(BorrowReturn *borrow_return)
{
    int totals = 0;
    int i;
    for (i = 0; i < borrow_return->totalBooks; i++)
    {
        totals += borrow_return->infors[i].quantity;
    }
    return totals;
}

int stat_total_books_by_reader(int readerId)
{
    BorrowReturn *borrow_return = search_borrow_by_reader(readerId);
    if (borrow_return == NULL)
        return 0;
    return stat_total_books_from_object(borrow_return);
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