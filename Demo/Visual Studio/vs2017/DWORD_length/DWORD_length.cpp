// DWORD_length.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <Windows.h>
using namespace std;

int main()
{
	std::cout << "Hello World!\n";

	DWORD dwNeed = 0;

	int length = dwNeed / sizeof(DWORD);
	cout << dwNeed << ':' << length << endl;

	dwNeed = 123213;

	length = dwNeed / sizeof(DWORD);
	cout << dwNeed << ':' << length << endl;
	cout << sizeof(DWORD) << endl;
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
