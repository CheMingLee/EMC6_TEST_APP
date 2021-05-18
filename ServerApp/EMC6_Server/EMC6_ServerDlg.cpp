
// EMC6_ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EMC6_Server.h"
#include "EMC6_ServerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRITICAL_SECTION g_CS;
CString g_strLogPath;
HANDLE g_hLogFile;
char g_ReadBuffer[1024];
char g_SendBuffer[1024];

void GetLogFilePath()
{
	char szCurPath[MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	
	GetModuleFileName(NULL, szCurPath, MAX_PATH);
	_splitpath_s(szCurPath, drive, dir, fname, ext);
	g_strLogPath.Format("%s", drive);
	g_strLogPath.Append(dir);
	g_strLogPath.Append(CTime::GetCurrentTime().Format("%Y%m%d"));
	g_strLogPath.Append(".log");
}

void WriteLog(CString strMsg)
{
	CString str, strTime;

	strTime = CTime::GetCurrentTime().Format("%H:%M:%S - ");
	str = strTime + strMsg;
	str.Append("\n");
	AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_STATIC_LOG)->SetWindowTextA(str);

	EnterCriticalSection(&g_CS);
    SetFilePointer(g_hLogFile, 0, NULL, FILE_END);
    WriteFile(g_hLogFile, str, strlen(str), NULL, NULL);
	LeaveCriticalSection(&g_CS);
}

