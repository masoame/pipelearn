#include "pipe2shell.h"

pipe2shell::pipe2shell(const char* lpApplicationName, char* lpCommandLine)
{
	//确定继承句柄
	attr.bInheritHandle = TRUE;

	//填充结构体参数
	attr.nLength = sizeof(SECURITY_ATTRIBUTES);

	//默认为0
	attr.lpSecurityDescriptor = NULL;

	//创建两个管道一个接收数据一个发送数据
	::CreatePipe(&In_read,&In_write,&attr,0);
	::CreatePipe(&Out_read, &Out_write, &attr, 0);

	si.cb = sizeof(STARTUPINFOA);
	si.wShowWindow = SW_HIDE;
	si.hStdError = In_write;
	si.hStdOutput = In_write;
	si.hStdInput = Out_read;
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

	//创建程序
	if (!::CreateProcessA(lpApplicationName, lpCommandLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		MessageBoxA(NULL, "程序创建失败", "提示", MB_ICONERROR);
		return;
	}
	
	//重复读取程序发往管道的内容并呈现
	std::thread([&]
		{
			//读取到的字节数，总字节数，剩下字节数
			DWORD BytesRead, TotalBytesAvail, BytesLeftThisMessage,temp;
			while (true)
			{
				::PeekNamedPipe(In_read, lpbuffer,4096, &BytesRead, &TotalBytesAvail, 0);
				Sleep(1000);			
				ReadFile(In_read, lpbuffer, BytesRead, &temp, NULL);
				lpbuffer[BytesRead] = 0;
				std::cout << lpbuffer;
			}
		}).detach();
	
	std::thread([&]
		{
			DWORD temp;
			char str[1024] = { 0 };

			while (true)
			{
				std::cin.getline(str,1024);
				int i = strnlen_s(str, 1024);
				str[i] = '\n';
				WriteFile(Out_write, str, i+1, &temp, NULL);
			}
		}).detach();
}
pipe2shell::~pipe2shell()
{
	CloseHandle(Out_read);
	CloseHandle(In_write);
	CloseHandle(Out_write);
	CloseHandle(In_read);
}
