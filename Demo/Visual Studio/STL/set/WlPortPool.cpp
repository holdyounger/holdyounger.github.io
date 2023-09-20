/**************************************************************************
 *
 *        Copyright (C) 2022  Beijing Winicssec Technologies Co.,Ltd
 *        All rights reserved
 *
 *        Filename    :	WLPortPool.cpp
 *        Description : ��̬�˿���ռ����ϵ�����£���ȡ��̬�˿ڵĺ����ӿ�ʵ���ļ�
 *
 *        Created by  mingming.shi on Jan. 24th, 2022
 *
 *************************************************************************/
#include "StdAfx.h"
#include "WLPortPool.h"
#include <process.h>


#define STU_MEMBER_NUM		2
#define CMD_RESULT_BUF_SIZE 1024
#define SCAN_FREQUENCY		1000 * 30
#define CMD_QUERY_DYNAMIC_TCP "netsh int ipv4 show dynamicport tcp"

std::vector<STU_PORT_POOL> g_stuPortPool;

bool WNTPORT::g_bFlagGetStaticPort = false;
bool WNTPORT::g_bStartThread = false;
int WNTPORT::g_nCurPoint = 0;


#if 0 /* �ӿ����� */
#endif
/************************************************************************/
/* ������������ 2022-1-24 14:33:28                                    */
/************************************************************************/
// ��ȡ��̬�˿�
int executeCMD(__out char *pChRet);
int getDynamicPortFromstr(__out STU_PORT_POOL& stuDynamicPort);

// ��ȡ��ǰ��ռ�ö˿�
int getAllTcpConnectionsPort(__out std::set<UINT>& setRet);

// ��ȡʣ�ද̬�˿�
int getFreeDynamicPort(__in STU_PORT_POOL stuDynamicPort, __in std::set<UINT> CurSysUsedPort, __out std::vector<STU_PORT_POOL>& stuFreeDynamicPort);
int getFreeStaticPort(__in STU_PORT_POOL stuDynamicPort, __in std::set<UINT> CurSysUsedPort, __out std::vector<STU_PORT_POOL>& stuFreeStaticPort);

// ���¶˿�
void updatePort(__in __out std::vector<STU_PORT_POOL>::iterator it);

// �����˿ڳ�
int createPortPool();

// ��ȡʣ�ද̬�˿����� 
int getFreeDynamicPortsNum(__out int& nNum);

// �̣߳���ȡϵͳ��ǰʣ�ද̬�˿�����
unsigned int WINAPI scanSysDynamicPortsNum(void* lpParam);

#if 0 /* ����ӿڶ��� */
#endif
/*
* @fn			getPort
* @brief		����һ�����ö˿ں��䷶Χ
* @param[in]	
* @param[out]   STU_PORT_POOL: ����һ�����ö˿ڣ��������ʧ�ܣ����޸�stuPort�е�BEnableΪfalse
* @return		0 ��ʾ�ɹ���������ʾʧ��
*               
* @detail      	���η��ض˿ڳ��еĶ˿ںͷ�Χ
* @author		mingming.shi
* @date			2022-1-24
*/
void WINAPI WNTPORT::getPort(__out STU_PORT_POOL& stuPort)
{
	int iRet = -1;
	std::vector<STU_PORT_POOL>::iterator it = g_stuPortPool.begin();

	if ( 0 == g_stuPortPool.size() ) // ��ǰ�˿ڳ�Ϊ��
	{
		iRet = createPortPool();
		if (NO_ERROR != iRet)
		{
			printf(("LINE [%d] Create Port Pool FAILED!"), __LINE__);
			return;
		}
	}

	it = g_stuPortPool.begin();
	for (int i = 0; it != g_stuPortPool.end(); it++)
	{
		if (false == it->isLegal())
		{
			updatePort(it);
		}

		if ( true == it->bEnable &&		// ��ǰ�˿ڿ���
			true == it->isLegal() &&	// ��ǰ�˿ںϷ�
			i == g_nCurPoint)			// ��ѯ
		{
			STU_PORT_POOL stuTemp;
			stuTemp = *it;

			stuPort.nStartPort = stuTemp.nStartPort;
			stuPort.nRange = stuTemp.nRange;
			stuPort.bEnable = stuTemp.bEnable;

			g_nCurPoint++;
			if (g_nCurPoint >= 10)
			{
				g_nCurPoint = 0;
			}
			break;
		}
		i++;
	}

}

