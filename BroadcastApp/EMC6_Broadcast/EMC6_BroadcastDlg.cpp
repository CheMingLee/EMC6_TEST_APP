
// EMC6_BroadcastDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EMC6_Broadcast.h"
#include "EMC6_BroadcastDlg.h"
#include "ctrlcard.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CEMC6_BroadcastDlg dialog




CEMC6_BroadcastDlg::CEMC6_BroadcastDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEMC6_BroadcastDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEMC6_BroadcastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_BROADCAST, m_progress);
}

BEGIN_MESSAGE_MAP(CEMC6_BroadcastDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FIRST_SHOWN, OnDialogShown)
END_MESSAGE_MAP()


// CEMC6_BroadcastDlg message handlers

BOOL CEMC6_BroadcastDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    WSADATA wsaData;
	int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        CString str;

		str.Format(_T("WSAStartup failed with error: %d"), iResult);
		MessageBox(str);
		EndDialog(IDCANCEL);
    }

	GetINIFilePath();
	
	for(int i = 0;i < MAX_DEVICE;i++)
		memset(m_Address[i], 0, 256);

	m_dwCardNum = 0;
	PostMessage(WM_FIRST_SHOWN);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEMC6_BroadcastDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEMC6_BroadcastDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEMC6_BroadcastDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CEMC6_BroadcastDlg::Search_Dev(int iRetryTime,int iSearchTime) //ms
{
	//Setup Broadcast
    SOCKET sock;
	struct sockaddr_in Recv_addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    char broadcast = '1';
    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        MessageBox(_T("Error in setting Broadcast option"));
        closesocket(sock);
        return FALSE;
    }

    Recv_addr.sin_family = AF_INET;
    Recv_addr.sin_port = htons(PORT_BROADCAST);
    Recv_addr.sin_addr.s_addr = INADDR_BROADCAST;

	if(iRetryTime > 0 && iSearchTime > 0)
	{
		m_progress.SetRange(0, iRetryTime * iSearchTime);
	}
	else
		return FALSE;
	
	//Send Cmd
	int i, j;
	char sndbuffer[1024], rcvbuffer[1024];
	int iSendSize, iRecvSize;
	fd_set oRead;
	int iReady;
	timeval timeout;	
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus,dfFreq,dfTime;

	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;

	i = 0;
	while(i < iRetryTime)
	{
		memset(sndbuffer, 0, sizeof(sndbuffer));
		*((unsigned short *)sndbuffer) = 0xffff;
		*((unsigned short *)sndbuffer + 1) = CMD_GET_IP_ADDRESS;
		*((unsigned short *)sndbuffer + 2) = 0;
		iSendSize = 6;
		sendto(sock, sndbuffer, iSendSize, 0, (sockaddr*)&Recv_addr, sizeof(Recv_addr));

		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;
		do
		{
			FD_ZERO(&oRead);
			FD_SET(sock,&oRead);
			iReady = select(sock + 1,&oRead,NULL,NULL,&timeout);
			if(iReady > 0 && FD_ISSET(sock,&oRead))
			{
				memset(rcvbuffer, 0, sizeof(rcvbuffer));
				iRecvSize = recv(sock, rcvbuffer, sizeof(rcvbuffer), 0);
				if(iRecvSize >= 6)
				{
					unsigned short usSN,usCmd,usSize;

					usSN = *((unsigned short *)rcvbuffer);
					usCmd = *((unsigned short *)rcvbuffer + 1);
					usSize = *((unsigned short *)rcvbuffer + 2);				
					if(usSN == 0xffff && usCmd == CMD_GET_IP_ADDRESS)
					{
						char szBuf[256];
						int iStrLen = strlen(rcvbuffer + 6);
						if(iStrLen == usSize && iStrLen < 255)
						{
							memcpy(szBuf,rcvbuffer + 6,iStrLen);
							szBuf[iStrLen] = 0;
							for(j = 0;j < (int)m_dwCardNum;j++)
							{
								if(strcmp(szBuf,&m_Address[j][0]) == 0)
									break;
							}
							if(j == m_dwCardNum)
							{
								memcpy(&m_Address[m_dwCardNum][0],szBuf,iStrLen);
								m_dwCardNum++;
							}
						}
					}
				}
			}
			QueryPerformanceCounter(&litmp);
			QPart2 = litmp.QuadPart;		
			dfMinus = (double)(QPart2 - QPart1);
			dfTime = dfMinus / dfFreq * 1000.;
			m_progress.SetPos(iSearchTime * i + (int)dfTime);
		}while(dfTime < iSearchTime);
		i++;
	}
	closesocket(sock);

	return TRUE;
}

LRESULT CEMC6_BroadcastDlg::OnDialogShown(WPARAM, LPARAM)
{
	int iRet, i;

	iRet = Search_Dev(2,2000);
	if(iRet)
	{
		CString strDEV, strAddress;

		for(i = 0;i < m_dwCardNum;i++)
		{
			strDEV.Format(_T("DEV%d"), i);
			strAddress.Format(_T("%s"), m_Address[i]);
			WritePrivateProfileString(_T("DEVICE"), strDEV, strAddress, m_strINIpath);
		}
	}

	WSACleanup();

	EndDialog(IDCANCEL);
	
	return 0;
}

void CEMC6_BroadcastDlg::GetINIFilePath()
{
	char szCurPath[MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString strTmp;
	
	GetModuleFileNameA(NULL, szCurPath, MAX_PATH);
	_splitpath_s(szCurPath, drive, dir, fname, ext);
	m_strINIpath.Format(_T("%s"), drive);
	strTmp.Format(_T("%s"), dir);
	m_strINIpath.Append(strTmp);
	m_strINIpath.Append(_T("DevIPAddress.ini"));
}
