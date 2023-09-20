// GetUSN.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <iostream>
#include <Windows.h>

#pragma comment(lib, "Kernel32.lib")

/* *********************
 * CreateIoCompletionPort() �������������ܣ�
 * 1������һ����ɶ˿ڣ�
 * 2����һ���豸��һ�����ڵ���ɶ˿ڽ��а󶨣���������������豸����һ��SOCKET��
 * ********************* */
void getUSN()
{
	HANDLE		fileHd;
	HANDLE		ExistingCompletionPort;
	ULONG_PTR	CompletionKey;
	DWORD		NumberOfConcurrentThreads;
	HANDLE		hRet;

	fileHd = (HANDLE)CreateFileA((LPCSTR)"D:\\1.bat", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if(fileHd == INVALID_HANDLE_VALUE)
	{
		fileHd = NULL;
		MessageBoxA(NULL, "Can not open the file", "Playwav", MB_OK);
	}
	else
	{
		MessageBoxA(NULL, "Open the file", "Playwav", MB_OK);
	}

	hRet = CreateIoCompletionPort(fileHd, ExistingCompletionPort, CompletionKey, NumberOfConcurrentThreads);

	printf("ExistingCompletionPort(%d)\nCompletionKey(%d)\nNumberOfConcurrentThreads(%d)\n", ExistingCompletionPort, CompletionKey, NumberOfConcurrentThreads);
}


int main(int argc, char* argv[])
{
	getUSN();
	return 0;
}