/*
* @fn			SetPortIsAvailable
* @brief		���õ�ǰ�˿ڵĿ���״̬
* @param[in]	STU_PORT_POOL: ��Ҫɾ���Ķ˿�
* @param[out]   
* @return		
*               
* @detail      	����ݵ�ǰ�������˿ںͷ�Χ�ڶ˿ڳ��в���һ�µĲ�ɾ��,Ȼ����������¶˿ڳ�
* @author		mingming.shi
* @date			2022-1-24
*/
void WINAPI WNTPORT::SetPortIsAvailable(__in STU_PORT_POOL stuPort)
{
	std::vector<STU_PORT_POOL>::iterator it = g_stuPortPool.begin();
	for (; it != g_stuPortPool.end(); it++)
	{
		if (it->nStartPort == stuPort.nStartPort &&
			it->nRange == stuPort.nRange && 
			stuPort.bEnable == false) // ��ǰ�˿ڲ�����
		{
			// g_stuPortPool.erase(it);
			break;
		}
	}

	updatePort(it);
}

/*
* @fn			mainThread
* @brief		�˿ڳ��߳�,��ں���
* @param[in]    
* @param[out]	
* @return		pthread handler.
*               
* @detail      	caller can teminal pthread by this handler.
* @author		mingming.shi
* @date			2022-1-27
*/
void WNTPORT::startScanPortsThread()
{
	g_bStartThread = true;
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, scanSysDynamicPortsNum, 0, 0, NULL);
	if (NULL == hThread)
	{
		printf(("Create scanSysDynamicPortsNum thread error. [%d]"), GetLastError());
	}
	CloseHandle(hThread);
	
	Sleep( SCAN_FREQUENCY );
}

/*
* @fn			killScanPortsThread
* @brief		ɱ��ɨ�趯̬�˿ڵ��߳�
* @param[in]    
* @param[out]	
* @return		
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-28
*/
void WNTPORT::killScanPortsThread()
{
	g_bStartThread = false;

	if (g_stuPortPool.size())
	{
		g_stuPortPool.clear();
	}
}

bool WINAPI WNTPORT::getPortPoolStatus()
{
	return g_bFlagGetStaticPort;
}

#if 0 /* �ӿڶ��� */
#endif
/************************************************************************/
/* �ӿ�ʵ��                                                     */
/************************************************************************/



/*
* @fn			CreatPortPool
* @brief		�����˿���Դ��
* @param[in]    
* @param[out]	
* @return		0 �����ɹ���������ʾʧ��
*               
* @detail      	��ϵͳ��̬�˿�ʹ�����������IEGʱ���ô˽ӿڻ�ȡ��̬�˿���Դ�أ����浽 g_stuPortPool ������
* @author		mingming.shi
* @date			2022-1-24
*/
int createPortPool()
{
	int iRet = 0;
	std::vector<STU_PORT_POOL> stuPortPool;			// ����ӿڷ��صĶ˿ڷ�Χ
	std::vector<STU_PORT_POOL> stuFreeDynamicPort;	// ����δ��ʹ�ö˿ڷ�Χ
	STU_PORT_POOL	stuDynamicPort;
	std::set<UINT>	CurSysUsedPort;

	// 1 ��ȡ��̬�˿�״̬����ʼ��ַ�ͷ�Χ��
	iRet = getDynamicPortFromstr(stuDynamicPort);
	if(NO_ERROR != iRet)
	{
		iRet = -1;
		goto _END_;
	}

	// 2 ��ȡ�˿�ռ�����
	iRet = getAllTcpConnectionsPort(CurSysUsedPort);
	if(NO_ERROR != iRet)
	{
		iRet = -2;
		goto _END_;
	}

	// 3 ��Ĭ�ϵ�ַ��ʼ���ҿ��ö˿ڷ�Χ
	iRet = getFreeStaticPort(stuDynamicPort, CurSysUsedPort, stuFreeDynamicPort);
	if(NO_ERROR == iRet)
	{
		g_stuPortPool = stuFreeDynamicPort;
	}

	iRet = 0;
_END_:
	return iRet;
}

