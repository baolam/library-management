#include "choose_options.h"

GtkWidget *menu = NULL;

void popup_menu_init(GtkBuilder *builder) {
    menu = GTK_WIDGET(gtk_builder_get_object(builder, "row_menu"));
}

void popup_menu_show(GtkTreeView *treeview) {
    if (menu) {
        gtk_menu_popup_at_widget(GTK_MENU(menu),
                                 GTK_WIDGET(treeview),
                                 GDK_GRAVITY_CENTER,
                                 GDK_GRAVITY_CENTER,
                                 NULL);
    }
}

// Callback gắn trong Glade
void on_treeview_row_activated(GtkTreeView *treeview,
                                GtkTreePath *path,
                                GtkTreeViewColumn *column,
                                gpointer user_data) {
    popup_menu_show(treeview);
}

void on_book_add(GtkMenuItem *item, gpointer user_data) {
    g_print(">> Thêm được chọn!\n");
}

void on_book_edit(GtkMenuItem *item, gpointer user_data) {
    g_print(">> Sửa được chọn!\n");
}

void on_book_delete(GtkMenuItem *item, gpointer user_data) {
    g_print(">> Xoá được chọn!\n");
}
