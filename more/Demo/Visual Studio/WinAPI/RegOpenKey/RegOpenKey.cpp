// RegOpenKey.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <atlstr.h>
#include <vector>
#include <iostream>
#include <WinError.h>
#include <string>
#include <strsafe.h>  

using namespace std;

#pragma warning(disable:4996)

void wchar2strstring(std::string & szDst,WCHAR * wchart)
{
	wchar_t * wtext = wchart;
	DWORD dwNmu = WideCharToMultiByte(CP_OEMCP,NULL,wtext,-1,NULL,0, NULL,FALSE);
	char * psTest;
	psTest = new char[dwNmu];
	WideCharToMultiByte(CP_OEMCP, NULL, wtext, -1, psTest, dwNmu, NULL, FALSE);
	szDst = psTest;
	delete[]psTest;
}


void _tmain(int argc, char* argv[])
{
	HKEY hKey = NULL;
	
	//KEY_READ | KEY_WOW64_32KEY   HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Run
	DWORD samDesire = KEY_READ | KEY_WOW64_64KEY; 
	LPCTSTR lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	DWORD  dwErr;
	if (dwErr = RegOpenKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey) != ERROR_SUCCESS)
	{
		printf("RegOpenKeyExW Failed!!\n");
		// FORMAT_MESSAGE_FROM_SYSTEM 
		{
			const DWORD size = 1000+1;
			WCHAR buffer[size];
			LPVOID lpMsgBuf; 
			CHAR szBuf[1280];

			if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER ,
				NULL, 
				dwErr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  
				buffer,  
				size, NULL))
			{
				wprintf(L"Format message failed with 0x%x\n", GetLastError());
				return;
			}

			printf("dwErr:%d\ngetlastError:%d\n",dwErr,GetLastError());
			wprintf(L"----------%s\n", buffer);
			

			string strTemp;
			wchar2strstring(strTemp, buffer);
			sprintf(szBuf,
                    "%s ������Ϣ (�����룽%d): %s",
                    "RegOpenKeyExW", dwErr, strTemp.c_str());
			// OutputDebugString(szBuf);
			printf("%s\n", szBuf);

			
		}

		return;
	}

	FILETIME timeFile;
	long nError = 0;
	VALENT pValent[MAX_PATH];
	DWORD nValentCount = MAX_PATH;
	memset(pValent, 0, sizeof(pValent));

	// 1.��ȡ�������
	DWORD dwSizeValueName = MAX_PATH;
	DWORD dwSizeofKeyName = MAX_PATH;
	DWORD dwIndex = 0;
	DWORD dwType;
	TCHAR szValueName[MAX_PATH] = { 0 };
	BYTE szKeyName[MAX_PATH] = { 0 };
	vector<wstring> keyName;

	int nCount = 0;
	do
	{
		nError = RegEnumValueW(hKey, dwIndex++, szValueName, &dwSizeValueName, NULL, &dwType, szKeyName, &dwSizeofKeyName);
		if (nError == ERROR_SUCCESS)
		{
			wprintf(L"Key Name = %s\n", (wchar_t *)szValueName);
			//wprintf(L"Key Name = %s\n", (wchar_t *)szKeyName);
			wchar_t wsStr[128];
			_wcsncpy_l(wsStr, (wchar_t *)szValueName,sizeof(szValueName)/sizeof(szValueName[0]),0);
			keyName.push_back(wsStr);

			//VALENT pVal;
			//pVal.ve_valuename = (LPWSTR)wsStr.c_str();//����ֵ������
		}

		dwSizeValueName = MAX_PATH;
		dwSizeofKeyName = MAX_PATH;

	} while (nError != ERROR_NO_MORE_ITEMS);

	for (nCount = 0; nCount <  keyName.size(); nCount++)
	{
		pValent[nCount].ve_valuename = (LPWSTR)keyName[nCount].c_str();
	}

	// 2.���ݼ�����ȡֵ
	{
		char szVale[1024];
		DWORD nValueLen = 1024;

		printf("2.���ݼ�����ȡֵ Success!\n");

		if (ERROR_SUCCESS == (nError = RegQueryMultipleValues(hKey, pValent, nCount, (LPWSTR)szVale, &nValueLen)))
		{

			printf("RegQueryMultipleValues Success!\n");

			CString szResult;
			printf("------------------------------------------\n");
			for (int i = 0; i < nCount; ++i)
			{

				CString szTemp;
				wcout.imbue(std::locale("chs"));

				/*if (REG_SZ == pVal.ve_type)
				{

					szTemp.Format(_T("��%d������[%s]��ֵ��:%s\r"), i,
						pVal.ve_valuename, (char*)pVal.ve_valueptr);

					wcout << "s=" << szTemp.GetString() << endl;
				}
				else if (REG_DWORD == pVal.ve_type)
				{
					szTemp.Format(_T("��%d������[%s]��ֵ��:%d\r"), i,
						pVal.ve_valuename, *((DWORD*)pVal.ve_valueptr));

					wcout << "s=" << szTemp.GetString() << endl;
				}*/



				if (REG_SZ == pValent[i].ve_type)
				{

					szTemp.Format(_T("��%d������[%s]��ֵ��:%s\r"), i,
						pValent[i].ve_valuename, (char*)pValent[i].ve_valueptr);

					wcout << "s=" << szTemp.GetString() << endl;
				}
				else if (REG_DWORD == pValent[i].ve_type)
				{
					szTemp.Format(_T("��%d������[%s]��ֵ��:%d\r"), i,
						pValent[i].ve_valuename, *((DWORD*)pValent[i].ve_valueptr));

					wcout << "s=" << szTemp.GetString() << endl;
				}


				if (!szTemp.IsEmpty())

					szResult += szTemp;
			}
			printf("------------------------------------------\n");

		}
		else
		{
			printf("erroe code: %d", nError);
		}


	// Close registry keys.
	if (hKey)
		RegCloseKey(hKey);

	}
	
}