/************************************************************************/
/* ����ʵ��  2022-1-24 14:33:41                                        */
/************************************************************************/
/*
* @fn			ExecuteCMD
* @brief		ִ�в�ѯTCP��̬�˿������ȡϵͳ��̬�˿ڷ�Χ
* @param[in]    
* @param[out]	pChRet: ������ִ�к������ַ���
* @return		0 ��ʾ�ɹ������� ʧ��
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
static int executeCMD(__out char *pChRet)
{
	int iRet = -1;
	char buf_ps[CMD_RESULT_BUF_SIZE];
	char pChBuf[CMD_RESULT_BUF_SIZE] = { 0 };
	FILE *ptr;

	strcpy_s(pChBuf, _countof(CMD_QUERY_DYNAMIC_TCP) ,CMD_QUERY_DYNAMIC_TCP);

	if ( NULL != (ptr = _popen(pChBuf, "r")) )
	{
		while (NULL != fgets(buf_ps, sizeof(buf_ps), ptr) )
		{
			strcat_s(pChRet, _countof(buf_ps), buf_ps);
			if (strlen(pChRet) > CMD_RESULT_BUF_SIZE)
			{
				iRet = -2;
				break;
			}
		}
		_pclose(ptr);
		ptr = NULL;
		iRet = 0;  // ����ɹ�
	}
	else
	{
		printf(("popen %s error\n"), pChBuf);
		iRet = -1; // ����ʧ��
	}

	return iRet;
}

/*
* @fn			getDynamicPortFromstr
* @brief		��cmd���������л�ȡ��̬�˿ڵ�������ַ�ͷ�Χ
* @param[in]    
* @param[out]	stuDynamicPort: ��̬�˿ڵ���ʼ��ַ�ͷ�Χ
* @return		0 ��ʾ�ɹ���������ʾʧ��
*               
* @detail      	�ö�̬�˿�ΪTCP�˿�
* @author		mingming.shi
* @date			2022-1-24
*/
int getDynamicPortFromstr(__out STU_PORT_POOL& stuDynamicPort)
{
	int iRet = 0;
	std::string str = "";
	std::vector<unsigned int> vecNumFromStr;

	// 1 ִ��CMD��ȡ���������ʽ����
	/*
		Э�� tcp ��̬�˿ڷ�Χ
		---------------------------------
		�����˿�        : 49152
		�˿���          : 16384
	*/
	char chRet[CMD_RESULT_BUF_SIZE] = { 0 };
	if ( 0 != executeCMD(chRet) )
	{
		iRet = -1;
		goto _END_;
	}

	str = chRet;

	if(str.empty())
	{
		iRet = -2;
		goto _END_;
	}

	// 2 ��ȡCMD����е�����

	size_t nLen = str.length();

	for (size_t i = 0; i < nLen; i++) {
		int CurNum = 0;
		bool flag = false;
		while ( !(str[i] >= '0' && str[i] <= '9') && i < nLen ) // ����������
		{
			i++;
		}

		while ( (str[i] >= '0' && str[i] <= '9') && i < nLen ) // ��������ĸ
		{
			flag = true;
			CurNum = CurNum * 10 + (str[i] - '0');
			i++;
		}

		if (flag)
		{
			vecNumFromStr.push_back(CurNum);
		}
	}
	
	if (STU_MEMBER_NUM != vecNumFromStr.size())
	{
		iRet = -3;
		goto _END_;
	}
	stuDynamicPort.nStartPort	= vecNumFromStr[0];
	stuDynamicPort.nRange		= vecNumFromStr[1];
	
	iRet = 0;
_END_:
	return iRet; //���ϴ�С���ǲ�ͬ��������
}

