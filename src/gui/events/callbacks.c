#include "gui/events/callbacks.h"

void open_book_entry_layout()
{
    clear_book_to_entry();
    GtkWidget *add_window = GTK_WIDGET(gtk_builder_get_object(builder, "book_entry"));
    if (add_window)
    {
        gtk_widget_show_all(add_window);
    }
    else
    {
        g_warning("Không tìm thấy cửa sổ !");
    }
}

void open_reader_entry_layout()
{
    clear_reader_to_entry();
    GtkWidget *add_window = GTK_WIDGET(gtk_builder_get_object(builder, "reader_entry"));
    if (add_window)
    {
        gtk_widget_show_all(add_window);
    }
    else
    {
        g_warning("Không tìm thấy cửa sổ !");
    }
}

void open_borrow_book_layout()
{
    GtkWidget *add_window = GTK_WIDGET(gtk_builder_get_object(builder, "borrow_detail_layout"));
    if (add_window)
    {
        gtk_widget_show_all(add_window);
    }
    else
    {
        g_warning("Không tìm thấy cửa sổ !");
    }
}

void open_book_borrow_entry_layout()
{
    GtkWidget *add_window = GTK_WIDGET(gtk_builder_get_object(builder, "book_borrow_entry"));
    if (add_window)
    {
        gtk_widget_show_all(add_window);
    }
    else
    {
        g_warning("Không tìm thấy cửa sổ !");
    }
}

gboolean on_window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    gtk_widget_hide(widget);
    printf(">>> Su kien dong cua so duoc kich hoat \n");
    return TRUE;
}

gboolean on_main_window_delete_event(GtkWidget *widge, GdkEvent *event, gpointer user_data)
{
    /// Hàm gọi đóng toàn bộ cửa sổ và thoát chương trình
    // gtk_main_quit();
    return FALSE;
}

void on_reader_add_clicked(GtkWidget *widget, gpointer user_data)
{
    reader_chosen_action = READER_ADD_STATUS;
    open_reader_entry_layout();
}

void on_book_add_clicked(GtkWidget *widget, gpointer user_data)
{
    /// Tiến hành nhớ trạng thái
    book_chosen_action = BOOK_ADD_STATUS;
    open_book_entry_layout();
}

void on_borrow_add_clicked(GtkWidget *widget, gpointer user_data)
{
    /// Tiến hành nhớ trạng thái
    borrow_chosen_action = BORROW_ADD_STATUS;
    open_borrow_book_layout();
}

void show_warning_not_found()
{
    GtkWindow *parent_window = GTK_WINDOW(gtk_builder_get_object(builder, "borrow_detail_layout"));
    GtkWidget *dialog = gtk_message_dialog_new(
        parent_window,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Không có sách nào được chọn (Bạn hãy nhấn giữ cuốn sách bạn muốn cho mượn)!");

    gtk_window_set_title(GTK_WINDOW(dialog), "Thông báo");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_warning_existed()
{
    GtkWindow *parent_window = GTK_WINDOW(gtk_builder_get_object(builder, "borrow_detail_layout"));
    GtkWidget *dialog = gtk_message_dialog_new(
        parent_window,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Sách bạn chọn đã được cho mượn trước đó!");

    gtk_window_set_title(GTK_WINDOW(dialog), "Thông báo");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_book_borrow_add_clicked(GtkWidget *widget, gpointer user_data)
{
    /// Tiến hành lấy bookId
    gint chosen_id = get_book_id("book_borrow_treeview_show");
    if (chosen_id == -1)
    {
        show_warning_not_found();
        return;
    }

    printf(">>> Id sach duoc chon la : %d \n", chosen_id);
    printf(">>> Temp_borrow co khac rong khong : %d \n", temp_borrow != NULL);

    /// Kiểm thử id sách đó đã tồn tại hay chưa
    if (exist_bookid_bookborrow(temp_borrow, chosen_id))
    {
        show_warning_existed();
        return;
    }

    BookBorrowEntry entry;
    entry.bookId = chosen_id;

    Book *wanted_book = search_book(chosen_id);
    if (wanted_book == NULL)
    {
        printf("Loi khong ngo toi o day \n");
        return;
    }

    entry.left = wanted_book->stock;

    entry.quantity = 1;

    open_book_borrow_entry_layout();
    show_book_borrow_to_entry(entry);
}

void on_to_index_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder, "GtkStack"));
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "index");
}

void on_to_main_ui_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder, "GtkStack"));
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "main_ui");
}

void on_to_credits_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder, "GtkStack"));
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "credits");
}
