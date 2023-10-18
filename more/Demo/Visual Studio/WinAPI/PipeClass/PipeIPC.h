#pragma once

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <Windows.h>
#include <string>

#include <memory>

enum PIPEUSERTYPE {
	USER_CLIENT, //�ͻ���
	USER_SERVER, //�����
};

class PipeIPC {
public:
	PipeIPC();
	~PipeIPC();

	bool InitPipeIPC(std::string pipe_name, PIPEUSERTYPE states); //��ʼ���ܵ�ͨ��

	//��д����
	bool WriteData(std::string datas); //д������
	bool ReadData(std::string& datas, bool& isPipeEnd); //��ȡ����

private:
	std::string m_pipe_name;
	HANDLE m_hPipeHandle;
	PIPEUSERTYPE m_state; //״̬����
};

