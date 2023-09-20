/********************************************************************************
 *
 *        Copyright (C) 2021  Beijing winicssec Technology
 *        All rights reserved
 *
 *        filename :      main.cpp
 *        description :    ��ת������ں���
 *
 *        created by  mingming.shi on Jul. 12th, 2021
 *
 ********************************************************************************/
#include "MyList.h"

void Str2Reverselist(char* str);

int main(int argc, const char* argv[])
{

	char *inputStr;
	char *strList,*numList = NULL,*letterList = NULL;
	char *p;
	
	inputStr = (char*)malloc(sizeof(char)*100);

	/*��ʹ�������з�ʽ*/
	gets(inputStr);
	strList = (char*)malloc(strlen(inputStr)+1); // Ϊstrlist����ռ�
	strcpy(strList,inputStr);
	

	p = strtok(inputStr, " ");
	while(p)
	{    
		if(IsNum(p))
		{
			if(numList == NULL)
			{
				numList = (char*)malloc(strlen(p) + 1);
				strcpy(numList,p);
			}
			else
			{
				char* temp = (char*)malloc(strlen(p)+1);
				strcpy(temp,p);
				strcat(numList," ");
				strcat(numList,temp);
			}

		}
		else
		{
			if(letterList == NULL)
			{
				letterList = (char*)malloc(strlen(p) + 1);
				strcpy(letterList,p);
			}
			else
			{
				char* temp = (char*)malloc(strlen(p)+1);
				strcpy(temp,p);
				strcat(letterList," ");
				strcat(letterList,temp);
			}
		}
		p = strtok(NULL, " ");  
	}

	free(inputStr);
	
	/*ʹ�������з�ʽ*/
	//for(int i = 1; i < argc; ++i)
	//{
	//	//cout << argv[i] << '\t';
	//	strcat(strList,argv[i]);
	//	strcat(strList," ");
	//	if(IsNum((char*)argv[i]) == true)
	//	{
	//		strcat(numList,argv[i]);
	//		strcat(numList," ");
	//	}
	//	else
	//	{
	//		strcat(letterList,argv[i]);
	//		strcat(letterList," ");
	//	}
	//}



	Str2Reverselist(strList);
	
	//Str2Reverselist(numList);

	//Str2Reverselist(letterList);


	free(strList);
	free(numList);
	free(letterList);

	printf("end\n");

}


/*
* @fn           Str2Reverselist
* @brief		��������ַ���ת��Ϊ�б������ת   
* @param[in]    str :�ַ���
* @param[out]   
* @return
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-12
*/
void Str2Reverselist(char* str)
{
	ListNode *list = InitListNode(str);
	cout << "��������Ϊ:";
	PrintListNode(list);

	ListNode *numList,*letterList;
	numList = (ListNode *)malloc(sizeof(ListNode));
	letterList = (ListNode *)malloc(sizeof(ListNode));

	ListSeparation(list,numList,letterList);

	ListNode *retList = ReverseList(list);
	cout << "��ת����Ϊ:";
	PrintListNode(retList);

	Destory(numList);
	Destory(letterList);
	Destory(retList);
}