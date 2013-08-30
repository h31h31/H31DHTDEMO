// H31DHTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "H31DHT.h"
#include "H31DHTDlg.h"

#include "dht.h"
#include "SHA1.h"
#include "Md5.h"


#if WIN32
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CH31DHTDlg 对话框




CH31DHTDlg::CH31DHTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CH31DHTDlg::IDD, pParent)
	, m_myID(_T(""))
	, m_showID(_T(""))
	, m_showLog(_T(""))
	, m_port(1234)
	, m_hashList(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CH31DHTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_myID);
	DDX_Text(pDX, IDC_EDIT_SHOWID, m_showID);
	DDX_Text(pDX, IDC_EDIT_LOG, m_showLog);
	DDX_Control(pDX, IDC_EDIT_LOG, m_showLogCtrl);
	DDX_Text(pDX, IDC_EDIT2, m_port);
	DDV_MinMaxInt(pDX, m_port, 80, 65535);
	DDX_Control(pDX, IDC_EDIT_HASH, m_hashListCtrl);
	DDX_Text(pDX, IDC_EDIT_HASH, m_hashList);
}

BEGIN_MESSAGE_MAP(CH31DHTDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_GETID, &CH31DHTDlg::OnBnClickedButtonGetid)
	ON_BN_CLICKED(IDC_BEGIN, &CH31DHTDlg::OnBnClickedBegin)
	ON_BN_CLICKED(IDC_STOP, &CH31DHTDlg::OnBnClickedStop)
	ON_MESSAGE(WM_MYUPDATEDATA,OnUpdateMyData)
END_MESSAGE_MAP()


// CH31DHTDlg 消息处理程序

BOOL CH31DHTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//初始化网络环境
	WORD wVersionRequested = MAKEWORD(2,0);
	WSADATA wsaData;
	memset(&wsaData, 0, sizeof(wsaData));
	int err = WSAStartup(wVersionRequested, &wsaData);


	m_myID="miao31198402240912";
	m_showID="F54B1C381AB9E7D20D5DC63DCBBE074227618736";
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CH31DHTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CH31DHTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
LRESULT CH31DHTDlg::OnUpdateMyData(WPARAM wParam, LPARAM lParam)  
{  
	UpdateData(wParam);  
	return 1;  
}  
//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CH31DHTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CH31DHTDlg::OnBnClickedButtonGetid()
{
	UpdateData(TRUE);
	//unsigned char p[20];
	unsigned char p1[41];

	CSHA1 sha1;
	sha1.Reset();
	sha1.Update((const unsigned char *)m_myID.GetBuffer(),   m_myID.GetLength());
	sha1.Final();
	//sha1.GetHash(p);

	sha1.ReportHash((TCHAR*)p1, CSHA1::REPORT_HEX_SHORT);
	m_showID.Format("%s",p1);
	//sha1.ReportHash((LPTSTR)(LPCTSTR)m_showID, CSHA1::REPORT_HEX_SHORT);
	UpdateData(FALSE);

}
static CMd5 md5Mgr;

static volatile int dumping = 0;
static volatile int searching = 1;
static volatile int exiting = 0;

unsigned char hashList[3][20] = 
{
	{0x54, 0x57, 0x87, 0x89, 0xdf, 0xc4, 0x23, 0xee, 0xf6, 0x03,0x1f, 0x81, 0x94, 0xa9, 0x3a, 0x16, 0x98, 0x8b, 0x72, 0x7b},
	{0x55, 0x57, 0x87, 0x89, 0xdf, 0xc4, 0x23, 0xee, 0xf6, 0x03,0x1f, 0x81, 0x94, 0xa9, 0x3a, 0x16, 0x98, 0x8b, 0x72, 0x7b},
	{0x56, 0x57, 0x87, 0x89, 0xdf, 0xc4, 0x23, 0xee, 0xf6, 0x03,0x1f, 0x81, 0x94, 0xa9, 0x3a, 0x16, 0x98, 0x8b, 0x72, 0x7b},
};

struct LoadIPList
{
	struct sockaddr fromaddr;
	unsigned char ID[20];
	unsigned long lasttime;                
	int resverd;
};

int readFileIpNum=0;
LoadIPList ipRecvPingList[MAX_LOACL_IP_NUM];
char logshowbuffer[1024];


