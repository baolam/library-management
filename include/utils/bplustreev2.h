/**
 * @file bplustreev2.h
 * @brief Những cài đặt cần thiết để khai thác cây B+ tree
 * @author Nguyễn Đức Bảo Lâm
 * @note Nội dung file có dựa trên https://www.programiz.com/dsa/b-plus-tree, chân thành cảm ơn!
 * @version 1.0
 *
 * File này chứa những định nghĩa, khai báo cần thiết để có thể ứng dụng được B+ Tree vào
 * quản lí thư viện
 */

#ifndef BPLUSTREEV2_H
#define BPLUSTREEV2_H

/**
 * @brief Định nghĩa có cho phép In cây ra hay không, màn hình console
 */
#define PRINT_TREE

/**
 * @brief Số dữ liệu quản lí tối đa của một node
 */
#define ORDER 10

/**
 * @brief Độ dài tối đa của tên file
 */
#define MAX_FILE_NAME_LENGTH 128

#ifdef PRINT_TREE
#include <stdio.h>
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * @struct Record
 * @brief Bản ghi dữ liệu lưu trữ
 *
 * Node không lưu trữ trực tiếp dữ liệu mà chỉ lưu trữ thông tin quản lí
 * trong file lưu trữ
 */
typedef struct Record
{
    /**
     * @brief file nội dung lưu trữ
     */
    char _from[MAX_FILE_NAME_LENGTH];

    /**
     * @brief Vị trí bắt đầu lưu trữ
     */
    long offset;

    /**
     * @brief Độ dài dữ liệu lưu trữ
     */
    long length;

    /**
     * @brief Trạng thái xoá hay chưa (xoá quản lí, cho phương thức soft-delete)
     */
    bool deleted;
} Record;

/**
 * @struct Node
 * @brief Node trong cây B+ tree
 *
 * Một node sẽ lưu trữ thông tin quản lí của nhiều bản ghi nếu là node lá
 * Hoặc quản lí nhiều node con khác.
 *
 * Số lượng quản lí tối đa phụ thuộc vào ORDER được định nghĩa
 */
typedef struct Node
{
    /**
     * @brief dùng để lưu trữ dữ liệu (Record), hoặc các node con
     */
    void **pointers;

    /**
     * @brief lưu trữ các khóa, định hướng quá trình tìm kiếm
     */
    int *keys;

    /**
     * @brief Node tổ tiên, kết nối dùng để quản lí
     */
    struct Node *parent;

    /**
     * @brief Là node lá hay không, chỉ thị trạng thái dùng cho liên kết
     */
    bool is_leaf;

    /**
     * @brief Số lượng khoá
     */
    int num_keys;

    /**
     * @brief Trỏ tiếp theo (phục vụ enqueue, dequeue)
     */
    struct Node *next;
} Node;

/**
 * @brief Khởi tạo record
 *
 * Tạo dựng record mới từ
 * @param _from bản ghi nguồn
 * @param offset vị trí bắt đầu
 * @param length kích thước dữ liệu
 */
Record *makeRecord(char _from[MAX_FILE_NAME_LENGTH], long offset, long length);

/**
 * @brief hàm trả về node lá tận cùng bên trái
 *
 * Hàm này được dùng cho nhóm tác vụ liên quan đến thống kê, báo cáo
 * Yêu cầu duyệt toàn bộ nội dung file, số sách đã lưu trữ, ...
 *
 * @param root node gốc cần quản lí
 */
Node *leftMost(Node *root);

/**
 * @brief Khởi tạo một node
 */
Node *makeNode(void);

/**
 * @brief Khởi tạo một node lá
 *
 * Bản chất là một tinh chỉnh nhỏ của makeNode, thông qua thay đổi thuộc tính is_leaf
 */
Node *makeLeaf(void);

/// Phương thức tìm kiếm

/**
 * @brief Tìm kiếm node lá tương ứng với Key
 *
 * Dùng phương pháp duyệt cây tương tự như trong cây tìm kiếm nhị phân
 * @param root node gốc
 * @param key khoá tìm kiếm
 */
Node *findLeaf(Node *root, int key);

/**
 * @brief Tìm kiếm bản ghi từ Key
 *
 * Bản chất là một kế thừa mở rộng từ findLeaf
 * @param root node gốc
 * @param key khoá tìm kiếm
 */
Record *find(Node *root, int key);

/// Phương thức bổ trợ

/**
 * @brief Hàm hỗ trợ cho việc chia tách node, dành cho trường hợp yêu cầu chia tách
 *
 * Cách chia tách sẽ khác nhau tuỳ theo đầu vào là chẵn hay lẻ
 * @param length số bậc (dựa trên order)
 */
int cut(int length);

/**
 * @brief Hàm hỗ trợ cho việc trả về vị trí bắt đầu chỉ số bên trái đầu tiên của node
 * được chỉ định
 *
 * @param parent node cha
 * @param left node left, node bên trái
 *
 * @return vị trí bắt đầu chỉ số bên trái đầu tiên
 */
int getLeftIndex(Node *parent, Node *left);

#ifdef PRINT_TREE
extern Node *queue;

int pathToLeaves(Node *const root, Node *child);

void enqueue(Node *new_node);
Node *dequeue(void);

void printTree(Node *const root);
#endif

/// Phương thức thêm phần tử

/// @brief Khởi tạo một cây mới (bản chất là Node root) khi chưa tồn tại
/// @param key (khoá tìm kiếm)
/// @param pointer (con trỏ trỏ đến dữ liệu ghi)
/// @return Node gốc
Node *startNewTree(int key, Record *pointer);

Node *insertIntoLeaf(Node *leaf, int key, Record *record);
Node *insertIntoNode(Node *root, Node *n, int left_index, int key, Node *right);
Node *insertIntoNodeAfterSplitting(Node *root, Node *old_node, int left_index, int key, Node *right);
Node *insertIntoNewRoot(Node *left, int key, Node *right);
Node *insertIntoParent(Node *root, Node *left, int key, Node *right);
Node *insertIntoLeafAfterSplitting(Node *root, Node *leaf, int key, Record *record);

/**
 * @brief Hàm hỗ trợ cho việc thêm phần tử vào cây
 *
 * Thêm phần tử vào cây sẽ thêm nội dung vào node lá, và các node không là node lá sẽ chứa dữ liệu
 * định hướng tìm kiếm.
 *
 * @param root node gốc ban đầu
 * @param key khoá tìm kiếm
 * @param _from file nội dung lưu trữ
 * @param offset vị trí bắt đầu lưu trữ trong file
 * @param length kích thước dữ liệu
 *
 * @return cây mới sau khi thêm
 */
Node *insert(Node *root, int key, char _from[MAX_FILE_NAME_LENGTH], long offset, long length);

/// Phương thức lưu trữ

/**
 * @brief Hàm hỗ trợ lưu trữ dữ liệu vào file
 *
 * File được lưu dưới dạng đuôi .bin
 * Dữ liệu được thành một mảng byte liên tiếp
 *
 * @param root tượng trưng cho cây muốn lưu trữ
 * @param filename tên file muốn lưu trữ
 */
void saveTree(Node *root, char *filename);

/**
 * @brief Hàm hỗ trợ lấy dữ liệu ra từ cây
 *
 * Việc tải nội dung được tiến hành tương ứng thông qua hình
 * thức lưu trữ
 *
 * @param filename file đã lưu trữ trước đó
 */
Node *loadTree(char *filename);

#endif