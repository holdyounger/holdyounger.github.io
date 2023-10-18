// GetIsDeleteDefaultRot.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "define.h"

#include <windows.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <Shlwapi.h>
#include <atlstr.h>

#define WL_INI_SECDETECT_CONFIG  _T("\\config\\SecDetectConfig.ini") 

#pragma comment(lib, "iphlpapi.lib")

std::wstring GetRunDir()
{
	std::wstring wsPath;
	TCHAR szModule[1024];
	memset(szModule, 0, sizeof(szModule));
	GetModuleFileName(NULL, szModule, sizeof(szModule) / sizeof(szModule[0]));
	PathRemoveFileSpec(szModule);
	wsPath = szModule;
	return wsPath;
}

CString SwitchStatusStr(int iSwitch)
{
	CString strSwitch;
	strSwitch.Format(_T("%d"), iSwitch);
	return strSwitch;
}

/*
* @fn           DeleteDefaultRout
* @brief        ɾ��/�ָ�Ĭ������
* @param[in]    dwDeleteIpForwardEntry: Ϊ`1`��ʾҪ�ָ�Ĭ��·�ɣ�Ϊ`0`��ʾɾ��Ĭ��·��
* @param[out]
* @return       BOOL:  ����ζ��������Ĭ��·��״̬�����Ҫ��һ�»��߲��������Ҫ��һ�£��򷵻�TRUE��
					��֮���򷵻�FALSE��������Ĭ��·��״̬�����Ҫ��һ��
*
* @detail
* @author      mingming.shi
* @date        2021-08-27
*/
BOOL GetIsDeleteDefaultRout(DWORD bToDeleteorAdd = 2)
{
	BOOL bRet = FALSE;
	// Declare and initialize variables
	PMIB_IPFORWARDTABLE pIpForwardTable = NULL;
	PMIB_IPFORWARDROW pRow = NULL; // Ĭ�����أ����ʱʹ��
	DWORD dwSize = 0;
	BOOL bOrder = FALSE;
	DWORD dwStatus = 0;

	// Ĭ��·�ɱ���·��
	std::wstring wsFilePath = L"c:\\2\\1.ini";

	// Identify the required size of the buffer.
	dwStatus = GetIpForwardTable(pIpForwardTable, &dwSize, bOrder);
	if (dwStatus == ERROR_INSUFFICIENT_BUFFER)
	{
		// Allocate memory for the table.
		if (!(pIpForwardTable = (PMIB_IPFORWARDTABLE)malloc(dwSize)))
		{
			printf(("pipForwardTable Malloc failed. Out of memory.\n"));
			goto _END_;
		}
		// Retrieve the table.
		dwStatus = GetIpForwardTable(pIpForwardTable, &dwSize, bOrder);
	}

	if (dwStatus != ERROR_SUCCESS)
	{
		printf(("getIpForwardTable failed.\n"));
		if (pIpForwardTable)
		{
			free(pIpForwardTable);
		}
		goto _END_;
	}

	// Search for the required row in the table. The default gateway has a destination
	// of 0.0.0.0. Be aware the table continues to be searched, but only
	// one row is copied. This is to ensure that, if multiple gateways exist, all of them are deleted.
	for (int i = 0; i < pIpForwardTable->dwNumEntries; i++)
	{
		if (pIpForwardTable->table[i].dwForwardDest == 0)
		{
			// Delete the old default gateway entry.
			if (bToDeleteorAdd == 0)
			{
				printf("��ǰ����ɾ��Ĭ��·��\n");
				if (!pRow) {
					// Allocate some memory to store the row in; this is easier than filling
					// in the row structure ourselves, and we can be sure we change only the
					// gateway address.
					pRow = (PMIB_IPFORWARDROW)malloc(sizeof(MIB_IPFORWARDROW));
					if (!pRow) {
						printf("Malloc failed. Out of memory.\n");
						exit(1);
					}
					// Copy the row
					memcpy(pRow, &(pIpForwardTable->table[i]),
						sizeof(MIB_IPFORWARDROW));
				}

				// �ȱ���Ĭ��·�������Ϣ
				printf("��ʼ����Ĭ��·��\n");
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardDest"), SwitchStatusStr(pRow->dwForwardDest), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardMask"), SwitchStatusStr(pRow->dwForwardMask), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardPolicy"), SwitchStatusStr(pRow->dwForwardPolicy), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardNextHop"), SwitchStatusStr(pRow->dwForwardNextHop), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardIfIndex"), SwitchStatusStr(pRow->dwForwardIfIndex), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardType"), SwitchStatusStr(pRow->dwForwardType), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardProto"), SwitchStatusStr(pRow->dwForwardProto), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardAge"), SwitchStatusStr(pRow->dwForwardAge), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardNextHopAS"), SwitchStatusStr(pRow->dwForwardNextHopAS), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardMetric1"), SwitchStatusStr(pRow->dwForwardMetric1), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardMetric2"), SwitchStatusStr(pRow->dwForwardMetric2), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardMetric3"), SwitchStatusStr(pRow->dwForwardMetric3), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardMetric4"), SwitchStatusStr(pRow->dwForwardMetric4), wsFilePath.c_str());
				WritePrivateProfileString(_T("RECOVER"), _T("dwForwardMetric5"), SwitchStatusStr(pRow->dwForwardMetric5), wsFilePath.c_str());

				// ɾ��Ĭ��·��
				dwStatus = DeleteIpForwardEntry(&(pIpForwardTable->table[i]));
				if (dwStatus == NO_ERROR)
				{
					printf(("DeleteIpForwardEntry Success!"));
				}
				else
				{
					printf(("DeleteIpForwardEntry Error: %d\n"), dwStatus);
				}
				printf("ɾ������\n");
			}

			bRet = TRUE;
			goto _END_;
		}
	}

	if (bToDeleteorAdd == 1) // �ָ�Ĭ��·��
	{
		printf("Ҫ�ָ�Ĭ��·��\n");
		pRow = (PMIB_IPFORWARDROW)malloc(sizeof(MIB_IPFORWARDROW));

		pRow->dwForwardDest = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardDest"), -1, wsFilePath.c_str());
		pRow->dwForwardMask = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardMask"), -1, wsFilePath.c_str());
		pRow->dwForwardPolicy = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardPolicy"), -1, wsFilePath.c_str());
		pRow->dwForwardNextHop = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardNextHop"), -1, wsFilePath.c_str());
		pRow->dwForwardIfIndex = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardIfIndex"), -1, wsFilePath.c_str());
		pRow->dwForwardType = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardType"), -1, wsFilePath.c_str());
		pRow->dwForwardProto = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardProto"), -1, wsFilePath.c_str());
		pRow->dwForwardAge = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardAge"), -1, wsFilePath.c_str());
		pRow->dwForwardNextHopAS = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardNextHopAS"), -1, wsFilePath.c_str());
		pRow->dwForwardMetric1 = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardMetric1"), -1, wsFilePath.c_str());
		pRow->dwForwardMetric2 = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardMetric2"), -1, wsFilePath.c_str());
		pRow->dwForwardMetric3 = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardMetric3"), -1, wsFilePath.c_str());
		pRow->dwForwardMetric4 = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardMetric4"), -1, wsFilePath.c_str());
		pRow->dwForwardMetric5 = GetPrivateProfileIntW(_T("RECOVER"), _T("dwForwardMetric5"), -1, wsFilePath.c_str());

		// Create a new route entry for the default gateway.
		dwStatus = CreateIpForwardEntry(pRow);
	}

