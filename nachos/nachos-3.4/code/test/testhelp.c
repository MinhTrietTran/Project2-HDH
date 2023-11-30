#include "syscall.h"
#include "copyright.h"

int main()
{   
    // Goi system call PrintString de in gioi thieu co ban ve nhom
    PrintString("\n==========HE DIEU HANH- 2023==========\n");
    PrintString("THONG TIN NHOM\n");
    PrintString("21120515 - TRAN PHUOC NHAN\n");
    PrintString("21120600 - NGUYEN THI NHU Y\n");
    PrintString("21120578 - TRAN MINH TRIET\n");
    PrintString("21120585 - LE ANH TU\n");
    PrintString("21120587 - NGUYEN MINH TUAN\n");
    PrintString("\n======================================\n");
    // Goi system call PrintString de in mo ta van tat ve chuong trinh ascii
    PrintString("\n==========IN MA ASCII==========\n");
    PrintString("Bang ma ASCII se co ky tu in duoc va khong\n");
    PrintString("Trong bang ma co 1 ky tu dang tien chuong khong duoc hien thi\n");
    PrintString("Chuong trinh sap xep la testascii.c nam trong thu muc test\n");
    PrintString("Dat terminal tai ../nachos/nachos-3.4/code\n");
    PrintString("Dung lenh: ./userprog/nachos -rs 1023 -x ./test/testascii\n");
    PrintString("\n======================================\n");
    // Goi system call PrintString de in mo ta van tat ve chuong trinh sort
    PrintString("\n==========SORT==========\n");
    PrintString("Thuat toan Bubble Sort\n");
    PrintString("Nhap vao so luong phan tu n\n");
    PrintString("Nhap cac phan tu trong mang can sap xep\n");
    PrintString("Chuong trinh sap xep la testsort.c nam trong thu muc test\n");
    PrintString("Dat terminal tai ../nachos/nachos-3.4/code\n");
    PrintString("Dung lenh: ./userprog/nachos -rs 1023 -x ./test/testsort\n");
    PrintString("\n======================================\n");


    Halt();
}
