/********************************************************************************
 *
 *        All rights reserved
 *
 *        filename :       wmiTools.CPP
 *        description :    ����ϵͳ�����С�
 *
 ********************************************************************************/
#pragma once
#include "wmiTools.h"


#include <comdef.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")

#define CMD_DEP_CHECK	 _T("wmic OS Get DataExecutionPrevention_SupportPolicy")

 //CWmiIface::Garbo  CWmiIface::garbo;

CWmiIface::CWmiIface(void)
{
	m_bInitialize = FALSE;
	m_pWbemSvc = NULL;
	m_pWbemLoc = NULL;
	m_pWbemClsObj = NULL;
	m_pEnumClsObj = NULL;
}

/*
* @fn           GetSingleItemInfo
* @brief        ���ݲ�ѯ�����һ�����Ա
* @param[in]    ClassName:�����磺��Win32_Processor��
				ClassMember:��Ŀ���磺��SerialNumber��
* @param[out]
* @return
*
* @detail
* @author       mingming.shi
* @date         2021-08-27
*/

BOOL CWmiIface::GetSingleItemInfo(__in const wstring &ClassName, __in const wstring &ClassMember, __out wstring &RetValue)
{
	USES_CONVERSION;
	CComBSTR query("SELECT * FROM ");
	CString	CstrRetValue;

	VARIANT    vtProp;
	ULONG      uReturn;
	HRESULT    hr;
	BOOL       bRet = FALSE;

	InitWmi();

	if (NULL != m_pWbemSvc)
	{
		//��ѯ��ClassName�е������ֶ�,���浽m_pEnumClsObj��
		query += CComBSTR(ClassName.c_str());
		hr = m_pWbemSvc->ExecQuery(CComBSTR("WQL"), query, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			0, &m_pEnumClsObj);

		if (m_pEnumClsObj == NULL)
		{
			printf(("m_pEnumClsObj == NULL : %x"), hr);
			goto END;
		}

		if (SUCCEEDED(hr))
		{
			//��ʼ��vtPropֵ
			VariantInit(&vtProp);
			uReturn = 0;

			//���شӵ�ǰλ����ĵ�һ������m_pWbemClsObj��
			hr = m_pEnumClsObj->Next(WBEM_INFINITE, 1, &m_pWbemClsObj, &uReturn);
			if (m_pWbemClsObj == NULL)
			{
				printf(("m_pWbemClsObj == NULL : %x"), hr);
				goto END;
			}

			if (SUCCEEDED(hr) && uReturn > 0)
			{
				//��m_pWbemClsObj���ҳ�ClassMember��ʶ�ĳ�Ա����ֵ,�����浽vtProp������
				hr = m_pWbemClsObj->Get(CComBSTR(ClassMember.c_str()), 0, &vtProp, 0, 0);
				if (SUCCEEDED(hr))
				{
					VariantToString(&vtProp, CstrRetValue);
					RetValue = CstrRetValue.GetString();
					VariantClear(&vtProp);//���vtProp
					bRet = TRUE;
				}
			}
		}
	}
END:
	ReleaseWmi();
	return bRet;

}

/*
* @fn           GetGroupItemInfo
* @brief        ���ݲ�ѯ�����һ����Ķ����Ա
* @param[in]    ClassName:  �����磺��Win32_Processor��
				ClassMember:��Ŀ���磺��SerialNumber��
				n: ��Ա����
* @param[out]   chRetValue: ���ػ�õĳ�Ա
* @return
*
* @detail      Example:
				CString strRetValue;
				CString [] strClassMem =
				{_T("Caption"),_T("CurrentClockSpeed"),_T("DeviceID"),_T("Manufacturer"),_T("Manufacturer")};
				GetGroupItemInfo(_T("Win32_Processor"),strClassMem,5,strRetValue);
* @author       mingming.shi
* @date         2021-08-27
*/
BOOL CWmiIface::GetGroupItemInfo(__in const wstring &ClassName, __in const vector<wstring> &vectClassMember, __out std::vector<wstring> &vecRetValue)
{
	USES_CONVERSION;

	CComBSTR query("SELECT * FROM ");
	CString result, info;
	VARIANT vtProp;
	ULONG uReturn;
	HRESULT hr;
	int i;
	int n = vectClassMember.size();
	BOOL bRet = FALSE;

	InitWmi();

	if (NULL != m_pWbemSvc)
	{
		query += CComBSTR(ClassName.c_str());
		hr = m_pWbemSvc->ExecQuery(CComBSTR("WQL"), query, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 0, &m_pEnumClsObj);
		if (SUCCEEDED(hr))
		{
			VariantInit(&vtProp); //��ʼ��vtProp����
			if (m_pEnumClsObj)
			{
				Sleep(10);
				uReturn = 0;
				hr = m_pEnumClsObj->Next(WBEM_INFINITE, 1, &m_pWbemClsObj, &uReturn);
				if (m_pWbemClsObj == NULL)
				{
					printf(("m_pServer == NULL : %x"), hr);
					goto END;
				}


				if (SUCCEEDED(hr) && uReturn > 0)
				{
					for (i = 0; i < n; ++i)
					{
						CString chTemp = TEXT("");
						hr = m_pWbemClsObj->Get(CComBSTR(vectClassMember[i].c_str()), 0, &vtProp, 0, 0);
						if (SUCCEEDED(hr))
						{
							VariantToString(&vtProp, info);
							chTemp = info;
							VariantClear(&vtProp);
							bRet = TRUE;
						}
						vecRetValue.push_back(chTemp.GetString());
					}
				}
			}
		}
	}

END:

	ReleaseWmi();

	return bRet;
}

