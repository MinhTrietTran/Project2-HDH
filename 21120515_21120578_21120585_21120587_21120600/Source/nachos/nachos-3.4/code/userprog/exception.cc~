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
// #include "synchcons.h"

#include <iostream>
#define MaxFileLength 32
#define MaxBufferSize 255

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
				case SC_Create:
				{
					int virtAddr;
					char* filename;
					DEBUG('a',"\n SC_Create call ...");
					DEBUG('a',"\n Reading virtual address of filename");
					// Lấy tham số tên tập tin từ thanh ghi r4
					virtAddr = machine->ReadRegister(4);
					DEBUG ('a',"\n Reading filename.");
					// MaxFileLength là = 32
					filename = User2System(virtAddr,MaxFileLength+1);
					if (filename == NULL)
					{
						printf("\n Not enough memory in system");
						DEBUG('a',"\n Not enough memory in system");
						machine->WriteRegister(2,-1); // trả về lỗi cho chương
						// trình người dùng
						delete filename;
						break;
					}
					DEBUG('a',"\n Finish reading filename.");
					//DEBUG(‘a’,"\n File name : '"<<filename<<"'");
					// Create file with size = 0
					// Dùng đối tượng fileSystem của lớp OpenFile để tạo file,
					// việc tạo file này là sử dụng các thủ tục tạo file của hệ điều
					// hành Linux, chúng ta không quản ly trực tiếp các block trên
					// đĩa cứng cấp phát cho file, việc quản ly các block của file
					// trên ổ đĩa là một đồ án khác
					if (!fileSystem->Create(filename,0))
					{
						printf("\n Error create file '%s'",filename);
						machine->WriteRegister(2,-1);
						delete filename;
						break;
					}
					machine->WriteRegister(2,0); // trả về cho chương trình
					// người dùng thành công
					delete filename;
					break;
				}
				case SC_Halt:{
					DEBUG('a',"\nShut down, initialize by user program\n");
					printf("\nShut down, initialize by user program\n");
					interrupt->Halt();
					break;
				}
				case SC_ReadInt:{
					// Handle this method here
					char* buffer = new char[MaxBufferSize + 1];

					int bytesRead = gSynchConsole->Read(buffer,MaxBufferSize);

					buffer[bytesRead] = '\0';
					
					int sign = 1;
					int i = 0;
					int result = 0;

					bool isValid = true;

					if(bytesRead > 0){
						if(buffer[0] == '-'){
							sign = -1;
							i++;
						}

						for(; i < bytesRead; i++){
							if(buffer[i] >= '0' && buffer[i] <= '9'){
								result = result * 10 + (buffer[i] - '0');
							}
							else{
								isValid = false;
								break;
							}
						}

						if(bytesRead == 1){
							if(sign == -1){
								isValid = false;
							}
						}
						if(isValid == true){
							result = result * sign;
							machine->WriteRegister(2,result);
						}
						else{
							DEBUG('a',"\nInvalid - Input is non Integer\n");
							printf("\nInvalid - Invalid - Input non Integer\n");
							machine->WriteRegister(2,0);
						}
					}
					else{
						DEBUG('a',"\nInvalid - Input is Empty\n");
						printf("\nInvalid - Invalid - Input is Empty\n");
					}
					delete[] buffer;
					break;
				}
				case SC_PrintInt:{
					int number = machine->ReadRegister(4);
					char *buffer = new char[MaxBufferSize + 1];

					if(number == 0){
						gSynchConsole->Write("0", 1);
					}
					else{
						if(number < 0){
							number = number * -1;
							gSynchConsole->Write("-", 1);
						}

						int length = 0;
						int temp = number;
						while(temp > 0){
							length++;
							temp /= 10;
						}

						for(int i = length - 1; i >= 0; i--){
							buffer[i] = ((number % 10) + '0');
							number /= 10;
						}

						buffer[length] = '\0';

						gSynchConsole->Write(buffer, length + 1);

					}
					delete[] buffer;

					break;
				}
				case SC_ReadChar:{
					char *input = new char[MaxBufferSize + 1];
					int bytes = gSynchConsole -> Read(input, MaxBufferSize);
					if(bytes > 1){
						DEBUG('a',"\nInvalid - Number of characters is more than 1\n");
						printf("\nInvalid - Number of characters is more than 1\n");
						machine->WriteRegister(2,0);
					}
					else if( bytes == 0){
						DEBUG('a',"\nInvalid - Input character is empty\n");
						printf("\nInvalid - Input character is empty\n");
						machine->WriteRegister(2,0);
					}
					else if(bytes == 1){
						char onechar = *input;
						machine->WriteRegister(2, onechar);
					}
					delete input;
					break;
				}
				case SC_PrintChar:{
					char ch = (char)machine->ReadRegister(4); // Ham ReadRegister tra ve kieu int -> ep kieu thanh char
					gSynchConsole->Write(&ch, 1);
					break;
				}
				case SC_ReadString:{
					int bufferAddress = machine->ReadRegister(4);
					int maxlength = machine->ReadRegister(5);

					// Get data from gSynchConsole
					char *buffer = new char[maxlength + 1]; // +1 for '/0'
					int bytesRead = gSynchConsole->Read(buffer,maxlength);

					buffer[bytesRead] = '\0';

					if(bytesRead == 0){
						DEBUG('a',"\nInvalid - The string is empty\n");
						printf("\nInvalid - The string is empty - \n");
					}
					else {
						int byteCopied = System2User(bufferAddress, bytesRead + 1, buffer);
						if(byteCopied <= 0){
							DEBUG('a',"\nFailed - Some errors have been occured when tranfers from system to user\n");
							printf("\nFailed - Some errors have been occured when tranfers from system to user\n");
						}
					}

					delete buffer;
					break;
				}
				case SC_PrintString:{
					int bufferAddress = machine->ReadRegister(4);
					
					char *buffer = User2System(bufferAddress,MaxBufferSize);

					int length = strlen(buffer);
					gSynchConsole->Write(buffer, length + 1);

					break;
				}
				default:{
					interrupt->Halt();
					break;
				}
			}
			IncreatePC();
		}
		default:
			break;

	}
}

