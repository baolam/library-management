# Dự án quản lí thư viện

# Thành viên

- Nguyễn Đức Bảo Lâm
- Nguyễn Huy Phong
- Phạm Đức Phúc
- Bùi Hải Đăng

# Mô tả

Dự án này được tạo ra để quản lí thư viện theo các chức năng mà trainC yêu cầu.
Ngoài ra dự án có bổ sung thêm và mở rộng một số giải quyết sau:

- Nếu số lượng sách lớn (cỡ 100k hay tới đơn vị triệu thì sao?)
- Nếu số lượng reader lớn (cỡ 100k hay tới đơn vị triệu thì sao?)
  Thông qua việc trả lời hai câu hỏi trên, nhóm xác định việc lưu trữ và xử lí dữ liệu theo dạng file thông thường sẽ gặp khó khăn, vì vậy nhóm đã ứng dụng bổ sung thêm một số cấu trúc dữ liệu bổ trợ và thay đổi cách đọc file.

# Cụ thể

- Mỗi đối tượng quản lí (Book, Reader) được tổ chức lưu trữ thành các file riêng (trong thư mục data)
- File lưu trữ là file nhị phân (đuôi .bin)
- Bên cạnh lưu trữ thêm file nội dung, sẽ có lưu thêm file quản lí (gồm hai file)
- Cách đọc dữ liệu, do lưu trữ trực tiếp struct Book/Reader vào file, nên việc đọc là đọc trực tiếp từ file ra, thông qua định hướng của file quản lí.

# Cấu trúc dữ liệu được dùng

- B+ Tree. Là cấu trúc quản lí chính được dùng trong dự án.
- Trie. Là cấu trúc quản lí phụ, dùng cho hỗ trợ thao tác tìm kiếm gợi ý.
- Array. Dùng cho việc hứng, đọc dữ liệu

# Một số bài toán

Do mục tiêu cần quản lí, cách tổ chức dữ liệu sẽ quy vào và tập trung vào các Id. Xem các Id là trường khoá chính để thao tác ở các mức độ rộng lớn hơn (tương tác giữa Book và Reader làm phát sinh mối quan hệ mượn trả. Giữa Book id với Book title dùng cho lưu trữ trong cây Trie, ...).

Như vậy, ta sẽ có một số bài toán phụ sau:

- Làm sao kết nối được giữa B+ Tree/Trie với thao tác quản lí.
- Làm sao kết nối được giữa B+ Tree/Trie với thao tác lưu trữ.
- Đối với vấn đề thống kê, báo cáo. Khai thác ra sao.
- Đối với vấn đề tìm kiếm gợi ý. Khai thác ra sao.
- Đối với vấn đề quản lí mượn trả. Khai thác ra sao.

# Một số mức khái niệm

Trong quá trình khai thác và viết chương trình. Đây là mẫu quan hệ chủ đạo được dùng:

B+ Tree
--> Management --> Book/Reader/Statistic/BorrowBook... --> Layout
Trie

Trong mẫu trên, B+Tree/Trie sẽ được cài đặt như thông thường (thao tác quản lí). Sau đó thông qua management để kết nối giữa thao tác quản lí và lưu trữ. Các phần chương trình còn lại như Book/Reader/... sẽ đa số dùng những hàm được định nghĩa ở Management để thực hiện các thao tác.
