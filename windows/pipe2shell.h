#pragma once
#include"common.h"
class pipe2shell
{
public:
	//������ȡ�Լ�д����Ҫ�Ĺܵ����
	HANDLE In_read, In_write, Out_read, Out_write;

	SECURITY_ATTRIBUTES attr;
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	//���ڽ��ܹܵ�������
	char lpbuffer[4096];
	//����һ:�ܵ�Ҫ�����ĳ�����Ը�������
	pipe2shell(const char* lpApplicationName = "cmd.exe", char* lpCommandLine = nullptr);
	virtual ~pipe2shell();
};

