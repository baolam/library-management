#include "gui/events/callbacks.h"

void open_book_entry_layout()
{
    GtkWidget *add_window = GTK_WIDGET(gtk_builder_get_object(builder, "book_entry"));
    if (add_window)
    {
        gtk_widget_show_all(add_window);
    }
    else
    {
        g_warning("Không tìm thấy cửa sổ 'add_window'!");
    }
}

void open_reader_entry_layout()
{
    GtkWidget *add_window = GTK_WIDGET(gtk_builder_get_object(builder, "reader_entry"));
    if (add_window)
    {
        gtk_widget_show_all(add_window);
    }
    else
    {
        g_warning("Không tìm thấy cửa sổ 'add_window'!");
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
        g_warning("Không tìm thấy cửa sổ 'add_window'!");
    }
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
