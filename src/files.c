#include "../include/files.h"
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

char *get_file_path(const char *folder_path, const char *file_name)
{
    if (!folder_path || !file_name)
        return NULL;

    size_t path_len = strlen(folder_path) + strlen(file_name) + 2;
    char *path = (char *)malloc(path_len);
    if (!path)
        return NULL;

    snprintf(path, path_len, "%s/%s", folder_path, file_name);
    return path;
}

bool is_exist_folder(const char *path)
{
    struct stat stats;
    return (stat(path, &stats) == 0) && S_ISDIR(stats.st_mode);
}

bool is_exist_file(const char *folder_path, const char *file_name)
{
    char *file_path = get_file_path(folder_path, file_name);
    if (!file_path)
        return false;

    struct stat stats;
    bool exists = (stat(file_path, &stats) == 0) && S_ISREG(stats.st_mode);

    free(file_path);
    return exists;
}

bool delete_file(const char *folder_path, const char *file_name)
{
    char *file_path = get_file_path(folder_path, file_name);
    if (!file_path)
        return false;

    int result = remove(file_path);
    free(file_path);
    return result == 0;
}

bool create_folder(const char *path)
{
    if (!path)
        return false;

#ifdef _WIN32
    return mkdir(path) == 0;
#else
    return mkdir(path, 0755) == 0;
#endif
}

bool copy_file(const char *src_path, const char *dest_path)
{
    if (!src_path || !dest_path)
        return false;

    FILE *src = fopen(src_path, "rb");
    FILE *dest = fopen(dest_path, "wb");
    if (!src || !dest)
    {
        if (src)
            fclose(src);
        if (dest)
            fclose(dest);
        return false;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0)
    {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);
    return true;
}

char *read_file(const char *folder_path, const char *file_name)
{
    char *file_path = get_file_path(folder_path, file_name);
    if (!file_path)
        return NULL;

    FILE *file = fopen(file_path, "r");
    free(file_path);
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *content = (char *)malloc(file_size + 1);
    if (!content)
    {
        fclose(file);
        return NULL;
    }

    fread(content, 1, file_size, file);
    content[file_size] = '\0';

    fclose(file);
    return content;
}

bool write_file(const char *folder_path, const char *file_name, const char *content)
{
    if (!folder_path || !file_name || !content)
        return false;

    char *file_path = get_file_path(folder_path, file_name);
    if (!file_path)
        return false;

    FILE *file = fopen(file_path, "w");
    free(file_path);
    if (!file)
        return false;

    fprintf(file, "%s", content);
    fclose(file);
    return true;
}

char **list_files(const char *folder_path, int *file_count)
{
    if (!folder_path || !file_count)
        return NULL;

    DIR *dir = opendir(folder_path);
    if (!dir)
    {
        *file_count = 0;
        return NULL;
    }

    struct dirent *entry;
    int count = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        struct stat path_stat;
        char *full_path = get_file_path(folder_path, entry->d_name);
        if (full_path && stat(full_path, &path_stat) == 0 && S_ISREG(path_stat.st_mode))
        {
            count++;
        }
        free(full_path);
    }

    rewinddir(dir);

    char **files = (char **)malloc(count * sizeof(char *));
    if (!files)
    {
        closedir(dir);
        *file_count = 0;
        return NULL;
    }

    int index = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        char *full_path = get_file_path(folder_path, entry->d_name);
        struct stat path_stat;
        if (full_path && stat(full_path, &path_stat) == 0 && S_ISREG(path_stat.st_mode))
        {
            files[index] = strdup(entry->d_name);
            if (!files[index])
            {
                for (int i = 0; i < index; i++)
                    free(files[i]);
                free(files);
                closedir(dir);
                *file_count = 0;
                return NULL;
            }
            index++;
        }
        free(full_path);
    }

    closedir(dir);
    *file_count = count;
    return files;
}