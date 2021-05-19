
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
	g_strLogPath.Format(_T("%s"), drive);
	g_strLogPath.Append(dir);
	g_strLogPath.Append(CTime::GetCurrentTime().Format("%Y%m%d"));
	g_strLogPath.Append(_T(".log"));
}

void WriteLog(CString strMsg)
{
	CString str, strTime;

	strTime = CTime::GetCurrentTime().Format("%H:%M:%S - ");
	str = strTime + strMsg;
	str.Append(_T("\n"));
	AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_STATIC_LOG)->SetWindowTextA(str);

	EnterCriticalSection(&g_CS);
    SetFilePointer(g_hLogFile, 0, NULL, FILE_END);
    WriteFile(g_hLogFile, str, str.GetLength(), NULL, NULL);
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
	*((unsigned short *)&g_SendBuffer[0]) = usSN;
	*((unsigned short *)&g_SendBuffer[2]) = usCmd;
	*((unsigned short *)&g_SendBuffer[4]) = usDataSize;
	
	if(usDataSize > 0)
		memcpy(&g_SendBuffer[6], pData, usDataSize);
	
	if(send(sd_connect, g_SendBuffer, usDataSize + 6, 0) < 0)
		bRet = FALSE;

	return bRet;
}

int CmdTransfer(SOCKET sd_connect)
{
    int iReadSize = 0;
	unsigned short usDataSize = 0;
	long lData[32];

	memset(lData, 0, sizeof(lData));
	memset(g_ReadBuffer, 0, sizeof(g_ReadBuffer));
	iReadSize = recv(sd_connect, g_ReadBuffer, 1024, 0);
    if (iReadSize > 0)
	{
		BOOL bSendRet, bReplyNull;
		CString strLog, strTmp;
		unsigned short usSN, usCmd, usSize;

		bSendRet = TRUE;
		bReplyNull = TRUE;
		usSN = *((unsigned short *)&g_ReadBuffer[0]);
		usCmd = *((unsigned short *)&g_ReadBuffer[2]);
		usSize = *((unsigned short *)&g_ReadBuffer[4]);
		if (usCmd == CMD_GET_LIST_SIZE)
		{
			strLog = "GET_LIST_SIZE;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_RS232_TEST)
		{
			strLog = "RS232_TEST;";
		}
		else if(usCmd == CMD_GET_LABEL)
		{
			strLog = "GET_LABEL;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_COUNT)
		{
			strLog = "GET_COUNT;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_HEAD_STATUS)
		{
			strLog = "GET_HEAD_STATUS;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_HEX_VERSION)
		{
			strLog = "GET_HEX_VERSION;";
			usDataSize = 4;
			lData[0] = 0x00010101;
			strTmp.Format(_T("0x%x"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_HI_DATA)
		{
			strLog = "GET_HI_DATA;";
			usDataSize = 16;
			lData[0] = 0;
			lData[1] = 0;
			lData[2] = 0;
			lData[3] = 0;
			strTmp.Format(_T("%d,%d,%d,%d"), lData[0], lData[1], lData[2], lData[3]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_INPUT_POINTER)
		{
			strLog = "GET_INPUT_POINTER;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_IO_STATUS)
		{
			strLog = "GET_IO_STATUS;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_LIST_SPACE)
		{
			strLog = "GET_LIST_SPACE;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_CARD_VERSION)
		{
			strLog = "GET_CARD_VERSION;";
			usDataSize = 4;
			lData[0] = 0x00030500;
			strTmp.Format(_T("0x%x"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_SERIAL_NUMBER)
		{
			strLog = "GET_SERIAL_NUMBER;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_STARTSTOP_INFO)
		{
			strLog = "GET_STARTSTOP_INFO;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_STATUS)
		{
			strLog = "GET_STATUS;";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_TIME)
		{
			strLog = "GET_TIME;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_VALUE)
		{
			strLog = "GET_VALUE;";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_WAIT_STATUS)
		{
			strLog = "GET_WAIT_STATUS;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_WAVEFORM)
		{
			unsigned long channel, stop;

			channel = *((unsigned long *)&g_ReadBuffer[6]);
			stop = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("GET_WAIT_STATUS,%d,%d;"), channel, stop);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_XY_POS)
		{
			strLog = "GET_XY_POS;";
			usDataSize = 12;
			lData[0] = 0;
			lData[1] = 0;
			lData[2] = 0;
			strTmp.Format(_T("%d,%d,%d"), lData[0], lData[1], lData[2]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_MEASUREMENT_STATUS)
		{
			strLog = "MEASUREMENT_STATUS;";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_READ_IO_PORT)
		{
			strLog = "READ_IO_PORT;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_READ_PIXEL_AD)
		{
			unsigned long pos;

			pos = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("READ_PIXEL_AD,%d;"), pos);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_READ_STATUS)
		{
			strLog = "READ_STATUS;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_MOTION_PULSE_COUNT)
		{
			long lAxis;

			lAxis = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_MOTION_PULSE_COUNT,%d;"), lAxis);
			usDataSize = 4;
			lData[0] = 250;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_MOTION_INPOS)
		{
			long lAxis;

			lAxis = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_MOTION_INPOS,%d;"), lAxis);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_MOTION_STATUS)
		{
			long lAxis;

			lAxis = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_MOTION_STATUS,%d;"), lAxis);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_MOTION_ENCODER)
		{
			long lAxis;

			lAxis = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_MOTION_ENCODER,%d;"), lAxis);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_MOTION_SENSOR)
		{
			long lAxis;

			lAxis = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_MOTION_SENSOR,%d;"), lAxis);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_DEBUG_WATCH_DOG)
		{
			strLog = "DEBUG_WATCH_DOG;";
			usDataSize = 24;
			lData[0] = 0;
			lData[1] = 0;
			lData[2] = 0;
			lData[3] = 0;
			lData[4] = 0;
			lData[5] = 0;
			strTmp.Format(_T("%d,%d,%d,%d,%d,%d"), lData[0], lData[1], lData[2], lData[3], lData[4], lData[5]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_MOTION_ARC_GET_SIZE)
		{
			strLog = "MOTION_ARC_GET_SIZE;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_IO_STATUS_EX)
		{
			strLog = "GET_IO_STATUS_EX;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_AUTO_CAL)
		{
			unsigned long ulData[2];

			ulData[0] = *((unsigned long *)&g_ReadBuffer[6]);
			ulData[1] = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("AUTO_CAL,%d,%d;"), ulData[0], ulData[1]);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_CONTROL_COMMAND)
		{
			unsigned long ulData[3];

			ulData[0] = *((unsigned long *)&g_ReadBuffer[6]);
			ulData[1] = *((unsigned long *)&g_ReadBuffer[10]);
			ulData[2] = *((unsigned long *)&g_ReadBuffer[14]);

			strLog.Format(_T("CONTROL_COMMAND,%d,%d,%d;"), ulData[0], ulData[1], ulData[2]);
		}
		else if(usCmd == CMD_GET_ENCODER_SPEED)
		{
			strLog = "GET_ENCODER_SPEED;";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_SET_HW_CFG)
		{
			unsigned short iByteCnt;

			iByteCnt = *((unsigned short *)&g_ReadBuffer[6]);

			strLog.Format(_T("SET_HW_CFG,%d;"), iByteCnt);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_HW_CFG)
		{
			unsigned long iIndex, iByteCnt;

			iIndex = *((unsigned long *)&g_ReadBuffer[6]);
			iByteCnt = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("GET_HW_CFG,%d,%d;"), iIndex, iByteCnt);
			usDataSize = (unsigned short)iByteCnt;
			if(iByteCnt > 0 && iByteCnt <= 128)
			{
				strTmp.Format(_T("0,%dBytes"), iByteCnt);
				bReplyNull = FALSE;
			}
		}
		else if(usCmd == CMD_ERASE_FLASH)
		{
			unsigned long iMode, ulAddr;

			iMode = *((unsigned long *)&g_ReadBuffer[6]);
			ulAddr = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("ERASE_FLASH,%d,%d;"), iMode, ulAddr);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_READ_MEM)
		{
			unsigned long iSize, ulOffset;

			iSize = *((unsigned long *)&g_ReadBuffer[6]);
			ulOffset = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("READ_MEM,%d,%d;"), iSize, ulOffset);
			usDataSize = (unsigned short)iSize;
			strTmp.Format(_T("0,%dBytes"), iSize);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_MEM_TO_QSPI)
		{
			unsigned long Addr, Size;

			Addr = *((unsigned long *)&g_ReadBuffer[6]);
			Size = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("MEM_TO_QSPI,%d,%d;"), Addr, Size);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_QSPI_TO_MEM)
		{
			unsigned long Addr, Size;

			Addr = *((unsigned long *)&g_ReadBuffer[6]);
			Size = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("QSPI_TO_MEM,%d,%d;"), Addr, Size);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_SHA204_WAKEUP)
		{
			strLog = "SHA204_WAKEUP;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_SHA204_SLEEP)
		{
			strLog = "SHA204_SLEEP;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_SHA204_EXECUTE_CMD)
		{
			strLog.Format(_T("SHA204_EXECUTE_CMD,%dBytes;"), usSize);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_SHA204_GET_RESPONSE)
		{
			strLog = "SHA204_GET_RESPONSE;";
			usDataSize = 40;
			strTmp.Format(_T("0,%dBytes"), usDataSize);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_MOTION_INDEX_STATUS)
		{
			strLog = "GET_MOTION_INDEX_STATUS;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_CORRECT_GRID)
		{
			strLog = "GET_CORRECT_GRID;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_DNA_CODE)
		{
			strLog = "GET_DNA_CODE;";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_SET_DNA_CODE)
		{
			strLog = "SET_DNA_CODE;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_ENCODER_POS_TRIGGER)
		{
			strLog = "GET_ENCODER_POS_TRIGGER;";
			usDataSize = 12;
			lData[0] = 0;
			lData[1] = 0;
			lData[2] = 0;
			strTmp.Format(_T("%d,%d,%d"), lData[0], lData[1], lData[2]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_ETAB_TRG_ENCODER)
		{
			long lIndex;

			lIndex = *((long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_ETAB_TRG_ENCODER,%d;"), lIndex);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_ETAB_REC_ENCODER)
		{
			long lIndex;

			lIndex = *((long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_ETAB_REC_ENCODER,%d;"), lIndex);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_SELECT_LIST)
		{
			strLog = "GET_SELECT_LIST;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_MOTION_ARC_GET_SIZE_EX)
		{
			strLog = "MOTION_ARC_GET_SIZE_EX;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_RECORD_MARK_TIME)
		{
			long lIndex;

			lIndex = *((long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_RECORD_MARK_TIME,%d;"), lIndex);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_RECORD_AMOF_DIFF)
		{
			long lSetInterval;

			lSetInterval = *((long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_RECORD_AMOF_DIFF,%d;"), lSetInterval);
			usDataSize = 36;
			strTmp.Format(_T("0,%dBytes"), usDataSize);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_NG_RECORD)
		{
			strLog = "GET_NG_RECORD;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_IPG_PD_READ)
		{
			strLog = "GET_IPG_PD_READ;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_INT_FREQ_STATUS)
		{
			strLog = "GET_INT_FREQ_STATUS;";
			usDataSize = 12;
			lData[0] = 0;
			lData[1] = 0;
			lData[2] = 0;
			strTmp.Format(_T("%d,%d,%d"), lData[0], lData[1], lData[2]);
			bReplyNull = FALSE;
		}
		else if(usCmd == CMD_GET_ECM_SK_DATA)
		{
			unsigned int uiData[2];

			uiData[0] = *((unsigned int *)&g_ReadBuffer[6]);
			uiData[1] = *((unsigned int *)&g_ReadBuffer[10]);

			strLog.Format(_T("GET_ECM_SK_DATA,%d,%d"), uiData[0], uiData[1]);
			if(uiData[0] == 0 && uiData[1] == 0)
			{
				usDataSize = 4;
				lData[0] = 0;
				strTmp.Format(_T("%d"), lData[0]);
				bReplyNull = FALSE;
			}
			else
			{
				usDataSize = sizeof(unsigned int) * uiData[1];
				strTmp.Format(_T("0,%dBytes"), usDataSize);
				bReplyNull = FALSE;
			}
		}
		else if(usCmd == CMD_GET_ECM_SK_DATA_EX)
		{
			long lOffset, lSize;

			lOffset = *((long *)&g_ReadBuffer[6]);
			lSize = *((long *)&g_ReadBuffer[10]);

			strLog.Format(_T("GET_ECM_SK_DATA_EX,%d,%d"), lOffset, lSize);
			if(lOffset == 0 && lSize == 0)
			{
				usDataSize = 4;
				lData[0] = 0;
				strTmp.Format(_T("%d"), lData[0]);
				bReplyNull = FALSE;
			}
			else
			{
				usDataSize = (unsigned short)lSize;
				strTmp.Format(_T("0,%dBytes"), usDataSize);
				bReplyNull = FALSE;
			}
		}
		else if(usCmd == CMD_AUTO_CHANGE)
		{
			long lList;

			lList = *((long *)&g_ReadBuffer[6]);
			strLog.Format(_T("AUTO_CHANGE,%d;"), lList);
		}
		else if(usCmd == CMD_AUTO_CHANGE_POS)
		{
			// long lList;

			// lList = *((long *)&g_ReadBuffer[6]);
			// strLog.Format(_T("AUTO_CHANGE,%d;"), lList);
		}
		else
		{
			strLog.Format(_T("CMD0x%x,%dBytes;"), usCmd, usSize);
		}

		if(bReplyNull)
		{
			bSendRet = TCP_Send_Datas(sd_connect, usSN, usCmd, NULL, 0);
			strTmp = "NULL";
		}
		else
			bSendRet = TCP_Send_Datas(sd_connect, usSN, usCmd, (char *)lData, usDataSize);

		if(bSendRet <= 0)
			strTmp.Format(_T("Error:%d"), WSAGetLastError());
		
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
		strLog.Format(_T("Open log file failed!\nErrCode: %d"), GetLastError());
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
		strLog.Format(_T("WSAStartup failed with error: %d"), iResult);
		WriteLog(strLog);
		return FALSE;
    }
	m_bWSA = true;

	// Create a SOCKET for receiving the broadcast
	m_sock_udp_rcvr = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_sock_udp_rcvr == INVALID_SOCKET)
	{	
		strLog.Format(_T("socket failed with error: %d"), WSAGetLastError());
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
		strLog.Format(_T("socket failed with error: %d"), WSAGetLastError());
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
		strLog.Format(_T("bind failed with error: %d"), WSAGetLastError());
		WriteLog(strLog);
		return FALSE;
    }
	
	//Binding part - Server
	iResult = bind(m_sock_server, (sockaddr*)&m_tcp_server, sizeof(m_tcp_server));
	if (iResult == SOCKET_ERROR)
	{
		strLog.Format(_T("bind failed with error: %d"), WSAGetLastError());
		WriteLog(strLog);
		return FALSE;
	}
	
	// Setup the TCP listening socket
	iResult = listen(m_sock_server, 5);
	if (iResult == SOCKET_ERROR)
	{
		strLog.Format(_T("listen failed with error: %d"), WSAGetLastError());
		WriteLog(strLog);
		return FALSE;
	}
	
	//make it non blocking
	iResult = ioctlsocket(m_sock_udp_rcvr, FIONBIO, &b);
	if(iResult != 0)
	{
		strLog.Format(_T("Set non blocking failed with error: %d"), WSAGetLastError());
		WriteLog(strLog);
		return FALSE;
	}

	iResult = ioctlsocket(m_sock_server, FIONBIO, &b);
	if(iResult != 0)
	{
		strLog.Format(_T("Set non blocking failed with error: %d"), WSAGetLastError());
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
					strTmp.Format(_T("Send IP error: %d"), WSAGetLastError());
					strLog.Append(strTmp);
				}
				else
				{
					CString strIP;
					strIP.Format(_T("%s"), szIPName);
					strLog.Append(strIP);
				}
				WriteLog(strLog);
			}
			else
			{
				strLog.Format(_T("Cmd: 0x%x, DataSize: %d Bytes"), usCmd, usSize);
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
		CString strLog = _T("Server: closed");
		WriteLog(strLog);
	}
	
	if(m_bWSA)
	{
		WSACleanup();
		m_bWSA = false;
	}
}
