/**
 * @file trie.h
 * @brief Cấu trúc dữ liệu Trie, dùng cho lưu trữ và quản
 * lí các chuỗi kí tự
 * @author Nguyễn Đức Bảo Lâm
 * @version 1.0
 *
 * Trong dự án quản lí thư viện, để có thể hỗ trợ tìm kiếm cho khi nhập lần
 * lượt từng từ vào thì ta cần một cấu trúc dữ liệu có thể lưu các từ tìm kiếm
 * theo một dạng nào đó.
 *
 * Cụ thể hơn, do một chuỗi đầu vào được lưu thành từng kí tự kết nối với nhau.
 * Nên thao tác tìm kiếm là thao tác gom nhóm lại các từ theo cây được tổ chức.
 */

#ifndef TRIE_H
#define TRIE_H

/**
 * @brief Số lượng kí tự tối đa của một con, kí tự 26 tượng trưng cho khoảng trắng
 */
#define ALPHABET_SIZE 27

/**
 * @brief độ dài chuỗi tối đa
 */
#define MAX_CHAR_LENGTH 50

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/**
 * @struct TrieNode
 * @brief Một TrieNode dùng để lưu trữ dữ liệu
 *
 * Do việc tiến hành quản lí dữ liệu được cài đặt chính trên Id,
 * nên cần phải tích hợp thêm khoá Id vào trong cấu trúc dữ liệu này.
 *
 * Mặt khác do name có thể tồn tại trùng nhau nên Id ở đây là mảng.
 */
typedef struct TrieNode
{
    /**
     * @struct TrieNode
     * @brief Mảng các TrieNode con dùng để kết nối
     * đến các kí tự khác
     */
    struct TrieNode *children[ALPHABET_SIZE];

    /**
     * @brief Flag dùng để đánh dấu khi một TrieNode là node kết thúc
     */
    bool isEndOfWord;

    /**
     * @brief Biến lưu số lượng khoá tìm kiếm ứng với node kết thúc
     */
    int numIds;

    /**
     * @brief Biến lưu chi tiết các Ids
     */
    int *ids;
} TrieNode;

/// Hàm tiện ích

/**
 * @brief Hàm chuyển đổi một kí tự thành số
 *
 * Lưu ý đối với kí tự khoảng trống, ánh xạ mặc định ra số 27
 *
 * @param ch kí tự đầu vào
 * @return vị trí tương ứng trong mảng
 */
int charToIndex(char ch);
// void hashWord(mpz_t result, char *word);

/**
 * @brief Hàm trả về kết quả tìm kiếm một tiền tố
 *
 * @param root node gốc
 * @param prefix tiền tố bắt đầu
 *
 * @return nếu tồn tại trả về TrieNode kết thúc tại tiền tố đó, nếu ko thì NULL
 */
TrieNode *searchPrefix(TrieNode *root, char *prefix);

/**
 * @brief Hàm tìm kiếm từ
 *
 * Hàm này là một mở rộng của searchPrefix, coi prefix là toàn bộ
 * từ muốn tìm
 *
 * Sự tồn tại của hàm này là do trong bài toán tìm kiếm các từ có chứa
 * id.
 *
 * @param root node gốc
 * @param word chuỗi từ muốn tìm kiếm
 *
 * @return nếu tồn tại trả về TrieNode kết thúc tại từ đó, nếu ko thì NULL
 */
TrieNode *searchWord(TrieNode *root, char *word);

/**
 * @brief Hàm tạo ra TrieNode
 * @return node mới
 */
TrieNode *makeTrieNode(void);

/// Một số phương thức quản lí

/**
 * @brief Hàm kiểm tra sự tồn tại của Prefix
 *
 * Chuyển đổi kết quả của hàm searchPrefix
 * Dùng cho một số trường hợp chỉ yêu cầu sự tồn tại
 *
 * @param root node gốc
 * @param prefix tiền tố muốn tìm kiếm
 * @return true nếu tồn tại ngược lại thì false
 */
bool existPrefix(TrieNode *root, char *prefix);

/**
 * @brief Hàm kiểm tra sự tồn tại của từ
 *
 * Tương tự như hàm searchWord. Một khai thác của hàm searchWord
 *
 * @param root node gốc
 * @param word từ muốn tìm kiếm
 * @return true nếu tồn tại ngược lại thì false
 */
bool existWord(TrieNode *root, char *word);

/**
 * @brief Hàm thêm từ vào Trie
 *
 * Thêm từ vào trie là thêm luôn cả Id ứng với từ đó. Lưu ý khái niệm từ ở đây có thể mở
 * rộng ra cho một chuỗi dài gồm nhiều khoảng cách. "Ví dụ: The Science Book" có thể xem như từ
 *
 * @param root node gốc
 * @param words từ muốn thêm
 * @param id khoá ứng với từ đó
 */
void insertIntoTrie(TrieNode *root, char *words, int id);

/**
 * @brief Hàm xóa từ trong Trie
 *
 * Dùng trong trường hợp không muốn thông tin quản lí nữa
 *
 * @param root node gốc
 * @param word từ muốn xóa
 */
void removeWord(TrieNode *root, char *word);

/**
 * @brief Hàm đưa ra một chuỗi các từ bắt đầu = prefix
 *
 * Hàm này sẽ làm hàm hỗ trợ đưa ra gợi ý
 *
 * @param root node gốc
 * @param prefix tiền tố muốn tìm kiếm
 * @param maxRecommend số lượng gợi ý tối đa
 * @param recommend List danh sách gợi ý
 * @param recommendSize kích thước danh sách gợi ý thực tế
 */
void recommendPrefix(TrieNode *root, char *prefix, int maxRecommend, char **recommend, int *recommendSize);

/**
 * @brief Hàm hỗ trợ xoá bỏ Node, giải phóng
 *
 * @param root node gốc
 */
void freeTrie(TrieNode *root);

/**
 * @brief Hàm lưu trữ cây Trie vào file
 *
 * @param filename tên file muốn lưu trữ (đuôi .bin)
 * @param root node gốc
 */
void saveTrieTree(char *filename, TrieNode *root);

/**
 * @brief Hàm đọc file lưu trữ và đưa ra node gốc
 *
 * @param filename tên file muốn đọc
 * @return kết quả đọc được, NULL nếu có lỗi
 */
TrieNode *loadTrieTree(char *filename);

#endif