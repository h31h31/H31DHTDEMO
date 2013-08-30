// H31DHTDlg.h : 头文件
//

#pragma once
#include "ThreadWrapper.h"
#include <wininet.h>
#include "afxwin.h"
#include "math.h"
#define MAXBLOCKSIZE 1024
#define WM_MYUPDATEDATA WM_USER+100 
#define  MAX_LOACL_IP_NUM 10000

// CH31DHTDlg 对话框
class CH31DHTDlg : public CDialog
{
// 构造
public:
	CH31DHTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_H31DHT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	int GetPongIPList(unsigned char* id,const struct sockaddr* fromaddr);
	int AddShowLog(char *log);
	int AddShowHashLog(char *log);
private:
	/**  数据处理线程 */
	CThreadWrapper m_dataThread;
	/**  是否开启线程对象 */
	bool m_bDataThread; 
	/**  监听新连接的句柄 */
	int m_soListen;	
	/**  主线程句柄 */
	HANDLE m_hMainThread;
	/**  错误号 */
	int m_iErrorNo;	

	char buf[4096];

	/**  
	* 主要处理线程 
	* @param lpParam 传入类指针.
	* @return 返回DWORD类型
	*/
	static DWORD WINAPI DataMgrThread(LPVOID lpParameter);

	int ManageDHTProcess();
	void Tr_Sha1_To_Hex (char *out, const unsigned char *sha1);

	int ReadDHTFile();
	int SaveDHTFile();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CString m_myID;
	CString m_showLog;
	CEdit m_showLogCtrl;
	int m_port;
	CEdit m_hashListCtrl;
	CString m_hashList;
	CString m_showID;

public:
	afx_msg void OnBnClickedButtonGetid();
	afx_msg void OnBnClickedBegin();
	afx_msg void OnBnClickedStop();
};
