// stdafx.cpp : 只包括标准包含文件的源文件
// H31DHT.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


void _dout(const char *format, ...)
{
	char debugStr[1024];	
	memset(debugStr, 0, sizeof(debugStr));
	va_list vaList;
	va_start(vaList, format);
	int length = 0;
	length = vsnprintf(debugStr, sizeof(debugStr) - 1, format, vaList);
	va_end(vaList);
	if (length != 0) 
	{
		OutputDebugString(debugStr);
	}
}