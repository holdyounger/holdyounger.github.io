// GetProcess.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>	//���̿��պ���ͷ�ļ�
#include <stdio.h>

int main()
{
	int countProcess = 0;									//��ǰ����������������
	PROCESSENTRY32W currentProcess;						//��ſ��ս�����Ϣ��һ���ṹ��
	currentProcess.dwSize = sizeof(currentProcess);		//��ʹ������ṹ֮ǰ�����������Ĵ�С
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//��ϵͳ�ڵ����н�����һ������

	if (hProcess == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot()����ʧ��!\n");
		return -1;
	}

	bool bMore = Process32First(hProcess, &currentProcess);	//��ȡ��һ��������Ϣ
	while (bMore)
	{
		printf("PID=%5u    PName= %ls \n", currentProcess.th32ProcessID, currentProcess.szExeFile);	//�������̿��գ�������ʾÿ��������Ϣ
		bMore = Process32Next(hProcess, &currentProcess);	//������һ��
		countProcess++;
	}

	CloseHandle(hProcess);	//���hProcess���
	printf("��������%d������������\n", countProcess);
	system("pause");
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
