/********************************************************************************
 *
 *        Copyright (C) 2021  Beijing winicssec Technology
 *        All rights reserved
 *
 *        filename :      ReverseList.CPP
 *        description :    ��ת����
 *
 *        created by  mingming.shi on Jul. 12th, 2021
 *
 ********************************************************************************/

#include <iostream>
#include <string>
#include <vld.h>
using namespace std;


struct ListNode {
	string str;
	ListNode *next;
	ListNode(void):str(" "),next(NULL){}
	ListNode(string x):str(x),next(NULL){}
};

ListNode* InitListNode(string str);
ListNode* ReverseList(ListNode* phead);
void PrintListNode(ListNode* phead);
bool IsNum(string str);
void Destory(ListNode* phead);

/*
* @fn           InitListNode
* @brief        �����ַ������ɵ�����   
* @param[in]    str :������Կո�Ϊ�ָ������ַ���
* @param[out]   
* @return       listNode* :����ṹ���ͷ���
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-12
*/
ListNode* InitListNode(string str)
{
	ListNode *head = new ListNode;
	ListNode *pf = head;

	for (size_t i =0; i < str.size(); ++i)
	{
		string tempStr;
		while(str[i] != ' ')
		{
			tempStr += str[i++];
			if(i >= str.size())
				break;
		}

		ListNode* tempNode = new ListNode(tempStr);
		pf->next = tempNode;
		pf = pf->next;
	}
	ListNode* tempNode = head->next;
	delete head;
	return tempNode;
}

/*
* @fn           ReverseList
* @brief        ��ת����  
* @param[in]    phead:��Ҫ��ת������ͷ���
* @param[out]   
* @return       listNode* :��ת������ṹ���ͷ���
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-12
*/
ListNode* ReverseList(ListNode* phead)
{
	ListNode* cur = phead;
	ListNode* pre = NULL;
	ListNode* nex = NULL;
	while(cur)
	{
		nex = cur->next;
		cur->next = pre;
		pre = cur;
		cur = nex;
	}
	return pre;
}

/*
* @fn           PrintListNode
* @brief        �����ַ������ɵ�����   
* @param[in]    phead:��Ҫ��ӡ������ͷ���
* @param[out]   
* @return       void
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-12
*/
void PrintListNode(ListNode* phead)
{
	if(phead->str.empty())
	{
		return;
	}

	ListNode *cur = phead;
	while (cur->next != NULL)
	{
		cout << cur->str << ",";
		cur = cur->next;
	}
	cout << cur->str << "."<< endl;
}

/*
* @fn           IsNum
* @brief        �ж�string�Ƿ�Ϊ�����ָ�ʽ   
* @param[in]    str :�ַ���
* @param[out]   
* @return       True/False
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-12
*/
bool IsNum(string str)
{
	if(str.empty())
	{
		return false;
	}

	for (size_t i=0; i<str.size(); i++ )
	{
		int temp = (int)str[i];
		
		if(temp >='0' && temp <='9'){
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;

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
void Str2Reverselist(string str)
{
	ListNode *list = InitListNode(str);
	cout << "��������Ϊ:";
	PrintListNode(list);

	ListNode *retList = ReverseList(list);
	cout << "��ת����Ϊ:";
	PrintListNode(retList);

	Destory(retList);
	//Destory(retList);
}

/*
* @fn           Destory
* @brief		�ͷ������ڴ�   
* @param[in]    phead :��Ҫ���ͷŵ�����ͷ���
* @param[out]   
* @return		void
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-12
*/
void Destory(ListNode* phead)
{
	if(phead == NULL)
	{
		return;
	}

	ListNode *p=phead;
	ListNode *q;

	while(p != NULL)
	{
		q=p->next;
		delete p;
		p=q;
	}
	p = NULL;
}


// ��ں���
int main(int argc, const char* argv[])
{

	string strList,numList,letterList;
	
	/*��ʹ�������з�ʽ*/
	//while(cin)
	//{
	//		string str;
	//		cin >> str;
	//		strList += str;
	//		strList += " ";

	//		if(IsNum(str) == true)
	//		{
	//			numList += str;
	//			numList += " ";
	//		}
	//		else
	//		{
	//			letterList += str;
	//			letterList += " ";
	//		}

	//		if(getchar() == '\n')
	//		{
	//			break;
	//		}

	//}


	/*ʹ�������з�ʽ*/
	for(int i = 1; i < argc; ++i)
	{
		//cout << argv[i] << '\t';
		strList += argv[i];
		strList += " ";
		if(IsNum(argv[i]) == true)
		{
			numList += argv[i];
			numList += " ";
		}
		else
		{
			letterList += argv[i];
			letterList += " ";
		}
	}
	
	Str2Reverselist(strList);
	Str2Reverselist(numList);
	Str2Reverselist(letterList);

	system("pause");

}