static void callback(void *closure,int event,unsigned char *info_hash,void *data, size_t data_len)
{
	CH31DHTDlg* pthis=(CH31DHTDlg*)closure;
	memset(logshowbuffer,0,sizeof(logshowbuffer));
	if(event == DHT_EVENT_SEARCH_DONE)
	{
		_dout("Search done.\n");
		searching=1;
		sprintf(logshowbuffer,">>Search done.\r\n");
		pthis->AddShowLog(logshowbuffer);
	}
	else if(event == DHT_EVENT_VALUES)
	{
		_dout("Received %d values.\n", (int)(data_len / 6));
		sprintf(logshowbuffer,">>Received %d values.\r\n", (int)(data_len / 6));
		pthis->AddShowLog(logshowbuffer);
	}
	else if(event==DHT_EVENT_PONG_VALUES)
	{
		pthis->GetPongIPList(info_hash,(const struct sockaddr*)data);
	}
	else if(event==DHT_EVENT_FINDNODE_VALUES)
	{
		
	}
	else if(event==DHT_EVENT_GETPEER_VALUES)
	{
		if(pthis!=NULL)
		{
			
		}
	}
	else if(event==DHT_EVENT_ANNOUNCE_PEER_VALUES)
	{
		//获取到了别人发过来的资源请求信息
		if(pthis!=NULL)
		{
			
		}
	}
}

/* Functions called by the DHT. */

int dht_blacklisted(const struct sockaddr *sa, int salen)
{
    return 0;
}

/* We need to provide a reasonably strong cryptographic hashing function.
   Here's how we'd do it if we had RSA's MD5 code. */

void dht_hash(unsigned char *hash_return, int hash_size,char *v1, int len1,char *v2, int len2,char *v3, int len3)
{
	md5Mgr.TargetStr(v1, len1);
	md5Mgr.TargetStr(v2, len2);
	md5Mgr.TargetStr(v3, len3);
	if(hash_size > 16)
		memset((char*)hash_return + 16, 0, hash_size - 16);
	memcpy(hash_return, md5Mgr.GetDigestKey(), hash_size > 16 ? 16 : hash_size);
	////-------------------------------------------------------------

}


int dht_random_bytes(void *buf, size_t size)
{
	srand(time(NULL));
	int maxint=(int)(pow(10.0,(double)size)-1);
	int rnd=rand();
	int i=rnd%maxint;

	unsigned char p1[41];
	unsigned char p[20];
	memcpy(p,&i,sizeof(i));
	CSHA1 sha1;
	sha1.Reset();
	sha1.Update(p,sizeof(i));
	sha1.Final();

	sha1.ReportHash((TCHAR*)p1, CSHA1::REPORT_HEX_SHORT);
	memcpy(buf,p1,size);
    return 1;
};
int random(void)
{
	return rand();
};

int CH31DHTDlg::AddShowLog(char *log)
{
	if(m_showLogCtrl.GetLineCount()>10000)
		m_showLog="";
	CString temp1;
	temp1.Format("%s",log);
	m_showLog+=temp1;
	SendMessage(WM_MYUPDATEDATA,FALSE);
	m_showLogCtrl.LineScroll(m_showLogCtrl.GetLineCount());//m_cEdit2是CEdit类的实例
	return 1;
};

int CH31DHTDlg::AddShowHashLog(char *log)
{
	if(m_hashListCtrl.GetLineCount()>10000)
		m_showLog="";
	CString temp1;
	temp1.Format("%s",log);
	m_hashList+=temp1;
	SendMessage(WM_MYUPDATEDATA,FALSE);
	m_hashListCtrl.LineScroll(m_hashListCtrl.GetLineCount());//m_cEdit2是CEdit类的实例
	return 1;
};

