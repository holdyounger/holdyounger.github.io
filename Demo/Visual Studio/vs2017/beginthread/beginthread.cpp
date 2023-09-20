// beginthread.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <windows.h>
#include <process.h>
#include <string.h>
#include <tchar.h>
using std::cout;
using std::endl;

typedef struct _STU_PEOPLE
{
	char szName[50];
	int age;
}STU_PEOPLE,*PSTU_PEOPLE;

void thread_1(void *)
{
	
	cout << "�������߳� thread_1 �޽������" << endl;
}

void thread_2(void *lpVoid)
{
	Sleep(10);
	int pnInt = (int)lpVoid;
	cout << "�������߳� thread_2 �������Ϊ��" << pnInt << endl;
}

void thread_3(void *lpVoid)
{
	Sleep(50);
	PSTU_PEOPLE pnInt = (PSTU_PEOPLE)lpVoid;
	cout << "�������߳� thread_3 �������Ϊ��" << pnInt->szName << ':' << pnInt->age << endl;
}


int main()
{
	STU_PEOPLE stuChild;

	char name[] = { "Child_LI" };
	strcpy_s(stuChild.szName, name);
	stuChild.age = 6;


    std::cout << "Start Thread!\n";
	_beginthread(thread_1, 0, NULL);
	_beginthread(thread_2, 0, (void*)stuChild.age);
	_beginthread(thread_3, 0, (void*)&stuChild);

	std::cout << "End Thread!\n";

	Sleep(100);
	system("pause");
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
