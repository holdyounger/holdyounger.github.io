#include <iostream>
#include <WinSock2.h>
#include <string>
#pragma  comment (lib,"ws2_32.lib") // ���� ws32_32.dll
using namespace std;
int main()
{
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

	// ��ͻ��˷�������
	char *str = new char[100];
	gets(str);
	send(clntSock, str, strlen(str)+sizeof(char),NULL);
	while (strcmpi(str,"exit()") != 0)
	{
		gets(str);
		send(clntSock, str, strlen(str)+sizeof(char),NULL);
	}

	// �ر��׽���
	closesocket(clntSock);
	closesocket(servSock);

	return 0;
}