#include "gui/events/choose_option.h"

GtkWidget *menu = NULL;

void popup_menu_init(GtkBuilder *builder)
{
    menu = GTK_WIDGET(gtk_builder_get_object(builder, "book_menu"));
    gtk_widget_show_all(menu);
}

gboolean on_treeview_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    popup_menu_show(GTK_TREE_VIEW(widget), event);
    return FALSE;
}

void popup_menu_show(GtkTreeView *treeview, GdkEventButton *event)
{
    if (event == NULL)
        return;

    if (event->type == GDK_2BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY)
    {
        if (menu)
        {
            gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent *)event);
        }
    }
}


void on_treeview_row_activated(GtkTreeView *treeview,
    GtkTreePath *path,
    GtkTreeViewColumn *column,
    gpointer user_data)
{
GdkEventButton *event = (GdkEventButton *)user_data;
popup_menu_show(treeview, event);
}




void on_book_edit(GtkMenuItem *item, gpointer user_data)
{
    // g_print(">> Sửa được chọn!\n");
}

void on_book_delete(GtkMenuItem *item, gpointer user_data)
{
    // g_print(">> Xoá được chọn!\n");
}
