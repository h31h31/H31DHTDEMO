/** @file ThreadWrapper.h
 *  @brief 线程封装类定义
 */
#pragma once

/** 无限等待 */
#define WAIT_FOREVER	INFINITE

/** 线程错误常量起始 */
#define THDMBASEERR              600

/** 线程对象已经创建 */
#define THD_OBJ_CREATED			(THDMBASEERR+1)
/** 线程名称错误 */
#define THD_NAME_ERROR			(THDMBASEERR+2)
/** 线程创建失败 */
#define THD_CREATE_FAILED		(THDMBASEERR+3)
/** 线程设置优先级失败 */
#define THD_PRI_FAILED			(THDMBASEERR+4)

/** 线程函数定义 */
typedef unsigned long (WINAPI *ThreadFunc)(void *);

/**	@class CThreadWrapper
 *	@brief 线程封装类
 *	@author miao
 *	@version 1.0.0.0
 *	@date 2011-10-09
 *
 *	包装了对线程的使用,主要对 CreateThread() 函数进行了封装。使用方法：定义线程对象后然后使用
 *	 Start() 函数启动线程，线程结束的时候调用 Close() 释放线程句柄， Start() 函数入参优先级
 *	priority一般设置为THREAD_PRIORITY_NORMAL。
 *
 */
class CThreadWrapper
{
public:
	/**
	 * 构造
	 */
	CThreadWrapper();	
	/**
	 * 析构
	 */
	virtual ~CThreadWrapper();	

protected:
	/** 线程ID */
	unsigned long m_threadID;
	/** 线程名 */
	char m_threadNameString[128];
	/** 线程Handle */
	HANDLE m_hThreadHandle;	
	/** 线程优先级 -15~15,normal 0 */
	int m_nPriority;
	/** 栈大小 */
	int m_nStackSize;	
	/** 是否启动 */
	bool m_bStarted;	
	/** 错误类型 */	 
	int m_iError; 

public:
	/**
	 * 创建线程 
	 * @param func 线程函数
	 * @param name 对于Windows不使用
	 * @param priority 线程优先级
	 * @param stackSize 栈大小
	 * @param param 线程函数入参
	 * @return 是否创建启动成功
	 */	
	bool Start(ThreadFunc func,char* pName, int priority,int stackSize,void *param);
	/**
	 * 关闭
	 * @param waitTime 等待线程结束时间，默认一秒
	 * @param bForceClose 等待线程超时，线程还没有退出，是否强制结束线程
	 * @return 是否关闭成功
	 */	
	bool Close(unsigned long waitTime=1000,bool bForceClose=false); 
	/**
	 * 判断线程是否存在
	 * @return 是否存在
	 */	
	bool IsExist(); 
	/**
	 * 获取错误
	 * @return 返回错误
	 */		
	int GetError(); 
};


