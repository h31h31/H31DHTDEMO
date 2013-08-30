#ifndef SHA1_H_A545E61D43E9404E8D736869AB3CBFE7
#define SHA1_H_A545E61D43E9404E8D736869AB3CBFE7

#if !defined(SHA1_UTILITY_FUNCTIONS) && !defined(SHA1_NO_UTILITY_FUNCTIONS)
#define SHA1_UTILITY_FUNCTIONS
#endif

#if !defined(SHA1_STL_FUNCTIONS) && !defined(SHA1_NO_STL_FUNCTIONS)
#define SHA1_STL_FUNCTIONS
#if !defined(SHA1_UTILITY_FUNCTIONS)
#error STL functions require SHA1_UTILITY_FUNCTIONS.
#endif
#endif

#include <memory.h>
#include <limits.h>

#ifdef SHA1_UTILITY_FUNCTIONS
#include <stdio.h>
#include <string.h>
#endif

#ifdef SHA1_STL_FUNCTIONS
#include <string>
#endif

#ifdef _MSC_VER
#include <stdlib.h>
#endif

// You can define the endian mode in your files without modifying the SHA-1
// source files. Just #define SHA1_LITTLE_ENDIAN or #define SHA1_BIG_ENDIAN
// in your files, before including the SHA1.h header file. If you don't
// define anything, the class defaults to little endian.
#if !defined(SHA1_LITTLE_ENDIAN) && !defined(SHA1_BIG_ENDIAN)
#define SHA1_LITTLE_ENDIAN
#endif

// If you want variable wiping, #define SHA1_WIPE_VARIABLES, if not,
// #define SHA1_NO_WIPE_VARIABLES. If you don't define anything, it
// defaults to wiping.
#if !defined(SHA1_WIPE_VARIABLES) && !defined(SHA1_NO_WIPE_VARIABLES)
#define SHA1_WIPE_VARIABLES
#endif

#if defined(SHA1_HAS_TCHAR)
#include <tchar.h>
#else
#ifdef _MSC_VER
#include <tchar.h>
#else
#ifndef TCHAR
#define TCHAR char
#endif
#ifndef _T
#define _T(__x) (__x)
#define _tmain main
#define _tprintf printf
#define _getts gets
#define _tcslen strlen
#define _tfopen fopen
#define _tcscpy strcpy
#define _tcscat strcat
#define _sntprintf snprintf
#endif
#endif
#endif

///////////////////////////////////////////////////////////////////////////
// Define variable types

#ifndef UINT_8
#ifdef _MSC_VER // Compiling with Microsoft compiler
#define UINT_8 unsigned __int8
#else // !_MSC_VER
#define UINT_8 unsigned char
#endif // _MSC_VER
#endif

#ifndef UINT_32
#ifdef _MSC_VER // Compiling with Microsoft compiler
#define UINT_32 unsigned __int32
#else // !_MSC_VER
#if (ULONG_MAX == 0xFFFFFFFFUL)
#define UINT_32 unsigned long
#else
#define UINT_32 unsigned int
#endif
#endif // _MSC_VER
#endif // UINT_32

#ifndef INT_64
#ifdef _MSC_VER // Compiling with Microsoft compiler
#define INT_64 __int64
#else // !_MSC_VER
#define INT_64 long long
#endif // _MSC_VER
#endif // INT_64

#ifndef UINT_64
#ifdef _MSC_VER // Compiling with Microsoft compiler
#define UINT_64 unsigned __int64
#else // !_MSC_VER
#define UINT_64 unsigned long long
#endif // _MSC_VER
#endif // UINT_64

///////////////////////////////////////////////////////////////////////////
// Declare SHA-1 workspace

typedef union
{
	UINT_8 c[64];
	UINT_32 l[16];
} SHA1_WORKSPACE_BLOCK;

class CSHA1
{
public:
#ifdef SHA1_UTILITY_FUNCTIONS
	// Different formats for ReportHash(Stl)
	enum REPORT_TYPE
	{
		REPORT_HEX = 0,
		REPORT_DIGIT = 1,
		REPORT_HEX_SHORT = 2
	};
#endif

	// Constructor and destructor
	CSHA1();

#ifdef SHA1_WIPE_VARIABLES
	~CSHA1();
#endif

	void Reset();

	// Hash in binary data and strings
	void Update(const UINT_8* pbData, UINT_32 uLen);

#ifdef SHA1_UTILITY_FUNCTIONS
	// Hash in file contents
	bool HashFile(const TCHAR* tszFileName);
#endif

	// Finalize hash; call it before using ReportHash(Stl)
	void Final();

#ifdef SHA1_UTILITY_FUNCTIONS
	bool ReportHash(TCHAR* tszReport, REPORT_TYPE rtReportType = REPORT_HEX) const;
#endif

#ifdef SHA1_STL_FUNCTIONS
	bool ReportHashStl(std::basic_string<TCHAR>& strOut, REPORT_TYPE rtReportType =
		REPORT_HEX) const;
#endif

	// Get the raw message digest (20 bytes)
	bool GetHash(UINT_8* pbDest20) const;

private:
	// Private SHA-1 transformation
	void Transform(UINT_32* pState, const UINT_8* pBuffer);

	// Member variables
	UINT_32 m_state[5];
	UINT_32 m_count[2];
	UINT_32 m_reserved0[1]; // Memory alignment padding
	UINT_8 m_buffer[64];
	UINT_8 m_digest[20];
	UINT_32 m_reserved1[3]; // Memory alignment padding

	UINT_8 m_workspace[64];
	SHA1_WORKSPACE_BLOCK* m_block; // SHA1 pointer to the byte array above
};

#endif // SHA1_H_A545E61D43E9404E8D736869AB3CBFE7
