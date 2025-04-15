#include "gui/events/reader_treeview.h"
#include <stdio.h>
#include <string.h>

void show_onereader_to_layout(GtkListStore *store, Readers reader)
{
    gchar *readerId = g_strdup_printf("%d", reader.readerId);

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, readerId,
                       1, reader.fullName,
                       2, reader.phoneNumber,
                       3, reader.address,
                       -1);

    g_free(readerId);
}

GtkListStore *reference_store_reader()
{
    GtkListStore *store = GTK_LIST_STORE(gtk_builder_get_object(builder, "reader_data"));
    gtk_list_store_clear(store);
    return store;
}

void show_reader_to_layout(Readers *reader, int size)
{
    GtkListStore *store = reference_store_reader();

    int i;
    for (i = 0; i < size; i++)
    {
        show_onereader_to_layout(store, reader[i]);
    }
}

void load_reader_to_layout(int beginingKey)
{

    int size = 0;
    Readers *reader = retrieve_bucket_readers(beginingKey, MAX_ROW_ONEPAGE, &size);
    show_reader_to_layout(reader, size);
}

void on_previous_reader_page_clicked(GtkWidget *widget, gpointer user_data)
{
    if (current_reader_page > MAX_ROW_ONEPAGE)
    {
        current_reader_page -= MAX_ROW_ONEPAGE;
        load_reader_to_layout(current_reader_page);
    }
}

void on_next_reader_page_clicked(GtkWidget *widget, gpointer user_data)
{
    current_reader_page += MAX_ROW_ONEPAGE;
    load_reader_to_layout(current_reader_page);
}