
// EMC6_ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EMC6_Server.h"
#include "EMC6_ServerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRITICAL_SECTION g_CS;
CString g_strLogPath, g_strINIpath;
HANDLE g_hLogFile;
char g_ReadBuffer[1024];
char g_SendBuffer[1024];
BOOL g_bLogEnable;

void GetINIFilePath()
{
	char szCurPath[MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString strTmp;
	
	GetModuleFileNameA(NULL, szCurPath, MAX_PATH);
	_splitpath_s(szCurPath, drive, dir, fname, ext);
	g_strINIpath.Format(_T("%s"), drive);
	strTmp.Format(_T("%s"), dir);
	g_strINIpath.Append(strTmp);
	g_strINIpath.Append(_T("config.ini"));
}

void GetLogFilePath()
{
	char szCurPath[MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString strTmp;
	
	GetModuleFileNameA(NULL, szCurPath, MAX_PATH);
	_splitpath_s(szCurPath, drive, dir, fname, ext);
	g_strLogPath.Format(_T("%s"), drive);
	strTmp.Format(_T("%s"), dir);
	g_strLogPath.Append(strTmp);
	g_strLogPath.Append(CTime::GetCurrentTime().Format("%Y%m%d"));
	g_strLogPath.Append(_T(".log"));
}

void WriteLog(CString strMsg)
{
	CString str, strTime;

	strTime = CTime::GetCurrentTime().Format("%H:%M:%S - ");
	str = strTime + strMsg;
	str.Append(_T("\n"));
	AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_STATIC_LOG)->SetWindowText(str);
	
	if(g_bLogEnable)
	{
		EnterCriticalSection(&g_CS);
		SetFilePointer(g_hLogFile, 0, NULL, FILE_END);
		WriteFile(g_hLogFile, str, str.GetLength(), NULL, NULL);
		LeaveCriticalSection(&g_CS);
	}
}

BOOL OpenLog()
{
	if(g_bLogEnable)
	{
		LPCWSTR pstrPath;

		pstrPath = g_strLogPath.AllocSysString();
		g_hLogFile = CreateFileW(pstrPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(g_hLogFile == INVALID_HANDLE_VALUE)
			return FALSE;
	}

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
	BOOL bSendRet, bReplyNull;
	BOOL bRead_short, bRead_long, bRead_ulong, bRead_double, bRead_MultiTypes;
	BOOL bCmdLogEnable;
	CString strLog, strTmp, strCmd, strRead;
	unsigned short usSN, usCmd, usSize;

	memset(lData, 0, sizeof(lData));
	memset(g_ReadBuffer, 0, sizeof(g_ReadBuffer));
	iReadSize = recv(sd_connect, g_ReadBuffer, 1024, 0);
    if (iReadSize > 0)
	{
		bSendRet = TRUE;
		bReplyNull = TRUE;
		bCmdLogEnable = TRUE;
		bRead_MultiTypes = FALSE;
		bRead_short = FALSE;
		bRead_long = FALSE;
		bRead_ulong = FALSE;
		bRead_double = FALSE;
		usSN = *((unsigned short *)&g_ReadBuffer[0]);
		usCmd = *((unsigned short *)&g_ReadBuffer[2]);
		usSize = *((unsigned short *)&g_ReadBuffer[4]);
		switch (usCmd)
		{
		case CMD_GET_LIST_SIZE:
		{
			strCmd = "GET_LIST_SIZE";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_RS232_TEST:
		{
			strCmd = "RS232_TEST";
			break;
		}
		case CMD_GET_LABEL:
		{
			strCmd = "GET_LABEL";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_COUNT:
		{
			strCmd = "GET_COUNT";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_HEAD_STATUS:
		{
			strCmd = "GET_HEAD_STATUS";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_HEX_VERSION:
		{
			strCmd = "GET_HEX_VERSION";
			usDataSize = 4;
			lData[0] = 0x00010101;
			strTmp.Format(_T("0x%x"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_HI_DATA:
		{
			strCmd = "GET_HI_DATA";
			usDataSize = 16;
			lData[0] = 0;
			lData[1] = 0;
			lData[2] = 0;
			lData[3] = 0;
			strTmp.Format(_T("%d,%d,%d,%d"), lData[0], lData[1], lData[2], lData[3]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_INPUT_POINTER:
		{
			strCmd = "GET_INPUT_POINTER";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_IO_STATUS:
		{
			strCmd = "GET_IO_STATUS";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_LIST_SPACE:
		{
			strCmd = "GET_LIST_SPACE";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_CARD_VERSION:
		{
			strCmd = "GET_CARD_VERSION";
			usDataSize = 4;
			lData[0] = 0x00030500;
			strTmp.Format(_T("0x%x"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_SERIAL_NUMBER:
		{
			strCmd = "GET_SERIAL_NUMBER";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_STARTSTOP_INFO:
		{
			strCmd = "GET_STARTSTOP_INFO";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_STATUS:
		{
			strCmd = "GET_STATUS";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_TIME:
		{
			strCmd = "GET_TIME";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_VALUE:
		{
			strCmd = "GET_VALUE";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_WAIT_STATUS:
		{
			strCmd = "GET_WAIT_STATUS";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_WAVEFORM:
		{
			strCmd = "GET_WAIT_STATUS";
			bRead_ulong = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_XY_POS:
		{
			strCmd = "GET_XY_POS";
			usDataSize = 12;
			lData[0] = 0;
			lData[1] = 0;
			lData[2] = 0;
			strTmp.Format(_T("%d,%d,%d"), lData[0], lData[1], lData[2]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_MEASUREMENT_STATUS:
		{
			strCmd = "MEASUREMENT_STATUS";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_READ_IO_PORT:
		{
			strCmd = "READ_IO_PORT";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_READ_PIXEL_AD:
		{
			strCmd = "READ_PIXEL_AD";
			bRead_ulong = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_READ_STATUS:
		{
			strCmd = "READ_STATUS";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_PULSE_COUNT:
		{
			strCmd = "GET_MOTION_PULSE_COUNT";
			bRead_long = TRUE;
			usDataSize = 4;
			lData[0] = 250;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_INPOS:
		{
			strCmd = "GET_MOTION_INPOS";
			bRead_long = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_STATUS:
		{
			strCmd = "GET_MOTION_STATUS";
			bRead_long = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_ENCODER:
		{
			strCmd = "GET_MOTION_ENCODER";
			bRead_long = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_SENSOR:
		{
			strCmd = "GET_MOTION_SENSOR";
			bRead_long = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_DEBUG_WATCH_DOG:
		{
			strCmd = "DEBUG_WATCH_DOG";
			usDataSize = 24;
			lData[0] = 0;
			lData[1] = 0;
			lData[2] = 0;
			lData[3] = 0;
			lData[4] = 0;
			lData[5] = 0;
			strTmp.Format(_T("%d,%d,%d,%d,%d,%d"), lData[0], lData[1], lData[2], lData[3], lData[4], lData[5]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_MOTION_ARC_GET_SIZE:
		{
			strCmd = "MOTION_ARC_GET_SIZE";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_IO_STATUS_EX:
		{
			strCmd = "GET_IO_STATUS_EX";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_AUTO_CAL:
		{
			strCmd = "AUTO_CAL";
			bRead_ulong = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_CONTROL_COMMAND:
		{
			strCmd = "CONTROL_COMMAND";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_GET_ENCODER_SPEED:
		{
			strCmd = "GET_ENCODER_SPEED";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SET_HW_CFG:
		{
			unsigned short iByteCnt = *((unsigned short *)&g_ReadBuffer[6]);
			strCmd = "SET_HW_CFG";
			strLog.Format(_T("%s,%dBytes"), strCmd, iByteCnt);
			bRead_MultiTypes = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_HW_CFG:
		{
			strCmd = "GET_HW_CFG";
			bRead_ulong = TRUE;
			unsigned long iByteCnt = *((unsigned long *)&g_ReadBuffer[10]);
			usDataSize = (unsigned short)iByteCnt;
			if(iByteCnt > 0 && iByteCnt <= 128)
			{
				strTmp.Format(_T("0,%dBytes"), iByteCnt);
				bReplyNull = FALSE;
			}
			break;
		}
		case CMD_ERASE_FLASH:
		{
			strCmd = "ERASE_FLASH";
			bRead_ulong = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_READ_MEM:
		{
			strCmd = "READ_MEM";
			bRead_ulong = TRUE;
			unsigned long iSize = *((unsigned long *)&g_ReadBuffer[6]);
			usDataSize = (unsigned short)iSize;
			strTmp.Format(_T("0,%dBytes"), iSize);
			bReplyNull = FALSE;
			break;
		}
		case CMD_MEM_TO_QSPI:
		{
			strCmd = "MEM_TO_QSPI";
			bRead_ulong = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_QSPI_TO_MEM:
		{
			strCmd = "QSPI_TO_MEM";
			bRead_ulong = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SHA204_WAKEUP:
		{
			strCmd = "SHA204_WAKEUP";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SHA204_SLEEP:
		{
			strCmd = "SHA204_SLEEP";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SHA204_EXECUTE_CMD:
		{
			strCmd = "SHA204_EXECUTE_CMD";
			strLog.Format(_T("%s,%dBytes"), strCmd, usSize);
			bRead_MultiTypes = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SHA204_GET_RESPONSE:
		{
			strCmd = "SHA204_GET_RESPONSE";
			usDataSize = 40;
			strTmp.Format(_T("0,%dBytes"), usDataSize);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_INDEX_STATUS:
		{
			strCmd = "GET_MOTION_INDEX_STATUS";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_CORRECT_GRID:
		{
			strCmd = "GET_CORRECT_GRID";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_DNA_CODE:
		{
			strCmd = "GET_DNA_CODE";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SET_DNA_CODE:
		{
			strCmd = "SET_DNA_CODE";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_ENCODER_POS_TRIGGER:
		{
			strCmd = "GET_ENCODER_POS_TRIGGER";
			usDataSize = 12;
			lData[0] = 0;
			lData[1] = 0;
			lData[2] = 0;
			strTmp.Format(_T("%d,%d,%d"), lData[0], lData[1], lData[2]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_ETAB_TRG_ENCODER:
		{
			strCmd = "GET_ETAB_TRG_ENCODER";
			bRead_long = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_ETAB_REC_ENCODER:
		{
			strCmd = "GET_ETAB_REC_ENCODER";
			bRead_long = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_SELECT_LIST:
		{
			strCmd = "GET_SELECT_LIST";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_MOTION_ARC_GET_SIZE_EX:
		{
			strCmd = "MOTION_ARC_GET_SIZE_EX";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_RECORD_MARK_TIME:
		{
			strCmd = "GET_RECORD_MARK_TIME";
			bRead_long = TRUE;
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_RECORD_AMOF_DIFF:
		{
			strCmd = "GET_RECORD_AMOF_DIFF";
			bRead_long = TRUE;
			usDataSize = 36;
			strTmp.Format(_T("0,%dBytes"), usDataSize);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_NG_RECORD:
		{
			strCmd = "GET_NG_RECORD";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_IPG_PD_READ:
		{
			strCmd = "GET_IPG_PD_READ";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_INT_FREQ_STATUS:
		{
			strCmd = "GET_INT_FREQ_STATUS";
			usDataSize = 12;
			lData[0] = 0;
			lData[1] = 0;
			lData[2] = 0;
			strTmp.Format(_T("%d,%d,%d"), lData[0], lData[1], lData[2]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_ECM_SK_DATA:
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
			break;
		}
		case CMD_GET_ECM_SK_DATA_EX:
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
			break;
		}
		case CMD_AUTO_CHANGE:
		{
			strCmd = "AUTO_CHANGE";
			bRead_long = TRUE;
			break;
		}
		case CMD_AUTO_CHANGE_POS:
		{
			strCmd = "AUTO_CHANGE_POS";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_DISABLE_LASER:
		{
			strCmd = "DISABLE_LASER";
			break;
		}
		case CMD_ENABLE_LASER:
		{
			strCmd = "ENABLE_LASER";
			break;
		}
		case CMD_EXECUTE_AT_POINTER:
		{
			strCmd = "EXECUTE_AT_POINTER";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_GOTO_XY:
		{
			strCmd = "GOTO_XY";
			bRead_long = TRUE;
			break;
		}
		case CMD_HOME_POSITION:
		{
			strCmd = "HOME_POSITION";
			bRead_long = TRUE;
			break;
		}
		case CMD_LOAD_PROGRAM_FILE:
		{
			strCmd = "LOAD_PROGRAM_FILE";
			break;
		}
		case CMD_QUIT_LOOP:
		{
			strCmd = "QUIT_LOOP";
			break;
		}
		case CMD_RELEASE_WAIT:
		{
			strCmd = "RELEASE_WAIT";
			break;
		}
		case CMD_RESTART_LIST:
		{
			strCmd = "RESTART_LIST";
			break;
		}
		case CMD_SELECT_COR_TABLE:
		{
			strCmd = "SELECT_COR_TABLE";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SELECT_LIST:
		{
			strCmd = "SELECT_LIST";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SELECT_RTC:
		{
			strCmd = "SELECT_RTC";
			break;
		}
		case CMD_SET_CONTROL_MODE:
		{
			strCmd = "SET_CONTROL_MODE";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_DELAY_MODE:
		{
			strCmd = "SET_DELAY_MODE";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_INPUT_POINTER:
		{
			strCmd = "SET_INPUT_POINTER";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_LIST_MODE:
		{
			strCmd = "SET_LIST_MODE";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_MAX_COUNT:
		{
			strCmd = "SET_MAX_COUNT";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_PISO_CONTROL:
		{
			strCmd = "SET_PISO_CONTROL";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_SOFTSTART_LEVEL:
		{
			strCmd = "SET_SOFTSTART_LEVEL";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SOFTSTART_MODE:
		{
			strCmd = "SOFTSTART_MODE";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_START_LOOP:
		{
			strCmd = "START_LOOP";
			bRead_long = TRUE;
			break;
		}
		case CMD_WRITE_8BIT_PORT:
		{
			strCmd = "WRITE_8BIT_PORT";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_WRITE_DA_X:
		{
			strCmd = "WRITE_DA_X";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_WRITE_IO_PORT:
		{
			strCmd = "WRITE_IO_PORT";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_Z_OUT:
		{
			strCmd = "CMD_Z_OUT";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_FLYSPEED_UPDATE_TIME:
		{
			strCmd = "SET_FLYSPEED_UPDATE_TIME";
			break;
		}
		case CMD_SET_PWM_VARPERIOD:
		{
			strCmd = "SET_PWM_VARPERIOD";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_STANDBY:
		{
			strCmd = "SET_STANDBY";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LASER_SIGNAL_ON:
		{
			strCmd = "LASER_SIGNAL_ON";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_LASER_FPK:
		{
			strCmd = "SET_LASER_FPK";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_LASER_MODE:
		{
			strCmd = "SET_LASER_MODE";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_MATRIX:
		{
			strCmd = "SET_MATRIX";
			bRead_double = TRUE;
			break;
		}
		case CMD_SET_OFFSET:
		{
			strCmd = "SET_OFFSET";
			bRead_long = TRUE;
			break;
		}
		case CMD_MOTION_MOVETO:
		{
			strCmd = "MOTION_MOVETO";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_PARAM1:
		{
			strCmd = "SET_MOTION_PARAM1";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_PARAM2:
		{
			strCmd = "SET_MOTION_PARAM2";
			break;
		}
		case CMD_SET_MOTION_PARAM3:
		{
			strCmd = "SET_MOTION_PARAM3";
			break;
		}
		case CMD_SET_MOTION_XYTABLE:
		{
			strCmd = "SET_MOTION_XYTABLE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_HOME:
		{
			strCmd = "SET_MOTION_HOME";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_FLY_X:
		{
			strCmd = "SET_FLY_X";
			bRead_double = TRUE;
			break;
		}
		case CMD_SET_FLY_Y:
		{
			strCmd = "SET_FLY_Y";
			bRead_double = TRUE;
			break;
		}
		case CMD_SET_FLY_DELAY:
		{
			strCmd = "SET_FLY_DELAY";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_SIMULATE_ENCODER:
		{
			strCmd = "SET_SIMULATE_ENCODER";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_REBOOT:
		{
			strCmd = "REBOOT";
			break;
		}
		case CMD_JOGSTART:
		{
			strCmd = "JOGSTART";
			bRead_long = TRUE;
			break;
		}
		case CMD_JOGEND:
		{
			strCmd = "JOGEND";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_BACKSPACE:
		{
			strCmd = "SET_MOTION_BACKSPACE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_HOME_REAL:
		{
			strCmd = "SET_MOTION_HOME_REAL";
			bRead_long = TRUE;
			break;
		}
		case CMD_RESET_ENCODER:
		{
			strCmd = "RESET_ENCODER";
			bRead_long = TRUE;
			break;
		}
		case CMD_R05_CONFIG:
		{
			strCmd = "R05_CONFIG";
			bRead_long = TRUE;
			break;
		}
		case CMD_R05_LEVEL:
		{
			strCmd = "R05_LEVEL";
			bRead_long = TRUE;
			break;
		}
		case CMD_WRITE_IO_PORT_EX:
		{
			strCmd = "WRITE_IO_PORT_EX";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_DSP_START:
		{
			strCmd = "DSP_START";
			break;
		}
		case CMD_SET_PROGRAM_READY:
		{
			strCmd = "SET_PROGRAM_READY";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_MARKING_READY:
		{
			strCmd = "SET_MARKING_READY";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_MARKING_END:
		{
			strCmd = "SET_MARKING_END";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_MARKING_END_TIME:
		{
			strCmd = "SET_MARKING_END_TIME";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_RESET_PULSE_COUNT:
		{
			strCmd = "RESET_PULSE_COUNT";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_LABEL:
		{
			strCmd = "SET_LABEL";
			bRead_long = TRUE;
			break;
		}
		case CMD_GOTO_XYZ:
		{
			strCmd = "GOTO_XYZ";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ENCODER_MODE:
		{
			strCmd = "SET_ENCODER_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ENCODER_COMP:
		{
			strCmd = "SET_ENCODER_COMP";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_XY:
		{
			strCmd = "SET_MOTION_XY";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_CARD_CONTROL_STATUS:
		{
			strCmd = "SET_CARD_CONTROL_STATUS";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_FLY_PREVIEW_MODE:
		{
			strCmd = "SET_FLY_PREVIEW_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_IO_STARTSTOP:
		{
			strCmd = "SET_IO_STARTSTOP";
			bRead_long = TRUE;
			break;
		}
		case CMD_EXEC_LIST:
		{
			strCmd = "EXEC_LIST";
			bRead_short = TRUE;
			break;
		}
		case CMD_STOP_LIST:
		{
			strCmd = "STOP_LIST";
			break;
		}
		case CMD_STOP_EXEC:
		{
			strCmd = "STOP_EXEC";
			break;
		}
		case CMD_START_LIST:
		{
			strCmd = "START_LIST";
			bRead_short = TRUE;
			break;
		}
		case CMD_MOTION_ARC_AXIS:
		{
			strCmd = "MOTION_ARC_AXIS";
			bRead_long = TRUE;
			break;
		}
		case CMD_MOTION_ARC_LON_STEP:
		{
			strCmd = "MOTION_ARC_LON_STEP";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_MOTION_ARC_CLR:
		{
			strCmd = "MOTION_ARC_CLR";
			break;
		}
		case CMD_MOTION_ARC_START:
		{
			strCmd = "MOTION_ARC_START";
			break;
		}
		case CMD_MOTION_ARC_ADD:
		{
			strCmd = "MOTION_ARC_ADD";
			bRead_long = TRUE;
			break;
		}
		case CMD_ENCODER_MOTION:
		{
			strCmd = "ENCODER_MOTION";
			break;
		}
		case CMD_SET_STARTSTOP_FILTER:
		{
			strCmd = "SET_STARTSTOP_FILTER";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_IO_FILTER_LAG:
		{
			strCmd = "SET_IO_FILTER_LAG";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SCAN_XY_ENABLE:
		{
			strCmd = "SCAN_XY_ENABLE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LOAD_CORRECTION_FILE:
		{
			strCmd = "LOAD_CORRECTION_FILE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LOAD_CORRECTION_FILE_3D:
		{
			strCmd = "LOAD_CORRECTION_FILE_3D";
			break;
		}
		case CMD_LOAD_VARPOLYDELAY:
		{
			strCmd = "LOAD_VARPOLYDELAY";
			bRead_long = TRUE;
			break;
		}
		case CMD_WRITE_MEM:
		{		
			strCmd = "WRITE_MEM";
			unsigned long ulOffset = *(unsigned long *)&g_ReadBuffer[6];
			strLog.Format(_T("WRITE_MEM,%d,%dBytes"), ulOffset, usSize-4);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_LENS_TRANSFORM:
		{
			strCmd = "SET_LENS_TRANSFORM";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_XY_SWAP:
		{
			strCmd = "SET_XY_SWAP";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_XYZ_REVERSE:
		{
			strCmd = "SET_XYZ_REVERSE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_INDEX_DELAY:
		{
			strCmd = "SET_MOTION_INDEX_DELAY";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_SOFT_LIMIT:
		{
			strCmd = "SET_MOTION_SOFT_LIMIT";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ENCODER_PARAM:
		{
			long Axis, Inverse;
			double dlScale;
			
			Axis = *(long *)&g_ReadBuffer[6];
			Inverse = *(long *)&g_ReadBuffer[10];
			dlScale = *(double *)&g_ReadBuffer[14];
			strCmd = "SET_ENCODER_PARAM";
			strLog.Format(_T("%s,%d,%d,%.3f"), strCmd, Axis, Inverse, dlScale);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_ENABLE_3D_MULTI_COR:
		{
			strCmd = "ENABLE_3D_MULTI_COR";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_Z_COMP_TABLE:
		{
			strCmd = "SET_Z_COMP_TABLE";
			strLog.Format(_T("%s,%dBytes"), strCmd, usSize);
			bRead_MultiTypes =TRUE;
			break;
		}
		case CMD_SET_ENCODER_POS_TRIGGER:
		{
			strCmd = "SET_ENCODER_POS_TRIGGER";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_GANTRY:
		{
			strCmd = "SET_MOTION_GANTRY";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_IO:
		{
			strCmd = "SET_MOTION_IO";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_POS_ERR:
		{
			strCmd = "SET_MOTION_POS_ERR";
			bRead_long = TRUE;
			break;
		}
		case CMD_MOTION_SERVO_ON:
		{
			strCmd = "MOTION_SERVO_ON";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_SCAN_PSO:
		{
			strCmd = "SET_SCAN_PSO";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ETAB_INTERVAL:
		{
			strCmd = "SET_ETAB_INTERVAL";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_PSO_FILTER:
		{
			strCmd = "SET_PSO_FILTER";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_PSO_PWM2:
		{
			strCmd = "SET_PSO_PWM2";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_PSO_LASER_MODE:
		{
			strCmd = "SET_PSO_LASER_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ETAB_TRG_IN:
		{
			strCmd = "SET_ETAB_TRG_IN";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ETAB_OUT:
		{
			strCmd = "SET_ETAB_OUT";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ENCODER_FLY_REF_MODE:
		{
			strCmd = "SET_ENCODER_FLY_REF_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_SCAN_PARAMS:
		{
			long lRefMode, lOffsetX, lOffsetY;
			double dRateX, dRateY;
			unsigned short usPtr = 0;
			
			lRefMode = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lOffsetX = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lOffsetY = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dRateX = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			dRateY = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strCmd = "SET_MOTION_SCAN_PARAMS";
			strLog.Format(_T("%s,%d,%d,%d,%.3f,%.3f"), strCmd, lRefMode, lOffsetX, lOffsetY, dRateX, dRateY);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_DOTLINE_PARAMS:
		{
			strCmd = "SET_DOTLINE_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_3D_Z_SCALE:
		{
			strCmd = "SET_3D_Z_SCALE";
			bRead_double = TRUE;
			break;
		}
		case CMD_SET_SCAN_PSO_DIRECT:
		{
			strCmd = "SET_SCAN_PSO_DIRECT";
			bRead_long = TRUE;
			break;
		}
		case CMD_MOTION_ARC_CLR_EX:
		{
			strCmd = "MOTION_ARC_CLR_EX";
			break;
		}
		case CMD_MOTION_ARC_START_EX:
		{
			strCmd = "MOTION_ARC_START_EX";
			break;
		}
		case CMD_MOTION_ARC_ADD_EX:
		{
			strCmd = "MOTION_ARC_ADD_EX";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ETAB_PREVIEW_MODE:
		{
			long lEnable;
			double dSpeed;
			unsigned short usPtr = 0;
			
			lEnable = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dSpeed = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strCmd = "SET_ETAB_PREVIEW_MODE";
			strLog.Format(_T("%s,%d,%.3f"), strCmd, lEnable, dSpeed);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_NG_KEEP_DISTANCE:
		{
			strCmd = "SET_NG_KEEP_DISTANCE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_IPG_POWER_SETTING:
		{
			strCmd = "SET_IPG_POWER_SETTING";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_NG_DISABLE_DISTANCE:
		{
			strCmd = "SET_NG_DISABLE_DISTANCE";
			bRead_long = TRUE;
			break;
		}
		case CMD_DISABLE_STOP:
		{
			strCmd = "DISABLE_STOP";
			bRead_long = TRUE;
			break;
		}
		case CMD_LOAD_POWER_COR_FILE:
		{
			strCmd = "LOAD_POWER_COR_FILE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_POWER_COR_PARAMS:
		{
			strCmd = "SET_POWER_COR_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_DIGITAL_POWER_OUT:
		{
			strCmd = "SET_DIGITAL_POWER_OUT";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_POWER:
		{
			strCmd = "SET_POWER";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MAXMIN_POWER_VALUE:
		{
			strCmd = "SET_MAXMIN_POWER_VALUE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ETAB_SPEED_POWER_CONTROL:
		{
			long lMode, lMaxPower, lMinPower;
			double dMaxSpeed, dMinSpeed, dCutOffSpeed;
			unsigned short usPtr = 0;
			
			lMode = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lMaxPower = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lMinPower = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dMaxSpeed = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			dMinSpeed = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			dCutOffSpeed = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strCmd = "SET_ETAB_SPEED_POWER_CONTROL";
			strLog.Format(_T("%s,%d,%d,%d,%.3f,%.3f,%.3f"), strCmd, lMode, lMaxPower, lMinPower, dMaxSpeed, dMinSpeed, dCutOffSpeed);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_SCAN_FEEDBACK_MODE:
		{
			strCmd = "SET_SCAN_FEEDBACK_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_PSO_MODE:
		{
			strCmd = "SET_PSO_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_EXT_COR_TABLE:
		{
			strCmd = "SET_EXT_COR_TABLE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_NG_RESET_POINT:
		{
			strCmd = "SET_NG_RESET_POINT";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ETAB_ENCODER_DIRECTION:
		{
			strCmd = "SET_ETAB_ENCODER_DIRECTION";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_SCAN_FB_COMP:
		{
			strCmd = "SET_MOTION_SCAN_FB_COMP";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_SCAN_FB_COMP_DATA:
		{
			strCmd = "SET_MOTION_SCAN_FB_COMP_DATA";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_NG_JUMP_CONDITION:
		{
			strCmd = "SET_NG_JUMP_CONDITION";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MULTI_PATH_WOBBEL_PARAM:
		{
			strCmd = "SET_MULTI_PATH_WOBBEL_PARAM";
			bRead_long = TRUE;
			break;
		}
		case CMD_LOAD_MULTI_PATH_WOBBEL_SEGMENT:
		{
			strCmd = "LOAD_MULTI_PATH_WOBBEL_SEGMENT";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_AMOF_PREDICTION_TIME:
		{
			strCmd = "SET_AMOF_PREDICTION_TIME";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_NG2_PARAMS:
		{
			strCmd = "SET_NG2_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ETAB_SPEED_FREQ_CONTROL:
		{
			long lMode, lMaxFreq, lMinFreq;
			double dMaxSpeed, dMinSpeed;
			unsigned short usPtr = 0;
			
			lMode = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lMaxFreq = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lMinFreq = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dMaxSpeed = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			dMinSpeed = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strCmd = "SET_ETAB_SPEED_FREQ_CONTROL";
			strLog.Format(_T("%s,%d,%d,%d,%.3f,%.3f"), strCmd, lMode, lMaxFreq, lMinFreq, dMaxSpeed, dMinSpeed);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_LASER_SIGNAL_LAG:
		{
			strCmd = "SET_LASER_SIGNAL_LAG";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ENCODER_JUMP_UPDATE:
		{
			strCmd = "SET_ENCODER_JUMP_UPDATE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_SCAN_CTRL_MODE:
		{
			strCmd = "SET_SCAN_CTRL_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_WOBBEL_EX:
		{
			long amplitude;
			float theta;
			long minamp;
			double dAmpInc;
			unsigned short usPtr = 0;
			
			amplitude = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			theta = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			minamp = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dAmpInc = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strCmd = "SET_WOBBEL_EX";
			strLog.Format(_T("%s,%d,%.3f,%d,%.3f"), strCmd, amplitude, theta, minamp, dAmpInc);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_IPG_PD_CFG:
		{
			strCmd = "SET_IPG_PD_CFG";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_IPG_PD_CMD:
		{
			strCmd = "SET_IPG_PD_CMD";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_LASER_SHIFT:
		{
			strCmd = "SET_LASER_SHIFT";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_INT_FREQ_ALIGN_PARAMS:
		{
			strCmd = "SET_INT_FREQ_ALIGN_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_GOTO_SPEED:
		{
			strCmd = "SET_GOTO_SPEED";
			bRead_double = TRUE;
			break;
		}
		case CMD_ENABLE_IPG_POWER_MODE:
		{
			strCmd = "ENABLE_IPG_POWER_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_IPG_DIGITAL_POWER:
		{
			strCmd = "SET_IPG_DIGITAL_POWER";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_CUT_SCAN_RATE:
		{
			double dRate[3];
			long lOffset[3];
			unsigned short usPtr = 0;
			
			dRate[0] = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			dRate[1] = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			dRate[2] = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			lOffset[0] = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lOffset[1] = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lOffset[2] = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			strCmd = "SET_CUT_SCAN_RATE";
			strLog.Format(_T("%s,%.3f,%.3f,%.3f,%d,%d,%d"), strCmd, dRate[0], dRate[1], dRate[2], lOffset[0], lOffset[1], lOffset[2]);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_ENCODER_MOTION:
		{
			strCmd = "SET_ENCODER_MOTION";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_MODEL_ENABLE:
		{
			long lMode;
			double dPredictionTime;
			unsigned short usPtr = 0;

			lMode = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dPredictionTime = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strCmd = "SET_MOTION_MODEL_ENABLE";
			strLog.Format(_T("%s,%d,%.3f"), strCmd, lMode, dPredictionTime);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_MOTION_MODEL_PARAMS:
		{
			long lAxis, lIndex;
			double dParamA, dParamB;
			unsigned short usPtr = 0;

			lAxis = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lIndex = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dParamA = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			dParamB = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strCmd = "SET_MOTION_MODEL_PARAMS";
			strLog.Format(_T("%ㄋ,%d,%d,%.3f,%.3f"), strCmd, lAxis, lIndex, dParamA, dParamB);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_SCAN_CTRL_LASERON_KEEP:
		{
			strCmd = "SET_SCAN_CTRL_LASERON_KEEP";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_SPEED_POWER_PARAMS:
		{
			strCmd = "SET_MOTION_SPEED_POWER_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_LASER_SIGNAL_DELAY:
		{
			strCmd = "SET_LASER_SIGNAL_DELAY";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_ROUTE_PARAMS:
		{
			long lPulseScale, AccMode, StopAcc;
			double dMaxAcc;
			unsigned short usPtr = 0;

			lPulseScale = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			AccMode = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			StopAcc = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dMaxAcc = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strCmd = "SET_MOTION_ROUTE_PARAMS";
			strLog.Format(_T("%s,%d,%d,%d,%.3f"), strCmd, lPulseScale, AccMode, StopAcc, dMaxAcc);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_MOTION_PSO_MODE:
		{
			strCmd = "SET_MOTION_PSO_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ROT_CENTER:
		{
			strCmd = "SET_ROT_CENTER";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_ENCODER:
		{
			strCmd = "SET_MOTION_ENCODER";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ECM_SK_CONFIG:
		{
			strCmd = "SET_ECM_SK_CONFIG";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ECM_SK_DATA:
		{
			strCmd = "SET_ECM_SK_DATA";
			strLog.Format(_T("%s,%dBytes"), strCmd, usSize);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_ECM_SK_DATA_EX:
		{
			strCmd = "SET_ECM_SK_DATA_EX";
			strLog.Format(_T("%s,%dBytes"), strCmd, usSize);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_SET_MOTION_COMP_PARAMS:
		{
			strCmd = "SET_MOTION_COMP_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_COMP_DATA:
		{
			strCmd = "SET_MOTION_COMP_DATA";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_ROUTE_AXIS_MAP:
		{
			strCmd = "SET_MOTION_ROUTE_AXIS_MAP";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_WAIT_ENCODER_TRG_PARAM:
		{
			strCmd = "SET_WAIT_ENCODER_TRG_PARAM";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_EXEC_LIST_JUMP_POS:
		{
			strCmd = "SET_EXEC_LIST_JUMP_POS";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_LASER_SHIFT_TIME:
		{
			strCmd = "SET_LASER_SHIFT_TIME";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_AMOF_AXIS_MAP:
		{
			strCmd = "SET_AMOF_AXIS_MAP";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_SCAN_MODEL_MODE:
		{
			strCmd = "SET_SCAN_MODEL_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_SCAN_ACC_MODEL_PARAMS:
		{
			long lAxis;
			double dAcc;
			double dToleErr;
			unsigned short usPtr = 0;

			lAxis = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dAcc = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			dToleErr = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strCmd = "SET_SCAN_ACC_MODEL_PARAMS";
			strLog.Format(_T("%s,%d,%.3f,%.3f"), strCmd, lAxis, dAcc, dToleErr);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_RING_BUFFER_COUNT_ADD:
		{
			strCmd = "RING_BUFFER_COUNT_ADD";
			break;
		}
		case CMD_SET_PWM_BURST_PARAMS:
		{
			strCmd = "SET_PWM_BURST_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_JUMP_TO:
		{
			strCmd = "LIST_JUMP_TO";
			break;
		}
		case CMD_LIST_MARK_TO:
		{
			strCmd = "LIST_MARK_TO";
			break;
		}
		case CMD_LIST_JUMP_SPEED:
		{
			strCmd = "LIST_JUMP_SPEED";
			bRead_double = TRUE;
			break;
		}
		case CMD_LIST_MARK_SPEED:
		{
			strCmd = "LIST_MARK_SPEED";
			bRead_double = TRUE;
			break;
		}
		case CMD_LIST_SET_LASER_DELAY:
		{
			strCmd = "LIST_SET_LASER_DELAY";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_SCANNER_DELAY:
		{
			strCmd = "LIST_SET_SCANNER_DELAY";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_LASER_TIMING:
		{
			strCmd = "LIST_SET_LASER_TIMING";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_LASER_ON:
		{
			strCmd = "LIST_LASER_ON";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_LASER_SIGNAL_ON:
		{
			strCmd = "LIST_LASER_SIGNAL_ON";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_LASER_FPK:
		{
			strCmd = "LIST_SET_LASER_FPK";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_STANDBY:
		{
			strCmd = "LIST_SET_STANDBY";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_ARC_ABS:
		{
			long x;
			long y;
			float fAngle;
			unsigned short usPtr = 0;

			x = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			y = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			fAngle = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_ARC_ABS";
			strLog.Format(_T("%s,%d,%d,%.3f"), strCmd, x, y, fAngle);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_ARC_REL:
		{
			long x;
			long y;
			float fAngle;
			unsigned short usPtr = 0;

			x = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			y = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			fAngle = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_ARC_REL";
			strLog.Format(_T("%s,%d,%d,%.3f"), strCmd, x, y, fAngle);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_CLEAR_IO_COND_LIST:
		{
			strCmd = "LIST_CLEAR_IO_COND_LIST";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_JUMP_ABS:
		{
			strCmd = "LIST_JUMP_ABS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_JUMP_REL:
		{
			strCmd = "LIST_JUMP_REL";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_LIST_CALL:
		{
			strCmd = "LIST_LIST_CALL";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_CALL_COND:
		{
			strCmd = "LIST_CALL_COND";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_JUMP_COND:
		{
			strCmd = "LIST_JUMP_COND";
			bRead_ulong = TRUE;
			break;
		}		
		case CMD_LIST_LIST_NOP:
		{
			strCmd = "LIST_LIST_NOP";
			break;
		}
		case CMD_LIST_LIST_RETURN:
		{
			strCmd = "LIST_LIST_RETURN";
			break;
		}
		case CMD_LIST_LONG_DELAY:
		{
			strCmd = "LIST_LONG_DELAY";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_MARK_ABS:
		{
			strCmd = "LIST_MARK_ABS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MARK_REL:
		{
			strCmd = "LIST_MARK_REL";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SAVE_AND_RESTART_TIMER:
		{
			strCmd = "LIST_SAVE_AND_RESTART_TIMER";
			break;
		}
		case CMD_LIST_SET_CONTROL_MODE_LIST:
		{
			strCmd = "LIST_SET_CONTROL_MODE_LIST";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_SET_END_OF_LIST:
		{
			strCmd = "LIST_SET_END_OF_LIST";
			break;
		}
		case CMD_LIST_SET_EXTSTARTPOS_LIST:
		{
			strCmd = "LIST_SET_EXTSTARTPOS_LIST";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_SET_IO_COND_LIST:
		{
			strCmd = "LIST_SET_IO_COND_LIST";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_SET_LIST_JUMP:
		{
			strCmd = "LIST_SET_LIST_JUMP";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_MATRIX_LIST:
		{
			unsigned long uli, ulj;
			double m_ij;
			unsigned short usPtr = 0;

			uli = *(unsigned long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(unsigned long);
			ulj = *(unsigned long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(unsigned long);
			m_ij = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strCmd = "LIST_SET_MATRIX_LIST";
			strLog.Format(_T("%s,%d,%d,%.3f"), strCmd, uli, ulj, m_ij);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_SET_OFFSET_LIST:
		{
			strCmd = "LIST_SET_OFFSET_LIST";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_PIXEL:
		{
			strCmd = "LIST_SET_PIXEL";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_SET_PIXEL_LINE:
		{
			unsigned long pixel_mode, pixel_period, da_ch;
			float X, Y;
			unsigned short usPtr = 0;

			pixel_mode = *(unsigned long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(unsigned long);
			pixel_period = *(unsigned long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(unsigned long);
			X = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			Y = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			da_ch = *(unsigned long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(unsigned long);
			strCmd = "LIST_SET_PIXEL_LINE";
			strLog.Format(_T("%s,%d,%d,%.3f,%.3f,%d"), strCmd, pixel_mode, pixel_period, X, Y, da_ch);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_SET_SCANNER_DELAYS:
		{
			strCmd = "LIST_SET_SCANNER_DELAYS";
			break;
		}
		case CMD_LIST_SET_TRIGGER:
		{
			strCmd = "LIST_SET_TRIGGER";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_WAIT:
		{
			strCmd = "LIST_SET_WAIT";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_SET_WOBBEL:
		{
			unsigned long amplitude;
			float theta;
			unsigned short usPtr = 0;

			amplitude = *(unsigned long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(unsigned long);
			theta = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_SET_WOBBEL";
			strLog.Format(_T("%s,%d,%.3f"), strCmd, amplitude, theta);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_TIMED_JUMP_ABS:
		{
			strCmd = "LIST_TIMED_JUMP_ABS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_TIMED_JUMP_REL:
		{
			strCmd = "LIST_TIMED_JUMP_REL";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_TIMED_MARK_ABS:
		{
			strCmd = "LIST_TIMED_MARK_ABS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_TIMED_MARK_REL:
		{
			strCmd = "LIST_TIMED_MARK_REL";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_WRITE_8BIT_PORT_LIST:
		{
			strCmd = "LIST_WRITE_8BIT_PORT_LIST";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_WRITE_DA_X_LIST:
		{
			strCmd = "LIST_WRITE_DA_X_LIST";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_WRITE_IO_PORT_LIST:
		{
			strCmd = "LIST_WRITE_IO_PORT_LIST";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_Z_OUT_LIST:
		{
			strCmd = "LIST_Z_OUT_LIST";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_MOVETO:
		{
			strCmd = "LIST_MOTION_MOVETO";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_MOTION_XYTABLE:
		{
			strCmd = "LIST_SET_MOTION_XYTABLE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MARK_SPOT:
		{
			strCmd = "LIST_MARK_SPOT";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_WRITE_IO_PORT_LIST_EX:
		{
			strCmd = "LIST_WRITE_IO_PORT_LIST_EX";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_MARKING_READY_LIST:
		{
			strCmd = "SET_MARKING_READY_LIST";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_MOTION_HOME_LIST:
		{
			strCmd = "SET_MOTION_HOME_LIST";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_XYTABLE_HOME_LIST:
		{
			strCmd = "SET_XYTABLE_HOME_LIST";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_LABEL_LIST:
		{
			strCmd = "SET_LABEL_LIST";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_COND_TIMEOUT:
		{
			strCmd = "SET_COND_TIMEOUT";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_FLY_X_LIST:
		{
			strCmd = "SET_FLY_X_LIST";
			bRead_double = TRUE;
			break;
		}
		case CMD_SET_FLY_Y_LIST:
		{
			strCmd = "SET_FLY_Y_LIST";
			bRead_double = TRUE;
			break;
		}
		case CMD_SET_FLY_DELAY_LIST:
		{
			strCmd = "SET_FLY_DELAY_LIST";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_SIMULATE_ENCODER_LIST:
		{
			strCmd = "SET_SIMULATE_ENCODER_LIST";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_JUMP_3D_ABS:
		{
			strCmd = "LIST_JUMP_3D_ABS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_JUMP_3D_REL:
		{
			strCmd = "LIST_JUMP_3D_REL";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MARK_3D_ABS:
		{
			strCmd = "LIST_MARK_3D_ABS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MARK_3D_REL:
		{
			strCmd = "LIST_MARK_3D_REL";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_TIMED_JUMP_3D_ABS:
		{
			strCmd = "LIST_TIMED_JUMP_3D_ABS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_TIMED_JUMP_3D_REL:
		{
			strCmd = "LIST_TIMED_JUMP_3D_REL";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_TIMED_MARK_3D_ABS:
		{
			strCmd = "LIST_TIMED_MARK_3D_ABS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_TIMED_MARK_3D_REL:
		{
			strCmd = "LIST_TIMED_MARK_3D_REL";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_RESET_PULSE_COUNT:
		{
			strCmd = "LIST_RESET_PULSE_COUNT";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_MOTION_XY:
		{
			strCmd = "LIST_SET_MOTION_XY";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_LAYER_ENCODER_CNT:
		{
			strCmd = "LIST_SET_LAYER_ENCODER_CNT";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_WAIT_ENCODER_CNT:
		{
			strCmd = "LIST_WAIT_ENCODER_CNT";
			break;
		}
		case CMD_LIST_JOGSTART:
		{
			strCmd = "LIST_JOGSTART";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_JOGEND:
		{
			strCmd = "LIST_JOGEND";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MARK_POLY:
		{
			strCmd = "LIST_MARK_POLY";
			break;
		}
		case CMD_LIST_DOT_MARK_ABS:
		{
			strCmd = "LIST_DOT_MARK_ABS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_DOT_MARK_3D_ABS:
		{
			strCmd = "LIST_DOT_MARK_3D_ABS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_PWM_VARPERIOD:
		{
			strCmd = "LIST_SET_PWM_VARPERIOD";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_WOBBEL_2:
		{
			strCmd = "LIST_SET_WOBBEL_2";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_SKYWRITING_MODE:
		{
			strCmd = "LIST_SET_SKYWRITING_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SCAN_XY_ENABLE:
		{
			strCmd = "LIST_SCAN_XY_ENABLE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SYN_CONDITION:
		{
			strCmd = "LIST_SYN_CONDITION";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_ADD_XY:
		{
			strCmd = "LIST_MOTION_ARC_ADD_XY";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_ADD_RZ:
		{
			strCmd = "LIST_MOTION_ARC_ADD_RZ";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_ADD_TIME:
		{
			strCmd = "LIST_MOTION_ARC_ADD_TIME";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_LON_STEP:
		{
			strCmd = "LIST_MOTION_ARC_LON_STEP";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_START:
		{
			strCmd = "LIST_MOTION_ARC_START";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_TIMED_ARC_ABS:
		{
			long x, y, step;
			float fAngle;
			unsigned short usPtr = 0;

			x = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			y = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			fAngle = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			step = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			strCmd = "LIST_TIMED_ARC_ABS";
			strLog.Format(_T("%s,%d,%d,%.3f,%d"), strCmd, x, y, fAngle, step);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_TIMED_ARC_REL:
		{
			long dx, dy, step;
			float fAngle;
			unsigned short usPtr = 0;

			dx = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dy = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			fAngle = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			step = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			strCmd = "LIST_TIMED_ARC_ABS";
			strLog.Format(_T("%s,%d,%d,%.3f,%d"), strCmd, dx, dy, fAngle, step);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_ARC_3D_ABS:
		{
			long x, y, z;
			float fAngle;
			unsigned short usPtr = 0;

			x = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			y = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			z = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			fAngle = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_ARC_3D_ABS";
			strLog.Format(_T("%s,%d,%d,%d,%.3f"), strCmd, x, y, z, fAngle);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_ARC_3D_REL:
		{
			long dx, dy, dz;
			float fAngle;
			unsigned short usPtr = 0;

			dx = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dy = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dz = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			fAngle = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_ARC_3D_REL";
			strLog.Format(_T("%s,%d,%d,%d,%.3f"), strCmd, dx, dy, dz, fAngle);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_TIMED_ARC_3D_ABS:
		{
			long step, x, y, z;
			float fAngle;
			unsigned short usPtr = 0;

			step = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			x = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			y = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			z = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			fAngle = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_TIMED_ARC_3D_ABS";
			strLog.Format(_T("%s,%d,%d,%d,%d,%.3f"), strCmd, step, x, y, z, fAngle);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_TIMED_ARC_3D_REL:
		{
			long step, dx, dy, dz;
			float fAngle;
			unsigned short usPtr = 0;

			step = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dx = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dy = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dz = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			fAngle = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_TIMED_ARC_3D_REL";
			strLog.Format(_T("%s,%d,%d,%d,%d,%.3f"), strCmd, step, dx, dy, dz, fAngle);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_MOTION_MOVETO_MULTI:
		{
			strCmd = "LIST_MOTION_MOVETO_MULTI";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_SCAN_PSO:
		{
			strCmd = "LIST_SET_SCAN_PSO";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_WAIT_ENCODER:
		{
			strCmd = "LIST_WAIT_ENCODER";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_ENCODER_JUMP:
		{
			strCmd = "LIST_ENCODER_JUMP";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_ENCODER_FLY:
		{
			long lAxis;
			double dSpeed;
			unsigned short usPtr = 0;

			lAxis = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dSpeed = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strCmd = "LIST_SET_ENCODER_FLY";
			strLog.Format(_T("%s,%d,%.3f"), strCmd, lAxis, dSpeed);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_SET_LOOP_CNT:
		{
			strCmd = "LIST_SET_LOOP_CNT";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_LOOP_JUMP:
		{
			strCmd = "LIST_SET_LOOP_JUMP";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_DIRECT_START:
		{
			strCmd = "LIST_MOTION_DIRECT_START";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_DIRECT_MOVE:
		{
			strCmd = "LIST_MOTION_DIRECT_MOVE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_SCAN_ENABLE:
		{
			strCmd = "LIST_MOTION_SCAN_ENABLE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_WAIT_MOTION_POS:
		{
			strCmd = "LIST_SET_WAIT_MOTION_POS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_WAIT_MOTION_POS:
		{
			strCmd = "LIST_WAIT_MOTION_POS";
			break;
		}
		case CMD_LIST_SET_DOTLINE_PARAMS:
		{
			strCmd = "LIST_SET_DOTLINE_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SELECT_LIST:
		{
			strCmd = "LIST_SELECT_LIST";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_DOTLINE_ONOFF:
		{
			strCmd = "LIST_SET_DOTLINE_ONOFF";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_ELLIPSE:
		{
			long a, b;
			float Phi0, Phi;
			unsigned short usPtr = 0;

			a = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			b = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			Phi0 = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			Phi = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_SET_ELLIPSE";
			strLog.Format(_T("%s,%d,%d,%.3f,%.3f"), strCmd, a, b, Phi0, Phi);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_ELLIPSE_ABS:
		{
			long mode, x, y;
			float theta;
			unsigned short usPtr = 0;

			mode = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			x = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			y = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			theta = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_ELLIPSE_ABS";
			strLog.Format(_T("%s,%d,%d,%d,%.3f"), strCmd, mode, x, y, theta);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_ELLIPSE_REL:
		{
			long mode, x, y;
			float theta;
			unsigned short usPtr = 0;

			mode = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			x = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			y = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			theta = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_ELLIPSE_REL";
			strLog.Format(_T("%s,%d,%d,%d,%.3f"), strCmd, mode, x, y, theta);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_SET_NG_RESET_POINT:
		{
			strCmd = "LIST_SET_NG_RESET_POINT";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_ADD_X_EX:
		{
			strCmd = "LIST_MOTION_ARC_ADD_X_EX";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_ADD_Y_EX:
		{
			strCmd = "LIST_MOTION_ARC_ADD_Y_EX";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_ADD_TIME_EX:
		{
			strCmd = "LIST_MOTION_ARC_ADD_TIME_EX";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_START_EX:
		{
			strCmd = "LIST_MOTION_ARC_START_EX";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_RECORD_MARK_TIME:
		{
			strCmd = "LIST_RECORD_MARK_TIME";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_POWER:
		{
			strCmd = "LIST_SET_POWER";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_NG_LIST_JUMP:
		{
			strCmd = "LIST_NG_LIST_JUMP";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_SCAN_FEEDBACK_MODE:
		{
			strCmd = "LIST_SET_SCAN_FEEDBACK_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_PSO_MODE:
		{
			strCmd = "LIST_SET_PSO_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_AMOF_SYNC:
		{
			strCmd = "LIST_AMOF_SYNC";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_ADV_WOBBEL_MODE:
		{
			long lEnable, lUpdateCnt;
			float theta1, theta2;
			unsigned short usPtr = 0;

			lEnable = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lUpdateCnt = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			theta1 = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			theta2 = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_SET_ADV_WOBBEL_MODE";
			strLog.Format(_T("%s,%d,%d,%.3f,%.3f"), strCmd, lEnable, lUpdateCnt, theta1, theta2);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_SET_ADV_WOBBEL_WAVE:
		{
			long lAmpV, lAmpH;
			float theta1, start1, theta2, start2;
			unsigned short usPtr = 0;

			lAmpV = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			theta1 = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			start1 = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			lAmpH = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			theta2 = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			start2 = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			strCmd = "LIST_SET_ADV_WOBBEL_WAVE";
			strLog.Format(_T("%s,%d,%.3f,%.3f,%d,%.3f,%.3f"), strCmd, lAmpV, theta1, start1, lAmpH, theta2, start2);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_ENCODER_JUMP_UPDATE:
		{
			strCmd = "LIST_ENCODER_JUMP_UPDATE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_WOBBEL_EX:
		{
			long amplitude, lDelay;
			float theta;
			unsigned short usPtr = 0;

			amplitude = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			theta = *(float *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(float);
			lDelay = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			strCmd = "LIST_SET_ADV_WOBBEL_WAVE";
			strLog.Format(_T("%s,%d,%.3f,%d"), strCmd, amplitude, theta, lDelay);
			bRead_MultiTypes = TRUE;
			break;
		}
		case CMD_LIST_SET_IPG_PD_CMD:
		{
			strCmd = "LIST_SET_IPG_PD_CMD";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_WAIT_IPG_PD:
		{
			strCmd = "LIST_WAIT_IPG_PD";
			break;
		}
		case CMD_LIST_SET_IPG_DIGITAL_POWER:
		{
			strCmd = "LIST_SET_IPG_DIGITAL_POWER";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_PIXEL_EX:
		{
			strCmd = "LIST_SET_PIXEL_EX";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_LIST_SET_CUT_SCAN_PARAMS:
		{
			strCmd = "LIST_SET_CUT_SCAN_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_WOBBEL_EX2:
		{
			long minamp;
			double dAmpInc;
			unsigned short usPtr = 0;

			minamp = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dAmpInc = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strLog.Format(_T("LIST_SET_WOBBEL_EX2,%d,%.3f"), minamp, dAmpInc);
			break;
		}
		case CMD_LIST_SET_SPIRAL_PARAM1:
		{
			long lIndex, lDir, lInCircle, lOutCircle, lStepMode;
			double dVar;
			unsigned short usPtr = 0;

			lIndex = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lDir = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lInCircle = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lOutCircle = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lStepMode = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dVar = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strLog.Format(_T("LIST_SET_SPIRAL_PARAM1,%d,%d,%d,%d,%d,%.3f"), lIndex, lDir, lInCircle, lOutCircle, lStepMode, dVar);
			break;
		}
		case CMD_LIST_SET_SPIRAL_PARAM2:
		{
			long lIndex;
			double dMaxAngle, dStAngle, dR;
			unsigned short usPtr = 0;

			lIndex = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dMaxAngle = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			dStAngle = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			dR = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strLog.Format(_T("LIST_SET_SPIRAL_PARAM2,%d,%.3f,%.3f,%.3f"), lIndex, dMaxAngle, dStAngle, dR);
			break;
		}
		case CMD_LIST_SPIRAL_ABS:
		{
			strCmd = "LIST_SPIRAL_ABS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SPIRAL_REL:
		{
			strCmd = "LIST_SPIRAL_REL";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_POWER_COR_PARAMS:
		{
			strCmd = "LIST_SET_POWER_COR_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_LASER_SIGNAL_DELAY:
		{
			strCmd = "LIST_SET_LASER_SIGNAL_DELAY";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_ADD_R_EX:
		{
			strCmd = "LIST_MOTION_ARC_ADD_R_EX";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_ADD_Z_EX:
		{
			strCmd = "LIST_MOTION_ARC_ADD_Z_EX";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_MOTION_ARC_ADD_TIME2_EX:
		{
			strCmd = "LIST_MOTION_ARC_ADD_TIME2_EX";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_MOTION_SPEED_POWER_PARAMS:
		{
			strCmd = "LIST_SET_MOTION_SPEED_POWER_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_MOTION_SPEED_UNIT_RATE:
		{
			strCmd = "LIST_SET_MOTION_SPEED_UNIT_RATE";
			bRead_double = TRUE;
			break;
		}
		case CMD_LIST_SET_ROT_CENTER:
		{
			strCmd = "LIST_SET_ROT_CENTER";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_FLY_ROT:
		{
			strCmd = "LIST_SET_FLY_ROT";
			bRead_double = TRUE;
			break;
		}
		case CMD_LIST_SET_INT_FREQ_ALIGN_PARAMS:
		{
			strCmd = "LIST_SET_INT_FREQ_ALIGN_PARAMS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_WAIT_ENCODER_TRIGGER:
		{
			strCmd = "LIST_WAIT_ENCODER_TRIGGER";
			break;
		}
		case CMD_LIST_SET_EXEC_LIST_JUMP:
		{
			strCmd = "LIST_SET_EXEC_LIST_JUMP";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_CURVE_DOTLINE_MODE:
		{
			strCmd = "LIST_CURVE_DOTLINE_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_DASH_LINE_PARAMS:
		{
			long lMode, lPeriod;
			double dDx, dDy;
			unsigned short usPtr = 0;

			lMode = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			lPeriod = *(long *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(long);
			dDx = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			dDy = *(double *)&g_ReadBuffer[6+usPtr];
			usPtr += sizeof(double);
			strLog.Format(_T("LIST_DASH_LINE_PARAMS,%d,%d,%.3f,%.3f"), lMode, lPeriod, dDx, dDy);
			break;
		}
		case CMD_LIST_DASH_LINE:
		{
			strCmd = "LIST_DASH_LINE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_EXT_START_POS:
		{
			strCmd = "LIST_SET_EXT_START_POS";
			bRead_long = TRUE;
			break;
		}
		case CMD_LIST_SET_PWM_BURST_PARAMS:
		{
			strCmd = "LIST_SET_PWM_BURST_PARAMS";
			bRead_long = TRUE;
			break;
		}
		default:
		{
			strCmd.Format(_T("CMD0x%x,%dBytes"), usCmd, usSize);
			break;
		}
		}

		if(!bRead_MultiTypes)
			strLog = strCmd;

		CString strData;

		if(bRead_short)
		{
			short sReadData[512];

			memset(sReadData, 0, sizeof(sReadData));
			for(int i = 0; i < usSize/2; i++)
			{
				sReadData[i] = *((short *)&g_ReadBuffer[6 + 2*i]);
				strData.Format(_T(",%d"), sReadData[i]);
				strLog.Append(strData);
			}
		}

		if(bRead_long)
		{
			long lReadData[256];

			memset(lReadData, 0, sizeof(lReadData));
			for(int i = 0; i < usSize/4; i++)
			{
				lReadData[i] = *((long *)&g_ReadBuffer[6 + 4*i]);
				strData.Format(_T(",%d"), lReadData[i]);
				strLog.Append(strData);
			}
		}

		if(bRead_ulong)
		{
			unsigned long ulReadData[256];

			memset(ulReadData, 0, sizeof(ulReadData));
			for(int i = 0; i < usSize/4; i++)
			{
				ulReadData[i] = *((unsigned long *)&g_ReadBuffer[6 + 4*i]);
				strData.Format(_T(",%d"), ulReadData[i]);
				strLog.Append(strData);
			}
		}

		if(bRead_double)
		{
			double dReadData[128];

			memset(dReadData, 0, sizeof(dReadData));
			for(int i = 0; i < usSize/8; i++)
			{
				dReadData[i] = *((double *)&g_ReadBuffer[6 + sizeof(double)*i]);
				strData.Format(_T(",%.3f"), dReadData[i]);
				strLog.Append(strData);
			}
		}

		strData = ";";
		strLog.Append(strData);

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
		bCmdLogEnable = GetPrivateProfileInt(_T("CMD"), strCmd, 1, g_strINIpath);
		if(bCmdLogEnable)
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
	
	GetINIFilePath();

	g_bLogEnable = GetPrivateProfileInt(_T("LOG"), _T("ENABLE"), 0, g_strINIpath);

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
	CString strLog, strCmd;
	int ilen = sizeof(struct sockaddr_in);
	char szIPName[256];
	char sndbuffer[1024], rcvbuffer[1024];
	int iSendSize, iRecvSize, iDataSize;
	BOOL bCmdLogEnable = TRUE;

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
				strCmd = "GET_IP_ADDRESS";
				strLog = strCmd;
				
				memset(sndbuffer, 0, sizeof(sndbuffer));
				*((unsigned short *)sndbuffer) = 0xffff;
				*((unsigned short *)sndbuffer + 1) = CMD_GET_IP_ADDRESS;
				*((unsigned short *)sndbuffer + 2) = (unsigned short)iDataSize;
				memcpy(sndbuffer+6, szIPName, iDataSize);
				iSendSize = iDataSize + 6;
				strLog.Append(_T(";"));
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
				bCmdLogEnable = GetPrivateProfileInt(_T("CMD"), strCmd, 1, g_strINIpath);
				if(bCmdLogEnable)
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
