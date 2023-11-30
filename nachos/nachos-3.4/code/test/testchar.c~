#include "syscall.h"
#include "copyright.h"

int main()
{
	//Sử dụng syscall đã định nghĩa để tiến hành đọc 1 từ màn hình console và lưu vào biến ch
	char ch = ReadChar();
	
	//Tiến hành kiểm tra xem giá trị của biến "ch" trả về có bằng 0 không, nếu bằng 0 tức là input đã bị sai và kết thúc chương trình
	//Nếu đúng thì tiến hành dùng syscall PrintChar để in "ch" ra màn hình Console
	if(ch != 0){
		PrintChar(ch);
	}

	Halt(); // Gọi syscall để đóng
	return 0;
}
