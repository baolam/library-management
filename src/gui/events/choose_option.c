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
    popup_book_menu_show(GTK_TREE_VIEW(widget), event);
    return FALSE;
}

/// ----------------------------------------------------------------------------------
/// Nhóm phần thao tác book
/// ----------------------------------------------------------------------------------

gint get_book_id()
{
    GtkWidget *treeview = GTK_WIDGET(gtk_builder_get_object(builder, "book_treeview"));
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

void on_book_edit(GtkMenuItem *item, gpointer user_data)
{
    printf(">> Sua duoc chon!\n");

    chosen_action = BOOK_UPDATE_STATUS;
    gint id = get_book_id();
    chosen_id = id;

    /// Tiến hành hiển thị giao diện
    open_book_entry_layout();

    /// Tiến hành hiển thị dữ liệu đã sửa
    Book *book = search_book(id);
    if (book)
        show_book_to_entry(*book);
}

void on_book_delete(GtkMenuItem *item, gpointer user_data)
{
    gint id = get_book_id();
    printf("Chon id : %d\n", id);

    delete_book(id);
    load_book_to_layout(current_book_page);
}

/// ----------------------------------------------------------------------------------
/// Nhóm phần thao tác reader
/// ----------------------------------------------------------------------------------
void on_reader_edit(GtkMenuItem *item, gpointer user_data)
{
    printf(">> Sua duoc chon!\n");
}

void on_reader_delete(GtkMenuItem *item, gpointer user_data)
{
    printf(">> Xoa duoc chon!\n");
}