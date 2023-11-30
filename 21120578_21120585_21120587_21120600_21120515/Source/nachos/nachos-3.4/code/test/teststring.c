#include "syscall.h"
#include "copyright.h"

#define MaxSize 255

int main()
{
	//Tạo 1 mảng gồm 255 ký tự ( có thể nhiều hơn tùy vào sở thích)
	//Đặt biến len bằng MaxSize có thể có

	char buffer[255];
	int len = MaxSize;
	
	//Gọi syscall ReadString với 2 tham số buffer, và MaxSize để tiến hành đọc dữ liệu từ màn hình Console
	ReadString(buffer, MaxSize);
	
	// Tiến hành in ra màn hính với syscall PrintString
	PrintString(buffer);

	Halt();
}
