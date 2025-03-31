#include <gtk/gtk.h>

// Hàm xử lý sự kiện khi nhấn vào button
G_MODULE_EXPORT void on_button_clicked(GtkWidget *widget, gpointer user_data) {
    GtkNotebook *notebook = GTK_NOTEBOOK(user_data);

    if (!notebook) {
        g_print("LỖI: Notebook NULL!\n");
        return;
    }

    // Lấy ID của button từ Glade
    const gchar *button_name = gtk_buildable_get_name(GTK_BUILDABLE(widget));
    g_print("Button clicked: %s\n", button_name);

    // Kiểm tra button nào được bấm và chuyển tab
    if (g_strcmp0(button_name, "home-button") == 0) {
        gtk_notebook_set_current_page(notebook, 0);
    } else if (g_strcmp0(button_name, "manage-book") == 0) {
        gtk_notebook_set_current_page(notebook, 1);
    } else if (g_strcmp0(button_name, "manage-reader") == 0) {
        gtk_notebook_set_current_page(notebook, 2);
    } else if (g_strcmp0(button_name, "borrow-return") == 0) {
        gtk_notebook_set_current_page(notebook, 3);
    } else if (g_strcmp0(button_name, "summary") == 0) {
        gtk_notebook_set_current_page(notebook, 4);
    } else if (g_strcmp0(button_name, "manual") == 0) {
        gtk_notebook_set_current_page(notebook, 5);
    } else {
        g_print("Không tìm thấy tab tương ứng!\n");
    }
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkNotebook *notebook;

    gtk_init(&argc, &argv);

    // Tải UI từ Glade
    builder = gtk_builder_new_from_file("UI.glade");
    if (!builder) {
        g_print("LỖI: Không thể load UI.glade\n");
        return 1;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    notebook = GTK_NOTEBOOK(gtk_builder_get_object(builder, "visual"));

    if (!window) {
        g_print("LỖI: Không tìm thấy Window trong Glade!\n");
        return 1;
    }
    if (!notebook) {
        g_print("LỖI: Không tìm thấy Notebook 'visual' trong Glade!\n");
        return 1;
    }

    // Danh sách ID của các button
    const char *buttons[] = {
        "home-button", "manage-book", "manage-reader", 
        "borrow-return", "summary", "manual"
    };

    // Kết nối các button với sự kiện click
    for (int i = 0; i < 6; i++) {
        GtkWidget *btn = GTK_WIDGET(gtk_builder_get_object(builder, buttons[i]));
        if (btn) {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_button_clicked), notebook);
        } else {
            g_print("LỖI: Không tìm thấy button '%s' trong Glade!\n", buttons[i]);
        }
    }

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
