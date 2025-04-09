#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delete.h"

void deleteBook(int bookId) {
    FILE *file = fopen("book_infor.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!file || !temp) {
        perror("Không mở được file");
        return;
    }

    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file)) {
        int currentId;
        char lineCopy[1024];
        strcpy(lineCopy, buffer);

        char *token = strtok(lineCopy, ",");
        if (token != NULL)
            currentId = atoi(token);
        else
            continue;

        if (currentId == bookId) {
            // Ghi lại dòng này với trạng thái xóa (0)
            char *newline = strrchr(buffer, '\n');
            if (newline) *newline = '\0';  // loại bỏ '\n'
            
            fprintf(temp, "%s,0\n", buffer);  // cập nhật trạng thái xóa
        } else {
            fputs(buffer, temp);  // giữ nguyên dòng
        }
    }

    fclose(file);
    fclose(temp);

    remove("book_infor.txt");
    rename("temp.txt", "book_infor.txt");
}
