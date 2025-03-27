#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256

// Structure to hold book data
typedef struct {
    char id[10];
    char title[100];
    char author[100];
    char genre[50];
    char year[10];
    char quantity[10];
} Book;

// Global variables
GtkListStore *liststore;
GtkTreeView *treeview;
const char *filename = "books.txt";

// Function to load books from file into the list store
void load_books_from_file() {
    FILE *file = fopen(filename, "r");
    if (!file) {
        g_print("Không thể mở file %s!\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        Book book;
        char *token = strtok(line, "|");
        strcpy(book.id, token ? token : "");
        token = strtok(NULL, "|");
        strcpy(book.title, token ? token : "");
        token = strtok(NULL, "|");
        strcpy(book.author, token ? token : "");
        token = strtok(NULL, "|");
        strcpy(book.genre, token ? token : "");
        token = strtok(NULL, "|");
        strcpy(book.year, token ? token : "");
        token = strtok(NULL, "|");
        strcpy(book.quantity, token ? token : "");

        // Add to list store
        GtkTreeIter iter;
        gtk_list_store_append(liststore, &iter);
        gtk_list_store_set(liststore, &iter,
                           0, book.id,
                           1, book.title,
                           2, book.author,
                           3, book.genre,
                           4, book.year,
                           5, book.quantity,
                           -1);
    }

    fclose(file);
}

// Function to save books from list store to file
void save_books_to_file() {
    FILE *file = fopen(filename, "w");
    if (!file) {
        g_print("Không thể mở file %s để ghi!\n", filename);
        return;
    }

    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(liststore), &iter);
    while (valid) {
        char id[10], title[100], author[100], genre[50], year[10], quantity[10];
        gtk_tree_model_get(GTK_TREE_MODEL(liststore), &iter,
                           0, id,
                           1, title,
                           2, author,
                           3, genre,
                           4, year,
                           5, quantity,
                           -1);
        fprintf(file, "%s|%s|%s|%s|%s|%s\n", id, title, author, genre, year, quantity);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(liststore), &iter);
    }

    fclose(file);
}

// Callback for "Add" button
void on_add_button_clicked(GtkButton *button, gpointer user_data) {
    // Create dialog for adding a book
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Thêm sách",
                                                    GTK_WINDOW(user_data),
                                                    GTK_DIALOG_MODAL,
                                                    "_OK",
                                                    GTK_RESPONSE_OK,
                                                    "_Cancel",
                                                    GTK_RESPONSE_CANCEL,
                                                    NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    // Create entry fields
    GtkWidget *id_entry = gtk_entry_new();
    GtkWidget *title_entry = gtk_entry_new();
    GtkWidget *author_entry = gtk_entry_new();
    GtkWidget *genre_entry = gtk_entry_new();
    GtkWidget *year_entry = gtk_entry_new();
    GtkWidget *quantity_entry = gtk_entry_new();

    // Add labels and entries to grid
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Tên sách:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Tác giả:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), author_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Thể loại:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), genre_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Năm xuất bản:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), year_entry, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Số lượng:"), 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), quantity_entry, 1, 5, 1, 1);

    gtk_widget_show_all(dialog);

    // Run dialog
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        const char *id = gtk_entry_get_text(GTK_ENTRY(id_entry));
        const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const char *author = gtk_entry_get_text(GTK_ENTRY(author_entry));
        const char *genre = gtk_entry_get_text(GTK_ENTRY(genre_entry));
        const char *year = gtk_entry_get_text(GTK_ENTRY(year_entry));
        const char *quantity = gtk_entry_get_text(GTK_ENTRY(quantity_entry));

        // Add to list store
        GtkTreeIter iter;
        gtk_list_store_append(liststore, &iter);
        gtk_list_store_set(liststore, &iter,
                           0, id,
                           1, title,
                           2, author,
                           3, genre,
                           4, year,
                           5, quantity,
                           -1);

        // Save to file
        save_books_to_file();
    }

    gtk_widget_destroy(dialog);
}

// Callback for "Edit" button
void on_edit_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
        // Get current values
        char id[10], title[100], author[100], genre[50], year[10], quantity[10];
        gtk_tree_model_get(GTK_TREE_MODEL(liststore), &iter,
                           0, id,
                           1, title,
                           2, author,
                           3, genre,
                           4, year,
                           5, quantity,
                           -1);

        // Create dialog for editing
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Sửa sách",
                                                        GTK_WINDOW(user_data),
                                                        GTK_DIALOG_MODAL,
                                                        "_OK",
                                                        GTK_RESPONSE_OK,
                                                        "_Cancel",
                                                        GTK_RESPONSE_CANCEL,
                                                        NULL);

        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *grid = gtk_grid_new();
        gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
        gtk_container_add(GTK_CONTAINER(content_area), grid);

        // Create entry fields with current values
        GtkWidget *id_entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(id_entry), id);
        GtkWidget *title_entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(title_entry), title);
        GtkWidget *author_entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(author_entry), author);
        GtkWidget *genre_entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(genre_entry), genre);
        GtkWidget *year_entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(year_entry), year);
        GtkWidget *quantity_entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(quantity_entry), quantity);

        // Add labels and entries to grid
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID:"), 0, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Tên sách:"), 0, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Tác giả:"), 0, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), author_entry, 1, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Thể loại:"), 0, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), genre_entry, 1, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Năm xuất bản:"), 0, 4, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), year_entry, 1, 4, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Số lượng:"), 0, 5, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), quantity_entry, 1, 5, 1, 1);

        gtk_widget_show_all(dialog);

        // Run dialog
        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
            const char *new_id = gtk_entry_get_text(GTK_ENTRY(id_entry));
            const char *new_title = gtk_entry_get_text(GTK_ENTRY(title_entry));
            const char *new_author = gtk_entry_get_text(GTK_ENTRY(author_entry));
            const char *new_genre = gtk_entry_get_text(GTK_ENTRY(genre_entry));
            const char *new_year = gtk_entry_get_text(GTK_ENTRY(year_entry));
            const char *new_quantity = gtk_entry_get_text(GTK_ENTRY(quantity_entry));

            // Update list store
            gtk_list_store_set(liststore, &iter,
                               0, new_id,
                               1, new_title,
                               2, new_author,
                               3, new_genre,
                               4, new_year,
                               5, new_quantity,
                               -1);

            // Save to file
            save_books_to_file();
        }

        gtk_widget_destroy(dialog);
    } else {
        g_print("Vui lòng chọn một cuốn sách để sửa!\n");
    }
}

// Callback for "Delete" button
void on_delete_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
        // Confirm deletion
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_QUESTION,
                                                   GTK_BUTTONS_YES_NO,
                                                   "Bạn có chắc chắn muốn xóa cuốn sách này?");
        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES) {
            gtk_list_store_remove(liststore, &iter);
            save_books_to_file();
        }
        gtk_widget_destroy(dialog);
    } else {
        g_print("Vui lòng chọn một cuốn sách để xóa!\n");
    }
}

// Callback for window destroy
void on_main_window_destroy() {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Load the UI from the Glade file
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    if (!gtk_builder_add_from_file(builder, "library.glade", &error)) {
        g_print("Không thể tải file library.glade: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    // Get the main window
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_builder_connect_signals(builder, window);

    // Get the treeview and liststore
    treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "book_treeview"));
    liststore = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));

    // Load books from file
    load_books_from_file();

    // Show the window
    gtk_widget_show_all(window);

    gtk_main();

    g_object_unref(builder);
    return 0;
}