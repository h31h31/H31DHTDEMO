#include "StdAfx.h"
#include "Md5.h"

using namespace std;

const ULONG Context[4]={0x67452301,0xefcdab89,0x98badcfe,0x10325476};

//==========================<内联函数实现>=========================================

ULONG CMd5::LRotate(ULONG Sdata,int nBit)
{
	return (Sdata<<nBit)|(Sdata>>(32-nBit));
}
ULONG CMd5::F(ULONG x,ULONG y,ULONG z)
{
	return (x&y)|((~x)&z);
}
ULONG CMd5::G(ULONG x,ULONG y,ULONG z)
{
	return (x&z)|(y&(~z));
}
ULONG CMd5::H(ULONG x,ULONG y,ULONG z)
{
	return x^y^z;
}
ULONG CMd5::I(ULONG x,ULONG y,ULONG z)
{
	return y^(x|(~z));
}
void CMd5::FF(ULONG &a,ULONG b,ULONG c,ULONG d,ULongPackMD5* Msg,int nBit,ULONG Cnt)
{
	a+=F(b,c,d)+Msg->value+(ULONG)Cnt;
	a=LRotate(a,nBit);
	a+=b;
}
void CMd5::GG(ULONG &a,ULONG b,ULONG c,ULONG d,ULongPackMD5* Msg,int nBit,ULONG Cnt)
{
	a+=G(b,c,d)+Msg->value+(ULONG)Cnt;
	a=LRotate(a,nBit);
	a+=b;
}
void CMd5::HH(ULONG &a,ULONG b,ULONG c,ULONG d,ULongPackMD5* Msg,int nBit,ULONG Cnt)
{
	a+=H(b,c,d)+Msg->value+(ULONG)Cnt;
	a=LRotate(a,nBit);
	a+=b;
}
void CMd5::II(ULONG &a,ULONG b,ULONG c,ULONG d,ULongPackMD5* Msg,int nBit,ULONG Cnt)
{
	a+=I(b,c,d)+Msg->value+(ULONG)Cnt;
	a=LRotate(a,nBit);
	a+=b;
}
//==========================<成员函数实现>======================================

CMd5::CMd5()
{
	IsSuc=false;
	m_pFile=NULL;
	memset(strErr,0,50);
}

CMd5::~CMd5()
{
	if(m_pFile!=NULL)
	{
		fclose(m_pFile);
		m_pFile=NULL;
	}
}
bool CMd5::OpenFile(char *filename)
{
	if(!Init())SetErr(1);
	memset(fname,0,sizeof(fname));
	sprintf(fname,"%s",filename);
	if(m_pFile!=NULL)
	{
		fclose(m_pFile);
		m_pFile=NULL;
	}
	m_pFile=fopen(fname,"rb");
	if(m_pFile==NULL)
	{   
		SetErr(0);
		return false;
	}
	else
		return CaculateFile();
}
void CMd5::TargetStr(char* str,int len)
{
	if(!Init())
		SetErr(1);
	CaculateStr(str,len);
}
bool CMd5::Init()
{
	IsSuc=true;
	memset(Digest,0,sizeof(Digest));
	try{
		memset(Pointer,0,sizeof(ULONG)*16);
	}
	catch(...)
	{   
		SetErr(1);
	}
	FileLen=0;
	for(int i=0;i<4;i++)
		T[i]=Context[i];
	return true;
}

bool CMd5::CloseFile()
{
	if(m_pFile!=NULL)
	{
		fclose(m_pFile);
		m_pFile=NULL;
	}
	return true;
}
bool CMd5::Read64Byte()
{   
	size_t readcnt;
	if(!feof(m_pFile))
	{
		memset(Pointer,0,64);
		readcnt=fread(Pointer,1,64,m_pFile);
		FileLen+=readcnt;
		if(readcnt<64)
		{
			return false;			
		}
		return true;
	}
	else
	{
		return false;
	}
}

char* CMd5::GetDigestKey()
{
	if(IsSuc)
	{		
		return Digest;
	}
	SetErr(4);
	return NULL;
}
ULONG CMd5::GetMd5Number()
{
	if(IsSuc)
	{		
		return T[0]+T[1]+T[2]+T[3];
	}
	SetErr(4);
	return 0;
}
bool CMd5::CaculateFile()
{
	if(*fname==0)
	{ 
		SetErr(2);
		return false;
	}
	bool bl;
	do
	{
		bl=Read64Byte();
		//if(bl)
			TransForm();
	}while(bl);
	JustifytoStr();
	return true;
}
bool CMd5::CaculateStr(char* str,int len)
{
	ULONG n=len/64,ntail=len%64;
	for(ULONG i=0;i<n;i++)
	{
		memset(Pointer,0,64);
		memcpy(Pointer,&str[i*64],64);
		TransForm();
	}
	memset(Pointer,0,64);
	memcpy(Pointer,&str[n*64],ntail);
	TransForm();
	JustifytoStr();
	return true;
}
void CMd5::SetErr(int nErr)
{
	if(*strErr)return;
	IsSuc=false;
	if(nErr==0)
		sprintf(strErr,"Can not open the file [ %s ] !",fname);
	else if(nErr==1)
		sprintf(strErr,"he Memory is not enough !");
	else if(nErr==2)
		sprintf(strErr,"Have not assign a file !",fname);
	else if(nErr==3)
		sprintf(strErr,"Have not assign the text !");
	else if(nErr==4)
		sprintf(strErr,"Have something wrong with MD5!");
}
char* CMd5::GetErr()
{
	if(!*strErr)
	{
		return "No error !";
	}
	else
		return strErr;
}
void  CMd5::JustifytoStr()
{
	char temp[10];
	for(int i=0;i<4;i++)
	{
		T[i]=(T[i]<<24)|((T[i]&0xff00)<<8)|((T[i]&0xff0000)>>8)|(T[i]>>24);
		sprintf(temp,"%08x",T[i]);
		strcat(Digest,temp);
	}
}

