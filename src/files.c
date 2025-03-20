#include "files.h"

char *get_file_path(const char *folder_path, const char *file_name)
{
    char *temp = (char *)malloc(strlen(folder_path) + strlen(file_name) + 2);
    sprintf(temp, "%s/%s", folder_path, file_name);
    return temp;
}

bool is_exist_folder(const char *path)
{
    DIR *dir = opendir(path);
    bool state = false;
    if (dir)
    {
        state = true;
    }
    closedir(dir);
    return state;
}

bool is_exist_file(const char *folder_path, const char *file_name)
{
    FILE *f = fopen(get_file_path(folder_path, file_name), "r");
    bool state = true;
    if (f == NULL)
    {
        state = false;
    }
    fclose(f);
    return state;
}

bool delete_file(const char *folder_path, const char *file_name)
{
    if (!is_exist_file(folder_path, file_name))
        return false;
    if (remove(get_file_path(folder_path, file_name)) == 0)
    {
        return true;
    }
    return false;
}