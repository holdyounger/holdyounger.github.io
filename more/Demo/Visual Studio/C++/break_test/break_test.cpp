// break_test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{

	for(int i = 0; i < 10; i++)
	{
		if(i == 3)
		{
			break;
		}
		printf("%d\n", i);
	}

	return 0;
}

