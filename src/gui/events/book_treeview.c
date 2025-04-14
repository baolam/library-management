#include "gui/events/book_treeview.h"
#include <stdio.h>
#include <string.h>

int current_book_page = 1;

void show_onebook_to_layout(GtkListStore *store, Book book)
{
    gchar *bookId = g_strdup_printf("%d", book.bookId);
    gchar *year = g_strdup_printf("%d", book.publicationYear);
    gchar *stock = g_strdup_printf("%d", book.stock);

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, bookId,
                       1, book.title,
                       2, book.author,
                       3, book.genre,
                       4, year,
                       5, stock,
                       -1);

    g_free(bookId);
    g_free(year);
    g_free(stock);
}

GtkListStore *reference_store()
{
    GtkListStore *store = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
    gtk_list_store_clear(store);
    return store;
}

void show_book_to_layout(Book *book, int size)
{
    GtkListStore *store = reference_store();

    int i;
    for (i = 0; i < size; i++)
    {
        show_onebook_to_layout(store, book[i]);
    }
}

void load_book_to_layout(int beginingKey)
{
    int size = 0;
    Book *book = retrieve_bucket_books(beginingKey, MAX_ROW_ONEPAGE, &size);
    show_book_to_layout(book, size);
}

void on_previous_page1_clicked(GtkWidget *widget, gpointer user_data)
{
    if (current_book_page > MAX_ROW_ONEPAGE)
    {
        current_book_page -= MAX_ROW_ONEPAGE;
        load_book_to_layout(current_book_page);
    }
}

void on_next_page1_clicked(GtkWidget *widget, gpointer user_data)
{
    current_book_page += MAX_ROW_ONEPAGE;
    load_book_to_layout(current_book_page);
}