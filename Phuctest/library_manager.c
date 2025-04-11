#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global widget pointers (để được export symbol)
GtkWidget *main_window;
GtkWidget *book_treeview;
GtkWidget *search_entry;
GtkListStore *liststore_books;

void load_books_from_file() {
    FILE *file = fopen("books.txt", "r");
    if (!file)
        return;
    
    gtk_list_store_clear(liststore_books);
    
    GtkTreeIter iter;
    char id[100], title[256], author[256], genre[128];
    int year, quantity;
    char str_year[20], str_quantity[20];
    // Định dạng file: id "title" "author" "genre" year quantity
    while (fscanf(file, "%s \"%255[^\"]\" \"%255[^\"]\" \"%127[^\"]\" %d %d", 
                  id, title, author, genre, &year, &quantity) == 6) {
        sprintf(str_year, "%d", year);
        sprintf(str_quantity, "%d", quantity);
        gtk_list_store_append(liststore_books, &iter);
        gtk_list_store_set(liststore_books, &iter,
                           0, id,
                           1, title,
                           2, author,
                           3, genre,
                           4, str_year,
                           5, str_quantity,
                           -1);
    }
    fclose(file);
}

void save_books_to_file() {
    FILE *file = fopen("books.txt", "w");
    if (!file)
        return;
    
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(liststore_books), &iter);
    char *id, *title, *author, *genre, *year, *quantity;
    while (valid) {
        gtk_tree_model_get(GTK_TREE_MODEL(liststore_books), &iter, 
                           0, &id, 
                           1, &title, 
                           2, &author, 
                           3, &genre, 
                           4, &year, 
                           5, &quantity, -1);
        fprintf(file, "%s \"%s\" \"%s\" \"%s\" %s %s\n", id, title, author, genre, year, quantity);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(liststore_books), &iter);
    }
    fclose(file);
}

void on_main_window_destroy() {
    gtk_main_quit();
}

void on_search_entry_changed(GtkSearchEntry *entry, gpointer user_data) {
    // Ví dụ: Khi nội dung tìm kiếm thay đổi, reload lại file dữ liệu.
    // (Bạn có thể thay đổi để lọc nội bộ ListStore nếu muốn.)
    load_books_from_file();
}

void on_book_treeview_row_activated(GtkTreeView *treeview, GtkTreePath *path,
                                    GtkTreeViewColumn *column, gpointer user_data) {
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter(GTK_TREE_MODEL(liststore_books), &iter, path)) {
        char *id;
        gtk_tree_model_get(GTK_TREE_MODEL(liststore_books), &iter, 0, &id, -1);
        g_print("Row activated: %s\n", id);
    }
}

void on_add_button_clicked(GtkButton *button, gpointer user_data) {
    // Ví dụ: Thêm một cuốn sách mẫu vào ListStore
    GtkTreeIter iter;
    char id[100] = "001";
    char title[256] = "Sample Book";
    char author[256] = "Author";
    char genre[128] = "Genre";
    int year = 2023, quantity = 1;
    char str_year[20], str_quantity[20];
    sprintf(str_year, "%d", year);
    sprintf(str_quantity, "%d", quantity);
    
    gtk_list_store_append(liststore_books, &iter);
    gtk_list_store_set(liststore_books, &iter,
                       0, id,
                       1, title,
                       2, author,
                       3, genre,
                       4, str_year,
                       5, str_quantity,
                       -1);
    save_books_to_file();
}

void on_edit_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Edit button clicked\n");
    // Cập nhật chức năng chỉnh sửa theo nhu cầu
}

void on_delete_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(book_treeview));
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(main_window),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_QUESTION,
                                                   GTK_BUTTONS_YES_NO,
                                                   "Bạn có chắc chắn muốn xóa cuốn sách này?");
        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES) {
            gtk_list_store_remove(liststore_books, &iter);
            save_books_to_file();
        }
        gtk_widget_destroy(dialog);
    } else {
        g_print("Vui lòng chọn một cuốn sách để xóa!\n");
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    GtkBuilder *builder = gtk_builder_new_from_file("library.glade");
    
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    book_treeview = GTK_WIDGET(gtk_builder_get_object(builder, "book_treeview"));
    search_entry = GTK_WIDGET(gtk_builder_get_object(builder, "search_entry"));
    liststore_books = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
    
    // Kết nối các signal đã được khai báo trong Glade
    gtk_builder_connect_signals(builder, NULL);
    
    load_books_from_file();
    gtk_widget_show_all(main_window);
    gtk_main();
    return 0;
}