void CMd5::TransForm()
{
	ULONG a=T[0],b=T[1],c=T[2],d=T[3];
	//第一轮-------------------------------->
	FF (a, b, c, d, (ULongPackMD5*)&Pointer[ 0*4], S11, 0xd76aa478); //- 1 -
	FF (d, a, b, c, (ULongPackMD5*)&Pointer[ 1*4], S12, 0xe8c7b756); //- 2 -
	FF (c, d, a, b, (ULongPackMD5*)&Pointer[ 2*4], S13, 0x242070db); //- 3 -
	FF (b, c, d, a, (ULongPackMD5*)&Pointer[ 3*4], S14, 0xc1bdceee); //- 4 -
	FF (a, b, c, d, (ULongPackMD5*)&Pointer[ 4*4], S11, 0xf57c0faf); //- 5 -
	FF (d, a, b, c, (ULongPackMD5*)&Pointer[ 5*4], S12, 0x4787c62a); //- 6 -
	FF (c, d, a, b, (ULongPackMD5*)&Pointer[ 6*4], S13, 0xa8304613); //- 7 -
	FF (b, c, d, a, (ULongPackMD5*)&Pointer[ 7*4], S14, 0xfd469501); //- 8 -
	FF (a, b, c, d, (ULongPackMD5*)&Pointer[ 8*4], S11, 0x698098d8); //- 9 -
	FF (d, a, b, c, (ULongPackMD5*)&Pointer[ 9*4], S12, 0x8b44f7af); //- 10 -
	FF (c, d, a, b, (ULongPackMD5*)&Pointer[10*4], S13, 0xffff5bb1); //- 11 -
	FF (b, c, d, a, (ULongPackMD5*)&Pointer[11*4], S14, 0x895cd7be); //- 12 -
	FF (a, b, c, d, (ULongPackMD5*)&Pointer[12*4], S11, 0x6b901122); //- 13 -
	FF (d, a, b, c, (ULongPackMD5*)&Pointer[13*4], S12, 0xfd987193); //- 14 -
	FF (c, d, a, b, (ULongPackMD5*)&Pointer[14*4], S13, 0xa679438e); //- 15 -
	FF (b, c, d, a, (ULongPackMD5*)&Pointer[15*4], S14, 0x49b40821); //- 16 -

	//第二轮--------------------------------->
	GG (a, b, c, d, (ULongPackMD5*)&Pointer[ 1*4], S21, 0xf61e2562); //- 17 -
	GG (d, a, b, c, (ULongPackMD5*)&Pointer[ 6*4], S22, 0xc040b340); //- 18 -
	GG (c, d, a, b, (ULongPackMD5*)&Pointer[11*4], S23, 0x265e5a51); //- 19 -
	GG (b, c, d, a, (ULongPackMD5*)&Pointer[ 0*4], S24, 0xe9b6c7aa); //- 20 -
	GG (a, b, c, d, (ULongPackMD5*)&Pointer[ 5*4], S21, 0xd62f105d); //- 21 -
	GG (d, a, b, c, (ULongPackMD5*)&Pointer[10*4], S22,  0x2441453); //- 22 -
	GG (c, d, a, b, (ULongPackMD5*)&Pointer[15*4], S23, 0xd8a1e681); //- 23 -
	GG (b, c, d, a, (ULongPackMD5*)&Pointer[ 4*4], S24, 0xe7d3fbc8); //- 24 -
	GG (a, b, c, d, (ULongPackMD5*)&Pointer[ 9*4], S21, 0x21e1cde6); //- 25 -
	GG (d, a, b, c, (ULongPackMD5*)&Pointer[14*4], S22, 0xc33707d6); //- 26 -
	GG (c, d, a, b, (ULongPackMD5*)&Pointer[ 3*4], S23, 0xf4d50d87); //- 27 -
	GG (b, c, d, a, (ULongPackMD5*)&Pointer[ 8*4], S24, 0x455a14ed); //- 28 -
	GG (a, b, c, d, (ULongPackMD5*)&Pointer[13*4], S21, 0xa9e3e905); //- 29 -
	GG (d, a, b, c, (ULongPackMD5*)&Pointer[ 2*4], S22, 0xfcefa3f8); //- 30 -
	GG (c, d, a, b, (ULongPackMD5*)&Pointer[ 7*4], S23, 0x676f02d9); //- 31 -
	GG (b, c, d, a, (ULongPackMD5*)&Pointer[12*4], S24, 0x8d2a4c8a); //- 32 -

	//第三轮--------------------------------->
	HH (a, b, c, d, (ULongPackMD5*)&Pointer[ 5*4], S31, 0xfffa3942); //- 33 -
	HH (d, a, b, c, (ULongPackMD5*)&Pointer[ 8*4], S32, 0x8771f681); //- 34 -
	HH (c, d, a, b, (ULongPackMD5*)&Pointer[11*4], S33, 0x6d9d6122); //- 35 -
	HH (b, c, d, a, (ULongPackMD5*)&Pointer[14*4], S34, 0xfde5380c); //- 36 -
	HH (a, b, c, d, (ULongPackMD5*)&Pointer[ 1*4], S31, 0xa4beea44); //- 37 -
	HH (d, a, b, c, (ULongPackMD5*)&Pointer[ 4*4], S32, 0x4bdecfa9); //- 38 -
	HH (c, d, a, b, (ULongPackMD5*)&Pointer[ 7*4], S33, 0xf6bb4b60); //- 39 -
	HH (b, c, d, a, (ULongPackMD5*)&Pointer[10*4], S34, 0xbebfbc70); //- 40 -
	HH (a, b, c, d, (ULongPackMD5*)&Pointer[13*4], S31, 0x289b7ec6); //- 41 -
	HH (d, a, b, c, (ULongPackMD5*)&Pointer[ 0*4], S32, 0xeaa127fa); //- 42 -
	HH (c, d, a, b, (ULongPackMD5*)&Pointer[ 3*4], S33, 0xd4ef3085); //- 43 -
	HH (b, c, d, a, (ULongPackMD5*)&Pointer[ 6*4], S34,  0x4881d05); //- 44 -
	HH (a, b, c, d, (ULongPackMD5*)&Pointer[ 9*4], S31, 0xd9d4d039); //- 45 -
	HH (d, a, b, c, (ULongPackMD5*)&Pointer[12*4], S32, 0xe6db99e5); //- 46 -
	HH (c, d, a, b, (ULongPackMD5*)&Pointer[15*4], S33, 0x1fa27cf8); //- 47 -
	HH (b, c, d, a, (ULongPackMD5*)&Pointer[ 2*4], S34, 0xc4ac5665); //- 48 -

	//第四轮-------------------------------->
	II (a, b, c, d, (ULongPackMD5*)&Pointer[ 0*4], S41, 0xf4292244); //- 49 -
	II (d, a, b, c, (ULongPackMD5*)&Pointer[ 7*4], S42, 0x432aff97); //- 50 -
	II (c, d, a, b, (ULongPackMD5*)&Pointer[14*4], S43, 0xab9423a7); //- 51 -
	II (b, c, d, a, (ULongPackMD5*)&Pointer[ 5*4], S44, 0xfc93a039); //- 52 -
	II (a, b, c, d, (ULongPackMD5*)&Pointer[12*4], S41, 0x655b59c3); //- 53 -
	II (d, a, b, c, (ULongPackMD5*)&Pointer[ 3*4], S42, 0x8f0ccc92); //- 54 -
	II (c, d, a, b, (ULongPackMD5*)&Pointer[10*4], S43, 0xffeff47d); //- 55 -
	II (b, c, d, a, (ULongPackMD5*)&Pointer[ 1*4], S44, 0x85845dd1); //- 56 -
	II (a, b, c, d, (ULongPackMD5*)&Pointer[ 8*4], S41, 0x6fa87e4f); //- 57 -
	II (d, a, b, c, (ULongPackMD5*)&Pointer[15*4], S42, 0xfe2ce6e0); //- 58 -
	II (c, d, a, b, (ULongPackMD5*)&Pointer[ 6*4], S43, 0xa3014314); //- 59 -
	II (b, c, d, a, (ULongPackMD5*)&Pointer[13*4], S44, 0x4e0811a1); //- 60 -
	II (a, b, c, d, (ULongPackMD5*)&Pointer[ 4*4], S41, 0xf7537e82); //- 61 -
	II (d, a, b, c, (ULongPackMD5*)&Pointer[11*4], S42, 0xbd3af235); //- 62 -
	II (c, d, a, b, (ULongPackMD5*)&Pointer[ 2*4], S43, 0x2ad7d2bb); //- 63 -
	II (b, c, d, a, (ULongPackMD5*)&Pointer[ 9*4], S44, 0xeb86d391); //- 64 -

	T[0]+=a;
	T[1]+=b;
	T[2]+=c;
	T[3]+=d;
	return ;
}