int CH31DHTDlg::GetPongIPList(unsigned char* id,const struct sockaddr* fromaddr)
{
	unsigned long now1=(unsigned long)(GetTickCount()/1000);
	bool findok=false;
	int k=0;
	for(k=0;k<readFileIpNum;k++)
	{
		if(memcmp(&ipRecvPingList[k].fromaddr,fromaddr,sizeof(sockaddr))==0)
		{
			memcpy(ipRecvPingList[k].ID,id,20);
			ipRecvPingList[k].lasttime=now1;
			findok=true;
			break;
		}
	}
	if(findok==false&&readFileIpNum+1<MAX_LOACL_IP_NUM)
	{
		memcpy(&ipRecvPingList[readFileIpNum].fromaddr,fromaddr,sizeof(sockaddr));
		memcpy(ipRecvPingList[readFileIpNum].ID,id,20);
		ipRecvPingList[readFileIpNum].lasttime=now1;
		readFileIpNum++;
	}
	struct sockaddr_in* tempip=(struct sockaddr_in *)fromaddr;
	sprintf(logshowbuffer,">>>Pong IP:%s:%d.findPos[%d/%d].\r\n",inet_ntoa(tempip->sin_addr),tempip->sin_port,k,readFileIpNum);
	AddShowLog(logshowbuffer);
	return 1;
}

int CH31DHTDlg::ReadDHTFile()
{
	FILE* file1=NULL;
	if((file1=fopen("ipList.txt","rb"))==NULL)
		return 0;

	readFileIpNum=0;

	unsigned long now1=(unsigned long)(GetTickCount()/1000);
	unsigned long Version=0xEE31FF21;
	int num=0;
	fread(&Version,1,sizeof(Version),file1);
	fread(&num,1,sizeof(num),file1);
	for(int k=0;k<num&&num<MAX_LOACL_IP_NUM;k++)
	{
		memset(&ipRecvPingList[k],0,sizeof(LoadIPList));
		fread(&ipRecvPingList[k].fromaddr,1,sizeof(sockaddr),file1);
		ipRecvPingList[k].lasttime=now1;
		if(ipRecvPingList[k].fromaddr.sa_family!=0)
		{
			readFileIpNum++;
		}
	}
	sprintf(logshowbuffer,"<<ReadIPList[%d].\r\n",readFileIpNum);
	AddShowLog(logshowbuffer);

	fclose(file1);
	file1=NULL;
	return 1;
}

int CH31DHTDlg::SaveDHTFile()
{
	FILE* file1=NULL;
	if((file1=fopen("ipList.txt","wb+"))==NULL)
		return 0;


	unsigned long now1=(unsigned long)(GetTickCount()/1000);

	int realcnt=0;
	for(int k=0;k<readFileIpNum;k++)
	{
		if(now1<ipRecvPingList[k].lasttime+15*60)
		{
			realcnt++;
		}
	}

	if(realcnt==0)
	{
		_dout("Error save nothing.\r\n");
		return -1;
	}
	unsigned long Version=0xEE31FF21;
	fwrite(&Version,1,sizeof(Version),file1);
	fwrite(&realcnt,1,sizeof(readFileIpNum),file1);
	int cnt1=0;
	for(int k=0;k<readFileIpNum;k++)
	{
		if(now1<ipRecvPingList[k].lasttime+15*60)
		{
			fwrite(&ipRecvPingList[k].fromaddr,1,sizeof(sockaddr),file1);
			cnt1++;
		}
	}
	fclose(file1);
	file1=NULL;
	sprintf(logshowbuffer,"<<SaveIPList[%d/%d].\r\n",realcnt,readFileIpNum);
	AddShowLog(logshowbuffer);
	return 1;
}

