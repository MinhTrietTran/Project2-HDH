#include "syscall.h"
#include "copyright.h"

int main()
{   
	int row = 0;
	int col = 0;
	int value = 0;

	PrintString("\n---------------In bang ma ASCII---------------\n");

	for(row; row < 8; row++){
		PrintString("Cot ");
		PrintInt(row);
		PrintString("  ");
		for(col = 0; col < 16; col++){
			value = row * 16 + col;
			PrintChar(value);
			PrintString("  ");
		}
		PrintString("\n");
	}
	
	PrintString("\n---------------------------------------------");
	PrintString("\n");

	Halt();
}
