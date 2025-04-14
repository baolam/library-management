#include "gui/events/books_entry.h"
#include "gui/events/callbacks.h"
#include "gui/events/book_treeview.h"
#include "gui/gui.h"
#include "gui/ui_loader.h"
#include "books.h"

void on_book_button_confirm_clicked(GtkButton *button, gpointer user_data)
{
    Book book;

    book.bookId = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_id"))));

    strncpy(book.title, gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_title"))), MAX_TITLE - 1);
    book.title[MAX_TITLE - 1] = '\0';

    strncpy(book.author, gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_author"))), MAX_AUTHOR - 1);
    book.author[MAX_AUTHOR - 1] = '\0';

    strncpy(book.genre, gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_genre"))), MAX_GENRE_NO - 1);
    book.genre[MAX_GENRE_NO - 1] = '\0';

    book.publicationYear = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_year"))));
    book.stock = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_stock"))));

    gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(builder, "entry_book_quantity")));

    show_book(book);
}