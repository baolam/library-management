#include "gui/events/search.h"

void on_book_search_entry_changed(GtkSearchEntry *entry, gpointer user_data)
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
        {
            load_book_to_layout(current_book_page);
            return;
        }
        show_onebook_to_layout(reference_store_book(), *book);
    }
}

void on_reader_search_entry_changed(GtkSearchEntry *entry, gpointer user_data)
{
    // GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkToggleButton *toggle1 = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_mode1"));

    GtkSearchEntry *search_reader_entry = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "entry_search_reader"));

    const gchar *entry_search_reader = gtk_entry_get_text(GTK_ENTRY(search_reader_entry));
    // printf("Received data : %s\n", entry_search_reader);
    if (strlen(entry_search_reader) == 0)
        return;

    if (gtk_toggle_button_get_active(toggle1))
    {
        int size = 0;
        Readers *readers = search_reader_by_name_direct(entry_search_reader, &size, MAX_ROW_ONEPAGE);
        show_reader_to_layout(readers, size);
    }
    else
    {
        int reader_id = atoi(entry_search_reader);
        Readers *reader = search_reader(reader_id);
        if (reader == NULL)
            return;
        show_onereader_to_layout(reference_store_reader(), *reader);
    }
}