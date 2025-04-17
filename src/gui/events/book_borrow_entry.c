#include "gui/events/book_borrow_entry.h"

BookBorrowEntry get_book_borrow_input()
{
    BookBorrowEntry entry;

    gchar *bookId = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_borrow_id")));
    gchar *left = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_borrow_left")));
    gchar *quantity = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_borrow_quantity")));

    entry.bookId = atoi(bookId);
    entry.left = atoi(left);
    entry.quantity = atoi(quantity);

    return entry;
}

void show_book_borrow_to_entry(BookBorrowEntry book_borrow_entry)
{
    gchar *bookId = g_strdup_printf("%d", book_borrow_entry.bookId);
    gchar *left = g_strdup_printf("%d", book_borrow_entry.left);
    gchar *quantity = g_strdup_printf("%d", book_borrow_entry.quantity);

    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_borrow_id")), bookId);
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_borrow_left")), left);
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_borrow_quantity")), quantity);

    g_free(bookId);
    g_free(left);
    g_free(quantity);
}

void clear_book_borrow_to_entry()
{
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_borrow_id")), "");
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_borrow_left")), "");
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_book_borrow_quantity")), "");
}

void show_warning_quantity()
{
    GtkWindow *parent_window = GTK_WINDOW(gtk_builder_get_object(builder, "book_borrow_entry"));
    GtkWidget *dialog = gtk_message_dialog_new(
        parent_window,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Số lượng sách muốn mượn vượt quá số lượng còn lại!");

    gtk_window_set_title(GTK_WINDOW(dialog), "Thông báo");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_warning_add_borrow_failed()
{
    GtkWindow *parent_window = GTK_WINDOW(gtk_builder_get_object(builder, "book_borrow_entry"));
    GtkWidget *dialog = gtk_message_dialog_new(
        parent_window,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Lỗi không ngờ tới khi thêm mượn sách!");

    gtk_window_set_title(GTK_WINDOW(dialog), "Thông báo");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_book_borrow_confirmed_clicked(GtkButton *button, GtkBuilder *_builder)
{
    printf("Lenh goi them sach o book_borrow_layout \n");

    BookBorrowEntry entry = get_book_borrow_input();

    /// Kiểm thử sự hợp lí dữ liệu
    if (entry.quantity > entry.left)
    {
        show_warning_quantity();
        return;
    }

    printf("Chua hieu temp_borrow dung hem %d\n", temp_borrow != NULL);

    if (add_bookborrow(temp_borrow, entry.bookId, entry.quantity))
    {
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "book_borrow_entry")));

        load_borrowbook_to_layout();
        load_book_to_layout(current_book_page);
        load_borrow_to_layout(current_borrow_page);

        save_borrow_return_management();
        save_book_management();
        return;
    }

    show_warning_add_borrow_failed();
    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "book_borrow_entry")));
}
