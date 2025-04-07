#include "gui.h"
#include "bplustreev2.h"
#include "bookinfor.h"
#include "management.h"
#include "trie.h"

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

void readBook(FILE *f, long package_size)
{
    if (f == NULL || package_size == 0)
    {
        printf("Failed to read data!\n");
        return;
    }

    Book book1;
    fread(&book1, package_size, 1, f);

    printf("Book infor:\n");
    printf("Id : %s\n", book1.bookId);
    printf("Title : %s\n", book1.title);
    printf("Author : %s\n", book1.author);
    printf("Genre : %s\n", book1.genre);
    printf("Publication year : %d\n", book1.publicationYear);
    printf("Stock : %d\n", book1.stock);
    printf("\n");
}

void onAddBook(int id, int code, long offset, long length)
{
    printf("Id :%d, ", id);
    if (code == ADD_CONTENT_FAILED)
    {
        printf("Failed to add\n");
    }
    else
    {
        printf("Offset : %ld, Length : %ld\n", offset, length);
    }
}

int main()
{
    // Node *root = NULL;
    // char filename[MAX_FILE_NAME_LENGTH] = "data.bin";
    // int id;

    // for (id = 1; id <= 1000; id++)
    // {
    //     Book book = generate_book(id);
    //     root = add_content(root, id, filename, &book, sizeof(Book), onAddBook);
    // }

    // FILE *f = fopen(filename, "rb");
    // if (f)
    // {
    //     fseek(f, 0, SEEK_END);
    //     printf("File final size: %ld bytes\n", ftell(f));
    //     fclose(f);
    // }

    // printTree(root);

    // printf("Saving\n");
    // saveTree(root, "dsa.bin");
    // printf("Saved succesfully!\n");

    Node *root = loadTree("dsa.bin");
    // printTree(root);

    read_bucket_content(root, 5, 20, readBook);
    // printf("Exist record: %d\n", exist_record(root, 5));

    // TrieNode *root = makeTrieNode();
    // insertIntoTrie(root, "Science Fiction Book");
    // insertIntoTrie(root, "Book");
    // insertIntoTrie(root, "Hello world");
    // insertIntoTrie(root, "Hello Friend");

    // char *results[5];
    // int recommendSize = 0;

    // recommendPrefix(root, "he", 5, results, &recommendSize);

    // int i;
    // for (i = 0; i < recommendSize; i++)
    // {
    //     printf("%s\n", results[i]);
    // }

    return 0;
}