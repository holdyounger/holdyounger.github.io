#include "PipeIPC.h"
using namespace std;

//ʵ��
PipeIPC::PipeIPC()
{
	m_pipe_name = ""; //���ÿ�
	m_hPipeHandle = nullptr;
	m_state = USER_CLIENT; //Ĭ��Ϊ�ͻ���
}

PipeIPC::~PipeIPC()
{
	DisconnectNamedPipe(m_hPipeHandle);
	CloseHandle(m_hPipeHandle);//�رվ��
}

bool PipeIPC::InitPipeIPC(std::string pipe_name, PIPEUSERTYPE states)
{
	m_state = states;
	std::string spice_name = "\\\\.\\pipe\\";//��Ҫƴ�ӵ��ַ���
	spice_name.append(pipe_name); //���
	m_pipe_name.append(spice_name);
	if (m_state == USER_SERVER)
	{
		m_hPipeHandle = CreateNamedPipeA(m_pipe_name.c_str(), PIPE_ACCESS_DUPLEX, //˫��ģʽ
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
			PIPE_UNLIMITED_INSTANCES,//�����������ʵ����Ŀ
			0,
			0,
			NULL, nullptr);
		//����ܵ�����ʧ��
		if (m_hPipeHandle == INVALID_HANDLE_VALUE)
		{
			cout << "" << endl;
			return false;
		}
		//���سɹ�
		cout << "��ʼ�������PIPE�ɹ���" << endl;
	}
	else
	{
		if (WaitNamedPipeA(m_pipe_name.c_str(), NMPWAIT_WAIT_FOREVER) == FALSE)
			return 0;


		if (FALSE == ConnectNamedPipe(m_hPipeHandle, NULL))
		{

		}

		m_hPipeHandle = CreateFileA(

			m_pipe_name.c_str(), //������򿪵Ķ���(�ܵ�)����

			GENERIC_READ | //����ķ��ʷ�ʽ��������

			GENERIC_WRITE, //����ķ��ʷ�ʽ��д����

			0, //�����Ƿ���0��ʾ������

			NULL, //ָ��һ��SECURITY_ATTRIBUTES�ṹ��ָ��

			OPEN_EXISTING, //����Ĵ�����ʽ��OPEN_EXISTING��ʾ�򿪶���(�ܵ�)

			FILE_ATTRIBUTE_NORMAL, //���ö�������Ժͱ�־

			NULL);
		//����ܵ�����ʧ��
		if (m_hPipeHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		//���سɹ�
		cout << "��ʼ���ͻ���PIPE�ɹ���" << endl;
	}

	return true;
}

//��ȡ����
bool PipeIPC::ReadData(std::string& datas, bool& isPipeEnd)
{
	DWORD rLen = 0;//�������ݳ���
	//��ȡʱ����
	char szBuffer[4096] = { 0 };
	if (!ReadFile(m_hPipeHandle, szBuffer, 4096, &rLen, nullptr))
	{

		DWORD error = GetLastError();
		isPipeEnd = (error == ERROR_BROKEN_PIPE) ? true : false;
		//cout << "��ȡʧ��: error:" << error << endl;
		return false;
	}
	cout << "��ȡ����Ϊ:" << szBuffer << endl;
	datas = szBuffer;
	return true;
}

//д������
bool PipeIPC::WriteData(std::string datas)
{
	if (m_state == USER_SERVER)
	{
		bool connect = ConnectNamedPipe(m_hPipeHandle, NULL);
		DWORD error = GetLastError();
		if (!connect)
		{
			if (error != ERROR_PIPE_CONNECTED)
			{
				cout << "�ͻ��˻�û����" << endl;
				//����ʧ��
				return false;
			}
		}
	}
	//���ӳɹ���
	DWORD wLen;
	//��ȡ����
	WriteFile(m_hPipeHandle, datas.c_str(), strlen(datas.c_str()), &wLen, NULL);
	return true;
}