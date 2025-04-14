#include "gui/events/books_entry.h"

// Hàm xử lý khi nhấn nút "Xác nhận"
void on_button_confirm_clicked(GtkButton *button, GtkBuilder *_builder)
{
    Book book;

    book.bookId = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_builder, "entry_book_id"))));

    strncpy(book.title, gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_builder, "entry_title"))), MAX_TITLE - 1);
    book.title[MAX_TITLE - 1] = '\0';

    strncpy(book.author, gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_builder, "entry_author"))), MAX_AUTHOR - 1);
    book.author[MAX_AUTHOR - 1] = '\0';

    strncpy(book.genre, gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_builder, "entry_genre"))), MAX_GENRE_NO - 1);
    book.genre[MAX_GENRE_NO - 1] = '\0';

    book.publicationYear = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_builder, "entry_publication_year"))));
    book.stock = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_builder, "entry_stock"))));

    // Đóng cửa sổ
    gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(_builder, "book_entry")));

    show_book(book);
}