#include "gui.h"
#include "save.h"
#include "create.h"

void save_button_clicked(GtkButton *button, gpointer user_data){
    GtkBuilder *builder = GTK_BUILDER(user_data);

    GtkWidget *entry_title = GTK_WIDGET(gtk_builder_get_object(builder, "entry_title"));

    GtkWidget *entry_author = GTK_WIDGET(gtk_builder_get_object(builder, "entry_author"));

    GtkWidget *entry_id = GTK_WIDGET(gtk_builder_get_object(builder, "entry_id"));

    GtkWidget *entry_genre = GTK_WIDGET(gtk_builder_get_object(builder, "entry_genre"));

    GtkWidget *entry_pcy = GTK_WIDGET(gtk_builder_get_object(builder, "entry_pcy"));

    GtkWidget *entry_stock = GTK_WIDGET(gtk_builder_get_object(builder, "entry_stock"));

    Book book = createBook(entry_title, entry_author, entry_id,
                           entry_genre, entry_stock, entry_pcy);

    FILE *f = fopen("book_infor.txt", "a");

    fprintf(f, "%d, %s, %s, %d, %d", book.bookId, book.title,
             book.author, book.publicationYear, book.stock);
    for(int num = 0; num < MAX_GENRE_NO; num++){
        fprintf(f, ", %s", book.genre[num]);
    }
    fprintf(f, "\n");

    fclose(f);
}