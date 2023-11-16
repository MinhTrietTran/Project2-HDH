#include "syscall.h"

int
main(){
    int  n = ReadInt();
    PrintInt(5);
    ReadChar();
    PrintChar('A');
    ReadString();
    PrintString("available");
    Help();
    PrintASCII();
    Sort();
}