_END_:
	if (pRow)
	{
		free(pRow);
	}
	// Free the memory. 
	if (pIpForwardTable)
	{
		free(pIpForwardTable);
	}
	return bRet;
}

BOOL SetIsDeleteDefaultRout(DWORD dwDeleteIpForwardEntry)
{
	BOOL bRet = FALSE;
	BOOL bDefaultIp = FALSE;

	if (dwDeleteIpForwardEntry == 1 || dwDeleteIpForwardEntry == 0 )
	{
		printf("��ʼִ�У���ǰ����(%d)\n",dwDeleteIpForwardEntry);
	}
	else
	{
		printf(("SetIsDeleteDefaultRout wrong parameter(%d)\n"),dwDeleteIpForwardEntry);
		goto _END_;
	}

	bDefaultIp = GetIsDeleteDefaultRout();

	if (bDefaultIp == dwDeleteIpForwardEntry)
	{
		printf("��ǰ����״̬���������Ҫ�󣬳��������\n");
		bRet = TRUE;
		goto _END_;
	}
	else
	{
		// dwDeleteIpForwardEntry == 1 // �ָ�Ĭ��·��
		// dwDeleteIpForwardEntry == 0 // ɾ��Ĭ��·��
		GetIsDeleteDefaultRout(dwDeleteIpForwardEntry);
	}

_END_:
	return bRet;
}

int GetIsDeleteDftRoute()
{
    std::cout << "��������Ҫִ�еĲ���(����)��\n0. ɾ��Ĭ��·��\n1. �ָ�Ĭ��·��\n��������: ��ѯĬ��·��";
	DWORD dwIn;
	cin >> dwIn;
	return SetIsDeleteDefaultRout(dwIn);
}