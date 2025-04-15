#include "gui/events/borrow_return_treeview.h"

GtkListStore *reference_borrow_return_store()
{
    GtkListStore *store = GTK_LIST_STORE(gtk_builder_get_object(builder, "borrow_data"));
    gtk_list_store_clear(store);
    return store;
}

void show_oneborrow_to_layout(GtkListStore *store, BorrowReturn borrow)
{
    gchar *readerId = g_strdup_printf("%d", borrow.readerId);
    gchar *date = g_strdup_printf("%d", borrow.date);
    gchar *totalBooks = g_strdup_printf("%d", borrow.totalBooks);
    gchar *quanities = g_strdup_printf("%d", stat_total_books_from_object(&borrow));

    Readers *reader = search_reader(borrow.readerId);
    if (reader == NULL)
        return;

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, readerId,
                       1, reader->fullName,
                       2, totalBooks,
                       3, quanities,
                       4, "...",
                       5, date,
                       -1);

    g_free(readerId);
    g_free(date);
    g_free(totalBooks);
}

void show_borrow_to_layout(BorrowReturn *borrow, int size)
{
    GtkListStore *store = reference_borrow_return_store();

    int i;
    for (i = 0; i < size; i++)
    {
        show_oneborrow_to_layout(store, borrow[i]);
    }
}

void load_borrow_to_layout(int beginingKey)
{
    int size = 0;
    BorrowReturn *borrows = retrieve_bucket_borrows(current_borrow_page, MAX_ROW_ONEPAGE, &size);
    show_borrow_to_layout(borrows, size);
}

void on_previous_borrow_page_clicked(GtkWidget *widget, gpointer user_data)
{
    if (current_borrow_page > MAX_ROW_ONEPAGE)
    {
        current_borrow_page -= MAX_ROW_ONEPAGE;
        load_borrow_to_layout(current_borrow_page);
    }
}

void on_next_borrow_page_clicked(GtkWidget *widget, gpointer user_data)
{
    current_borrow_page += MAX_ROW_ONEPAGE;
    load_borrow_to_layout(current_borrow_page);
}