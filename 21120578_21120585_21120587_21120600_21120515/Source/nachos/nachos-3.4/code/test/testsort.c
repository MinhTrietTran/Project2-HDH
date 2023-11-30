// Chương trình triển khai thuật toán Bubble Sort để sắp xếp một mảng số nguyên.
// Sử dụng các syscall từ thư viện bên ngoài để nhập, xuất và kết thúc chương trình.
#include "syscall.h"
#include "copyright.h"


int main()
{

    int arr[101], n, i , j, temp, choice;
    

    PrintString("----------Sap xep noi bot----------\n");
    // Yêu cầu người dùng nhập số phần tử của mảng
    PrintString("Nhap so luong phan tu: ");
    n = ReadInt();
    // Kiểm tra xem đầu vào có nằm trong phạm vi hợp lệ không
    if(n <= 0 || n >= 100){
        PrintString("So luong phan tu chua dung");
        Halt();
    }
    // Nhắc người dùng nhập các phần tử vào mảng
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
    // Thực hiện Bubble Sort dựa trên lựa chọn của người dùng
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
    // Xuất mảng đã sắp xếp
    PrintString("Mang sau khi sap xep: \n");

    for(i = 0; i < n; i++){
        PrintInt(arr[i]);
        PrintString(" ");
    }
    PrintString("\n");

    Halt();
}
