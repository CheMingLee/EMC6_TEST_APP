
// EMC6_BroadcastDlg.h : header file
//

#pragma once
#include "afxcmn.h"

#define MAX_ID 16
#define MAX_DEVICE MAX_ID

#define PORT_BROADCAST 10001

#define WM_FIRST_SHOWN WM_USER + 100

// CEMC6_BroadcastDlg dialog
class CEMC6_BroadcastDlg : public CDialog
{
// Construction
public:
	CEMC6_BroadcastDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EMC6_BROADCAST_DIALOG };

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
	CProgressCtrl m_progress;
	BOOL Search_Dev(int iRetryTime,int iSearchTime);
	DWORD m_dwCardNum; // �����w�������O�d�ƶq
	char m_Address[MAX_DEVICE][256]; // �x�s�U�]�Ƥ� IP ��}
	LRESULT OnDialogShown(WPARAM, LPARAM);
	CString m_strINIpath;
};
