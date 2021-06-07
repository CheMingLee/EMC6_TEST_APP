
// EMC6_ServerDlg.h : header file
//

#pragma once

#include "ctrlcard.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT_SERVER 10000
#define PORT_BROADCAST 10001

#define MAX_AXIS 4

#define MODE_IDLE 0
#define MODE_MOVETO 1
#define MODE_JOG 2
#define MODE_JOGEND 3
#define MODE_HOME 4

//motion parameter structure 
typedef	struct rotary_param {
	long			m_lAccTime;				// unit[ms]
	long			m_lRotarySpeed;			// motion Speed [pulse/sec]
	long			m_lPulseCount;			// resolution : [pulse counts/degree * 1000] or  x-y-z : [pulse/mm * 1000]
	long			m_bGoTo0_MarkEnd;		// mark end motion to zero. (reserve, it is not used) 
	long			m_lMotorCount;			// pulse counts (reserve, it is not used)
	long			m_bAH_InPos;			// inpos snesor signal sensibility level (1/0)
	long			m_bAH_Home;				// mome snesor signal sensibility level (1/0)
	long			m_lIniSpeed;			// Init Speed [pulse/sec]
	long			m_bReverse;				// motion reverse
	long			m_bAH_Limit;			// limit snesor signal sensibility level (1/0)
	long			m_lHomeSpeed;			// enter Home Speed [pulse/sec]
	long			m_lHomeBackSpeed;		// leave Home Speed [pulse/sec]
	long			m_bEnable;				// enable (reserve, it is not used)
	long			m_lINPos_TimeOut;		// inpos sensor Timer out [ms]
	long			m_lINPos_Delay;			// Motion inposition dleay [ms]
} ROTARY_PARAM;

typedef struct position_param {
	int m_iMode;
	double m_dTarPos; // pulse
	double m_dCmdPos; // pulse
} POSITION_PARAM;

typedef struct jog_param {
	int m_iDir;
	double m_dSpeed;
	long m_lAccTime;
	double m_dAcc;
} JOG_PARAM;

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
