// wsCmp.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <string>
using namespace std;
#define  test(p, Z) do { p.#Z = Z; } while(0)
#define one(var)  #var
#define _double(var) nVar##1
struct people {
	int age;
	string name;
};

int main()
{
	people p;
    std::cout << "Hello World!\n";
	string name = "Hello";
	cout << one(name) << endl;

	int nVar1 = 1;
	cout << "nvar##1 = (" << _double(1) << ')' << endl;

	wstring wstr1 = L"asddsa";
	wstring wstr2 = L"asddsa";
	cout << wstr1 == wstr2 << endl;

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
