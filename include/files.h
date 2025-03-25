#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>

// Tạo đường dẫn đầy đủ cho file
char *get_file_path(const char *folder_path, const char *file_name);

// Kiểm tra sự tồn tại của thư mục
bool is_exist_folder(const char *path);

// Kiểm tra sự tồn tại của file trong thư mục
bool is_exist_file(const char *folder_path, const char *file_name);

// Xóa file trong thư mục
bool delete_file(const char *folder_path, const char *file_name);

// Tạo thư mục mới
bool create_folder(const char *path);

// Sao chép nội dung file từ nguồn sang đích
bool copy_file(const char *src_path, const char *dest_path);

// Đọc nội dung file thành chuỗi
char *read_file(const char *folder_path, const char *file_name);

// Ghi nội dung vào file (ghi đè)
bool write_file(const char *folder_path, const char *file_name, const char *content);

// Liệt kê danh sách file trong thư mục
char **list_files(const char *folder_path, int *file_count);

#endif
