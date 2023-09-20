#include "stdafx.h"
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib") // ����ws32_32.dll
using namespace std;

int addPort(vector<SOCKET>& vecSock, int nPort) 
{
	if(nPort < 0 || nPort > 0xFFFF)
	{
		return -1;
	}

	// ��ʼ��DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	// �����׽���
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	vecSock.push_back(sock);

	// ���������������
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr)); // ÿ���ֽڶ���0��� 
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("192.168.3.171");// 192.168.3.171  127.0.0.1
	sockAddr.sin_port = htons(nPort);
	int nRet = connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	
/*
	// ���շ��������ص�����
	char szBuffer[MAXBYTE] = {0};
	recv(sock,szBuffer,MAXBYTE,NULL);

	while (strcmpi(szBuffer,"exit()") != 0)
	{
		if(recv(sock,szBuffer,MAXBYTE,NULL) < 0)
		{
			printf("�ͻ��˶Ͽ�����~\n");
			return 0;
		}

		// ������յ�������
		printf("Message form server��%s\n", szBuffer);
	}*/
	

	return nRet;
}

int main()
{
	vector<SOCKET> vecSock;
ADD:
	int nComd = 2;
	int nPort = -1;
	int nRet = -1;
	printf("CLIENT \t ������˿ںţ������-1���˳�������\n");
	scanf_s("%d", &nPort);
	
	switch (nPort)
	{
	case -1:
		goto END;
		break;
	default:
		nRet = addPort(vecSock, nPort);
		if( 0 != nRet )
		{
			printf("Add this Port(%d) Failed, GetLastError(%d)\n", nPort, GetLastError());
		}
		goto ADD;
	}


END:
	for (int i = 0; i < vecSock.size(); i++)
	{
		if(NULL != vecSock[i])
		{
			// �ر��׽���
			closesocket(vecSock[i]);
		}
	}

	// ��ֹʹ�� DLL
	WSACleanup();

	return 0;
}