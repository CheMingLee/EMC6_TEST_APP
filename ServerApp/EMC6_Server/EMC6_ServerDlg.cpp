
// EMC6_ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EMC6_Server.h"
#include "EMC6_ServerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DWORD WINAPI EchoHandler(void* sd_)
{
	SOCKET sd = (SOCKET)sd_;

    // Read data from client
    char acReadBuffer[1024];
    int nReadBytes;
    do {
        nReadBytes = recv(sd, acReadBuffer, 1024, 0);
        if (nReadBytes > 0)
		{
			CString strLog;
			unsigned short usSN, usCmd, usSize;

			usSN = *((unsigned short *)acReadBuffer);
			usCmd = *((unsigned short *)acReadBuffer + 1);
			usSize = *((unsigned short *)acReadBuffer + 2);
			
			strLog.Format("Receive: %d Bytes. SN: 0x%x, Cmd: 0x%x, DataSize: %d Bytes", nReadBytes, usSN, usCmd, usSize);

			char szCurPath[MAX_PATH];
			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];
			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];
			CString strDate, strLogPath;

			strDate = CTime::GetCurrentTime().Format("%Y%m%d");
			
			GetModuleFileName(NULL, szCurPath, MAX_PATH);
			_splitpath_s(szCurPath, drive, dir, fname, ext);
			strLogPath.Format("%s", drive);
			strLogPath.Append(dir);
			strLogPath.Append(strDate);
			strLogPath.Append(".log");

			LPCWSTR pstrPath;

			pstrPath = strLogPath.AllocSysString();
			HANDLE hLogFile = CreateFileW(pstrPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			CString str, strTime;

			strTime = CTime::GetCurrentTime().Format("%H:%M:%S - ");
			str = strTime + strLog;
			str.Append("\n");

			SetFilePointer(hLogFile, 0, NULL, FILE_END);
			WriteFile(hLogFile, str, strlen(str), NULL, NULL);
        }
    } while (nReadBytes != 0);

	return 0;
}

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


// CEMC6_ServerDlg dialog




CEMC6_ServerDlg::CEMC6_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEMC6_ServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEMC6_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEMC6_ServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_START, &CEMC6_ServerDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CEMC6_ServerDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CEMC6_ServerDlg::OnBnClickedButtonExit)
END_MESSAGE_MAP()


// CEMC6_ServerDlg message handlers

