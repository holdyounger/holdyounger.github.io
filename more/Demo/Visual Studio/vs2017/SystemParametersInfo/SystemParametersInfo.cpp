// SystemParametersInfo.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "user32.lib")    
using namespace std;


void SaveOriginData()
{
	std::string strJson;
	DWORD dwAct;
	DWORD dwSec;
	DWORD dwTime;

	BOOL bAct;
	BOOL bSec;
	BOOL bRet;

	std::wstring wstrEXE;
	std::string strEXE;

	bRet = SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &bAct, 0);
	if (!bRet)
	{
		printf(("BaseLineScrnSave::RestoreOriginData Set 'SPI_SETSCREENSAVEACTIVE' Failed"));
	}

	bRet = SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &dwTime, 0);
	if (!bRet)
	{
		printf(("BaseLineScrnSave::RestoreOriginData Set 'SPI_SETSCREENSAVETIMEOUT' Failed"));
	}
	bRet = SystemParametersInfo(SPI_GETSCREENSAVESECURE, 0, &bSec, 0);
	if (!bRet)
	{
		printf(("BaseLineScrnSave::RestoreOriginData Set 'SPI_SETSCREENSAVESECURE' Failed"));
	}

	dwAct = bAct == TRUE ? 1 : 0; // 1 ��ʾ����
	dwSec = bSec == TRUE ? 1 : 0; // 1 ��ʾ����

	cout << "�ָ�ʱ��¼��" << dwSec << '\t' << dwTime << '\t' << dwAct << endl;

}

void main()
{
	SaveOriginData();
	BOOL fResult;
	int nTimeOut;
	BOOL bActive;
	DWORD dwTime = 0;
	DWORD dwRet;
	fResult = SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT,
		0,
		&dwTime,
		0);

	dwRet = (dwTime == 60) ? 1 : 0;

	cout << "��ǰϵͳϢ��ʱ��:" << dwTime << ':' << dwRet << endl;
	fResult = SystemParametersInfo(SPI_GETSCREENSAVEACTIVE,
		0,
		&bActive,
		0);

	// Double it. 
	cout << "ϵͳ����״̬" << ((bActive == 1) ? "����" : "δ����") << "\n����������ʱ�䣺";
	cin >> nTimeOut;
	cout << endl;

	bActive = bActive == 0 ? 1 : 1;

	if (fResult)
	{

		fResult = SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT,      // Set mouse information
			nTimeOut,
			0,        // Mouse information
			SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);  // Update Win.ini
		cout << ((fResult == 1) ? "���óɹ�" : "����ʧ��") << endl;
		fResult =  SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,
			bActive,
			0,
			SPIF_SENDCHANGE | SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
		cout << ((fResult == 1) ? "���óɹ�" : "����ʧ��") << endl;
		fResult = SystemParametersInfo(SPI_SETSCREENSAVESECURE,
			bActive,
			0,
			SPIF_SENDCHANGE | SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
		cout << ((fResult == 1) ? "���óɹ�" : "����ʧ��") << endl;
	}

	int i = 0;
	while (i < nTimeOut)
	{
		cout << i+1 << '/' << nTimeOut << endl;
		i++;
		Sleep(1000);
	}

	return;
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
