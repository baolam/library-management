#include "gui/ui_loader.h"

GtkBuilder *load_ui(const char *filename)
{
    GtkBuilder *builder = gtk_builder_new_from_file(filename);
    if (!builder)
    {
        g_print("Không thể load UI!\n");
    }
    return builder;
}
