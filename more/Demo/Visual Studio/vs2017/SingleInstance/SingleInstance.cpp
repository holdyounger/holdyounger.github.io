#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
class Printer {
public:
	static Printer * getinstance() {
		return instance;
	}
	void print(string text) {
		cout << "��ӡ������Ϊ��" << text << endl;
		sequence++;
		cout << "��ӡ������ʹ����" << sequence << "��" << endl;
	}
	static int getSequence() {
		return sequence;
	}
private:
	// ����һ����ȥ��������
	class Garbo {
	public:
		~Garbo() {
			if (instance != NULL) {
				printf("����������~~~");
				delete instance;
			}
		}
	};

	static Garbo garbo; // �ھ�̬���򿪱�һ������Garbo
	static int sequence;
	static Printer * instance;
};

int Printer::sequence = 0;
Printer *Printer::instance = new Printer;
Printer::Garbo Printer::garbo;

int main()
{
	//����pepleʹ�ô�ӡ��
	Printer * p1 = Printer::getinstance();
	p1->print("�ڷ����");

	Printer *p2 = Printer::getinstance();
	p2->print("һ�ݹ����㱨");

	Printer *p3 = Printer::getinstance();
	p3->print("��ְ����");

	system("pause");
	return 0;
}