/*
* @fn			getAllTcpConnectionsPort
* @brief		��ȡ�����Ӷ˿���
* @param[in]    
* @param[out]	ret�������Ӷ˿ڵ�set����
* @return		0 ��ʾִ�гɹ���������ʾʧ��
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
int getAllTcpConnectionsPort(__out std::set<UINT>& setRet)
{
	int iRet	= 0;
	int nNum	= 0; // TCP���ӵ���Ŀ

	PMIB_TCPTABLE_OWNER_PID pTcpTable(NULL);
	DWORD dwSize(0);
	GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
	pTcpTable = (MIB_TCPTABLE_OWNER_PID *)new char[dwSize];//���·��仺����

	if ( NO_ERROR != GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) )
	{
		delete pTcpTable;
		pTcpTable = NULL;
		{	
			iRet = -1;
			goto _END_;
		}
	}

	// TCP���ӵ���Ŀ
	nNum = (int)pTcpTable->dwNumEntries; 

	for (int i = 0; i < nNum; i++)
	{
		setRet.insert(htons(pTcpTable->table[i].dwLocalPort));
	}

_END_:
	if (pTcpTable != NULL) 
	{
		free(pTcpTable);
		pTcpTable = NULL;
	}

	return iRet;
}

/*
* @fn			getFreeDynamicPort
* @brief		��ȡδ��ռ�õĶ�̬�˿ڶ�
* @param[in]    stuDynamicPort��ϵͳ��ǰ���ŵĶ�̬�˿ڷ�Χ
				CurSysUsedPort��ϵͳ��ǰ��ʹ�ö˿�
* @param[out]	stuFreeStaticPort: ��Ĭ�϶˿ڿ�ʼ��ȡ�Ķ˿ڷ�Χ

* @return		
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
int getFreeDynamicPort(__in STU_PORT_POOL stuDynamicPort, __in std::set<UINT> CurSysUsedPort, __out std::vector<STU_PORT_POOL>& stuFreeDynamicPort)
{
	int iRet = 0;
	UINT nStart = stuDynamicPort.nStartPort;
	UINT nEnd	= stuDynamicPort.nStartPort + stuDynamicPort.nRange;

	for (UINT nPort = nStart; nPort != nEnd; ++nPort)
	{
		STU_PORT_POOL stuTempPort;
		if ( CurSysUsedPort.end() !=  CurSysUsedPort.find(nPort) ) // �ҵ���δʹ�ö˿�
		{
			int nRange = 0;

			// ������ǰ�����˿ڵĶ˿ڷ�Χ
			while ( CurSysUsedPort.end() !=  CurSysUsedPort.find( nPort + nRange) )
			{
				nRange++;
			}

			stuTempPort.nStartPort	= nPort;
			stuTempPort.nRange		= nRange;
		}

		stuFreeDynamicPort.push_back(stuTempPort);
		if (stuFreeDynamicPort.size() > MAX_PORT_NUM)
		{
			iRet = -1;
			goto _END_;
		}
	}
	
_END_:
	return iRet;
}

/*
* @fn			getFreeStaticPort
* @brief		��Ĭ�϶˿ڿ�ʼ��ȡδ��ϵͳʹ�õľ�̬�˿ںͷ�Χ
* @param[in]    stuDynamicPort��ϵͳ��ǰ���ŵĶ�̬�˿ڷ�Χ
				CurSysUsedPort��ϵͳ��ǰ��ʹ�ö˿�
* @param[out]	stuFreeStaticPort: ����
* @return		0 ��ʾ�ɹ���������ʾʧ��
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
int getFreeStaticPort(__in STU_PORT_POOL stuDynamicPort, __in std::set<UINT> CurSysUsedPort, __out std::vector<STU_PORT_POOL>& stuFreeStaticPort)
{
	int iRet = -1;
	UINT nStart = DEFAULT_START_PORT;
	UINT nEnd	= DEFAULT_STOP_PORT;

	for (UINT nPort = nStart; nPort != nEnd; ++nPort)
	{
		STU_PORT_POOL stuTempPort;

		if ( CurSysUsedPort.end() == CurSysUsedPort.find(nPort) && false == stuDynamicPort.isContain(nPort) ) // �ҵ���δʹ�ö˿ڣ����Ҳ��ٶ�̬�˿ڷ�Χ��
		{
			int nRange = 0;

			// ������ǰ�����˿ڵĶ˿ڷ�Χ
			do
			{
				nRange++;
			}while ( 
				CurSysUsedPort.end() ==  CurSysUsedPort.find(nPort + nRange) && // ϵͳδʹ��
				false == stuDynamicPort.isContain(nPort + nRange) && // ���ڶ�̬�˿ڷ�Χ
				nRange < DEFAULT_PORT_RANGE); // ��ΧС��Ĭ�϶˿ڷ�Χ

			stuTempPort.nStartPort	= nPort;
			stuTempPort.nRange		= nRange >= DEFAULT_MIN_PORTRANGE ? nRange : DEFAULT_MIN_PORTRANGE;
			stuTempPort.bEnable		= true;

			// ���������˿ڲ����Ϸ���
			nPort += nRange; 
			nPort = nPort >= DEFAULT_STOP_PORT ? DEFAULT_START_PORT : nPort;
		}

		stuFreeStaticPort.push_back(stuTempPort);
		if (stuFreeStaticPort.size() > MAX_PORT_NUM) // ֻ���������˶��ѣ���Ȼ���سɹ�
		{
			iRet = 0; 
			goto _END_;
		}
	}

	iRet = 0;
_END_:
	return iRet;
}

/*
* @fn			getFreeDynamicPortsNum
* @brief		��ȡʣ�ද̬�˿�����
* @param[in]    
* @param[out]	nNum��ϵͳ���ж�̬�˿�����
* @return		0 ��ʾ�ɹ���������ʾʧ��
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
int getFreeDynamicPortsNum(__out int& nNum)
{
	int iRet = 0;
	int nPortsNum = 0;

	STU_PORT_POOL stuDynamicPort;
	std::set<UINT> CurSysUsedPort;
	std::set<UINT>::iterator it;

	// 1 ��ȡ��̬�˿�״̬����ʼ��ַ�ͷ�Χ��
	iRet = getDynamicPortFromstr(stuDynamicPort);
	if ( NO_ERROR != iRet ) 
	{
		iRet = -1;
		goto _END_;
	}

	// 2 ��ȡ��ǰϵͳ�˿�ռ�����
	iRet = getAllTcpConnectionsPort(CurSysUsedPort);
	if ( NO_ERROR != iRet ) 
	{
		iRet = -2;
		goto _END_;
	}

	// 3 ͳ�ƶ�̬�˿ڸ���
	for (it = CurSysUsedPort.begin(); it != CurSysUsedPort.end(); it++)
	{
		if(true == stuDynamicPort.isContain(*it))
		{
			nPortsNum++;
		}
	}

	nNum = stuDynamicPort.nRange - nPortsNum;
	iRet = 0;

_END_:
	if (CurSysUsedPort.size())
	{
		CurSysUsedPort.clear();
	}
	return iRet;
}

/*
* @fn			updatePort
* @brief		����һ���˿ڵ��˿ڳ��У��ڲ�ʵ�ֽӿ�
* @param[in]    
* @param[out]	
* @return		
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-27
*/
void updatePort(__in __out std::vector<STU_PORT_POOL>::iterator it)
{
	UINT nStart = DEFAULT_START_PORT;
	UINT nEnd	= DEFAULT_STOP_PORT;
	STU_PORT_POOL	stuDynamicPort;
	std::set<UINT>	CurSysUsedPort;
	std::vector<STU_PORT_POOL>::reverse_iterator itPortPoolEnd = g_stuPortPool.rbegin();

	// 1 ��ȡ��̬�˿�״̬����ʼ��ַ�ͷ�Χ��
	getDynamicPortFromstr(stuDynamicPort);

	// 2 ��ȡ�˿�ռ�����
	getAllTcpConnectionsPort(CurSysUsedPort);

	// 3 ���������˿ڣ��Ӷ˿ڳ����һ���˿ڿ�ʼ
	if (itPortPoolEnd->isLegal())
	{
		nStart = itPortPoolEnd->nStartPort > DEFAULT_START_PORT ? itPortPoolEnd->nStartPort : DEFAULT_START_PORT;
	}
	if (nStart >= DEFAULT_STOP_PORT)
	{
		nStart = DEFAULT_START_PORT;
	}

	for (UINT nPort = nStart; nPort <= DEFAULT_STOP_PORT; ++nPort)
	{
		STU_PORT_POOL stuTempPort;

		if ( CurSysUsedPort.end() != CurSysUsedPort.find(nPort) && false == stuDynamicPort.isContain(nPort) ) // �ҵ���δʹ�ö˿ڣ����Ҳ��ٶ�̬�˿ڷ�Χ��
		{
			int nRange = 0;

			// ������ǰ�����˿ڵĶ˿ڷ�Χ
			while ( CurSysUsedPort.end() !=  CurSysUsedPort.find(nPort + nRange) && false == stuDynamicPort.isContain(nPort + nRange) )
			{
				nRange++;
				if (nRange >= DEFAULT_PORT_RANGE)
				{
					break;
				}
			}

			stuTempPort.nStartPort	= nPort;
			stuTempPort.nRange		= nRange;
			stuTempPort.bEnable		= true;

			// ���������˿ڲ����Ϸ���
			nPort += nRange; 
			nPort = nPort >= DEFAULT_STOP_PORT ? DEFAULT_START_PORT : nPort;
		}

		if (true == stuTempPort.isLegal())
		{
			it->nStartPort = stuTempPort.nStartPort;
			it->nRange = stuTempPort.nRange;
			it->bEnable = stuTempPort.bEnable;
		}
		
		if (g_stuPortPool.size() > MAX_PORT_NUM)
		{
			break;
		}
	}
}

