#include "search.h"

void on_search_entry_changed(GtkSearchEntry *entry, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_mode"));

    GtkSearchEntry *search_book = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "entry_search_book"));

    const gchar *entry_search_book = gtk_entry_get_text(GTK_ENTRY(search_bookbook));

    if (gtk_toggle_button_get_active(toggle)) {
        search_book_by_title(entry_search_book); 
    } else {
        int book_id = atoi(entry_search_book);
        search_book_by_id(book_id);  
    }
}