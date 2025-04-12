/**
 * @file management.h
 * @brief Những hàm tiện ích dùng để khai thác hiệu quả B+ Tree
 * @author Nguyễn Đức Bảo Lâm
 * @version 1.0
 *
 * Những hàm được định nghĩa trong file này sẽ tăng hiệu suất cho việc
 * đọc/ghi file, tìm kiếm, thêm/xoá dữ liệu. Đây là file ở mức trừu tượng cao hơn
 * so với B+ tree và Trie.
 *
 * Tận dụng những tiện ích trong đây sẽ xoá bỏ những khó khăn thao tác do B+ tree mang
 * đến. Đồng thời kết nối giữa B+ Tree và quản lí file nội dung.
 */

#ifndef MANAGEMENT_H
#define MANAGEMENT_H

/**
 * @brief Mã trạng thái xoá thành công
 */
#define DELETE_SUCCESS 2

/**
 * @brief Mã trạng thái xoá đã tồn tại
 * @note thuộc tính deleted của record được set thành true thì mã này sẽ được kích hoạt
 */
#define DELETE_EXISTED 1

/**
 * @brief Mã trạng thái xoá thất bại
 */
#define DELETE_FAILED 0

/**
 * @brief Mã trạng thái thêm thất bại
 */
#define ADD_CONTENT_FAILED 0

/**
 * @brief Mã trạng thái thêm thành công
 */
#define ADD_CONTENT_SUCCESS 1

#include <stdbool.h>
#include "utils/bplustreev2.h"

/**
 * @brief Hàm trả về vị trí bắt đầu tìm kiếm trong node lá
 *
 * Gọi hàm này để phục vụ tiện ích cho nhóm thao tác tìm kiếm
 * dữ liệu liên tục
 *
 * @param n Node lá
 * @param beginingKey Key bắt đầu tìm kiếm
 *
 * @return Vị trí bắt đầu tìm kiếm
 */
int getStartSearch(Node *n, int beginingKey);

/**
 * @brief Hàm kiểm tra sự tồn tại của bản ghi ứng với khoá được chỉ định hay không
 *
 * Hàm trả về false ở một số trường hợp sau:
 * + Không tìm thấy bản ghi ứng với khoá
 * + Bản ghi ứng với khoá đã bị xoá
 *
 * @param root Node quản lí chính
 * @param key khoá tìm kiếm
 * @return true là có, false là không
 */
bool exist_record(Node *root, int key);

/**
 * @brief Hàm thêm dữ liệu vào file lưu trữ, đồng thời cập nhật quản lí B+ tree
 *
 * @param root Node quản lí chính
 * @param key khoá tìm kiếm
 * @param content_file file lưu trữ dữ liệu muốn thêm vào
 * @param content nội dung muốn lưu (dùng con trỏ vô kiểu ở đây để tránh định nghĩa hành vi trùng nhau)
 * @param size kích thước dữ liệu lưu trữ
 * @param callback hàm gọi thực thi thông báo để biết trạng thái thêm thành công hay thất bại
 */
Node *add_content(Node *root, int key, char content_file[MAX_FILE_NAME_LENGTH], void *content, size_t size, void (*callback)(int id, int code, long offset, long length));

/**
 * @brief Hàm đọc dữ liệu
 *
 * Hàm này sẽ tiến hành một số thao tác cần thiết để chuẩn bị đọc dữ liệu.
 * Cụ thể, khi gọi hàm này, logic thực thi sẽ giúp kiểm tra sự tồn tại của file, kiểm tra tính khả thi
 * của dữ liệu, đồng thời nhảy đến vị trí đọc để đọc dữ liệu
 *
 * @param root Node quản lí chính
 * @param key Khoá tìm kiếm
 * @param callback hàm gọi thao tác để đọc dữ liệu (dùng lệnh fread và dùng struct hứng dữ liệu)
 */
void read_content(Node *root, int key, void (*callback)(FILE *f, long package_size));

/**
 * @brief Hàm đọc dữ liệu, một mở rộng của read_content, do có cung cấp thêm Record
 *
 * Hàm này tận dụng những gì đã được thực hiện ở hàm read_content, chỉ tinh chỉnh
 * thêm ở bước nhận tham số đầu vào và chỉ kiểm tra tính khả thi của việc đọc thui
 *
 * @param record bản ghi dữ liệu
 * @param callback hàm thao tác đọc dữ liệu
 */
void read_content_from_record(Record *record, void (*callback)(FILE *f, long package_size));

/**
 * @brief Hàm đọc dữ liệu có trả về
 *
 * Hàm này được dùng cho trường hợp gắn kết giao diện, khi
 * nhóm thao tác callback không được hiệu quả hoặc xử lí tương đối
 * phức tạp.
 *
 * Nguyên lí hoạt động tương tự như hàm read_content_from_record
 *
 * @param record bản ghi dữ liệu
 * @return kết quả đọc dữ liệu, con trỏ vô kiểu, cần ép kiểu để sử dụng
 */
void *read_content_from_record_return(Record *record);

/**
 * @brief Hàm đọc liên tiếp một nhóm bản ghi từ khoá chỉ định tìm kiếm ban đầu
 *
 * Đây là một lợi thế mà B+ Tree mang lại. Do dữ liệu được lưu trữ theo dạng tương tự linked list
 * ở những node lá, nên hàm này có thể được tận dụng để tăng tốc độ hiệu quả.
 *
 * @param root Node quản lí chính
 * @param beginingKey khoá tìm kiếm ban đầu
 * @param nums số lượng dữ liệu muốn đọc
 * @param callback hàm gọi thao tác đọc thêm, tương tự như các phương thức đọc trên
 */
void read_bucket_content(Node *root, int beginingKey, int nums, void (*callback)(FILE *f, long package_size));

/**
 * @brief Hàm cập nhật dữ liệu.
 *
 * Nếu chỉ với việc đọc file txt thông thường. Thao tác sẽ tương đối
 * khó khăn. Nhưng do cách quy về dạng file nhị phân và cách lưu trữ
 * nên việc cập nhật dữ liệu sẽ trở nên dễ dàng hơn (thông qua thao tác fwrite).
 *
 * @param root Node quản lí chính
 * @param key khoá tìm kiếm
 * @param callback hàm gọi lấy dữ liệu, định nghĩa dữ liệu và viết dữ liệu
 */
void update_content(Node *root, int key, void (*callback)(FILE *f, long package_size));

/**
 * @brief Hàm cập nhật dữ liệu từ bản ghi
 *
 * Là một mở rộng của update_content.
 *
 * @param record bản ghi dữ liệu
 * @param callback nhiệm vụ tương tự callback trên
 */
void update_content_from_record(Record *record, void (*callback)(FILE *f, long package_size));

/**
 * @brief Hàm xóa dữ liệu
 *
 * Bản chất chỉ là xoá quản lí. Dữ liệu vẫn còn tồn tại ở file quản lí
 *
 * @param root Node quản lí chính
 * @param key Khoá quản lí chính
 * @param callback hàm gọi trạng thái của thao tác xoá dữ liệu
 */
void soft_delete(Node *root, int key, void (*callback)(int code));

#endif