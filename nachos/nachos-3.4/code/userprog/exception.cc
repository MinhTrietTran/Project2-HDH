// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "machine.h"
#include "console.h"
#include "synchcons.h"

#include <iostream>


// additional


void IncreatePC(){
	// update preRegister to current register
	int current = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg,current);

	// update current register to next register
	int next = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg,next);
	// update next register to next of next
	machine->WriteRegister(NextPCReg,next+4);

}

/*
Input: - User space address (int)
- Limit of buffer (int)
Output:- Buffer (char*)
Purpose: Copy buffer from User memory space to System memory space
*/
char* User2System(int virtAddr,int limit)
{
	int i;// index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit +1];//need for terminal string
	if (kernelBuf == NULL)
	return kernelBuf;
	memset(kernelBuf,0,limit+1);
	//printf("\n Filename u2s:");
	for (i = 0 ; i < limit ;i++)
	{
		machine->ReadMem(virtAddr+i,1,&oneChar);
		kernelBuf[i] = (char)oneChar;
		//printf("%c",kernelBuf[i]);
		if (oneChar == 0)
		break;
	}
	return kernelBuf;
}


/*
Input: - User space address (int)
- Limit of buffer (int)
- Buffer (char[])
Output:- Number of bytes copied (int)
Purpose: Copy buffer from System memory space to User memory space
*/
int System2User(int virtAddr,int len,char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0 ;
	do{
		oneChar= (int) buffer[i];
		machine->WriteMem(virtAddr+i,1,oneChar);
		i ++;
	}while(i < len && oneChar != 0);
	return i;
}

void Write(char* buffer, int length){
	Console *console;
	for(int i = 0; i < length; ++i){
		console->PutChar(buffer[i]);
	}
}

// additional

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
  int type = machine->ReadRegister(2);

	switch(which){
		case NoException:
			return;
		case PageFaultException:
				DEBUG('a',"\n[Page Fault Exception] No valid translation found\n");
				printf("\n[Page Fault Exception] No valid translation found\n");
				ASSERT(false);
				interrupt->Halt();
				return;
		case ReadOnlyException:
				DEBUG('a',"\n[Read Only Exception] Write attemped to page marked\n");
				printf("\n[Read Only Exception] Write attemped to page marked\n");
				ASSERT(false);
				interrupt->Halt();
				return;
		case BusErrorException:
				DEBUG('a',"\n[Bus Errror Exception] Translation resulted in an valid physical address\n");
				printf("\n[Bus Error Exepption] Translation resulted in an valid physical address\n");
				ASSERT(false);
				interrupt->Halt();
				return;
		case AddressErrorException:
				DEBUG('a',"\n[Address Error Exception] Unaligned reference or one that was beyond the end of the address space\n");
				printf("\n [Address Error Exception] Unaligned reference or one that was beyond the end of the address space \n");
				ASSERT(false);
				interrupt->Halt();
				return;
		case OverflowException:
				DEBUG('a',"/n[OverFlow Exception] Integer overflow in add or sub\n");
				printf("\n[Overflow Exception] Integer overflow in add or sub\n");
				ASSERT(false);
				interrupt->Halt();
				return;
		case IllegalInstrException:
				DEBUG('a',"\n[Illegal Instruction Exception] Unimplemented or reserved instr\n");
				printf("\n[Illegal Instruction Exception] Unimplemented or reserved instr\n");
				ASSERT(false);
				interrupt->Halt();
				return;
		case NumExceptionTypes:
				DEBUG('a',"\n[Number Exception]\n");
				printf("\n[Number Exception]\n");
				ASSERT(false);
				interrupt->Halt();
				return;
		case SyscallException:{
			switch(type){
				case SC_Halt:{
					DEBUG('a',"\nShut down, initialize by user program\n");
					printf("\nShut down, initialize by user program\n");
					interrupt->Halt();
					break;
				}
				case SC_ReadInt:{
					printf("Read Int Function\n");
					// Handle this method here
					break;
				}
				case SC_PrintInt:{
					printf("Print int Function\n" );
					//handle this method here
					break;
				}
				case SC_ReadChar:{
					printf("Read Char Function\n");
					// handle this method here
					break;
				}
				case SC_PrintChar:{
					printf("Print Char Function\n");
					// handle this method here

					break;
				}
				case SC_ReadString:{
					printf("Read String Function\n");
					// handle this method here

					break;
				}
				case SC_PrintString:{
					printf("Print String Function\n");
					// handle this method here

					break;
				}
				case SC_Help:{
					printf("Help Function\n");
					// handle this method here

					break;
				}
				case SC_PrintASCII:{
					printf("Print Ascii Function\n");
					// handle this method here

					break;
				}
				case SC_Sort:{
					printf("Sort Function\n");
					// handle this method here

					break;
				}
				default:{
					break;
				}


			}
			IncreatePC();
		}
		default:
			break;

	}
}
