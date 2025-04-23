#include "gui/events/readers_entry.h"

Readers get_reader_input()
{
    Readers r;
    Readers invalid_reader = {0};

    gchar *id = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_id")));
    gchar *fullName = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_fullname")));
    gchar *phoneNumber = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_phonenumber")));
    gchar *address = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_address")));

    if (!is_name_valid(fullName)) {
        show_warning();
        return invalid_reader;
    } else {
    strncpy(r.fullName, fullName, MAX_FULLNAME - 1);
    strncpy(r.phoneNumber, phoneNumber, MAX_PHONENUMBER - 1);
    strncpy(r.address, address, MAX_ADDRESS - 1);

    r.fullName[MAX_FULLNAME - 1] = '\0';
    r.phoneNumber[MAX_PHONENUMBER - 1] = '\0';
    r.address[MAX_ADDRESS - 1] = '\0';

    r.readerId = atoi(id);

    return r;
    }
}

void show_reader_to_entry(Readers r)
{
    gchar *readerId = g_strdup_printf("%d", r.readerId);

    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_id")), readerId);
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_fullname")), r.fullName);
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_phonenumber")), r.phoneNumber);
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_address")), r.address);

    g_free(readerId);
}

void clear_reader_to_entry()
{
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_id")), "");
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_fullname")), "");
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_phonenumber")), "");
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_reader_address")), "");
}

void handle_reader_button_confirmed()
{
    Readers r = get_reader_input();
    switch (reader_chosen_action)
    {
    case READER_ADD_STATUS:
    {
        r.readerId = total_readers + 1;
        total_readers++;
        add_reader(&r);
        save_reader_management();
        break;
    }
    case READER_UPDATE_STATUS:
    {
        int status = update_reader_from_object(&r);
        if (status == UPDATE_SUCCESS)
        {
            printf("Update reader successfully !\n");
            save_reader_management();
            load_reader_to_layout(current_reader_page);
        }
        break;
    }
    default:
        break;
    }
}

void on_reader_button_confirm_clicked(GtkButton *button, GtkBuilder *_builder)
{
    printf("Trang thai hoat dong: %d\n", reader_chosen_action);
    handle_reader_button_confirmed();

    clear_reader_to_entry();

    reader_chosen_action = READER_NO_ACTION;
    reader_chosen_id = -1;

    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "reader_entry")));
}