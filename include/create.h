#ifndef CREATE_H
#define CREATE_H

#include "books.h"
#include <string.h>
#include <stdlib.h>

Book createBook(GtkWidget entry_title, GtkWidget entry_author, GtkWidget entry_id, 
                GtkWidget entry_genre, GtkWidget entry_stock, GtkWidget entry_pcy);


#endif