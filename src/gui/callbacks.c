#include <gtk/gtk.h>
#include "callbacks.h"
#include "books.h"
#include "files.h"
#include "book_manager.h"
// Xử lý khi nhấn "Mượn sách"
void on_borrow_button_clicked(GtkButton *button, gpointer user_data)
{
    // Lấy dữ liệu từ ô nhập tên sách
    GtkWidget *entry = GTK_WIDGET(user_data);
    const gchar *book_name = gtk_entry_get_text(GTK_ENTRY(entry));

    if (book_name == NULL || g_strcmp0(book_name, "") == 0)
    {
        g_print("Vui lòng nhập tên sách.\n");
        return;
    }

    // Gọi hàm mượn sách từ books.c
    if (borrow_book(book_name))
    {
        g_print("Đã mượn sách: %s\n", book_name);
    }
    else
    {
        g_print("Mượn sách thất bại.\n");
    }
}

// Xử lý khi nhấn "Trả sách"
void on_return_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *entry = GTK_WIDGET(user_data);
    const gchar *book_name = gtk_entry_get_text(GTK_ENTRY(entry));

    if (book_name == NULL || g_strcmp0(book_name, "") == 0)
    {
        g_print("Vui lòng nhập tên sách.\n");
        return;
    }

    if (return_book(book_name))
    {
        g_print("Đã trả sách: %s\n", book_name);
    }
    else
    {
        g_print("Trả sách thất bại.\n");
    }
}
