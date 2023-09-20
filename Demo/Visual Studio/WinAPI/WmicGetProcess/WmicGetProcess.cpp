// WmicGetProcess.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>
using namespace std;

void getProcess()
{
	cout << __cplusplus << endl;
	system("pause");
	return;
	FILE *file;
	fopen_s(&file,"G:\\allprocess.txt","a+");
	TCHAR c[] = { L"cmd.exe" }; //�����ַ�������ʼ��,cΪ8����,����β��'\0',����һ���ַ�Ϊ'x',
	HANDLE handle; //����CreateToolhelp32Snapshotϵͳ���վ��
	HANDLE handle1; //����Ҫ�������̾��

	//���ϵͳ���վ��
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 *info; //����PROCESSENTRY32�ṹ��ָ

	//PROCESSENTRY32 �ṹ�� dwSize ��Ա���ó� sizeof(PROCESSENTRY32)
	info = new PROCESSENTRY32;
	info->dwSize = sizeof(PROCESSENTRY32);

	//����һ�� Process32First �������ӿ����л�ȡ�����б�
	Process32First(handle, info);

	//�ظ����� Process32Next��ֱ���������� FALSE Ϊֹ
	while (Process32Next(handle, info) != FALSE)
	{
		info->szExeFile; //ָ���������
		wcout << info->szExeFile << endl;
		fprintf(file, "%ls", info->szExeFile);

		//strcmp�ַ����ȽϺ���ͬҪ������ͬ
		if (wcscmp(c, info->szExeFile) == 0)
		{
			//MessageBox (NULL, info->szExeFile, TEXT ("HelloMsg"), 0);
			//PROCESS_TERMINATE��ʾΪ����������,FALSE=�ɼ̳�,info->th32ProcessID=����ID
			handle1 = OpenProcess(PROCESS_TERMINATE, FALSE, info->th32ProcessID);
			//��������
			TerminateProcess(handle1, 0);
		}
	}
	fclose( file );
}


int getProcess2()
{
	int countProcess=0;									//��ǰ����������������
	PROCESSENTRY32 currentProcess;						//��ſ��ս�����Ϣ��һ���ṹ��
	currentProcess.dwSize = sizeof(currentProcess);		//��ʹ������ṹ֮ǰ�����������Ĵ�С
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);//��ϵͳ�ڵ����н�����һ������

	if (hProcess == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot()����ʧ��!\n");
		return -1;
	}

	bool bMore = Process32First(hProcess, &currentProcess);	//��ȡ��һ��������Ϣ
	while(bMore)
	{
		printf("PID=%5u    PName= %ls", currentProcess.th32ProcessID, &currentProcess.szExeFile);	//�������̿��գ�������ʾÿ��������Ϣ
		cout << &currentProcess.szExeFile << endl;
		bMore=Process32Next(hProcess, &currentProcess);	//������һ��
		countProcess++;
	}

	CloseHandle(hProcess);	//���hProcess���
	printf("��������%d������������\n",countProcess);
	system("pause");

	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	getProcess2();
	return 0;
}

