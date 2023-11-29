#include "syscall.h"
#include "copyright.h"


int main()
{

    int arr[101], n, i , j, temp, choice;
    

    PrintString("----------Sap xep noi bot----------\n");

    PrintString("Nhap so luong phan tu: ");
    n = ReadInt();
    if(n <= 0){
        PrintString("So luong phan tu chua dung");
        Halt();
    }
    PrintString("Nhap cac phan tu trong mang:\n");
    for (i = 0; i < n; i++) {
    	PrintString("Phan tu ");
	PrintInt(i+1);
	PrintString(": ");
        arr[i] = ReadInt();
    }

    do{
        PrintString("1: Tang dan\n");
        PrintString("2: Giam dan\n");
        PrintString("lua chon cua: ");
        choice = ReadInt();
    }while(choice != 1 && choice != 2);
    if(choice == 1){
        for (i = 0; i < n-1; i++){
            for (j = 0; j < n-i-1; j++){
                if (arr[j] > arr[j+1]){
                    temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }
    }
    else if(choice == 2){
        for (i = 0; i < n-1; i++){
            for (j = 0; j < n-i-1; j++){
                if (arr[j] < arr[j+1]){
                    temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }
    }
    PrintString("Mang sau khi sap xep: \n");

    for(i = 0; i < n; i++){
        PrintInt(arr[i]);
        PrintString(" ");
    }
    PrintString("\n");

    Halt();
}
