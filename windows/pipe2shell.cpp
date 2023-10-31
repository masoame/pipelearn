#include "pipe2shell.h"

pipe2shell::pipe2shell(const char* lpApplicationName, char* lpCommandLine)
{
	//ȷ���̳о��
	attr.bInheritHandle = TRUE;

	//���ṹ�����
	attr.nLength = sizeof(SECURITY_ATTRIBUTES);

	//Ĭ��Ϊ0
	attr.lpSecurityDescriptor = NULL;

	//���������ܵ�һ����������һ����������
	::CreatePipe(&In_read,&In_write,&attr,0);
	::CreatePipe(&Out_read, &Out_write, &attr, 0);

	si.cb = sizeof(STARTUPINFOA);
	si.wShowWindow = SW_HIDE;
	si.hStdError = In_write;
	si.hStdOutput = In_write;
	si.hStdInput = Out_read;
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

	//��������
	if (!::CreateProcessA(lpApplicationName, lpCommandLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		MessageBoxA(NULL, "���򴴽�ʧ��", "��ʾ", MB_ICONERROR);
		return;
	}
	
	//�ظ���ȡ�������ܵ������ݲ�����
	std::thread([&]
		{
			//��ȡ�����ֽ��������ֽ�����ʣ���ֽ���
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
