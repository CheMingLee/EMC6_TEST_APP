
// EMC6_ServerDlg.h : header file
//

#pragma once

#include "ctrlcard.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT_SERVER 10000
#define PORT_BROADCAST 10001


// CEMC6_ServerDlg dialog
class CEMC6_ServerDlg : public CDialog
{
// Construction
public:
	CEMC6_ServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EMC6_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	LRESULT OnDialogShown(WPARAM, LPARAM);
	BOOL StartServer();
	void CloseServer();
	int GetHostIP(char *IPName);
	void SetPeekMsg();
	bool m_bActive;
	bool m_bWSA;
	SOCKET m_sock_udp_rcvr;
	sockaddr_in m_udp_rcvr;
	SOCKET m_sock_server;
	sockaddr_in m_tcp_server;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonExit();
};
