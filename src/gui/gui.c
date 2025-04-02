#include "gui.h"
#include "callbacks.h"

void init_gui(int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *window;
    GtkNotebook *notebook;

    gtk_init(&argc, &argv);

<<<<<<< HEAD
    /// Thêm từng file builder
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ui/GUI/UI.glade", NULL);
    // gtk_builder_add_from_file(builder, "ui/search-book.ui", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
=======
    // Tải UI từ Glade
    builder = gtk_builder_new_from_file("ui/UI.glade");
    if (!builder)
    {
        g_print("LỖI: Không thể load UI.glade\n");
        return;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    notebook = GTK_NOTEBOOK(gtk_builder_get_object(builder, "visual"));
>>>>>>> 98c9b0f09a3372c5ccf277eb73001f7afb6bb0c6

    if (!window)
    {
        g_print("LỖI: Không tìm thấy Window trong Glade!\n");
        return;
    }
    if (!notebook)
    {
        g_print("LỖI: Không tìm thấy Notebook 'visual' trong Glade!\n");
        return;
    }

    // Danh sách ID của các button
    const char *buttons[] = {
        "home-button", "manage-book", "manage-reader",
        "borrow-return", "summary", "manual"};

    // Kết nối các button với sự kiện click
    for (int i = 0; i < 6; i++)
    {
        GtkWidget *btn = GTK_WIDGET(gtk_builder_get_object(builder, buttons[i]));
        if (btn)
        {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_button_clicked), notebook);
        }
        else
        {
            g_print("LỖI: Không tìm thấy button '%s' trong Glade!\n", buttons[i]);
        }
    }

    gtk_widget_show_all(window);
    gtk_main();
}
