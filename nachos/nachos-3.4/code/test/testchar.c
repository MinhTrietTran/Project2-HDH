#include "syscall.h"
#include "copyright.h"

int main()
{
	char ch = ReadChar();
	if(ch != 0){
		PrintChar(ch);
	}
	Halt();
	return 0;
}
