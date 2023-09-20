// set.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
// Set.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include "vld.h"
#include <set>
#include <vector>
using namespace std;

struct man	 
{
	int age;
	int height;
	bool isContain(int i)
	{
		if (this->age == i)
		{
			return true;
		}
		return false;
	}
};


void testSet()
{
	std::cout << "Hello World!\n";

	set<int> setAge;

	for (int i = 0; i < 10; i++)
	{
		setAge.insert(i);
	}
	set<int>::iterator it = setAge.begin();
	for (; it != setAge.end(); it++)
	{
		cout << *it << " ";
	}
	if (setAge.end() != setAge.find(10))
	{
		cout << "�ҵ���" << endl;
	}
	else
	{
		cout << "û�ҵ�" << endl;
	}

	STU_PORT_POOL stuDynamicPort;
	int nPortsNum = 0;
	stuDynamicPort.nStartPort = 50;
	stuDynamicPort.nRange = 20;
	set<int> setMan;
	for (int i = 0; i < 100; i++)
	{
		setMan.insert(i);
	}

	it = setMan.begin();
	for (; it != setMan.end(); it++)
	{
		if (stuDynamicPort.isContain(*it))
		{
			nPortsNum++;
		}
	}
	cout << "�ܹ���" << nPortsNum << "�����ַ�������" << endl;
}

void testVector()
{
	vector<man> mans;

	for (int i = 0; i < 10; i++)
	{
		man aMen;
		aMen.age = i + 1;
		aMen.height = i + 170;

		mans.push_back(aMen);
	}

	vector<man>::iterator it = mans.begin();
	for (; it != mans.end(); it++)
	{
		if (it->isContain((int)1))
		{
			cout << "11111111111111111" << endl;
		}
		cout << it->age << ' ' << it->height << endl;
	}

	it = mans.begin();
	for (; it != mans.end(); it++)
	{
		if (it->age >= 6)
		{
			it = mans.erase(it);
			it--;
		}
	}
	cout << "---------------" << endl;


	it = mans.begin();
	for (; it != mans.end(); it++)
	{
		cout << it->age << ' ' << it->height << endl;
	}

	cout << "---------------" << endl;
	man newMan = *(mans.begin() + 3);
	cout << newMan.age << ' ' << newMan.height << endl;

}


int main()
{
	int num = 0;
	STU_PORT_POOL stuPort;

	if (NO_ERROR == WNTPORT::getPortPoolStatus())
	{
		cout << "num:" << num << endl;
	}
	else
	{
		cout << "��ȡʧ��"<< endl;
	}
	
	WNTPORT::getPort(stuPort);
	cout << "start port:" << stuPort.nStartPort << endl;
	cout << "range:" << stuPort.nRange << endl;
	cout << "Enable:" << stuPort.bEnable << endl;

	stuPort.bEnable = false;
	WNTPORT::SetPortIsAvailable(stuPort);
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
