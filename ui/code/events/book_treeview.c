#include "book_treeview.h"
#include <stdio.h>
#include <string.h>

void load_books_from_file(GtkBuilder *builder, const char *filename) {
    GtkListStore *store = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
    if (!store) {
        g_warning("Không tìm thấy liststore1 trong Glade");
        return;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        g_warning("Không thể mở file: %s", filename);
        return;
    }

    char line[512];
    int line_count = 0;
    while (fgets(line, sizeof(line), file) && line_count < 15) {
        char *token;
        char *fields[7]; // ID, title, author, genre, year, quantity
        int i = 0;

        // Tách các phần bằng '|'
        token = strtok(line, "|");
        while (token && i < 7) {
            fields[i++] = g_strstrip(token);
            token = strtok(NULL, "|");
        }

        // Thêm dòng vào ListStore nếu đủ dữ liệu
        if (i >= 6) {
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                0, fields[0],
                1, fields[1],
                2, fields[2],
                3, fields[3],
                4, fields[4],
                5, fields[6], // Quantity là cột cuối (index 6 do có cả năm)
                -1);
        }

        line_count++;
    }

    fclose(file);
}