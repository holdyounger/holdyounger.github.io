// CharacterTransferNumber.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#define CHECKSUM_MAGIC ((unsigned int)'CKSM')					// 1129009997
#define SYSHASH_MAGIC ((unsigned int)'SHSH')					// 1397248840			// hash
#define INTEGRITY_MAGIC ((unsigned int)'MAGC')					//
#define CHECKSUM_MAGIC ((unsigned int)'CKSM')					//
#define TRUSTFILE_MAGIC	((unsigned int)'TRFL')					//						// trust file(dir)
#define TRUSTPROCESS_MAGIC	((unsigned int)'TRPS')				//						// trust process
#define TRUSTWEAKENPROCESS_MAGIC	((unsigned int)'TRWP')		//						// trust weaken process, it's child process will not have this function
#define PROTECTFILE	((unsigned int)'PRFL')						//						// protected system file(dir)
#define PROTECTFILE_EXCEPTION ((unsigned int )'PRFE')			//						// protected system file exclude
#define PROTECTREG	((unsigned int)'PREG')						//						// protected system register
#define PROTECTREG_K	((unsigned int)'REGK')					//						// protected system register key
#define PROTECTFILE_S	((unsigned int)'PRFS')					//						// protected system file(dir) - from server policy
#define PROTECTREG_S	((unsigned int)'PRES')					//						// protected system register - from server policy
#define PROCESSAUDIT	((unsigned int)'PRAC')					//						// process audit - from client policy
#define PROCESSAUDIT_S	((unsigned int)'PRAS')					//						// process audit - from server policy
#define WORKSHEET_MAGIC ((unsigned int)'WKST')					//
#define HEADER_HASH_MAGIC ((unsigned int)'@#$^')
#define HEADER_FILE_MAGIC ((unsigned int)'^$#@')
#define EMPTY_INPUT ((unsigned int)'ORRE')

#define OBJNAME(A) (#A)<<':'<<(##A)

#define INPUTDATA 0xFEFE
using namespace std;
#pragma warning(disable:4996)

int main()
{
	// cout << INT_MAX << endl;
	std::cout << "���룺" << OBJNAME(INPUTDATA) << endl;
	std::cout << "10����:" << INPUTDATA << endl;
	std::cout << "16����:" << std::hex << INPUTDATA << endl;
	std::cout << "-------------------------------------" << endl;
	__int64 flag = INPUTDATA;
	char str[4] = { 0 };
	char * array = (char *)&flag;
	char buff[9] = { 0 };
	strncpy(buff, array, sizeof flag);
	cout << "ת�������" << buff << endl;
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
