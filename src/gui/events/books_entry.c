#include "gui/events/books_entry.h"
#include "gui/events/callbacks.h"
#include "gui/events/book_treeview.h"
#include "gui/gui.h"
#include "gui/ui_loader.h"
#include "books.h"

void on_book_button_confirm_clicked(GtkButton *button, gpointer user_data)
{
    printf("Trang thai hoat dong: %d\n", chosen_action);

    Book book = get_book_input();
    if (chosen_action == BOOK_ADD_STATUS)
    {
        /// Tiến hành lấy lại Id hợp lí
        book.bookId = total_books + 1;
        total_books++;
        add_book(&book);
        save_book_management();
    }
    else if (chosen_action == BOOK_UPDATE_STATUS)
    {
        int status = update_book_from_object(&book);
        if (status == UPDATE_SUCCESS)
        {
            printf("Update book successfully!\n");
            save_book_management();
            load_book_to_layout(current_book_page);
        }
        else
        {
            printf("Update failed !\n");
        }
    }

    clear_book_to_entry();

    chosen_action = BOOK_NO_ACTION;
    chosen_id = -1;

    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "book_entry")));
}