void CH31DHTDlg::OnBnClickedBegin()
{
	UpdateData(TRUE);

	//初始化socket
	m_soListen =(int)socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (m_soListen == INVALID_SOCKET) {
		m_iErrorNo=WSAGetLastError();
		_dout(_T("CH31CarMonitorDlg Start Error(%d).\n"),m_iErrorNo);
		return;
	}
	//初始化服务器地址
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//绑定端口监听
	if (bind(m_soListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		m_iErrorNo=WSAGetLastError();
		_dout(_T("CH31CarMonitorDlg Start Error(%d).\n"),m_iErrorNo);
		MessageBox("端口已经被占用!");
		return;
	}

	//这里应该创建一个线程来处理 捕获数据
	m_bDataThread=m_dataThread.Start((ThreadFunc)DataMgrThread,"/H31CarDataMgrThread",THREAD_PRIORITY_ABOVE_NORMAL,10000,this);
	_dout(_T("CH31CarMonitorDlg Start Thread OK(%d).\n"),m_bDataThread);
	sprintf(logshowbuffer,"<<Start Thread OK(%d) PORT(%d).\r\n",m_bDataThread,m_port);
	AddShowLog(logshowbuffer);

	GetDlgItem(IDC_BEGIN)->EnableWindow(false);
}
void CH31DHTDlg::OnBnClickedStop()
{
	if(m_bDataThread)
	{
		GetDlgItem(IDC_BEGIN)->EnableWindow(true);
		m_bDataThread=false;
		m_dataThread.Close(1000,true);

		int res1=SaveDHTFile();
		dht_uninit();

		if (m_soListen != INVALID_SOCKET) closesocket(m_soListen);
		m_soListen = INVALID_SOCKET;
	}
}

//在这个线程里捕获数据
DWORD CH31DHTDlg::DataMgrThread(LPVOID lpParameter)
{
	_dout(_T("CH31DHTDlg MyCaptureThread Begin.\n"));
	CH31DHTDlg* pthis=(CH31DHTDlg*)lpParameter;
	pthis->ManageDHTProcess();
	_dout(_T("CH31DHTDlg MyCaptureThread End.\n"));
	return 1;
}

int CH31DHTDlg::ManageDHTProcess()
{
	int res=ReadDHTFile();
	struct sockaddr_storage bootstrap_nodes[10];
	int num_bootstrap_nodes = 0;
	int rc=0;

	struct addrinfo hints1, *info, *infop;
	memset(&hints1, 0, sizeof(hints1));
	hints1.ai_socktype = SOCK_DGRAM;
	hints1.ai_family = AF_INET;


	rc = getaddrinfo("router.utorrent.com","6881", &hints1, &info);
	//rc = getaddrinfo("router.bittorrent.com","6881", &hints1, &info);
	//rc = getaddrinfo("dht.transmissionbt.com","6881", &hints1, &info);
	if(rc != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
		exit(1);
	}
	infop = info;
	while(infop) 
	{
		memcpy(&bootstrap_nodes[num_bootstrap_nodes],infop->ai_addr, infop->ai_addrlen);
		infop = infop->ai_next;
		num_bootstrap_nodes++;
	}
	freeaddrinfo(info);

	rc = getaddrinfo("router.bittorrent.com","6881", &hints1, &info);
	if(rc != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
		exit(1);
	}
	infop = info;
	while(infop) 
	{
		memcpy(&bootstrap_nodes[num_bootstrap_nodes],infop->ai_addr, infop->ai_addrlen);
		infop = infop->ai_next;
		num_bootstrap_nodes++;
	}
	freeaddrinfo(info);

	rc = getaddrinfo("dht.transmissionbt.com","6881", &hints1, &info);
	if(rc != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
		exit(1);
	}
	infop = info;
	while(infop) 
	{
		memcpy(&bootstrap_nodes[num_bootstrap_nodes],infop->ai_addr, infop->ai_addrlen);
		infop = infop->ai_next;
		num_bootstrap_nodes++;
	}
	freeaddrinfo(info);



	int s6=-1;
	
	rc = dht_init(m_soListen, s6, (unsigned char*)m_showID.GetBuffer(),NULL);
	if(rc < 0) {
		perror("dht_init");
		exit(1);
	}


	for(int i = 0; i < num_bootstrap_nodes; i++) 
	{
		dht_ping_node((struct sockaddr*)&bootstrap_nodes[i],sizeof(bootstrap_nodes[i]));
		Sleep(random() % 1000);
	}

	int searchagainCnt=0;

	int opt;
	int quiet = 0, ipv4 = 1, ipv6 = 1;
	struct sockaddr_in sin;
	struct sockaddr_in6 sin6;
	struct sockaddr from1;
	socklen_t fromlen;
	time_t socketWaitSleep=1;
	time_t tosleep=0;

	int ipListPOS=0;
	struct timeval tv;
	fd_set readfds;
	tv.tv_sec = socketWaitSleep;
	tv.tv_usec = random() % 1000000;
	while(m_bDataThread)
	{

		FD_ZERO(&readfds);
		if(m_soListen >= 0)
			FD_SET(m_soListen, &readfds);
		if(s6 >= 0)
			FD_SET(s6, &readfds);
		rc = select(m_soListen > s6 ? m_soListen + 1 : s6 + 1, &readfds, NULL, NULL, &tv);
		if(rc <0) 
		{
			if(errno != EINTR) {
				perror("select");
				Sleep(1000);
			}
		}
	    
		if(exiting)
		    break;

		if(rc > 0) 
		{
			fromlen = sizeof(from1);
			memset(buf,0,sizeof(buf));
			if(m_soListen >= 0 && FD_ISSET(m_soListen, &readfds))
				rc = recvfrom(m_soListen, buf, sizeof(buf) - 1, 0,&from1, &fromlen);
			else if(s6 >= 0 && FD_ISSET(s6, &readfds))
				rc = recvfrom(s6, buf, sizeof(buf) - 1, 0,&from1, &fromlen);
			else
				abort();
		}

		if(rc > 0) 
		{
			buf[rc] = '\0';
			rc = dht_periodic(buf, rc, &from1, fromlen,&tosleep, callback, this);
			char showlog[256];
			struct sockaddr_in* tempip=(struct sockaddr_in *)&from1;
			sprintf_s(showlog,"%s:[%d] Len:%d [%s]\r\n",inet_ntoa(tempip->sin_addr),tempip->sin_port,rc,&buf[rc-20]);

		} 
		else 
		{
			rc = dht_periodic(NULL, 0, NULL, 0, &tosleep, callback, this);
		}
		if(rc < 0) 
		{
			if(errno == EINTR) {
				continue;
			} 
			else 
			{
				perror("dht_periodic");
				if(rc == EINVAL || rc == EFAULT)
					abort();
				socketWaitSleep = 1;
			}
		}

		/* This is how you trigger a search for a torrent hash.  If port (the second argument) is non-zero, it also performs an announce.
		   Since peers expire announced data after 30 minutes, it's a good idea to reannounce every 28 minutes or so. */
		if(searching) {
			dht_random_bytes((void*)hashList[2],20);
			if(m_soListen >= 0)
				dht_search(hashList[2], 0, AF_INET, callback, this);
			if(s6 >= 0)
				dht_search(hashList[2], 0, AF_INET6, callback, this);
			searching = 0;
		}

		//重新来搜索
		searchagainCnt++;
		if(searchagainCnt*socketWaitSleep>30)
		{
			searchagainCnt=0;
			searching=1;
		}


		unsigned long now1=(unsigned long)(GetTickCount()/1000);

		if(ipListPOS<readFileIpNum)
		{
			dht_ping_node(&ipRecvPingList[ipListPOS].fromaddr,sizeof(sockaddr));
			if(ipRecvPingList[ipListPOS].lasttime+15*60>now1)
			{
				unsigned char tid[16];
				make_tid(tid, "fn", 0);
				send_find_node(&ipRecvPingList[ipListPOS].fromaddr,sizeof(sockaddr),tid,4,ipRecvPingList[ipListPOS].ID,0,0);
				memset(tid,0,sizeof(tid));
				make_tid(tid, "gp", 0);
				send_get_peers(&ipRecvPingList[ipListPOS].fromaddr,sizeof(sockaddr),tid,4,hashList[0],0,0);
			}
			else
				dht_ping_node(&ipRecvPingList[ipListPOS].fromaddr,sizeof(sockaddr));
			ipListPOS++;
		}
		else
		{
			ipListPOS=0;
			dht_random_bytes((void*)hashList[0],20);
		}

		{
			struct sockaddr_in sin[500];
			struct sockaddr_in6 sin6[500];
			int num = 500, num6 = 500;
			int numno = 500, num6no = 500;
			int i;
			i = dht_get_nodes(sin, &num,&numno, sin6, &num6, &num6no);
			_dout("Found %d (%d + %d) good nodes and (%d + %d) no good nodes.\n", i, num, num6, numno, num6no);
		}

	}

}

void CH31DHTDlg::Tr_Sha1_To_Hex (char *out, const unsigned char *sha1) 
{ 
	int i;
	static const char hex[] = "0123456789ABCDEF"; 
	for (i=0; i<20; ++i) 
	{ 
		const unsigned int val = *sha1++; 
		*out++ = hex[val >> 4];
		*out++ = hex[val & 0xf]; 
	} 
	*out = '\0'; 
} 

