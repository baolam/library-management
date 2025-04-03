#include "management.h"

void readContent(const char *filename, FileInfor infor, void (*callback)(FILE *f, long length))
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fseek(file, infor.offset, SEEK_SET);
    callback(file, infor.length);
    fclose(file);
}

void writeContent(const char *filename, FileInfor infor, char *content)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fseek(file, infor.offset, SEEK_SET);
    fwrite(content, 1, infor.length, file);
    fclose(file);
}