#include "syscall.h"
#include "copyright.h"


int main()
{
	char buffer[255] ;
	buffer = "Nhap so nguyen n: ";
	PrintString(buffer);
	int n = ReadInt();
	buffer = "So nguyen da nhap: ";
	PrintString(buffer);
	PrintInt(n);
	

	Halt();
}
