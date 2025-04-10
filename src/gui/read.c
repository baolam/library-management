#include "read.h"

void readBookByLine(int lineNumber) {
    FILE *f = fopen("book_infor.txt", "r");
    if (!f) {
        perror("Không mở được file");
        return;
    }

    char buffer[512];
    int current = 1;

    while (fgets(buffer, sizeof(buffer), f)) {
        if (current == lineNumber) {
            char *token = strtok(buffer, ",");
            int field = 0;
            while (token != NULL) {
                switch (field) {
                    case 0: book.bookId = atoi(token); break;
                    case 1: strncpy(book.title, token, MAX_TITLE); break;
                    case 2: strncpy(book.author, token, MAX_AUTHOR); break;
                    case 3: book.publicationYear = atoi(token); break;
                    case 4: book.stock = atoi(token); break;
                    default:
                        if (field - 5 < MAX_GENRE_NO)
                            strncpy(book.genre[field - 5], token, MAX_GENRE_LENGTH);
                        break;
                }
                token = strtok(NULL, ",");
                field++;
            }
            break;
        }
        current++;
    }

    fclose(f);
}