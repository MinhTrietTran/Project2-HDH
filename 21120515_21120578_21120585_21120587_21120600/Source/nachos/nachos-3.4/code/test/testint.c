#include "syscall.h"
#include "copyright.h"

int main()
{
	// Đọc số nguyên sử dụng syscall ReadInt()
	int n = ReadInt();

	// In số nguyên sử dụng syscall PrintInt()
	PrintInt(n);
	

	Halt();
}
