
// EMC6_ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EMC6_Server.h"
#include "EMC6_ServerDlg.h"


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
	m_sock_udp_rcvr = INVALID_SOCKET;
	m_sock_server = INVALID_SOCKET;

	//PostMessage(WM_FIRST_SHOWN);

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
	
	CloseServer();
}

BOOL CEMC6_ServerDlg::StartServer()
{
	int iResult;
	WSADATA wsaData;
	CString str;
	int i = 1;
	char broadcast = '1';
	unsigned long b = 1;
	
	// Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
		str.Format("WSAStartup failed with error: %d", iResult);
		MessageBox(str);
		return FALSE;
    }

	// Create a SOCKET for receiving the broadcast
	m_sock_udp_rcvr = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_sock_udp_rcvr == INVALID_SOCKET)
	{	
		str.Format("socket failed with error: %d", WSAGetLastError());
		MessageBox(str);
		return FALSE;
	}

    iResult = setsockopt(m_sock_udp_rcvr, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	if(iResult < 0)
    {
        str = "Error in setting Broadcast option";
		MessageBox(str);
        return FALSE;
    }
	
	// Create a SOCKET for connecting to server
	m_sock_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_sock_server == INVALID_SOCKET)
	{	
		str.Format("socket failed with error: %d", WSAGetLastError());
		MessageBox(str);
		return FALSE;
	}

	iResult = setsockopt(m_sock_server, SOL_SOCKET, SO_REUSEADDR, (char*)&i, sizeof(i));
	if(iResult < 0)
	{
        str = "Error in setting Server option";
		MessageBox(str);
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
        str.Format("bind failed with error: %d", WSAGetLastError());
		MessageBox(str);
		return FALSE;
    }
	
	//Binding part - Server
	iResult = bind(m_sock_server, (sockaddr*)&m_tcp_server, sizeof(m_tcp_server));
	if (iResult == SOCKET_ERROR)
	{
		str.Format("bind failed with error: %d", WSAGetLastError());
		MessageBox(str);
		return FALSE;
	}
	
	// Setup the TCP listening socket
	iResult = listen(m_sock_server, 5);
	if (iResult == SOCKET_ERROR)
	{
		str.Format("listen failed with error: %d", WSAGetLastError());
		MessageBox(str);
		return FALSE;
	}
	
	//make it non blocking
	iResult = ioctlsocket(m_sock_udp_rcvr, FIONBIO, &b);
	if(iResult != 0)
	{
		str.Format("Set non blocking failed with error: %d", WSAGetLastError());
		MessageBox(str);
		return FALSE;
	}

	iResult = ioctlsocket(m_sock_server, FIONBIO, &b);
	if(iResult != 0)
	{
		str.Format("Set non blocking failed with error: %d", WSAGetLastError());
		MessageBox(str);
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
	CString str, strDateTime;
	int ilen = sizeof(struct sockaddr_in);
	char szIPName[256];
	char sndbuffer[1024], rcvbuffer[1024];
	int iSendSize, iRecvSize, iDataSize;

	m_bActive = false;

	strDateTime = CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S - ");
	str = "Server starting";
	GetDlgItem(IDC_STATIC_LOG)->SetWindowTextA(strDateTime + str);
	
	iRet = StartServer();
	if(!iRet)
	{
		EndDialog(IDCANCEL);
	}

	strDateTime = CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S - ");
	str = "Server running";
	GetDlgItem(IDC_STATIC_LOG)->SetWindowTextA(strDateTime + str);

	memset(szIPName, 0, sizeof(szIPName));
	iDataSize = GetHostIP(szIPName);

	m_bActive = true;
	// main loop
	while(m_bActive)
	{
		SetPeekMsg();
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
				strDateTime = CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S - ");
				str.Format("Receive: %d Bytes\nCmd: CMD_GET_IP_ADDRESS", iRecvSize);
				GetDlgItem(IDC_STATIC_LOG)->SetWindowTextA(strDateTime + str);
				
				memset(sndbuffer, 0, sizeof(sndbuffer));
				*((unsigned short *)sndbuffer) = 0xffff;
				*((unsigned short *)sndbuffer + 1) = CMD_GET_IP_ADDRESS;
				*((unsigned short *)sndbuffer + 2) = (unsigned short)iDataSize;
				memcpy(sndbuffer+6, szIPName, iDataSize);
				iSendSize = iDataSize + 6;
				if(sendto(m_sock_udp_rcvr, sndbuffer, iSendSize, 0, (sockaddr*)&m_udp_rcvr, sizeof(m_udp_rcvr)) < 0)
				{
					str.Format("Send IP error: %d", WSAGetLastError());
				}
				else
				{
					str.Format("Send: %d Bytes\nIP address: %s", iRecvSize, szIPName);
				}
				strDateTime = CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S - ");
				GetDlgItem(IDC_STATIC_LOG)->SetWindowTextA(strDateTime + str);
			}
			else
			{
				strDateTime = CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S - ");
				str.Format("Receive: %d Bytes\nSN: 0x%x, Cmd: 0x%x, DataSize: %d Bytes", iRecvSize, usSN, usCmd, usSize);
				GetDlgItem(IDC_STATIC_LOG)->SetWindowTextA(strDateTime + str);
			}
		}
	}
}

void CEMC6_ServerDlg::OnBnClickedButtonClose()
{
	CString str, strDateTime;
	
	m_bActive = false;

	strDateTime = CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S - ");
	str = "Server closing";
	GetDlgItem(IDC_STATIC_LOG)->SetWindowTextA(strDateTime + str);

	CloseServer();

	strDateTime = CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S - ");
	str = "Server closed";
	GetDlgItem(IDC_STATIC_LOG)->SetWindowTextA(strDateTime + str);
}

void CEMC6_ServerDlg::OnBnClickedButtonExit()
{
	m_bActive = false;
	EndDialog(IDCANCEL);
}

void CEMC6_ServerDlg::CloseServer()
{
	if(m_sock_udp_rcvr != INVALID_SOCKET)
	{
		closesocket(m_sock_udp_rcvr);
	}
	
	if(m_sock_server != INVALID_SOCKET)
	{
		closesocket(m_sock_server);
	}
	
	WSACleanup();
}
