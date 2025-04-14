#include "gui/events/books_entry.h"
#include "gui/events/callbacks.h"
#include "gui/events/book_treeview.h"
#include "gui/gui.h"
#include "gui/ui_loader.h"
#include "books.h"

<<<<<<< HEAD
void on_book_button_confirm_clicked(GtkButton *button, gpointer user_data)
=======
Book get_book_input()
{
    Book book;

    gchar *id = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_id")));
    gchar *title = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_title")));
    gchar *author = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_author")));
    gchar *genre = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_genre")));
    gchar *publicationYear = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_year")));
    gchar *quanity = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_quantity")));

    /// Tiến hành gán Object
    strncpy(book.title, title, MAX_TITLE - 1);
    strncpy(book.author, author, MAX_AUTHOR - 1);
    strncpy(book.genre, genre, MAX_GENRE_NO - 1);

    book.title[MAX_TITLE - 1] = '\0';
    book.author[MAX_AUTHOR - 1] = '\0';
    book.genre[MAX_GENRE_NO - 1] = '\0';

    book.bookId = atoi(id);
    book.publicationYear = atoi(publicationYear);
    book.stock = atoi(quanity);

    return book;
}

void show_book_to_entry(Book book)
{
    gchar *bookId = g_strdup_printf("%d", book.bookId);
    gchar *year = g_strdup_printf("%d", book.publicationYear);
    gchar *stock = g_strdup_printf("%d", book.stock);

    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_id")), bookId);
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_title")), book.title);
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_author")), book.author);
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_genre")), book.genre);
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_year")), year);
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_quantity")), stock);

    g_free(bookId);
    g_free(year);
    g_free(stock);
}

// Hàm xử lý khi nhấn nút "Xác nhận"
void on_button_confirm_clicked(GtkButton *button, GtkBuilder *_builder)
>>>>>>> e1a7e6a40cfb92d244d1ed2dcee7c0820c9dbbf5
{
    printf("Trang thai hoat dong: %d\n", chosen_action);

<<<<<<< HEAD
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
=======
    Book book = get_book_input();
    if (chosen_action == BOOK_ADD_STATUS)
    {
        /// Tiến hành lấy lại Id hợp lí
        book.bookId = total_books + 1;
        total_books++;
        add_book(&book);
        save_book_management();
    }
    else if (chosen_action == BOOK_UPDATE_STATUS)
    {
        int status = update_book_from_object(&book);
        if (status == UPDATE_SUCCESS)
        {
            printf("Update book successfully!\n");
            save_book_management();
            load_book_to_layout(current_book_page);
        }
        else
        {
            printf("Update failed !\n");
        }
    }

    chosen_action = BOOK_NO_ACTION;
    chosen_id = -1;

    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "book_entry")));
>>>>>>> e1a7e6a40cfb92d244d1ed2dcee7c0820c9dbbf5
}