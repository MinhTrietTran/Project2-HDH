#include "syscall.h"
#include "copyright.h"

int main()
{   
	int row = 0;
	int col = 0;
	int value = 0;

	PrintString("\n---------------In bang ma ASCII---------------\n");
	// Duyet qua tung dong cua bang
	for(row; row < 8; row++){ 
		PrintString("Cot ");
		PrintInt(row);
		PrintString("  ");
		// Duyet qua tung cot tren moi dong tuong ung
		for(col = 0; col < 16; col++){
			value = row * 16 + col;
			// Goi system call PrintChar de in gia tri duoi dang ma ascii
			PrintChar(value);
			PrintString("  ");
		}
		PrintString("\n");
	}
	
	PrintString("\n---------------------------------------------");
	PrintString("\n");
	
	Halt();
}