BOOL OpenLog()
{
	LPCWSTR pstrPath;

	pstrPath = g_strLogPath.AllocSysString();
	g_hLogFile = CreateFileW(pstrPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(g_hLogFile == INVALID_HANDLE_VALUE)
		return FALSE;

	return TRUE;
}

void CloseLog()
{
    if (g_hLogFile != NULL)
    {
		CloseHandle(g_hLogFile);
        g_hLogFile = NULL;
    }
}

BOOL TCP_Send_Datas(SOCKET sd_connect, unsigned short usSN, unsigned short usCmd, char *pData, unsigned short usDataSize)
{
	BOOL bRet = TRUE;

	memset(g_SendBuffer, 0, sizeof(g_SendBuffer));
	*((unsigned short *)g_SendBuffer) = usSN;
	*((unsigned short *)g_SendBuffer + 1) = usCmd;
	*((unsigned short *)g_SendBuffer + 2) = usDataSize;
	memcpy(g_SendBuffer + 6, pData, usDataSize);
	if(send(sd_connect, g_SendBuffer, usDataSize + 6, 0) < 0)
		bRet = FALSE;

	return bRet;
}

int CmdTransfer(SOCKET sd_connect)
{
    int iReadSize = 0;
	unsigned short usDataSize = 0;

	memset(g_ReadBuffer, 0, sizeof(g_ReadBuffer));
	iReadSize = recv(sd_connect, g_ReadBuffer, 1024, 0);
    if (iReadSize > 0)
	{
		BOOL bSendRet;
		CString strLog, strTmp;
		unsigned short usSN, usCmd, usSize;

		usSN = *((unsigned short *)g_ReadBuffer);
		usCmd = *((unsigned short *)g_ReadBuffer + 1);
		usSize = *((unsigned short *)g_ReadBuffer + 2);
		if (usCmd == CMD_GET_LIST_SIZE)
		{

		}
		else if(usCmd == CMD_RS232_TEST)
		{

		}
		else if(usCmd == CMD_GET_LABEL)
		{

		}
		else if(usCmd == CMD_GET_COUNT)
		{

		}
		else if(usCmd == CMD_GET_HEAD_STATUS)
		{

		}
		else if(usCmd == CMD_GET_HEX_VERSION)
		{
			strLog = "GET_HEX_VERSION;";
			usDataSize = 4;
			
			long lData = 0x22222222;

			bSendRet = TCP_Send_Datas(sd_connect, usSN, usCmd, (char *)&lData, usDataSize);
			strTmp.Format("0x%x", lData);
		}
		else if(usCmd == CMD_GET_HI_DATA)
		{

		}
		else if(usCmd == CMD_GET_INPUT_POINTER)
		{

		}
		else if(usCmd == CMD_GET_IO_STATUS)
		{

		}
		else if(usCmd == CMD_GET_LIST_SPACE)
		{

		}
		else if(usCmd == CMD_GET_CARD_VERSION)
		{
			strLog = "GET_CARD_VERSION;";
			usDataSize = 4;
			
			long lData = 0x11111111;
			bSendRet = TCP_Send_Datas(sd_connect, usSN, usCmd, (char *)&lData, usDataSize);
			strTmp.Format("0x%x", lData);
		}
		else if(usCmd == CMD_GET_SERIAL_NUMBER)
		{

		}
		else if(usCmd == CMD_GET_STARTSTOP_INFO)
		{

		}
		else if(usCmd == CMD_GET_STATUS)
		{
			strLog = "GET_STATUS;";
			usDataSize = 8;
			
			long lData[2];
			lData[0] = 0;
			lData[1] = 1;
			bSendRet = TCP_Send_Datas(sd_connect, usSN, usCmd, (char *)lData, usDataSize);
			strTmp.Format("%d,%d", lData[0], lData[1]);
		}
		else if(usCmd == CMD_GET_TIME)
		{

		}
		else if(usCmd == CMD_GET_VALUE)
		{

		}
		else if(usCmd == CMD_GET_WAIT_STATUS)
		{

		}
		else if(usCmd == CMD_GET_WAVEFORM)
		{

		}
		else if(usCmd == CMD_GET_XY_POS)
		{

		}
		else if(usCmd == CMD_MEASUREMENT_STATUS)
		{

		}
		else if(usCmd == CMD_READ_IO_PORT)
		{

		}
		else if(usCmd == CMD_READ_PIXEL_AD)
		{

		}
		else if(usCmd == CMD_READ_STATUS)
		{

		}
		else if(usCmd == CMD_GET_MOTION_PULSE_COUNT)
		{

		}
		else if(usCmd == CMD_GET_MOTION_INPOS)
		{

		}
		else if(usCmd == CMD_GET_MOTION_STATUS)
		{

		}
		else if(usCmd == CMD_GET_MOTION_ENCODER)
		{

		}
		else if(usCmd == CMD_GET_MOTION_SENSOR)
		{

		}
		else if(usCmd == CMD_DEBUG_WATCH_DOG)
		{

		}
		else if(usCmd == CMD_MOTION_ARC_GET_SIZE)
		{

		}
		else if(usCmd == CMD_GET_IO_STATUS_EX)
		{

		}
		else if(usCmd == CMD_AUTO_CAL)
		{

		}
		else if(usCmd == CMD_CONTROL_COMMAND)
		{

		}
		else if(usCmd == CMD_GET_ENCODER_SPEED)
		{

		}
		else if(usCmd == CMD_SET_HW_CFG)
		{

		}
		else if(usCmd == CMD_GET_HW_CFG)
		{

		}
		else if(usCmd == CMD_ERASE_FLASH)
		{

		}
		else if(usCmd == CMD_READ_MEM)
		{

		}
		else if(usCmd == CMD_MEM_TO_QSPI)
		{

		}
		else if(usCmd == CMD_QSPI_TO_MEM)
		{

		}
		else if(usCmd == CMD_SHA204_WAKEUP)
		{

		}
		else if(usCmd == CMD_SHA204_SLEEP)
		{

		}
		else if(usCmd == CMD_SHA204_EXECUTE_CMD)
		{

		}
		else if(usCmd == CMD_SHA204_GET_RESPONSE)
		{

		}
		else if(usCmd == CMD_GET_MOTION_INDEX_STATUS)
		{

		}
		else if(usCmd == CMD_GET_CORRECT_GRID)
		{

		}
		else if(usCmd == CMD_GET_DNA_CODE)
		{

		}
		else if(usCmd == CMD_SET_DNA_CODE)
		{

		}
		else if(usCmd == CMD_GET_ENCODER_POS_TRIGGER)
		{

		}
		else if(usCmd == CMD_GET_ETAB_TRG_ENCODER)
		{

		}
		else if(usCmd == CMD_GET_ETAB_REC_ENCODER)
		{

		}
		else if(usCmd == CMD_GET_SELECT_LIST)
		{

		}
		else if(usCmd == CMD_MOTION_ARC_GET_SIZE_EX)
		{

		}
		else if(usCmd == CMD_GET_RECORD_MARK_TIME)
		{

		}
		else if(usCmd == CMD_GET_RECORD_AMOF_DIFF)
		{

		}
		else if(usCmd == CMD_GET_NG_RECORD)
		{

		}
		else if(usCmd == CMD_GET_IPG_PD_READ)
		{

		}
		else if(usCmd == CMD_GET_INT_FREQ_STATUS)
		{

		}
		else if(usCmd == CMD_GET_ECM_SK_DATA)
		{

		}
		else if(usCmd == CMD_GET_ECM_SK_DATA_EX)
		{

		}
		else if(usCmd == CMD_GET_IP_ADDRESS)
		{

		}
		else
		{
			strLog.Format("Cmd:0x%x,DataSize:%d;", usCmd, usSize);
			bSendRet = TCP_Send_Datas(sd_connect, usSN, usCmd, NULL, usDataSize);
			strTmp.Format("Send:0x%x", NULL);
		}

		if(bSendRet <= 0)
			strTmp.Format("Error: %d", WSAGetLastError());
		
		strLog.Append(strTmp);
		WriteLog(strLog);
    }

	return iReadSize;
}

DWORD WINAPI CmdHandler(void* sd_)
{
	SOCKET sd = (SOCKET)sd_;

    // Read CMD from client
    int iReadBytes = 0;
    do
	{
		iReadBytes = CmdTransfer(sd);
    } while (iReadBytes != 0);

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
	g_hLogFile = NULL;
	m_sock_udp_rcvr = INVALID_SOCKET;
	m_sock_server = INVALID_SOCKET;
	m_bWSA = false;
	m_bActive = false;

	InitializeCriticalSection(&g_CS);
	
	GetLogFilePath();
	if(!OpenLog())
	{
		CString strLog;
		CString strDateTime = CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S - ");
		strLog.Format("Open log file failed!\nErrCode: %d", GetLastError());
		MessageBox(strDateTime + strLog);
		EndDialog(IDCANCEL);
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
	
	CloseServer();
	CloseLog();
	DeleteCriticalSection(&g_CS);
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
	
	iRet = StartServer();
	if(!iRet)
	{
		CloseServer();
		strLog = "Starting server failed!";
		WriteLog(strLog);
		return;
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

	strLog = "Server: started";
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
				strLog = "GET_IP_ADDRESS;";
				
				memset(sndbuffer, 0, sizeof(sndbuffer));
				*((unsigned short *)sndbuffer) = 0xffff;
				*((unsigned short *)sndbuffer + 1) = CMD_GET_IP_ADDRESS;
				*((unsigned short *)sndbuffer + 2) = (unsigned short)iDataSize;
				memcpy(sndbuffer+6, szIPName, iDataSize);
				iSendSize = iDataSize + 6;
				if(sendto(m_sock_udp_rcvr, sndbuffer, iSendSize, 0, (sockaddr*)&m_udp_rcvr, sizeof(m_udp_rcvr)) < 0)
				{
					CString strTmp;
					strTmp.Format("Send IP error: %d", WSAGetLastError());
					strLog.Append(strTmp);
				}
				else
				{
					strLog.Append(szIPName);
				}
				WriteLog(strLog);
			}
			else
			{
				strLog.Format("Cmd: 0x%x, DataSize: %d Bytes", usCmd, usSize);
				WriteLog(strLog);
			}
		}

		// Server
		SOCKET sd = accept(m_sock_server, (sockaddr*)&sinRemote, &ilen);
        if (sd != INVALID_SOCKET)
		{
            DWORD nThreadID;
            CreateThread(0, 0, CmdHandler, (void*)sd, 0, &nThreadID);
        }
	}
}

void CEMC6_ServerDlg::OnBnClickedButtonClose()
{
	m_bActive = false;
	CloseServer();
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
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
		m_sock_udp_rcvr = INVALID_SOCKET;
	}
	
	if(m_sock_server != INVALID_SOCKET)
	{
		closesocket(m_sock_server);
		m_sock_server = INVALID_SOCKET;
		WriteLog("Server: closed");
	}
	
	if(m_bWSA)
	{
		WSACleanup();
		m_bWSA = false;
	}
}
