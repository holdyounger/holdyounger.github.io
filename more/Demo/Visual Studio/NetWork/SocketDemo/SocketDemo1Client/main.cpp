#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib") // ����ws32_32.dll

int main()
{
	// ��ʼ��DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	// �����׽���
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// ���������������
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr)); // ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));



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
	}


	// �ر��׽���
	closesocket(sock);

	// ��ֹʹ�� DLL
	WSACleanup();

	return 0;
}