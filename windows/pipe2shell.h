#pragma once
#include"common.h"
class pipe2shell
{
public:
	//创建读取以及写入需要的管道句柄
	HANDLE In_read, In_write, Out_read, Out_write;

	SECURITY_ATTRIBUTES attr;
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	//用于接受管道的数组
	char lpbuffer[4096];
	//参数一:管道要监听的程序可以附带参数
	pipe2shell(const char* lpApplicationName = "cmd.exe", char* lpCommandLine = nullptr);
	virtual ~pipe2shell();
};

