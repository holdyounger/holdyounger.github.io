/**************************************************************************
 *
 *        Copyright (C) 2022  Beijing Winicssec Technologies Co.,Ltd
 *        All rights reserved
 *
 *        Filename    :	WLPortPool.h
 *        Description : ��̬�˿���ռ����ϵ�����£���ȡ��̬�˿ڵĺ����ӿ�
 *
 *        Created by  mingming.shi on Jan. 24th, 2022
 *
 *************************************************************************/
#pragma once

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <WinSock.h>
#include <tcpmib.h>
#include <IPHlpApi.h>
#include <set>

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "IPHlpApi.lib")

#define MAX_PORT_NUM		10		// ���ֻ�������10���˿ڳ�
#define DEFAULT_START_PORT	1025	// Ĭ�������˿ڣ�1025
#define DEFAULT_STOP_PORT	65535	// Ĭ����ֹ�˿ڣ�65535
#define DEFAULT_PORT_RANGE	100		// Ĭ�϶˿ڷ�Χ��100
#define DEFAULT_MIN_PORTRANGE 50	// ��С�˿ڷ�Χ
#define START_STATIC_PORT_FLAG 100	// ��̬�˿ڸ����������˿ڳصı�ʶ

/* �˿ڳؽṹ�� */
typedef struct _PORT_POOL {
	UINT nStartPort;	// �����˿�
	UINT nRange;		// �˿ڷ�Χ
	bool bEnable;

	_PORT_POOL(UINT nSta = 0, UINT nRan = 0, bool bEn = false):nStartPort(nSta),nRange(nRan),bEnable(bEn){};
	bool isContain(UINT uPORT)
	{
		if ( uPORT >= nStartPort && uPORT < (nStartPort + nRange) )
		{
			return true;
		}
		return false;
	}
	
	bool isLegal()
	{
		if( (this->nStartPort + this->nRange) <= DEFAULT_STOP_PORT &&
			(this->nStartPort) >= DEFAULT_START_PORT
			)
		{
			return true;
		}
		return false;
	}

	bool operator<(const _PORT_POOL& b) const
	{
		return this->nStartPort == b.nStartPort ? this->nRange <= b.nRange : this->nStartPort < b.nRange;
	}
}STU_PORT_POOL, *PSTU_PORT_POOL;

/* ȫ�ֱ���-�˿����� */
extern std::vector<STU_PORT_POOL> g_stuPortPool;

#ifdef __cplusplus
extern "C" {
#endif
	namespace WNTPORT {
	
		extern bool g_bFlagGetStaticPort;	// Ϊ���ʾ���ö˿ڳ�
		extern bool g_bStartThread;			// Ϊ�ٱ�ʾֹͣ�߳�
		extern int g_nCurPoint;				// �˿ڳ���ѯ
		
		/* ���õ�ǰ�˿�״̬ */
		void WINAPI SetPortIsAvailable(__in STU_PORT_POOL stuPort);

		/* �˿ڳ��߳� */
		void startScanPortsThread();

		/* ɱ���߳� */
		void killScanPortsThread();

		/* ����һ�����ö˿ں��䷶Χ */
		void WINAPI getPort(__out STU_PORT_POOL& stuPort);

		/* ���ض˿ڳ�״̬ */
		bool WINAPI getPortPoolStatus();
	}
#ifdef __cplusplus
}
#endif