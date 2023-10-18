// GetFullPathProcess.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>	//���̿��պ���ͷ�ļ�
#include <stdio.h>
#include <string>

#include <tchar.h>
#include <Psapi.h>
#pragma comment (lib,"Psapi.lib")

using namespace std;
BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
{
	TCHAR			szDriveStr[500];
	TCHAR			szDrive[3];
	TCHAR			szDevName[100];
	INT				cchDevName;
	INT				i;

	//������
	if (!pszDosPath || !pszNtPath)
		return FALSE;

	//��ȡ���ش����ַ���
	if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
	{
		for (i = 0; szDriveStr[i]; i += 4)
		{
			if (!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
				continue;

			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			if (!QueryDosDevice(szDrive, szDevName, 100))//��ѯ Dos �豸��
				return FALSE;

			cchDevName = lstrlen(szDevName);
			if (_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//����
			{
				lstrcpy(pszNtPath, szDrive);//����������
				lstrcat(pszNtPath, pszDosPath + cchDevName);//����·��

				return TRUE;
			}
		}
	}

	lstrcpy(pszNtPath, pszDosPath);

	return FALSE;
}

//��ȡ��������·��
BOOL GetProcessFullPath(DWORD dwPID, TCHAR pszFullPath[MAX_PATH])
{
	TCHAR		szImagePath[MAX_PATH];
	HANDLE		hProcess;
	if (!pszFullPath)
		return FALSE;

	pszFullPath[0] = '\0';
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwPID);
	if (!hProcess)
	{
		return FALSE;
	}	

	if (!GetProcessImageFileName(hProcess, szImagePath, MAX_PATH))
	{
		CloseHandle(hProcess);
		return FALSE;
	}

	if (!DosPathToNtPath(szImagePath, pszFullPath))
	{
		CloseHandle(hProcess);
		return FALSE;
	}

	CloseHandle(hProcess);

	_tprintf(_T("PID = %d, %s \r\n"), dwPID, pszFullPath);
	return TRUE;
}

string GetProcessInfo(__in HANDLE hProcess, __in WCHAR* processName)
{
	PROCESSENTRY32* pinfo = new PROCESSENTRY32; //������Ϣ ��pinfo->dwSize = sizeof(PROCESSENTRY32);��
	MODULEENTRY32* minfo = new MODULEENTRY32; //ģ����Ϣ ��minfo->dwSize = sizeof(MODULEENTRY32);��
	char shortpath[MAX_PATH];				//����·������

	int flag = Process32First(hProcess, pinfo);	// �ӵ�һ�����̿�ʼ
	while (flag) {

		if (wcscmp(pinfo->szExeFile, processName) == 0) {

			// �������̿���
			HANDLE hModule = CreateToolhelp32Snapshot(
				TH32CS_SNAPMODULE,		//��DWORD�� ���շ��صĶ���TH32CS_SNAPMODULE ��ָ "�ض����̵�ʹ��ģ����б�"
				pinfo->th32ProcessID	//��DWORD�� Ҫ��ȡ���ս��̵�PID����ǰ����/ϵͳ�б� ����ʱ��Ϊ0
			);

			// �ѵ�һ��ģ����Ϣ�� minfo
			Module32First(
				hModule,  //��HANDLE�� CreateToolhelp32Snapshot �ķ��ؾ��
				minfo	 // ��LPMODULEENTRY32��  ����ģ����Ϣ
			);

			// ���ļ�·���� shortpath
			GetShortPathName(
				minfo->szExePath,	//  �ļ�·��������ò�Ҫ���������Ϊ����������Ļ�������룩
				(LPWSTR)shortpath,		// �������� minfo->szExePath �������ĵ�ֵ
				256			// ��������С
			);

			return shortpath;
		}

		// ��һ������
		flag = Process32Next(hProcess, pinfo);
	}

	return NULL;
}

int getAllProcess()
{
	int countProcess = 0;	//��ǰ����������������
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // �������̿���
	PROCESSENTRY32W currentProcess;	// �������� hProcessSnap ����Ϣ

	currentProcess.dwSize = sizeof(currentProcess);		//��ʹ������ṹ֮ǰ�����������Ĵ�С
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//��ϵͳ�ڵ����н�����һ������

	if (hProcess == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot()����ʧ��!\n");
		return -1;
	}

	bool bMore = Process32First(hProcessSnap, &currentProcess);	//��ȡ��һ��������Ϣ
	while (bMore) {
		TCHAR szProcessName[MAX_PATH] = { 0 };
		GetProcessFullPath(currentProcess.th32ProcessID, szProcessName);
		bMore = Process32Next(hProcessSnap, &currentProcess);	//������һ��	
		countProcess++;
	}

	CloseHandle(hProcess);	//���hProcess���
	printf("��������%d������������\n", countProcess);
}

int main()
{
	getAllProcess();
	return 0;
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
