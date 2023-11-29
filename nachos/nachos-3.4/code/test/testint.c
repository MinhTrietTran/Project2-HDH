#include "syscall.h"
#include "copyright.h"


int main()
{
	PrintString("Nhap so nguyen n: ");
	int n = ReadInt();

	PrintString("So nguyen da nhap: ");
	PrintInt(n);
	

	Halt();
}
