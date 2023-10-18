// typeID.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <typeinfo>
using namespace std;

class A {
public:
	A() {};
	~A() {};
};

class AA : public A
{
public:
	AA() {};
	~AA() {};
};

class AB : public A
{
	AB() {};
	~AB() {};
};

int main()
{
    std::cout << "Hello World!\n";
	A *a = new A;
	A *b = new A;
	const type_info &info_a = typeid(a);
	const type_info &info_b = typeid(b);

	cout << info_a.name() << " | " << info_a.raw_name() << " | " << info_a.hash_code() << endl;
	cout << info_b.name() << " | " << info_b.raw_name() << " | " << info_b.hash_code() << endl;

	if (info_a == info_b)
	{
		cout << "a == b" << endl;
	}
	else
	{
		cout << "a != b" << endl;
	}
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