BOOL CEMC6_ServerDlg::OnInitDialog()
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
	CString strLog;

	m_sock_udp_rcvr = INVALID_SOCKET;
	m_sock_server = INVALID_SOCKET;
	m_hLogFile = NULL;
	m_bWSA = false;
	m_bActive = false;
	
	GetLogFilePath();
	if(!OpenLog())
	{
		CString strDateTime = CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S - ");
		strLog.Format("Open log file failed!\nErrCode: %d", GetLastError());
		MessageBox(strDateTime + strLog);
		EndDialog(IDCANCEL);
	}
	else
	{
		strLog = "Log file opened!";
		WriteLog(strLog);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEMC6_ServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEMC6_ServerDlg::OnPaint()
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
HCURSOR CEMC6_ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEMC6_ServerDlg::OnDestroy()
{
	CDialog::OnDestroy();
	
	CString strLog;
	
	CloseServer();
	strLog = "Exit!";
	WriteLog(strLog);
	CloseLog();
}

BOOL CEMC6_ServerDlg::StartServer()
{
	int iResult;
	WSADATA wsaData;
	CString strLog;
	int i = 1;
	char broadcast = '1';
	unsigned long b = 1;
	
	// Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
		strLog.Format("WSAStartup failed with error: %d", iResult);
		WriteLog(strLog);
		return FALSE;
    }
	m_bWSA = true;

	// Create a SOCKET for receiving the broadcast
	m_sock_udp_rcvr = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_sock_udp_rcvr == INVALID_SOCKET)
	{	
		strLog.Format("socket failed with error: %d", WSAGetLastError());
		WriteLog(strLog);
		return FALSE;
	}

    iResult = setsockopt(m_sock_udp_rcvr, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	if(iResult < 0)
    {
        strLog = "Error in setting Broadcast option";
		WriteLog(strLog);
        return FALSE;
    }
	
	// Create a SOCKET for connecting to server
	m_sock_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_sock_server == INVALID_SOCKET)
	{	
		strLog.Format("socket failed with error: %d", WSAGetLastError());
		WriteLog(strLog);
		return FALSE;
	}

	iResult = setsockopt(m_sock_server, SOL_SOCKET, SO_REUSEADDR, (char*)&i, sizeof(i));
	if(iResult < 0)
	{
        strLog = "Error in setting Server option";
		WriteLog(strLog);
        return FALSE;
	}
	
	// Setup sockaddr_in
	m_udp_rcvr.sin_family = AF_INET;
	m_udp_rcvr.sin_addr.s_addr = INADDR_ANY;
	m_udp_rcvr.sin_port = htons(PORT_BROADCAST);
	
	m_tcp_server.sin_family = AF_INET;
	m_tcp_server.sin_addr.s_addr = INADDR_ANY;
	m_tcp_server.sin_port = htons(PORT_SERVER);
	
	//Binding part - Broadcast receiver
	iResult = bind(m_sock_udp_rcvr, (sockaddr*)&m_udp_rcvr, sizeof(m_udp_rcvr));
    if (iResult == SOCKET_ERROR)
	{
		strLog.Format("bind failed with error: %d", WSAGetLastError());
		WriteLog(strLog);
		return FALSE;
    }
	
	//Binding part - Server
	iResult = bind(m_sock_server, (sockaddr*)&m_tcp_server, sizeof(m_tcp_server));
	if (iResult == SOCKET_ERROR)
	{
		strLog.Format("bind failed with error: %d", WSAGetLastError());
		WriteLog(strLog);
		return FALSE;
	}
	
	// Setup the TCP listening socket
	iResult = listen(m_sock_server, 5);
	if (iResult == SOCKET_ERROR)
	{
		strLog.Format("listen failed with error: %d", WSAGetLastError());
		WriteLog(strLog);
		return FALSE;
	}
	
	//make it non blocking
	iResult = ioctlsocket(m_sock_udp_rcvr, FIONBIO, &b);
	if(iResult != 0)
	{
		strLog.Format("Set non blocking failed with error: %d", WSAGetLastError());
		WriteLog(strLog);
		return FALSE;
	}

	iResult = ioctlsocket(m_sock_server, FIONBIO, &b);
	if(iResult != 0)
	{
		strLog.Format("Set non blocking failed with error: %d", WSAGetLastError());
		WriteLog(strLog);
		return FALSE;
	}

	return TRUE;
}

void CEMC6_ServerDlg::SetPeekMsg()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

int CEMC6_ServerDlg::GetHostIP(char *IPName)
{
	char hostName[256];
    struct hostent* pHost;
    struct sockaddr_in SocketAddress;

	gethostname(hostName, sizeof(hostName));
    pHost = gethostbyname(hostName);
    memcpy(&SocketAddress.sin_addr, pHost->h_addr_list[0], pHost->h_length);
    strcpy(IPName, inet_ntoa(SocketAddress.sin_addr));

	return strlen(IPName);
}

