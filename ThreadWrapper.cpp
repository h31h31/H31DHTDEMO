/** @file ThreadWrapper.cpp
 *  @brief 线程封装类实现
 */
#include "stdafx.h"
#include "ThreadWrapper.h"

#if WIN32
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////

CThreadWrapper::CThreadWrapper()
{
	// 初始化
	m_hThreadHandle=NULL;
	m_nPriority=100;
	m_nStackSize=1000;
	m_bStarted=false;
	m_threadID=0;
	memset(m_threadNameString,0,sizeof(m_threadNameString));
	m_iError=0;
}

CThreadWrapper::~CThreadWrapper()
{
	// 关闭
	Close();
}

//////////////////////////////////////////////////////////////////////////
// public

bool CThreadWrapper::Start(ThreadFunc func,char* pName, int priority,int stackSize,void *param)
{
	// 是否已经启动
	if(m_bStarted)
	{
		_dout(_T("ThreadWrapper the thread already started!\n"));
		m_iError=THD_OBJ_CREATED;
		return false;
	}	
	// 线程名 
	if (pName==NULL || (strcmp(pName,"")==0))
	{
		_dout(_T("ThreadWrapper input function name error!\n"));
		m_iError=THD_NAME_ERROR;
		return false;
	}

	sprintf_s(m_threadNameString,"%s",pName);

	// 优先级有效性判断
	if((priority < THREAD_PRIORITY_IDLE) || (priority > THREAD_PRIORITY_TIME_CRITICAL)) 
	{ 
		priority = THREAD_PRIORITY_NORMAL;
	}
	m_nPriority = priority;
	// 栈大小
	if((stackSize <= 0) || (stackSize > 100000000)) 
	{ 
		stackSize = 1000;
	}
	m_nStackSize=stackSize;
	
	// 创建线程
	m_hThreadHandle = CreateThread(0,   // Security Attributes,
		stackSize,						// Stack Size,
		func,							// Starting Address.
		param,							// Parmeters
		0,								// Creation Flags,
		&m_threadID);					// Thread ID (Can not be null on 95/98)

	if (m_hThreadHandle == 0)
	{
		_dout(_T("ThreadWrapper %s starting thread error,err(%d, %d)!\n"),m_threadNameString,GetLastError(), GetLastError());
		m_iError=THD_CREATE_FAILED;
		return false;
	}
	// 设置优先级
	if(!SetThreadPriority(m_hThreadHandle,m_nPriority))	
	{
		_dout(_T("ThreadWrapper %s set thread priority error,err(%d, %d)!\n"),m_threadNameString,GetLastError(), GetLastError());
		m_iError=THD_PRI_FAILED;
	}
	// 获取线程优先级
	m_nPriority=GetThreadPriority(m_hThreadHandle);	
	m_bStarted=true;	
	return true;
}

bool CThreadWrapper::IsExist()
{
	// 返回结果
	bool bRet=false;
	// 退出code
	unsigned long exitCode;
	if(m_bStarted)
	{
		if(GetExitCodeThread(m_hThreadHandle,&exitCode)==TRUE)
		{
			// 进程还在
			if(STILL_ACTIVE==exitCode)
			{
				bRet=true;
			}
		}
	}
	return bRet;
}

bool CThreadWrapper::Close(unsigned long  waitTime,bool bForceClose)
{
	if(m_bStarted)
	{
		// 释放线程句柄
		if(m_hThreadHandle!=NULL) 
		{
			_dout(_T("ThreadWrapper %s wait thread quit,wait time(%d).\n"), m_threadNameString,waitTime);
			int nRes = ::WaitForSingleObject(m_hThreadHandle, waitTime);
			// 等待到时间，线程没有退出，根据bForceClose决定是否强制退出
			if (nRes != WAIT_OBJECT_0) {
				_dout(_T("ThreadWrapper %s wait quit timeout,(err=%d).\n"), m_threadNameString,GetLastError());
				if(bForceClose) 
				{
					// 结束线程
					if(TerminateThread(m_hThreadHandle,0)==FALSE)
					{
						_dout(_T("ThreadWrapper %s terminate thread failed,(err=%d).\n"), m_threadNameString,GetLastError());
					}
				}
			}
			if(m_hThreadHandle!=NULL) 
			{
				CloseHandle(m_hThreadHandle);
				m_hThreadHandle=NULL;
			}
		}
		m_bStarted=false;
	}
	return true;
}

int CThreadWrapper::GetError()
{
	return m_iError;
}





