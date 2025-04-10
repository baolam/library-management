#include "gui.h"
#include <books.h>

/// QUản lí
/// CRUD
/// THêm dữ liệu vào file, đọc dữ liệu, cập nhật, xoá

/// VIết search

//// Giả sử có ông làm về sách
/// Data/book
/// --> book.bin (file lưu trữ dữ liệu chính)
/// --> management.bin (file quản lí dữ liệu)
/// hash function (chuyển string thành số)
/// string gần nhau --> thứ tự đc đảm bảo

/// QUản lí 10000000
/// Dữ liệu được lưu trữ trong file (một chuỗi tuần tự, file .bin) --> đọc, viết, xử lí, cập nhật --> dễ thở hoen
/// .txt (,,,,|,,,|) --> struct Book, fread(&book), fwrite()
/// Làm sao biết đc vị trí lưu trữ () --> vị trí bắt đầu, lượng dữ liệu lưu trữ --> offset, length (package_size)
/// B+ Tree (lưu trữ dữ liệu), lưu trữ các vị trí cần quản lí, điểm dữ liệu đó đã xoá hay chưa
/// Tổ chức dữ liệu tuần tự (node lá), lá 1 --> lá 2 --> lá 3 --> ...
/// 10 cuốn sách đầu tiên ()
/// TÌm kiếm hoàn toàn tương tự
/// Xoá dữ liệu, thêm dữ liệu,

/// B+ Tree, key (id), dễ rùi, tên cuốn sách quản lí như thế nào
/// ---> chuyển đổi tên cuốn sách thành 1 con số (lưu trữ con số này) (hash function)

/// H (Hahhahha, Hehahh, Bbaggfkm, H) --> những cuốn sách có tên bắt đầu bằng chữ cái đó
/// Cây Trie (), H --> DFS (duyệt)

int main()
{
    // add_book_stochastic(100);
    load_book_management();
    search_book_by_id(5);
    return 0;
}