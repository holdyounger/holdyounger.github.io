// server.cpp : �������̨Ӧ�ó������ڵ㡣
//

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
	if(nPort < 0 || nPort > 0xFF)
	{
		return -1;
	}

	// ��ʼ�� DLL
	WSADATA wsaData;
	WSAStartup( MAKEWORD(2,2), &wsaData);

	// �����׽���
	SOCKET servSock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);

	// ���׽���
	struct sockaddr_in sockAddr;
	memset (&sockAddr, 0, sizeof(sockAddr)); // ÿ���׽��ֶ���0���
	sockAddr.sin_family = PF_INET; // ʹ��IPv4��ַ
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);	// �˿�
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	// �������״̬
	listen(servSock,20);

	// ���տͻ�������
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	vecSock.push_back(clntSock);
	vecSock.push_back(servSock);
	
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{

	vector<SOCKET> vecSock;
ADD:
	int nComd = 2;
	int nPort = -1;
	int nRet = -1;
	printf("SERVER \t ������˿ںţ������-1���˳�������\n");
	scanf_s("%d", &nPort);

	switch (nPort)
	{
	case -1:
		goto END;
		break;
	default:
		if( 0 != addPort(vecSock, nPort) )
		{
			printf("Add this Port(%d)\n", nPort, GetLastError());
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
			closesocket(vecSock[i]);
		}
	}

	// ��ֹʹ�� DLL
	WSACleanup();

	return 0;
	return 0;
}

