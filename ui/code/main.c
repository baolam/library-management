#include "ui_loader.h"
#include "callbacks.h"
#include "popup_menu.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkBuilder *builder = load_ui("UI.glade");;
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));;

    //if (!builder) return 1;
    
    connect_signals(builder);

    popup_menu_init(builder);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
