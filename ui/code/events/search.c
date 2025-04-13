#include "search.h"

void on_search_entry_changed(GtkSearchEntry *entry, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_mode"));

    const gchar *query = gtk_entry_get_text(GTK_ENTRY(entry));

    if (gtk_toggle_button_get_active(toggle)) {
        search_book_by_title(query)
    } else {
        int id = atoi(query);
        search_book_by_id(id)
    }
}
