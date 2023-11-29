#include "syscall.h"
#include "copyright.h"

int main()
{
	int n = ReadInt();

	if(n != 0){
		PrintInt(n);
	}

	Halt();
}
