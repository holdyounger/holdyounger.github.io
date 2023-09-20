// GetRunDir.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

wstring GetRunDir()
{
	wstring Path;
	wchar_t szModule[1024] = { 0 };
	GetModuleFileName(NULL, szModule, sizeof(szModule) / sizeof(szModule[0]));
	Path = szModule;
	Path.erase(Path.find_last_of(L'\\'));
	return Path;
}


int main()
{
    std::wcout << GetRunDir();
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
