#include <iostream>
#include <windows.h>
using namespace std;
HANDLE g_hEvent;

DWORD WINAPI MyThreadProc1(PVOID pParam);
DWORD WINAPI MyThreadProc2(PVOID pParam);
int main()
{
	g_hEvent = NULL;
	cout << "test WaitForSingleObject" << endl;
	g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// SetEvent(g_hEvent);
	CreateThread(NULL, 0, MyThreadProc1, NULL, 0, NULL);
	CreateThread(NULL, 0, MyThreadProc2, NULL, 0, NULL);

	system("pause");

	return 0;
}

DWORD WINAPI MyThreadProc1(PVOID pParam)
{
	static int i = 0;
	while (1)
	{
		cout << "\nMyThreadProc1 come in " << i++ << endl;
		//DWORD dwReturn = WAIT_OBJECT_0;
		DWORD dwReturn = WaitForSingleObject(g_hEvent, 2 * 1000);
		switch (dwReturn)
		{
		case WAIT_OBJECT_0:
			// hProcess������Ľ�����5���ڽ���
			cout << "MyThreadProc1 >>> WAIT_OBJECT_0" << endl;
			return 0;

		case WAIT_TIMEOUT:
			// �ȴ�ʱ�䳬��5��
			cout << "MyThreadProc1 >>> WAIT_TIMEOUT" << endl;
			break;

		case WAIT_FAILED:
			// ��������ʧ�ܣ����紫����һ����Ч�ľ��
			cout << "MyThreadProc1 >>> WAIT_FAILED" << endl;
			break;
		default:
			cout << "MyThreadProc1 >>> default" << endl;
		}

		cout << "MyThreadProc1 leave" << endl;
	}
	
	return 0;
}

DWORD WINAPI MyThreadProc2(PVOID pParam)
{

	cout << "\n----------------------\nMyThreadProc2 come in" << endl;
	int i = 0;
	while (i++ < 8)
	{
		cout << "MyThreadProc2��" << i << endl; 
		Sleep(1 * 1000); // Ϊ��֤��ֻ���ͷ��źţ��߳�1����ִ��
	}

	//SetEvent(g_hEvent); // �ͷ��ź�

	cout << "MyThreadProc2 >>> send g_hEvent signal" << endl;
	SetEvent(g_hEvent);
	cout << "MyThreadProc2 leave\n----------------------" << endl;

	return 0;
}