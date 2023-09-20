// fileattribute.cpp: �������̨Ӧ�ó������ڵ㡣
//
//#include "stdafx.h"
#include <string>
#include <iostream>
#include "attribute.h"  

#pragma comment(lib, "version")  	//���б���Ҫ����

using namespace std;

bool QueryValue(const std::string& ValueName, const std::string& szModuleName, std::string& RetStr)
{
	bool bSuccess = FALSE;
	BYTE*  m_lpVersionData = NULL;
	DWORD   m_dwLangCharset = 0;
	CHAR *tmpstr = NULL;

	do
	{
		if (!ValueName.size() || !szModuleName.size())
			break;

		DWORD dwHandle;
		// �ж�ϵͳ�ܷ������ָ���ļ��İ汾��Ϣ
		DWORD dwDataSize = ::GetFileVersionInfoSizeA((LPCSTR)szModuleName.c_str(), &dwHandle);
		if (dwDataSize == 0)
			break;

		m_lpVersionData = new (std::nothrow) BYTE[dwDataSize];// ���仺����
		if (NULL == m_lpVersionData)
			break;

		// ������Ϣ
		if (!::GetFileVersionInfoA((LPCSTR)szModuleName.c_str(), dwHandle, dwDataSize,
			(void*)m_lpVersionData))
			break;

		UINT nQuerySize;
		DWORD* pTransTable;
		// ��������
		if (!::VerQueryValueA(m_lpVersionData, "\\VarFileInfo\\Translation", (void **)&pTransTable, &nQuerySize))
			break;

		m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
		if (m_lpVersionData == NULL)
			break;

		tmpstr = new (std::nothrow) CHAR[128];// ���仺����
		if (NULL == tmpstr)
			break;
		sprintf_s(tmpstr, 128, "\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, ValueName.c_str());
		LPVOID lpData;

		// ���ô˺�����ѯǰ��Ҫ�����ε��ú���GetFileVersionInfoSize��GetFileVersionInfo
		if (::VerQueryValueA((void *)m_lpVersionData, tmpstr, &lpData, &nQuerySize))
			RetStr = (char*)lpData;

		bSuccess = TRUE;
	} while (FALSE);

	// ���ٻ�����
	if (m_lpVersionData)
	{
		delete[] m_lpVersionData;
		m_lpVersionData = NULL;
	}
	if (tmpstr)
	{
		delete[] tmpstr;
		tmpstr = NULL;
	}

	return bSuccess;
}

bool	BaseFlow::Attribute::GetFileDescription(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("FileDescription", szModuleName, RetStr);
};   //��ȡ�ļ�˵��

bool	BaseFlow::Attribute::GetFileVersion(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("FileVersion", szModuleName, RetStr);
};	   //��ȡ�ļ��汾	

bool	BaseFlow::Attribute::GetInternalName(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("InternalName", szModuleName, RetStr);
};	   //��ȡ�ڲ�����
bool	BaseFlow::Attribute::GetCompanyName(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("CompanyName", szModuleName, RetStr);
};	   //��ȡ��˾����
bool	BaseFlow::Attribute::GetLegalCopyright(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("LegalCopyright", szModuleName, RetStr);
};    //��ȡ��Ȩ
bool	BaseFlow::Attribute::GetOriginalFilename(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("OriginalFilename", szModuleName, RetStr);
};  //��ȡԭʼ�ļ���
bool	BaseFlow::Attribute::GetProductName(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("ProductName", szModuleName, RetStr);
};	   //��ȡ��Ʒ����
bool	BaseFlow::Attribute::GetProductVersion(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("ProductVersion", szModuleName, RetStr);
};    //��ȡ��Ʒ�汾
