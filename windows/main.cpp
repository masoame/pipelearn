#include"common.h"
#include"pipe2shell.h"
//int __stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
int main()
{
	char str[] = "adb.exe shell"; 
	pipe2shell test(NULL, str);

	WaitForSingleObject(test.pi.hProcess, INFINITE);
	MessageBoxA(NULL, "�������", "��ʾ", MB_ICONINFORMATION);

	return 0;
}