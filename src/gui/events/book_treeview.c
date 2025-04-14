#include "gui/events/book_treeview.h"
#include <stdio.h>
#include <string.h>


int currnent_page = 1;

void load_book_to_layout(GtkBuilder *builder, int beginingKey)
{
    GtkListStore *store = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
    gtk_list_store_clear(store);

    int size = 0;
    Book *book = retrieve_bucket_books(beginingKey, MAX_ROW_ONEPAGE, &size);

    int i;
    for (i = 0; i < size; i++)
    {
        gchar *bookId = g_strdup_printf("%d", book[i].bookId);
        gchar *year = g_strdup_printf("%d", book[i].publicationYear);
        gchar *stock = g_strdup_printf("%d", book[i].stock);

        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, bookId,
                           1, book[i].title,
                           2, book[i].author,
                           3, book[i].genre,
                           4, year,
                           5, stock,
                           -1);

        g_free(bookId);
        g_free(year);
        g_free(stock);
                                          
    }
}

void on_previous_page1_clicked(GtkWidget *widget, gpointer user_data)
{
    if (currnent_page > MAX_ROW_ONEPAGE)
    {
        currnent_page-=MAX_ROW_ONEPAGE;
        load_book_to_layout(builder, currnent_page);
    }
}

void on_next_page1_clicked(GtkWidget *widget, gpointer user_data)
{
    currnent_page+=MAX_ROW_ONEPAGE;
    load_book_to_layout(builder, currnent_page);
}