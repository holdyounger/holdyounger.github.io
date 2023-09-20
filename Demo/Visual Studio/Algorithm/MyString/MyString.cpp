/********************************************************************************
 *
 *        Copyright (C) 2021  Beijing winicssec Technology
 *        All rights reserved
 *
 *        filename		: myString.cpp
 *        description	:
 *
 *        created by  mingming.shi on Jul. 15th, 2021
 *
 ********************************************************************************/
#include <iostream>
#include <assert.h>
#include <vld.h>
#include "mystring.h"
using namespace std;

char *LTrim(char *str);
char *RTrim(char *str);

/*
* @fn           RTrim
* @brief        ȥ��β������Ŀո�
* @param[in]    str:��Ҫ���������ַ���
* @param[out]   
* @return       char *:�����ַ���
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
char *RTrim(char *str)
{
	if (str == NULL || *str == '\0')
	{
		return str;
	}

	int len = strlen(str);
	char *p = str + len - 1;
	while (p >= str  && isspace(*p))
	{
		*p = '\0';
		--p;
	}

	return str;
}

/*
* @fn           LTrim
* @brief        ȥ���ײ�����Ŀո�
* @param[in]    str:��Ҫ���������ַ���
* @param[out]   
* @return       char *:�����ַ���
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
char *LTrim(char *str)
{
	if (str == NULL || *str == '\0')
	{
		return str;
	}

	int len = 0;
	char *p = str;
	while (*p != '\0' && isspace(*p))
	{
		++p;
		++len;
	}

	memmove(str, p, strlen(str) - len + 1);

	return str;
}

/*
* @fn           MyString
* @brief        ���캯��
* @param[in]    
* @param[out]   
* @return       MyString����
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
MyString::MyString(void)
{
	this->m_str = new char[1];
	m_str[0] = '\0';
}


MyString::MyString(const char *s)
{
	if(s == NULL)
	{
		this->m_str = new char[1];
		m_str[0] = '\0';
	}
	else
	{
		m_str = new char[strlen(s) + 1];
		strcpy(this->m_str, s);
	}
}

MyString::MyString(const MyString &s)
{
	if( s.m_str == NULL)
	{
		this->m_str = new char[1];
		m_str[0] = '\0';
	}
	else
	{
		m_str = new char[strlen(s.m_str) + 1];
		strcpy(this->m_str, s.m_str);
	}
}

/*
* @fn           ��������
* @brief        
* @param[in]    
* @param[out]   
* @return       
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
MyString::~MyString(void)
{
	if (this->m_str != NULL)
	{
		delete[] this->m_str;
	}
}

/*
* @fn           operator+
* @brief        ���� + ������
* @param[in]    ��Ҫ��ӵ�MyString����
* @param[out]   
* @return       MyString: ������ӽ�� MyString ����
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
MyString MyString::operator+(const MyString &s)
{
	MyString p;
	delete [] p.m_str;
	p.m_str = new char[strlen(this->m_str) + strlen(s.m_str) + 1];
	strcpy(p.m_str, this->m_str);
	strcat(p.m_str, s.m_str);
	return p;
}

/*
* @fn           operator=
* @brief        ���� = ������
* @param[in]    Դ MyString ����
* @param[out]   
* @return       MyString: ������� MyString ����
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
MyString& MyString::operator=(const MyString &s)
{
	int len = strlen(s.m_str) + 1;
	if (this == &s)
	{
		return *this;
	}
	if (this->m_str != NULL)
	{
		delete[] this->m_str;
	}
	this->m_str = new char[len];
	strcpy(this->m_str, s.m_str);
	return *this;
}

/*
* @fn           operator[]
* @brief        ����[]������
* @param[in]    ��Ҫ��ӵ�MyString����
* @param[out]   
* @return       MyString: ������ӽ�� MyString ����
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
char MyString::operator[](int i)
{
	int len = strlen(this->m_str) - 1;
	if (i > len)
	{
		return ('\0');
	}
	else
	{
		return (this->m_str[i]);
	}
}

/*
* @fn           operator+
* @brief        ����+������
* @param[in]    ��Ҫ��ӵ�MyString����
* @param[out]   
* @return       MyString: ������ӽ�� MyString ����
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
bool MyString::operator<(const MyString &s)
{
	int ret;
	ret = strcmp(this->m_str, s.m_str);

	if (ret < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
* @fn           operator>
* @brief        ���� > ������
* @param[in]    s: �뵱ǰ�������� MyString ����
* @param[out]   
* @return       true/false: �����ǰ������� s ���� true����֮���� false��
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
bool MyString::operator>(const MyString &s)
{
	int ret;
	ret = strcmp(this->m_str, s.m_str);
	if (ret > 0)
		return true;
	else
		return false;
}

/*
* @fn           operator==
* @brief        ����==������
* @param[in]    s: ��Ҫ�뵱ǰ�������� MyString ����
* @param[out]   
* @return       true/false: �������ȷ��� false�������ȷ��� true��
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
bool MyString::operator==(const MyString &s)
{
	int ret;
	ret = strcmp(this->m_str, s.m_str);
	if (ret == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
* @fn           operator!=
* @brief        ���� != ������
* @param[in]    s: ��Ҫ�뵱ǰ�������� MyString ����
* @param[out]   
* @return       true/false: �������ȷ��� true�������ȷ���false��
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
bool MyString::operator!=(const MyString &s)
{
	int ret;
	ret = strcmp(this->m_str, s.m_str);
	if (ret != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
* @fn           toLower
* @brief        ���ַ����еĴ�д��ĸ���Сд
* @param[in]    
* @param[out]   
* @return       myString����
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
MyString MyString::ToLower(void)
{
	char * temp = this->m_str;
	while (*temp != '\0')
	{
		if(*temp <= 'Z' && *temp >= 'A')
		{
			*temp += 32;
			temp++;
		}
		else
		{
			temp++;
		}

	}
	return *this;
}

/*
* @fn           toLower
* @brief        ���ַ����е�Сд��ĸ��ɴ�д
* @param[in]    
* @param[out]   
* @return       myString����
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
MyString MyString::ToUpper(void)
{
	char * temp = this->m_str;
	while (*temp != '\0')
	{
		if(*temp <= 'z' && *temp >= 'a')
		{
			*temp -= 32;
			temp++;
		}
		else
		{
			temp++;
		}

	}
	return *this;
}

/*
* @fn           findStr
* @brief        ����str��myString�������״γ��ֵ�λ��
* @param[in]    str	:��Ҫ���ҵ��ӷ���
* @param[out]   
* @return       ������ҵ����򷵻��״β��ҵ�ʱ���±ꣻ���û���ҵ����򷵻�-1
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
int MyString::FindStr(const char* str)
{
	// ˫ָ��
	for (size_t i = 0; i < strlen(m_str); i++)
	{
		size_t j = 0;
		if (this->m_str[i] == str[j])
		{
			while(i < strlen(m_str) 
				&& j < strlen(str) 
				&& this->m_str[i+j] == str[j])
			{
				if(j == (strlen(str)-1))
					return i;
				else
					j++;

			}
		}

	}
	return -1;
}

/*
* @fn           getStr
* @brief        ��ָ��λ�ÿ�ʼ����myString������ָ�����ȵ��Ӵ������û���򷵻�NULL
* @param[in]    n	:��ʼλ��
				len	:�ӿ�ʼλ�ü���ĳ���
* @param[out]   
* @return       myString����
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
MyString MyString::GetStr(const int& n,const size_t& len)
{
	assert(m_str[n] != NULL);

	char *p = this->m_str;

	MyString ret;
	delete [] ret.m_str;	// �����ͷŵ���ʱ��Ա�ռ䣬��Ȼ�����й©
	ret.m_str = new char[len+1];

	size_t i = 0;
	while (i < len)
	{
		assert(m_str[n+i] != NULL);
		ret.m_str[i] = p[n+i]; 
		i++;
	}
	ret.m_str[len] = '\0';
	
	return ret;
}

int MyString::Length(void)
{
	return strlen(m_str);
}

/*
* @fn           RemoveLRSpace
* @brief        �Ƴ���β�ո�
* @param[in]    
* @param[out]   
* @return       myString����
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
MyString MyString::RemoveLRSpace(void)
{
	if(m_str == NULL)
		return *this;
	RTrim(m_str);
	LTrim(m_str);
	return *this;
}

/*
* @fn           Compare
* @brief        �Ƚ�����MyString�����Ƿ����
* @param[in]    
* @param[out]   
* @return       int:���thisС��s������-1��
					���this����s������0��
					���this����s, ����1��
*               
* @detail      
* @author       mingming.shi
* @date         2021-7-15
*/
int MyString::Compare(const MyString &s)
{
	assert(m_str != NULL && s.m_str != NULL);

	int ret;
	ret = strcmp(this->m_str, s.m_str);

	return ret;
}


//�������������
ostream& operator<<(ostream &out, MyString &s)
{
	out << s.m_str;
	return out;
}

//�������������
istream& operator>>(istream &in, MyString &s)
{
	char p[50];
	in.getline(p, 50);
	s = p;
	return in;
}