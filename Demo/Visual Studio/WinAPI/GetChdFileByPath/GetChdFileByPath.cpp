// GetChdFileByPath.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <io.h>
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <ShObjIdl.h>
#include <cstring>
#include <shlobj.h>
using namespace std;

#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")

wstring getLnkFormPath(wstring lnkPath)
{
	cout << "getLnkFromPath:";
	//wstring wstr2=UNICODE(lnkPath);
	wcout.imbue(locale("chs"));
	wcout<<lnkPath<<endl;
	cout << endl;

	LPCOLESTR str = lnkPath.c_str();

	// ��ʼ��
	wstring sRet;
	wchar_t wRet[MAX_PATH];
	WIN32_FIND_DATA  fd;

	// ��ʼ�� COM ��
	CoInitialize(NULL);
	IPersistFile* pPF = NULL;

	// ���� COM ����
	HRESULT hr = CoCreateInstance(
		CLSID_ShellLink,			// CLSID
		NULL,						// IUnknown �ӿ�ָ��
		CLSCTX_INPROC_SERVER,		// CLSCTX_INPROC_SERVER���� Dll �ķ�ʽ��������� 
		IID_IPersistFile,			// COM ����ӿڱ�ʶ��
		(void**)(&pPF)				// ���� COM �����ָ��
		);
		
	if(FAILED(hr)){cout << "CoCreateInstance failed." << endl;}

	// �ж��Ƿ�֧�ֽӿ�
	IShellLink* pSL = NULL;
	hr = pPF->QueryInterface(
		IID_IShellLink,				// �ӿ� IID
		(void**)(&pSL)				// ����ָ������ӿں�������ָ��
		);
	if(FAILED(hr)){cout << "QueryInterface failed." << endl;}

	// ���ļ�
	hr = pPF->Load(
		str,					// �ļ�ȫ·��
		STGM_READ					// ����ģʽ��ֻ��
		);
	if(FAILED(hr)){cout << "Load failed ��" << GetLastError() << endl;}

	// ��ȡ Shell ������Դ
	hr = pSL->GetPath(wRet, MAX_PATH, &fd, 0);
	sRet = wRet;

	// �ر� COM ��
	pPF->Release();
	CoUninitialize();

	return sRet;
} 


void getFiles(std::string path, std::vector<std::string>& files, std::vector<std::string>& names) 
{
	//�ļ������win10��long long��win7��long�Ϳ�����
	long hFile = 0;
	//�ļ���Ϣ 
	struct _finddata_t fileinfo; 
	std::string p; 
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) 
	{ 
		do 
		{ 
			//�����Ŀ¼,����֮ //�������,�����б� 
			if ((fileinfo.attrib & _A_SUBDIR)) 
			{ 
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) 
				{ 
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files, names);
				} 
			}
			else 
			{ 
				files.push_back(p.assign(path).append("\\").append(fileinfo.name)); 
				names.push_back(fileinfo.name); 
			} 
		}while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile); 
	} 
}


string wstring2string(const wstring& ws)
{
	_bstr_t t = ws.c_str();  
	char* pchar = (char*)t;  
	string result = pchar;  
	return result;  
}

wstring string2wstring(const string& s)
{
	_bstr_t t = s.c_str();  
	wchar_t* pwchar = (wchar_t*)t;  
	wstring result = pwchar;  
	return result; 
}

int _tmain(int argc, _TCHAR* argv[])
{
	string path = "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs";
	vector<string> files;
	vector<string> names;

	getFiles(path,files,names);
 	for (int i = 0; i < files.size(); i++)
 	{
		printf("-----========files:%d========---\n",i);
 		// cout << "files" << i << ':' << files[i] << endl;

		wstring wsName;
		wsName = string2wstring(files[i]);	
		cout << "lnkPath: " << files[i] << endl;

		wstring wsDestPath = getLnkFormPath(wsName);
		cout << "destPath:";
		wcout << wsDestPath;
		cout << endl;
 	}
 	cout  << "-------------------\n";
// 	for (int i = 0; i < names.size(); i++)
// 	{
// 		string path1 = "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\";
// 		printf("-----========names:%d========---\n",i);
// 		cout << names[i] << endl;
// 		wstring wsName;
// 		wsName = string2wstring(path1.append(names[i]));	
// 		wcout << L"lnkPath: " << wsName << endl;
// 		// wprintf_s(L"lnkPath: %s\n", wsName.c_str());
// 		wstring wsDestPath = getLnkFormPath(wsName);
// 		wcout << L"destPath:" << wsDestPath << endl;
// 		// wprintf_s(L"destPath: %s\n", wsDestPath.c_str());
// 	}

	// QQ����.lnk
	// wstring wsDestPath = getLnkFormPath(L"C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\��ȸ.lnk");
	//printf("-----------------------------------\n%S\n", wsDestPath.c_str());
	// cout << "-----------------------------------\n" << wsDestPath.c_str() << endl;
	//system("pause");
	return 0;
}

