#ifndef PIPEIPC_H
#define PIPEIPC_H

#ifdef Q_OS_WIN
#ifdef STRICT
typedef void* HANDLE;
#if 0 && (_MSC_VER > 1000)
#define DECLARE_HANDLE(name) struct name##__; typedef struct name##__ *name
#else
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
#endif
#else
typedef PVOID HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#endif
typedef HANDLE* PHANDLE;
#endif
#include <iostream>
#include <windows.h>

enum PIPEUSERTYPE {
	USER_CLIENT, //�ͻ���
	USER_SERVER, //�����
};

class PipeIPC {
public:
	PipeIPC();
	PipeIPC(PIPEUSERTYPE role):m_Role(role),m_hPipeHandle(INVALID_HANDLE_VALUE) {};
	~PipeIPC();

	bool WriteData(const std::string& datas); //д������
	bool ReadData(__in std::string& datas, bool& isPipeEnd); //��ȡ����
	bool InitPipeIPC(); //��ʼ���ܵ�ͨ��

private:
	PIPEUSERTYPE m_Role;
	HANDLE m_hPipeHandle;
};

#endif