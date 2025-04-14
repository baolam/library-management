#include "gui/events/search.h"

void on_search_entry_changed(GtkSearchEntry *entry, gpointer user_data)
{
    // GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_mode"));

    GtkSearchEntry *search_book_entry = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "entry_search_book"));

    const gchar *entry_search_book = gtk_entry_get_text(GTK_ENTRY(search_book_entry));
    // printf("Received data : %s\n", entry_search_book);
    if (strlen(entry_search_book) == 0)
       return;

    if (gtk_toggle_button_get_active(toggle))
    {
        printf("Search by name,");
        printf("Ban co goi tui khum");
        int size = 0;
        Book *books = search_book_by_title_direct(entry_search_book, &size, MAX_ROW_ONEPAGE);
        for (int i = 0; i < size; i++)
        {
            show_book(books[i]);
        }
        // return;
        show_book_to_layout(books, size);
    }
    else
    {
        printf("Search by id,");
        int book_id = atoi(entry_search_book);
        Book *book = search_book(book_id);
        if (book == NULL)
           return;
        show_onebook_to_layout(reference_store(), *book);
    }
}