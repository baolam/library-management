#include "gui/events/search.h"

void on_search_entry_changed(GtkSearchEntry *entry, gpointer user_data)
{
    GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_mode"));

    GtkSearchEntry *search_book_entry = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "entry_search_book"));

    const gchar *entry_search_book = gtk_entry_get_text(GTK_ENTRY(search_book_entry));
    if (strlen(entry_search_book) == 0)
        return;

    if (gtk_toggle_button_get_active(toggle))
    {
        int size = 0;
        Book *books = search_book_by_title_direct(entry_search_book, &size, MAX_ROW_ONEPAGE);
        show_book_to_layout(books, size);
    }
    else
    {
        int book_id = atoi(entry_search_book);
        Book *book = search_book(book_id);
        if (book == NULL)
            return;
        show_onebook_to_layout(reference_store(), *book);
    }
}