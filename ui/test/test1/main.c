#include <gtk/gtk.h>

void switch_to(GtkButton *button, gpointer user_data)
{
    const char *page_name = (const char *)g_object_get_data(G_OBJECT(button), "page");
    GtkStack *stack = GTK_STACK(user_data);
    gtk_stack_set_visible_child_name(stack, page_name);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new_from_file("UI.glade");

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "main_stack"));

    GtkWidget *btn_home = GTK_WIDGET(gtk_builder_get_object(builder, "btn_home"));
    GtkWidget *btn_books = GTK_WIDGET(gtk_builder_get_object(builder, "btn_books"));
    GtkWidget *btn_settings = GTK_WIDGET(gtk_builder_get_object(builder, "btn_settings"));

    // Gán dữ liệu để biết chuyển sang đâu
    g_object_set_data(G_OBJECT(btn_home), "page", "home");
    g_object_set_data(G_OBJECT(btn_books), "page", "books");
    g_object_set_data(G_OBJECT(btn_settings), "page", "settings");

    g_signal_connect(btn_home, "clicked", G_CALLBACK(switch_to), stack);
    g_signal_connect(btn_books, "clicked", G_CALLBACK(switch_to), stack);
    g_signal_connect(btn_settings, "clicked", G_CALLBACK(switch_to), stack);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
