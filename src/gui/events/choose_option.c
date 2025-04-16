#include "gui/events/choose_option.h"

void popup_book_menu_init(GtkBuilder *_builder)
{
    book_menu = GTK_WIDGET(gtk_builder_get_object(_builder, "book_menu"));
    gtk_widget_show_all(book_menu);
}

void popup_book_menu_show(GtkTreeView *treeview, GdkEventButton *event)
{
    if (event == NULL)
        return;

    if (event->type == GDK_2BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY)
    {
        if (book_menu)
        {
            gtk_menu_popup_at_pointer(GTK_MENU(book_menu), (GdkEvent *)event);
        }
    }
}

void popup_reader_menu_init(GtkBuilder *_builder)
{
    reader_menu = GTK_WIDGET(gtk_builder_get_object(_builder, "reader_menu"));
    gtk_widget_show_all(reader_menu);
}

void popup_reader_menu_show(GtkTreeView *treeview, GdkEventButton *event)
{
    if (event == NULL)
        return;

    if (event->type == GDK_2BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY)
    {
        if (reader_menu)
        {
            gtk_menu_popup_at_pointer(GTK_MENU(reader_menu), (GdkEvent *)event);
        }
    }
}

void popup_borrow_menu_init(GtkBuilder *_builder)
{
    borrow_menu = GTK_WIDGET(gtk_builder_get_object(_builder, "borrow_menu"));
    gtk_widget_show_all(borrow_menu);
}

void popup_borrow_menu_show(GtkTreeView *treeview, GdkEventButton *event)
{
    if (event == NULL)
        return;

    if (event->type == GDK_2BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY)
    {
        if (borrow_menu)
        {
            gtk_menu_popup_at_pointer(GTK_MENU(borrow_menu), (GdkEvent *)event);
        }
    }
}

void popup_book_borrow_menu_init(GtkBuilder *_builder)
{
    book_borrow_menu = GTK_WIDGET(gtk_builder_get_object(_builder, "book_borrow_menu"));
    gtk_widget_show_all(book_borrow_menu);
}

void popup_book_borrow_menu_show(GtkTreeView *treeview, GdkEventButton *event)
{
    if (event == NULL)
        return;

    if (event->type == GDK_2BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY)
    {
        if (book_borrow_menu)
        {
            gtk_menu_popup_at_pointer(GTK_MENU(book_borrow_menu), (GdkEvent *)event);
        }
    }
}

gboolean on_book_treeview_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    popup_book_menu_show(GTK_TREE_VIEW(widget), event);
    return FALSE;
}

void on_book_treeview_row_activated(GtkTreeView *treeview,
                                    GtkTreePath *path,
                                    GtkTreeViewColumn *column,
                                    gpointer user_data)
{
    GdkEventButton *event = (GdkEventButton *)user_data;
    popup_book_menu_show(treeview, event);
}

void on_reader_treeview_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
    GdkEventButton *event = (GdkEventButton *)user_data;
    popup_reader_menu_show(treeview, event);
}

gboolean on_reader_treeview_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    popup_reader_menu_show(GTK_TREE_VIEW(widget), event);
    return FALSE;
}

void on_borrow_treeview_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
    GdkEventButton *event = (GdkEventButton *)user_data;
    popup_borrow_menu_show(treeview, event);
}

gboolean on_borrow_treeview_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    popup_borrow_menu_show(GTK_TREE_VIEW(widget), event);
    return FALSE;
}

void on_book_borrow_treeview_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
    GdkEventButton *event = (GdkEventButton *)user_data;
    popup_book_borrow_menu_show(treeview, event);
}

gboolean on_book_borrow_treeview_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    popup_book_borrow_menu_show(GTK_TREE_VIEW(widget), event);
    return FALSE;
}

/// ----------------------------------------------------------------------------------
/// Nhóm phần thao tác book
/// ----------------------------------------------------------------------------------

gint get_book_id(const gchar *title)
{
    GtkWidget *treeview = GTK_WIDGET(gtk_builder_get_object(builder, title));
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gchar *id_str;
        gtk_tree_model_get(model, &iter, 0, &id_str, -1);
        gint id = atoi(id_str);
        return id;
    }

    return -1;
}