/*
* @fn			scanSysDynamicPortsNum
* @brief		ɨ��ϵͳ��ǰʣ�ද̬�˿�����
* @param[in]    
* @param[out]	
* @return		
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-27
*/
unsigned int WINAPI scanSysDynamicPortsNum(void* lpParam)
{
	int iRet = -1;

	while(true == WNTPORT::g_bStartThread)
	{
		int nDynamicNum = INT_MAX;

		if ( NO_ERROR != getFreeDynamicPortsNum(nDynamicNum) )
		{
			printf(("LINE [%d] PortPool Get Dynamic Port Number Failed!\n"), __LINE__);
			iRet = -1;
			goto _END_;
		}

		if (START_STATIC_PORT_FLAG >= nDynamicNum) // ���Ҿ�̬�˿�
		{
			// WriteInfo(_T("LINE [%d] PortPool Start Get Static Port!\n"), __LINE__);
			iRet = createPortPool();
			if (NO_ERROR != iRet)
			{
				printf(("LINE [%d] PortPool Create Static PortPool FAILED!\n"), __LINE__);
				iRet = -2;
				goto _END_;
			}
			WNTPORT::g_bFlagGetStaticPort = true;
		}
		else
		{
			WNTPORT::g_bFlagGetStaticPort = false;
		}

		iRet = 0;
	}
	
_END_:
	return iRet;
}