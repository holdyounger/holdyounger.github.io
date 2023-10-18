// TestForOnlyC++.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include<io.h>
using namespace std;
void getAllFiles(string path, vector<string>& files)
{
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;  //�����õ��ļ���Ϣ��ȡ�ṹ
	string p;  //string�������˼��һ����ֵ����:assign()���кܶ����ذ汾
	if ((hFile = _findfirst(p.assign(path).append("\\*.*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))  //�ж��Ƿ�Ϊ�ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("/").append(fileinfo.name));//�����ļ�������
					getAllFiles(p.assign(path).append("/").append(fileinfo.name), files);//�ݹ鵱ǰ�ļ���
				}
			}
			else    //�ļ�����
			{
				files.push_back(p.assign(path).append("/").append(fileinfo.name));//�ļ���
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
		_findclose(hFile);
	}
	else
	{
		printf("Failed(%d)\n", GetLastError());
	}
}

//����
int main(int argc, const char *argv[])
{
	SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_UNICODE));

	printf("_________Start Test_________");
	string DATA_DIR = "C:/Users/DELL/Desktop/���Ĳ���";
	vector<string> files;
	//����
	char DistAll[] = { "AllFiles.txt" };
	if (argc < 2)
	{
		printf("ȱ�ٲ���");
		getAllFiles(DATA_DIR, files);//�����ļ����ļ��е�·�������
	}
	else
	{
		getAllFiles(argv[1], files);//�����ļ����ļ��е�·�������
	}
	
	//ofstream ofn(DistAll);  //����ļ���
	int size = files.size();
	int  FaiNum = 0;
	//ofn << size << endl;
	cout << "Get Files Nums:" << size << endl;
	for (int i = 0; i < size; i++)
	{
		//ofn << files[i] << endl;
		cout << files[i] << endl;
	}
	//ofn.close();
	return 0;
}