void CEMC6_ServerDlg::OnBnClickedButtonStart()
{
	int iRet;
	CString strLog;
	int ilen = sizeof(struct sockaddr_in);
	char szIPName[256];
	char sndbuffer[1024], rcvbuffer[1024];
	int iSendSize, iRecvSize, iDataSize;

	sockaddr_in sinRemote;

	strLog = "Server starting!";
	WriteLog(strLog);
	
	iRet = StartServer();
	if(!iRet)
	{
		CloseServer();
		strLog = "Starting server failed!";
		WriteLog(strLog);
		return;
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

	strLog = "Server running!";
	WriteLog(strLog);

	memset(szIPName, 0, sizeof(szIPName));
	iDataSize = GetHostIP(szIPName);

	m_bActive = true;
	// main loop
	while(m_bActive)
	{
		SetPeekMsg();

		// Broadcast receiver
		memset(rcvbuffer, 0, sizeof(rcvbuffer));
		iRecvSize = recvfrom(m_sock_udp_rcvr, rcvbuffer, sizeof(rcvbuffer), 0, (sockaddr*)&m_udp_rcvr, &ilen);
		if(iRecvSize >= 6)
		{
			unsigned short usSN, usCmd, usSize;

			usSN = *((unsigned short *)rcvbuffer);
			usCmd = *((unsigned short *)rcvbuffer + 1);
			usSize = *((unsigned short *)rcvbuffer + 2);
			if(usSN == 0xffff && usCmd == CMD_GET_IP_ADDRESS)
			{
				strLog.Format("Receive: %d Bytes. Cmd: CMD_GET_IP_ADDRESS", iRecvSize);
				WriteLog(strLog);
				
				memset(sndbuffer, 0, sizeof(sndbuffer));
				*((unsigned short *)sndbuffer) = 0xffff;
				*((unsigned short *)sndbuffer + 1) = CMD_GET_IP_ADDRESS;
				*((unsigned short *)sndbuffer + 2) = (unsigned short)iDataSize;
				memcpy(sndbuffer+6, szIPName, iDataSize);
				iSendSize = iDataSize + 6;
				if(sendto(m_sock_udp_rcvr, sndbuffer, iSendSize, 0, (sockaddr*)&m_udp_rcvr, sizeof(m_udp_rcvr)) < 0)
				{
					strLog.Format("Send IP error: %d", WSAGetLastError());
				}
				else
				{
					strLog.Format("Send: %d Bytes. IP address: %s", iSendSize, szIPName);
				}
				WriteLog(strLog);
			}
			else
			{
				strLog.Format("Receive: %d Bytes. SN: 0x%x, Cmd: 0x%x, DataSize: %d Bytes", iRecvSize, usSN, usCmd, usSize);
				WriteLog(strLog);
			}
		}

		// Server
		SOCKET sd = accept(m_sock_server, (sockaddr*)&sinRemote, &ilen);
        if (sd != INVALID_SOCKET)
		{
            DWORD nThreadID;
            CreateThread(0, 0, EchoHandler, (void*)sd, 0, &nThreadID);
        }
	}
}

void CEMC6_ServerDlg::OnBnClickedButtonClose()
{
	CString strLog;

	m_bActive = false;
	CloseServer();
	strLog = "Server closed!";
	WriteLog(strLog);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
}

void CEMC6_ServerDlg::OnBnClickedButtonExit()
{
	m_bActive = false;
	EndDialog(IDCANCEL);
}

void CEMC6_ServerDlg::CloseServer()
{
	CString strLog;
	
	if(m_sock_udp_rcvr != INVALID_SOCKET)
	{
		closesocket(m_sock_udp_rcvr);
		m_sock_udp_rcvr = INVALID_SOCKET;
		strLog = "Socket: broadcast receiver closed!";
		WriteLog(strLog);
	}
	
	if(m_sock_server != INVALID_SOCKET)
	{
		closesocket(m_sock_server);
		m_sock_server = INVALID_SOCKET;
		strLog = "Socket: server closed!";
		WriteLog(strLog);
	}
	
	if(m_bWSA)
	{
		WSACleanup();
		m_bWSA = false;
		strLog.Format("WSA clean up!");
		WriteLog(strLog);
	}
}

void CEMC6_ServerDlg::GetLogFilePath()
{
	char szCurPath[MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString strDate;

	strDate = CTime::GetCurrentTime().Format("%Y%m%d");
	
	GetModuleFileName(NULL, szCurPath, MAX_PATH);
	_splitpath_s(szCurPath, drive, dir, fname, ext);
	m_strLogPath.Format("%s", drive);
	m_strLogPath.Append(dir);
	m_strLogPath.Append(strDate);
	m_strLogPath.Append(".log");
}

void CEMC6_ServerDlg::WriteLog(CString strMsg)
{
	CString str, strTime;

	strTime = CTime::GetCurrentTime().Format("%H:%M:%S - ");
	str = strTime + strMsg;
	str.Append("\n");
	GetDlgItem(IDC_STATIC_LOG)->SetWindowTextA(str);

    SetFilePointer(m_hLogFile, 0, NULL, FILE_END);
    WriteFile(m_hLogFile, str, strlen(str), NULL, NULL);
}

BOOL CEMC6_ServerDlg::OpenLog()
{
	LPCWSTR pstrPath;

	pstrPath = m_strLogPath.AllocSysString();

	m_hLogFile = CreateFileW(pstrPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(m_hLogFile == INVALID_HANDLE_VALUE)
		return FALSE;

	return TRUE;
}

void CEMC6_ServerDlg::CloseLog()
{
    if (m_hLogFile != NULL)
    {
        CString strLog;
		
		strLog = "Log file closed!";
		WriteLog(strLog);
		
		CloseHandle(m_hLogFile);
        m_hLogFile = NULL;
    }
}
