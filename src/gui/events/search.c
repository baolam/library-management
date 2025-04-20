#include "gui/events/search.h"

void on_book_search_entry_changed(GtkSearchEntry *entry, gpointer user_data)
{
    GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_book_mode"));
    GtkSearchEntry *search_book_entry = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "entry_search_book"));

    const gchar *entry_search = gtk_entry_get_text(GTK_ENTRY(search_book_entry));
    if (strlen(entry_search) == 0)
    {
        load_book_to_layout(current_book_page);
        return;
    }

    if (gtk_toggle_button_get_active(toggle))
    {
        int size = 0;
        Book *books = search_book_by_title_direct(entry_search, &size, MAX_ROW_ONEPAGE);
        if (size == 0)
        {
            load_book_to_layout(1000100);
            return;
        } else {
        show_book_to_layout(books, size);
        }
    }
    else
    {
        if (isdigit(entry_search[0])) {
        int book_id = atoi(entry_search);
        Book *book = search_book(book_id);
        if (book == NULL)
        {
            load_book_to_layout(1000100);
            return;
        } else {
        show_onebook_to_layout(reference_store_book(), *book);
        }
    }
    }
}

void on_reader_search_entry_changed(GtkSearchEntry *entry, gpointer user_data)
{
    GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_reader_mode"));
    GtkSearchEntry *search_reader_entry = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "entry_search_reader"));

    const gchar *entry_search = gtk_entry_get_text(GTK_ENTRY(search_reader_entry));

    if (strlen(entry_search) == 0)
    {
        load_reader_to_layout(current_reader_page);
        return;
    }

    if (gtk_toggle_button_get_active(toggle))
    {
        int size = 0;
        Readers *readers = search_reader_by_name_direct(entry_search, &size, MAX_ROW_ONEPAGE);
        // if (size == 0)
        // {
        //     load_reader_to_layout(current_reader_page);
        //     return;
        // }
        show_reader_to_layout(readers, size);
    }
    else
    {
        if (isdigit(entry_search[0])) {
        int reader_id = atoi(entry_search);
        Readers *reader = search_reader(reader_id);
        // if (reader == NULL)
        // {
        //     load_reader_to_layout(1000100);
        //     return;
        // }
        show_onereader_to_layout(reference_store_reader(), *reader);
        }
    }
}

void on_borrow_search_entry_changed(GtkSearchEntry *entry, gpointer user_data)
{
    printf(">>> Tim kiem kich hoat \n");
    GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_borrow_mode"));
    GtkSearchEntry *search_reader_entry = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "entry_search_borrow"));

    const gchar *entry_search = gtk_entry_get_text(GTK_ENTRY(search_reader_entry));
    if (strlen(entry_search) == 0)
    {
        load_borrow_to_layout(current_borrow_page);
        return;
    }

    if (gtk_toggle_button_get_active(toggle))
    {
        printf(">>> Chuc nang dang bi khoa:\n");
        // int size = 0;
        // Readers *readers = search_reader_by_name_direct(entry_search, &size, MAX_ROW_ONEPAGE);
        // BorrowReturn borrowReturn[size];
        // for (int i = 0; i < size; i++)
        // {
        //     borrowReturn[i] = *((BorrowReturn *)search_borrow_by_reader(readers[i].readerId));
        // }
        // show_borrow_to_layout(borrowReturn, size);
        return;
    }
    else
    {
        if (isdigit(entry_search[0])) {
        int reader_id = atoi(entry_search);
        BorrowReturn *borrow_return = search_borrow_by_reader(reader_id);
        if (borrow_return == NULL)
        {
            load_borrow_to_layout(1000100);
            return;
        }
        show_oneborrow_to_layout(reference_borrow_return_store(), *borrow_return);
       }
    }
}

void on_entry_search_book_borrow_search_changed(GtkSearchEntry *entry, gpointer user_data)
{
    GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_mode_book_borrow"));
    GtkSearchEntry *search_book_entry = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "entry_search_book_borrow"));

    const gchar *entry_search = gtk_entry_get_text(GTK_ENTRY(search_book_entry));
    if (strlen(entry_search) == 0)
    {
        load_book_to_layout(current_book_page);
        return;
    }

    if (gtk_toggle_button_get_active(toggle))
    {
        int size = 0;
        Book *books = search_book_by_title_direct(entry_search, &size, MAX_ROW_ONEPAGE);
        // if (size == 0)
        // {
        //     load_book_to_layout(current_book_page);
        //     return;
        // }
        show_book_to_layout(books, size);
    }
    else
    {
        if (isdigit(entry_search[0])) {
        int book_id = atoi(entry_search);
        Book *book = search_book(book_id);
        if (book == NULL)
        {
            load_book_to_layout(1000100);
            return;
        }
        show_onebook_to_layout(reference_store_book(), *book);
        }
    }
}