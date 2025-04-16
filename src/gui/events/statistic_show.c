#include "gui/events/statistic_show.h"

GtkListStore *reference_statistic_store()
{
    GtkListStore *store = GTK_LIST_STORE(gtk_builder_get_object(builder, "overdue_borrowers_data"));
    gtk_list_store_clear(store);
    return store;
}

void load_statistic_to_layout()
{
    gchar *totalBooks = g_strdup_printf("%d", total_books);
    gchar *totalGenres = g_strdup_printf("%d", total_genres);
    gchar *totalBorrowBooks = g_strdup_printf("%d", total_borrowed_books);
    gchar *totalLateBooks = g_strdup_printf("%d", total_late_books);
    gchar *totalReaders = g_strdup_printf("%d", total_readers);

    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "total_books")), totalBooks);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "total_genres")), totalGenres);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "total_borrows")), totalBorrowBooks);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "total_overdue_books")), totalLateBooks);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "total_readers")), totalReaders);
}

int get_min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

void load_statistic_late_fess()
{
    GtkListStore *store = reference_statistic_store();

    int _min = get_min(current_statistic_borrower_page + MAX_ROW_BORROWER_PAGE, overdue_count);
    for (int i = current_statistic_borrower_page; i < _min; i++)
    {
        show_onestatisticlate_to_layout(store, &overdue_list[i]);
    }
}

void show_onestatisticlate_to_layout(GtkListStore *store, OverdueBorrower *borrower)
{
    gchar *readerId = g_strdup_printf("%d", borrower->readerId);
    gchar *lateFees = g_strdup_printf("%d", borrower->late_fees);

    Readers *r = search_reader(borrower->readerId);

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, readerId,
                       1, r->fullName,
                       2, lateFees,
                       -1);

    g_free(readerId);
    g_free(lateFees);
}

void on_previous_statistics_page_clicked(GtkWidget *widget, gpointer user_data)
{
    if (current_statistic_borrower_page > MAX_ROW_BORROWER_PAGE - 1)
    {
        current_statistic_borrower_page -= MAX_ROW_BORROWER_PAGE;
        load_statistic_late_fess();
    }
}

void on_next_statistics_page_clicked(GtkWidget *widget, gpointer user_data)
{
    current_statistic_borrower_page += MAX_ROW_BORROWER_PAGE;
    load_statistic_late_fess();
}