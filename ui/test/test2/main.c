#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkCssProvider *provider;
    GdkScreen *screen;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("ui.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    // Load CSS
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(screen,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
