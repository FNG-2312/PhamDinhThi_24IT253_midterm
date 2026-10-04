# Dự án Giữa kỳ - Mô phỏng lệnh ls(1)

## Thông tin sinh viên
- **Họ và tên:** Phạm Đình Thi
- **Mã sinh viên:** 24IT253
- **Lớp:** 24JIT
- **Lớp học phần:** Lập trình hệ thống (5)

## Mô tả dự án
Dự án này là một phiên bản mô phỏng lệnh `ls(1)` của hệ điều hành UNIX, được viết bằng ngôn ngữ C. Chương trình tương tác trực tiếp với hệ thống tệp UNIX thông qua các lời gọi hệ thống như `opendir`, `readdir`, và `stat` để duyệt thư mục và trích xuất siêu dữ liệu của tệp. Mã nguồn được tổ chức theo chuẩn module hóa.

## Các tính năng đã triển khai
Chương trình hoạt động chính xác theo tài liệu manual page được cung cấp, bao gồm đầy đủ 19 cờ tùy chọn:

| Cờ | Ý nghĩa / Chức năng |
| :---: | :--- |
| `-a` | Hiển thị tất cả các tệp, bao gồm cả tệp ẩn. |
| `-A` | Hiển thị các tệp ẩn, nhưng bỏ qua hai thư mục gốc là `.` và `..`. |
| `-l` | Hiển thị định dạng dài. |
| `-n` | Hoạt động giống `-l` nhưng hiển thị UID và GID dưới dạng số thay vì tên. |
| `-s` | Hiển thị số lượng block hệ thống thực tế mà tệp đang chiếm dụng. |
| `-k` | Sửa đổi cờ `-s`, hiển thị số lượng block theo đơn vị Kilobytes. |
| `-h` | Hiển thị kích thước tệp ở định dạng con người dễ đọc. |
| `-i` | In số sê-ri của tệp ra trước thông tin tệp. |
| `-F` | Thêm ký hiệu nhận dạng loại tệp. |
| `-q` | Ép in các ký tự không in được trong tên tệp dưới dạng dấu `?`. |
| `-w` | In thô các ký tự không in được thay vì thay thế bằng dấu `?`. |
| `-t` | Sắp xếp danh sách tệp theo thời gian sửa đổi. |
| `-S` | Sắp xếp danh sách tệp theo kích thước. |
| `-r` | Đảo ngược thứ tự sắp xếp. |
| `-f` | Tắt chức năng sắp xếp, in tệp ra theo đúng thứ tự thô trên ổ đĩa. |
| `-c` | Sử dụng thời gian thay đổi trạng thái để hiển thị hoặc sắp xếp. |
| `-u` | Sử dụng thời gian truy cập gần nhất để hiển thị hoặc sắp xếp. |
| `-R` | Liệt kê đệ quy nội dung của tất cả các thư mục con. |
| `-d` | Xử lý thư mục được truyền vào như một tệp thông thường. |

## Cấu trúc mã nguồn
- `main.c`: Phân tích tham số đầu vào bằng hàm `getopt()` và điều hướng luồng chương trình.
- `ls.c`: Chứa thuật toán cốt lõi để duyệt thư mục, sắp xếp, định dạng và trích xuất thông tin.
- `ls.h`: Tệp tiêu đề khai báo thư viện, định nghĩa cấu trúc dữ liệu và nguyên mẫu hàm.
- `Makefile`: Tự động hóa quá trình biên dịch mã nguồn.

## Xử lý ngoại lệ 
- Xử lý an toàn các đường dẫn tệp/thư mục không tồn tại và in ra thông báo lỗi chuẩn (`cannot open...`).
- Phân biệt, xử lý chính xác và thông minh khi đầu vào là tệp thông thường hay thư mục.
- Ngăn chặn hoàn toàn lỗi tràn bộ nhớ, kể cả khi gặp lỗi từ chối quyền truy cập vào các thư mục hệ thống.

## Hướng dẫn biên dịch và sử dụng

**1. Biên dịch chương trình:**
Mở terminal tại thư mục chứa mã nguồn dự án và gõ lệnh sau để biên dịch:
```bash
make
```

**2. Sử dụng chương trình:**
Cú pháp chung: `./myls [các_cờ_tùy_chọn] [tên_thư_mục_hoặc_tệp]`

**3. Dọn dẹp:**
Sau khi sử dụng xong, để xóa tệp thực thi `myls`, gõ lệnh:
```bash
make clean
```
