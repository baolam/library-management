#include "book_treeview.h"
#include "book.h"
#include <stdio.h>
#include <string.h>

void load_books_from_file(GtkBuilder *builder) {
    GtkListStore *store = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
    if (!store) {
        g_warning("Không tìm thấy liststore1 trong Glade");
        return;
    }

    int size = 0;
    Book *book = retrieve_bucket_books(beginKey, 15, &size);

    int i;
    for (i = 0; i < size; i++) {
    show_book(book[i]);

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
        0, book[i].bookId,
        1, book[i].title,
        2, book[i].author,
        3, book[i].genre,
        4, book[i].publicationYear,
        5, book[i].stock,
        -1);
    }
}