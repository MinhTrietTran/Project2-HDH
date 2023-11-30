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
				// Đoạn mã này xử lý lệnh gọi hệ thống SC_ReadInt, đọc một số nguyên từ đầu vào của bàn điều khiển.
				// Nó khởi tạo bộ đệm để lưu trữ dữ liệu đầu vào, đọc các ký tự từ bảng điều khiển và chuyển đổi chúng thành số nguyên.
				// Mã đảm bảo đầu vào là số nguyên hợp lệ và xử lý số âm một cách thích hợp.
				// Nếu đầu vào hợp lệ, số nguyên được ghi vào thanh ghi 2; nếu không, một thông báo lỗi sẽ được in.
				case SC_ReadInt:{
					// Cấp phát bộ nhớ cho bộ đệm đầu vào
					char* buffer = new char[MaxBufferSize + 1];

					// Đọc ký tự từ console vào bộ đệm
					int bytesRead = gSynchConsole->Read(buffer,MaxBufferSize);

					// Null-terminate buffer
					buffer[bytesRead] = '\0';
					
					// Khởi tạo các biến dấu, chỉ mục và kết quả
					int sign = 1;
					int i = 0;
					int result = 0;

					// Gắn cờ để theo dõi tính hợp lệ của đầu vào
					bool isValid = true;

					// Xử lý bộ đệm đầu vào
					if(bytesRead > 0){
						if(buffer[0] == '-'){
							sign = -1;
							i++;
						}

						// Chuyển ký tự thành số nguyên
						for(; i < bytesRead; i++){
							if(buffer[i] >= '0' && buffer[i] <= '9'){
								result = result * 10 + (buffer[i] - '0');
							}
							else{
								isValid = false;
								break;
							}
						}

						// Kiểm tra đầu vào một ký tự có dấu âm
						if(bytesRead == 1){
							if(sign == -1){
								isValid = false;
							}
						}
						
						// Xử lý kết quả dựa trên tính hợp lệ
						if(isValid == true){
							result = result * sign;
							// Viết kết quả vào thanh ghi 2
							machine->WriteRegister(2,result);
						}
						else{
							DEBUG('a',"\nInvalid - Input is non Integer\n");

							// In thông báo lỗi do đầu vào không hợp lệ
							printf("\nInvalid - Invalid - Input non Integer\n");

							// Viết 0 để thanh ghi 2 trong trường hợp đầu vào không hợp lệ
							machine->WriteRegister(2,0);
						}
					}
					else{
						// In thông báo lỗi khi đầu vào trống
						DEBUG('a',"\nInvalid - Input is Empty\n");
						printf("\nInvalid - Invalid - Input is Empty\n");
					}
					delete[] buffer;
					break;
				}
				// Đoạn mã này xử lý lệnh gọi hệ thống SC_PrintInt, in một số nguyên ra đầu ra của bàn điều khiển.
				// Nó đọc số nguyên từ thanh ghi 4, chuyển nó thành bộ đệm ký tự và sau đó ghi nó vào bàn điều khiển.
				// Số âm được xử lý bằng cách in dấu '-' và chuyển giá trị tuyệt đối thành ký tự.
				case SC_PrintInt:{

					// Đọc số nguyên từ thanh ghi 4
					int number = machine->ReadRegister(4);
					// Cấp phát bộ nhớ cho vùng đệm ký tự
					char *buffer = new char[MaxBufferSize + 1];
					// Xử lý số và in ra console
					if(number == 0){
						// Print '0' cho số 0
						gSynchConsole->Write("0", 1);
					}
					else{
						if(number < 0){
							number = number * -1;
							// Xử lý số âm bằng cách in dấu '-'
							gSynchConsole->Write("-", 1);
						}
						// Tính độ dài của số
						int length = 0;
						int temp = number;
						while(temp > 0){
							length++;
							temp /= 10;
						}
						// Chuyển số thành ký tự và lưu vào bộ đệm
						for(int i = length - 1; i >= 0; i--){
							buffer[i] = ((number % 10) + '0');
							number /= 10;
						}
						// Null-terminate buffer
						buffer[length] = '\0';
						// Ghi bộ đệm ra console
						gSynchConsole->Write(buffer, length + 1);

					}
					// Giải phóng bộ nhớ đệm
					delete[] buffer;

					break;
				}
				
				case SC_ReadChar:{
					// Syscall này tương ứng với hàm ReadChar được sử dụng "char ReadChar();"
					// Khởi tạo 1 bảng các ký tự với số lượng MaxBufferSize + 1. Thêm 1 phần tử phần kết thúc chuỗi
					char *input = new char[MaxBufferSize + 1];

					 // Lấy dữ liệu từ màn hình vào mảng input và số lượng byte đọc được sẽ được lưu vào biến bytes
					int bytes = gSynchConsole -> Read(input, MaxBufferSize);

					// Kiểm tra nếu số lượng bytes mà đọc được lớn hơn 1 tức là đã nhập 1 chuỗi
					// Chỉ chấp nhận 1 byte. 1 byte tương ứng với 1 ký thực char (char = 1 byte)
					if(bytes > 1){
						DEBUG('a',"\nInvalid - Number of characters is more than 1\n");
						printf("\nInvalid - Number of characters is more than 1\n");
						machine->WriteRegister(2,0);
					}
					else if( bytes == 0){ // Loại bõ trường hợp nhập vào empty (Trường hợp người dùng enter)
						DEBUG('a',"\nInvalid - Input character is empty\n");
						printf("\nInvalid - Input character is empty\n");
						machine->WriteRegister(2,0);
					}
					else if(bytes == 1){ // Nếu đúng 1 byte thì tiến hành
						// Tạo 1 biến char và gán nó chính là giá trị của phần tử đầu tiên của mảng input
						char onechar = *input;
						// Dùng machine( biến con trỏ dạng máy được khai báo toàn cục) để ghi onchar vào thanh ghi số 2
						// Thanh ghi số 2 là thành ghi trả về
						machine->WriteRegister(2, onechar);
					}
					delete input; // Sau khi thực hiện xong tiến hành lấy lại vùng nhớ cấp phát cho input
					break;
				}
				case SC_PrintChar:{
					// Syscall này dùng để in ra màn hình cho syscall ReadChar với định nghĩa hàm "void PrintChar(char value);"
					// Hàm nhận vào 1 "char value" làm tham số, tiến hành lấy tham số này từ thanh ghi số 4 bằng ReadRegister nhờ vào machine
					char ch = (char)machine->ReadRegister(4); // Hàm ReadRegister trả về kiểu int -> ép kiểu thành char
					// Hàm gSynchConsole để tiến hành ghi giá trị ký tự char đọc được ra màn hình Console
					gSynchConsole->Write(&ch, 1);
					break;
				}
				case SC_ReadString:{
					// Syscall này tương ứng cho hàm "void ReadString (char* buffer, int length);"
					// Hàm nhập vào 2 tham số: 1 là 1 con trỏ buffer, và 1 length cho số lượng phần tử
					
					// Tiền hành đọc dữ liệu mảng tham số buffer từ thanh ghi số 4
					int bufferAddress = machine->ReadRegister(4);
					// Đọc tham số thứ 2 là length từ thanh ghi số 5. Tham số này chính là số lượng phần tử người dùng định nghĩa lớn nhất
					int maxlength = machine->ReadRegister(5);

					// Tạo 1 mảng buffer để lưu trữ dữ liệu được đọc từ màng hình
					char *buffer = new char[maxlength + 1]; // +1 for '/0'
					// bytesRead là số phần tử đọc được thực tế từ việc đọc trong màn hình Console và lưu vào mảng buffer
					int bytesRead = gSynchConsole->Read(buffer,maxlength);

					// Gán ký hiệu kết thúc cho chuỗi vừa đọc được từ màn hình '\0'
					buffer[bytesRead] = '\0';

					// Tiến hành kiểm tra: nếu không có byte nào được đọc, thì thông báo cho người dùng chuỗi rỗng
					// Ngược lại thì tiến hành chuyển đổi dữ liệu đọc được từ system space sang user space
					// và lưu vào mảng bufferAddress chính là tham số được truyền vào hàm.
					if(bytesRead == 0){
						DEBUG('a',"\nInvalid - The string is empty\n");
						printf("\nInvalid - The string is empty - \n");
					}
					else {
						int byteCopied = System2User(bufferAddress, bytesRead + 1, buffer); // byteCopied chính là số lượng byte 
															// thực sự được chuyển đổi
						if(byteCopied <= 0){
							DEBUG('a',"\nFailed - Some errors have been occured when tranfers from system to user\n");
							printf("\nFailed - Some errors have been occured when tranfers from system to user\n");
						}
					}
					// Thu hồi vùng nhớ cho buffer
					delete buffer;
					break;
				}
				case SC_PrintString:{
					// Syscall này tương ứng với "void PrintString(char* buffer);"
					// Nhập vào 1 tham số buffer dạng con trỏ char chính là mảng ký tự
					// Tiến hành lấy dữ liệu tham số buffer từ thanh ghi số 4 ( thanh ghi tham số)
					int bufferAddress = machine->ReadRegister(4);
					// Tiến hành chuyển đổi dữ liệu mảng ký tự từ user space to system space vào mảng buffer
					char *buffer = User2System(bufferAddress,MaxBufferSize);

					// Lấy ra độ dài của mảng buffer và dùng gSynchConsole để ghi vào màn hình console cho người dùng
					int length = strlen(buffer);
					gSynchConsole->Write(buffer, length + 1);

					break;
				}
				default:{
					interrupt->Halt();
					break;
				}
			}
			IncreatePC(); // Tiến hành tăng thanh ghi PC mỗi khi thực hiện các syscall để tránh bị lặp vô tận
		}
		default:
			break;

	}
}