void on_book_edit_activate(GtkMenuItem *item, gpointer user_data)
{
    printf(">> Sua duoc chon!\n");

    book_chosen_action = BOOK_UPDATE_STATUS;
    book_chosen_id = get_book_id("book_treeview");
    printf("Chosen book id : %d\n", book_chosen_id);

    /// Tiến hành hiển thị giao diện
    open_book_entry_layout();

    /// Tiến hành hiển thị dữ liệu đã sửa
    Book *book = search_book(book_chosen_id);

    if (book)
        show_book_to_entry(*book);
    else
    {
        printf("Book la NULL \n");
    }
}

void on_book_delete_activate(GtkMenuItem *item, gpointer user_data)
{
    gint id = get_book_id("book_treeview");
    printf("Chon id : %d\n", id);

    delete_book(id);
    load_book_to_layout(current_book_page);
}

/// ----------------------------------------------------------------------------------
/// Nhóm phần thao tác reader
/// ----------------------------------------------------------------------------------
gint get_reader_id()
{
    GtkWidget *treeview = GTK_WIDGET(gtk_builder_get_object(builder, "reader_treeview"));
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gchar *id_str;
        gtk_tree_model_get(model, &iter, 0, &id_str, -1);
        gint id = atoi(id_str);
        return id;
    }

    return -1;
}

void on_reader_edit_activate(GtkMenuItem *item, gpointer user_data)
{
    printf(">> Sua duoc chon!\n");
    reader_chosen_action = READER_UPDATE_STATUS;
    reader_chosen_id = get_reader_id();

    open_reader_entry_layout();

    Readers *reader = search_reader(reader_chosen_id);
    if (reader)
        show_reader_to_entry(*reader);
}

void on_reader_delete_activate(GtkMenuItem *item, gpointer user_data)
{
    printf(">> Xoa duoc chon!\n");

    gint id = get_reader_id();
    printf("Chon id : %d\n", id);

    delete_reader(id);
    load_reader_to_layout(current_reader_page);
}

void on_reader_borrow_activate(GtkMenuItem *item, gpointer user_data)
{
    printf(">>> Muon sach duoc kich hoat \n");
    GtkWidget *parent_window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "Bạn có chắc muốn thực hiện hành động này?");
    gtk_window_set_title(GTK_WINDOW(dialog), "Xác nhận");
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (response == GTK_RESPONSE_YES)
    {
        borrow_chosen_id = get_reader_id();

        /// Kiểm thử không rỗng
        if (search_borrow_by_reader(borrow_chosen_id) == NULL)
        {
            /// Khởi tạo giả, rùi tiến hành add
            BorrowReturn default_borrow = default_borrow_return(borrow_chosen_id);
            add_borrow_record(&default_borrow);

            save_borrow_return_management();

            load_borrowbook_to_layout();
            load_borrow_to_layout(current_borrow_page);
        }

        open_borrow_book_layout();
    }
}

// ------------------------------------------------------------------------------
// Phần thao tác giao diện borrow
// ------------------------------------------------------------------------------
gint get_borrow_id()
{
    GtkWidget *treeview = GTK_WIDGET(gtk_builder_get_object(builder, "borrow_treeview"));
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gchar *id_str;
        gtk_tree_model_get(model, &iter, 0, &id_str, -1);
        gint id = atoi(id_str);
        return id;
    }

    return -1;
}

void on_borrow_detail_activate(GtkMenuItem *item, gpointer user_data)
{
    printf(">>> Chi tiet duoc kich hoat \n");
    borrow_chosen_id = get_borrow_id();
    printf(">>> ID duoc chon: %d \n", borrow_chosen_id);
    open_borrow_book_layout();
    load_borrowbook_to_layout();
}

void on_borrow_erase_activate(GtkMenuItem *item, gpointer user_data)
{
    printf(">>> Xoa borrow duoc kich hoat \n");
}

void show_latefee_if_existed(int lateFee)
{
    GtkWindow *parent_window = GTK_WINDOW(gtk_builder_get_object(builder, "borrow_detail_layout"));
    GtkWidget *dialog = gtk_message_dialog_new(
        parent_window,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Sách trả trễ hẹn. Phí phạt là : %d (VND)", lateFee);

    gtk_window_set_title(GTK_WINDOW(dialog), "Thông báo");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_book_borrow_return_activate(GtkMenu *item, gpointer user_data)
{
    gint chosen_id = get_book_id("book_borrow_treeview");
    printf(">>>> ID book duoc chon: %d \n", chosen_id);
    printf(">>>> ID Reader duoc chon: %d \n", borrow_chosen_id);

    int late_fees = return_books(borrow_chosen_id, chosen_id);
    if (late_fees > 0)
    {
        show_latefee_if_existed(late_fees);
    }

    load_borrowbook_to_layout();
}