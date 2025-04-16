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

void show_oneborrowbook_to_layout(GtkListStore *store, Book book, int status, int totals)
{
    gchar *bookId = g_strdup_printf("%d", book.bookId);
    gchar *totalBooks = g_strdup_printf("%d", totals);

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, bookId,
                       1, book.title,
                       2, book.author,
                       3, totalBooks,
                       4, get_status_book(status),
                       -1);

    g_free(bookId);
    g_free(totalBooks);
}

void show_borrowbook_to_layout(BorrowReturn *borrow, int size)
{
    GtkListStore *store = reference_borrow_book_return_store();

    int i;
    for (i = current_book_borrow_page - 1; i < borrow->totalBooks && size > 0; i++)
    {
        Book *book = search_book(borrow->bookIds[i]);
        show_oneborrowbook_to_layout(store, *book, borrow->status[i], borrow->quantities[i]);
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

    if (temp_borrow == NULL)
    {
        temp_borrow = search_borrow_by_reader(borrow_chosen_id);
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