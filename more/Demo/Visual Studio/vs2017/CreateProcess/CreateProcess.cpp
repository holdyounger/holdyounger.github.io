
// CreateProcess.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <process.h>
#include <windows.h>

bool g_stopTh1 = false;

void funcTh1(void *)
{
	int i = 0;
	while (g_stopTh1)
	{
		std::cout << i++ << std::endl;
		Sleep(1000);
	}
	return;
}

int main()
{
	g_stopTh1 = true;
    std::cout << "Hello World!\n";

	HANDLE handle = (HANDLE)_beginthread(funcTh1, 0, NULL);

	std::cout << "Hello World!\n";
	
	CloseHandle(handle);

	Sleep(10000);
	g_stopTh1 = false;
}

// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ����ʹ�ü���: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�
