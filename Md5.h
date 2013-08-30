#pragma once

#ifndef __MD5CLASS__
#define __MD5CLASS__

#include <stdio.h>
#if WIN32
	#include <io.h>
	#include <conio.h>
	#include <stdexcept>
#else
	#include "string.h"
#endif

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21
#ifdef WIN32
/** 定义结构体内存对齐为1字节 */
#pragma pack(1)
#else 
#pragma pack(1,1,1)
#endif
/**  
* @struct ULongPackMD5
* @brief unsigned long重新定义 
*/
typedef struct tagULongPackMD5
{
	/**  使用值 */
	unsigned long value;
	tagULongPackMD5()
	{
		value = 0;
	};
}ULongPackMD5;
typedef unsigned long ULONG;
#pragma  pack()
/**
*  @class CMD5
*  @brief 文件MD5加密算法类,支持PPC环境下的大小字节序问题
*  @author  Miao
*  @version  1.0.0.0
*  @date  2010-07-05
*
*  说明使用方法：该类主要用于生成文件的MD5值，用于检测文件是否传输完整;使用方法如下
*  CMd5 themd5;
*  方法1:打开文件方式
*  themd5.OpenFile("c://1.txt");
*  char md5str[32];
*  memset(md5str,0,sizeof(md5str));
*  memcpy(md5str,themd5.GetDigestKey(),32);
*  unsigned long a=themd5.GetMd5Number();
*  themd5.CloseFile();
*  方法1:传入字符串的方法
*  themd5.(char* str,int len);
*  memcpy(md5str,themd5.GetDigestKey(),32);
*  unsigned long a=themd5.GetMd5Number();
*/
class CMd5
{
public:
	/**  构造函数 */
	CMd5();
	/**  析构函数 */
	~CMd5();
	/**  输入文件名路径进行计算 */
	bool OpenFile(char *filename=NULL);
	/**  直接输入字符串进行计算 */
	void TargetStr(char* str,int len);
	/**  取得返回的MD5字符串 */
	char* GetDigestKey();
	/**  取得错误 */
	char* GetErr();
	/**  取得MD5的数字型数据 */
	ULONG GetMd5Number();
	/**  关闭文件指针 */
	bool CloseFile();

private:
	inline ULONG LRotate(ULONG Sdata,int nBit);
	inline ULONG F(ULONG x,ULONG y,ULONG z);
	inline ULONG G(ULONG x,ULONG y,ULONG z);
	inline ULONG H(ULONG x,ULONG y,ULONG z);
	inline ULONG I(ULONG x,ULONG y,ULONG z);
	inline void FF(ULONG &a,ULONG b,ULONG c,ULONG d,ULongPackMD5* Msg,int nBit,ULONG Cnt);
	inline void GG(ULONG &a,ULONG b,ULONG c,ULONG d,ULongPackMD5* Msg,int nBit,ULONG Cnt);
	inline void HH(ULONG &a,ULONG b,ULONG c,ULONG d,ULongPackMD5* Msg,int nBit,ULONG Cnt);
	inline void II(ULONG &a,ULONG b,ULONG c,ULONG d,ULongPackMD5* Msg,int nBit,ULONG Cnt);
private:
	bool Init();
	bool Read64Byte();
	void TransForm();
	bool CaculateFile();
	bool CaculateStr(char* str,int len);
	void SetErr(int nErr);
	void JustifytoStr();
protected:
	FILE* m_pFile;
	char Pointer[64];
	ULONG T[4];
	bool IsSuc;
	char fname[256];
	long long FileLen;
	char strErr[50];
	char Digest[33];
};
#endif