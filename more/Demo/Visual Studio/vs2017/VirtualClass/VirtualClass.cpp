// VirtualClass.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <vld.h>

using namespace std;

class People
{
public:
	int Level;
	int age;
	//int height;
public:
	People() 
	{
		cout << "People init" << endl;
	}
	virtual ~People()
	{
		cout << "People end~" << endl;
	}
	virtual void GetLevel() = 0;
	/*{
		Level = NULL;
		cout << "People:" << Level << endl;
	}*/
	virtual void GetAge()
	{
		Level = NULL;
		cout << "People:" << Level << endl;
	}
};

class Proer : public People
{
public:
	Proer()
	{
		cout << "Proer init" << endl;
	}
	~Proer()
	{
		cout << "Proer end" << endl;
	}
	virtual void GetLevel()
	{
		cout << "Proer:" << Level << endl;
	}
};

class Teaer : public People
{
public:
	Teaer()
	{
		cout << "Teaer init" << endl;
	}
	~Teaer()
	{
		cout << "Teaer end" << endl;
	}
	virtual void GetLevel()
	{
		cout << "Teaer:" << Level << endl;
	}
};


int main()
{
	People *p = NULL;
	People *p1 = NULL;
	People *p2 = NULL;

	cout << sizeof(People) << endl;
	cout << sizeof(int) << endl;

	p = new Proer;
	p1 = p;
	p->Level = 10;
	cout << &(p->Level) << endl;

	delete p;
	
	//  delete p;
	// p->GetLevel();

	cout << endl;
	p = new Teaer;
	p2 = p;
	p->Level = 100;
	cout << &(p->Level) << endl;
	/*delete p;*/

	cout << endl;
	cout << "ȫ������" << endl;
	p1->GetLevel();
	p2->GetLevel();
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