/*
* @fn           VariantToString
* @brief        ��Variant���͵ı���ת��ΪCString
* @param[in]    pVar: VARIANT����
* @param[out]   chRetValue: ���string�ַ���
* @return
*
* @detail
* @author      mingming.shi
* @date        2021-08-27
*/
void CWmiIface::VariantToString(__in const LPVARIANT pVar, __out CString &chRetValue) const
{
	USES_CONVERSION;

	CComBSTR HUGEP* pBstr;
	BYTE HUGEP* pBuf;
	LONG low, high, i;
	HRESULT hr;

	switch (pVar->vt)
	{
	case VT_BSTR:
	{
		chRetValue = W2T(pVar->bstrVal);
	}
	break;
	case VT_BOOL:
	{
		if (VARIANT_TRUE == pVar->boolVal)
			chRetValue = "1";
		else
			chRetValue = "0";
	}
	break;
	case VT_I4:
	{
		chRetValue.Format(_T("%d"), pVar->lVal);
	}
	break;
	case VT_UI1:
	{
		chRetValue.Format(_T("%d"), pVar->bVal);
	}
	break;
	case VT_UI4:
	{
		chRetValue.Format(_T("%d"), pVar->ulVal);
	}
	break;

	case VT_BSTR | VT_ARRAY:
	{
		hr = SafeArrayAccessData(pVar->parray, (void HUGEP**)&pBstr);
		hr = SafeArrayUnaccessData(pVar->parray);
		chRetValue = W2T(pBstr->m_str);
	}
	break;

	case VT_I4 | VT_ARRAY:
	{
		SafeArrayGetLBound(pVar->parray, 1, &low);
		SafeArrayGetUBound(pVar->parray, 1, &high);

		hr = SafeArrayAccessData(pVar->parray, (void HUGEP**)&pBuf);
		hr = SafeArrayUnaccessData(pVar->parray);
		CString strTmp;
		high = min(high, MAX_PATH * 2 - 1);
		for (i = low; i <= high; ++i)
		{
			strTmp.Format(_T("%02X"), pBuf[i]);
			chRetValue += strTmp;
		}
	}
	break;
	default:
		break;
	}
}

CWmiIface::~CWmiIface(void)
{
}

/*
* @fn           InitWmi
* @brief        wmi��ʼ��
* @param[in]
* @param[out]
* @return
*
* @detail      ֻ�ܻ�ȡӲ����Ϣ
* @author      mingming.shi
* @date        2021-08-27
*/
HRESULT CWmiIface::InitWmi()
{
	HRESULT hr;

	// 1����ʼ��COM���
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr) || RPC_E_CHANGED_MODE == hr)
	{
		m_bInitialize = TRUE;
		hr = CoInitializeSecurity(
			NULL,
			-1,
			NULL,
			NULL,
			RPC_C_AUTHN_LEVEL_DEFAULT,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			EOAC_NONE,
			NULL
		);
		hr = CoCreateInstance(
			CLSID_WbemLocator,
			0,
			CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID *)&m_pWbemLoc);

		hr = m_pWbemLoc->ConnectServer(
			_bstr_t(L"ROOT\\CIMV2"),
			NULL,
			NULL,
			0,
			NULL,
			0,
			0,
			&m_pWbemSvc
		);

		if (FAILED(hr))
		{
			printf(("IWbemLocator::ConnectServer is failed. hr=%x"), hr);
			goto END;
		}

		hr = CoSetProxyBlanket(
			m_pWbemSvc,
			RPC_C_AUTHN_WINNT,
			RPC_C_AUTHZ_NONE,
			NULL,
			RPC_C_AUTHN_LEVEL_CALL,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			EOAC_NONE
		);
		if (FAILED(hr))
		{
			printf(("CoSetProxyBlanket is failed. hr=%x"), hr);
			goto END;
		}
	}


	return(hr);

END:
	if (FAILED(hr))
	{
		ReleaseWmi();
	}

	return(hr);
}

/*
* @fn           ReleaseWmi
* @brief        �ͷ�wmi
* @param[in]
* @param[out]
* @return
*
* @detail
* @author       mingming.shi
* @date         2021-08-27
*/
HRESULT CWmiIface::ReleaseWmi()
{
	HRESULT hr;

	if (NULL != m_pWbemSvc)
	{
		hr = m_pWbemSvc->Release();
		m_pWbemSvc = NULL;
	}
	if (NULL != m_pWbemLoc)
	{
		hr = m_pWbemLoc->Release();
		m_pWbemLoc = NULL;
	}
	if (NULL != m_pWbemClsObj)
	{
		hr = m_pWbemClsObj->Release();
		m_pWbemClsObj = NULL;
	}
	if (NULL != m_pEnumClsObj)
	{
		hr = m_pEnumClsObj->Release();
		m_pEnumClsObj = NULL;
	}
	if (m_bInitialize)
	{
		::CoUninitialize();
		m_bInitialize = FALSE;
	}
	return(hr);
}


