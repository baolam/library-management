#include "gui.h"
#include "create.h"

Book createBook(GtkWidget *entry_title, GtkWidget *entry_author, GtkWidget *entry_id,
                GtkWidget *entry_genre, GtkWidget *entry_stock, GtkWidget *entry_pcy){
    Book book;

    strncpy(book.title, gtk_entry_get_text(GTK_ENTRY(entry_title)), MAX_TITLE - 1);
    book.title[MAX_TITLE - 1] = '\0';

    strncpy(book.author, gtk_entry_get_text(GTK_ENTRY(entry_author)), MAX_AUTHOR - 1);
    book.author[MAX_AUTHOR - 1] = '\0';

    book.bookId = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
    
    const char* input = gtk_entry_get_text(GTK_ENTRY(entry_genre));
    for(int num = 0; num < MAX_GENRE_NO; num++){
        int len;
        const char *genre = strchr(input, ',');

        if(genre != NULL){
            len = genre - input;
        }else{
            len = strlen(input);
        }

        strncpy(book.genre[num], input, len);
        book.genre[num][len] = '\0';

        if(genre != NULL){
            input = genre + 1;
        }else{
            for (int i = num + 1; i < MAX_GENRE_NO; i++) {
                book.genre[i][0] = '\0';
            }
            break;
        }
    }

    book.publicationYear = atoi(gtk_entry_get_text(GTK_ENTRY(entry_pcy)));

    book.stock = atoi(gtk_entry_get_text(GTK_ENTRY(entry_stock)));

    return book;
}