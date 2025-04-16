#include "gui/constant.h"

GtkListStore *reference_borrow_book_return_store()
{
    GtkListStore *store = GTK_LIST_STORE(gtk_builder_get_object(builder, "book_borrow_data"));
    gtk_list_store_clear(store);
    return store;
}

gchar *get_status_book(int status)
{
    switch (status)
    {
    case ON_BORROWING:
        return "Đang mượn sách";
    case BORROWED:
        return "Đã trả sách";
    }
    return "Chưa mượn";
}

gchar *get_overdate(bool onTime)
{
    if (onTime)
    {
        return "Không";
    }
    return "Có";
}

gchar *borrow_at(int date, int current_year)
{
    int month, day;
    get_borrow_time(date, current_year, &day, &month);
    return g_strdup_printf("%02d/%02d/%04d", day, month, current_year);
}

void show_oneborrowbook_to_layout(GtkListStore *store, Book book, BorrowReturn borrow_return)
{
    int i = getPosition(borrow_return.infors, borrow_return.totalBooks, book.bookId);

    gchar *bookId = g_strdup_printf("%d", book.bookId);
    gchar *totalBorrows = g_strdup_printf("%d", borrow_return.infors[i].quantity);

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, bookId,
                       1, book.title,
                       2, totalBorrows,
                       3, get_status_book(borrow_return.infors[i].status),
                       4, get_overdate(calculate_day_difference(borrow_return.infors[i].date, borrow_return.infors[i].current_year) <= OVER_DATE),
                       5, borrow_at(borrow_return.infors[i].date, borrow_return.infors[i].current_year),
                       -1);

    g_free(bookId);
    g_free(totalBorrows);
}

void show_borrowbook_to_layout(BorrowReturn *borrow, int size)
{
    GtkListStore *store = reference_borrow_book_return_store();

    int i;
    for (i = current_book_borrow_page - 1; i < borrow->totalBooks && size > 0; i++)
    {
        Book *book = search_book(borrow->infors[i].bookId);
        show_oneborrowbook_to_layout(store, *book, *borrow);
        size--;
    }
}

void load_borrowbook_to_layout()
{
    if (borrow_chosen_id == -1)
    {
        printf(">>> Chua co reader Id \n");
        return;
    }

    temp_borrow = search_borrow_by_reader(borrow_chosen_id);
    if (temp_borrow == NULL)
    {
        printf(">>> Loi temp_borrow \n");
        return;
    }

    show_borrowbook_to_layout(temp_borrow, MAX_ROW_ONEPAGE);
}

void on_book_borrow_previous_clicked(GtkWidget *widget, gpointer user_data)
{
    if (current_book_borrow_page > MAX_ROW_ONEPAGE)
    {
        current_book_borrow_page -= MAX_ROW_ONEPAGE;
        load_borrowbook_to_layout();
    }
}

void on_book_borrow_next_clicked(GtkWidget *widget, gpointer user_data)
{
    current_book_borrow_page += MAX_ROW_ONEPAGE;
    load_borrowbook_to_layout();
}