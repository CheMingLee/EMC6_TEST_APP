
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
	BOOL bSendRet, bReplyNull;
	BOOL bRead_short, bRead_long, bRead_ulong, bRead_double;
	CString strLog, strTmp;
	unsigned short usSN, usCmd, usSize;

	memset(lData, 0, sizeof(lData));
	memset(g_ReadBuffer, 0, sizeof(g_ReadBuffer));
	iReadSize = recv(sd_connect, g_ReadBuffer, 1024, 0);
    if (iReadSize > 0)
	{
		bSendRet = TRUE;
		bReplyNull = TRUE;
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
			strLog = "GET_LIST_SIZE;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_RS232_TEST:
		{
			strLog = "RS232_TEST;";
			break;
		}
		case CMD_GET_LABEL:
		{
			strLog = "GET_LABEL;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_COUNT:
		{
			strLog = "GET_COUNT;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_HEAD_STATUS:
		{
			strLog = "GET_HEAD_STATUS;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_HEX_VERSION:
		{
			strLog = "GET_HEX_VERSION;";
			usDataSize = 4;
			lData[0] = 0x00010101;
			strTmp.Format(_T("0x%x"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_HI_DATA:
		{
			strLog = "GET_HI_DATA;";
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
			strLog = "GET_INPUT_POINTER;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_IO_STATUS:
		{
			strLog = "GET_IO_STATUS;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_LIST_SPACE:
		{
			strLog = "GET_LIST_SPACE;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_CARD_VERSION:
		{
			strLog = "GET_CARD_VERSION;";
			usDataSize = 4;
			lData[0] = 0x00030500;
			strTmp.Format(_T("0x%x"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_SERIAL_NUMBER:
		{
			strLog = "GET_SERIAL_NUMBER;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_STARTSTOP_INFO:
		{
			strLog = "GET_STARTSTOP_INFO;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_STATUS:
		{
			strLog = "GET_STATUS;";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_TIME:
		{
			strLog = "GET_TIME;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_VALUE:
		{
			strLog = "GET_VALUE;";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_WAIT_STATUS:
		{
			strLog = "GET_WAIT_STATUS;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_WAVEFORM:
		{
			unsigned long channel, stop;

			channel = *((unsigned long *)&g_ReadBuffer[6]);
			stop = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("GET_WAIT_STATUS,%d,%d;"), channel, stop);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_XY_POS:
		{
			strLog = "GET_XY_POS;";
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
			strLog = "MEASUREMENT_STATUS;";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_READ_IO_PORT:
		{
			strLog = "READ_IO_PORT;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_READ_PIXEL_AD:
		{
			unsigned long pos;

			pos = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("READ_PIXEL_AD,%d;"), pos);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_READ_STATUS:
		{
			strLog = "READ_STATUS;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_PULSE_COUNT:
		{
			long lAxis;

			lAxis = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_MOTION_PULSE_COUNT,%d;"), lAxis);
			usDataSize = 4;
			lData[0] = 250;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_INPOS:
		{
			long lAxis;

			lAxis = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_MOTION_INPOS,%d;"), lAxis);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_STATUS:
		{
			long lAxis;

			lAxis = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_MOTION_STATUS,%d;"), lAxis);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_ENCODER:
		{
			long lAxis;

			lAxis = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_MOTION_ENCODER,%d;"), lAxis);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_SENSOR:
		{
			long lAxis;

			lAxis = *((unsigned long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_MOTION_SENSOR,%d;"), lAxis);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_DEBUG_WATCH_DOG:
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
			break;
		}
		case CMD_MOTION_ARC_GET_SIZE:
		{
			strLog = "MOTION_ARC_GET_SIZE;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_IO_STATUS_EX:
		{
			strLog = "GET_IO_STATUS_EX;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_AUTO_CAL:
		{
			unsigned long ulData[2];

			ulData[0] = *((unsigned long *)&g_ReadBuffer[6]);
			ulData[1] = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("AUTO_CAL,%d,%d;"), ulData[0], ulData[1]);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_CONTROL_COMMAND:
		{
			unsigned long ulData[3];

			ulData[0] = *((unsigned long *)&g_ReadBuffer[6]);
			ulData[1] = *((unsigned long *)&g_ReadBuffer[10]);
			ulData[2] = *((unsigned long *)&g_ReadBuffer[14]);

			strLog.Format(_T("CONTROL_COMMAND,%d,%d,%d;"), ulData[0], ulData[1], ulData[2]);
			break;
		}
		case CMD_GET_ENCODER_SPEED:
		{
			strLog = "GET_ENCODER_SPEED;";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SET_HW_CFG:
		{
			unsigned short iByteCnt;

			iByteCnt = *((unsigned short *)&g_ReadBuffer[6]);

			strLog.Format(_T("SET_HW_CFG,%d;"), iByteCnt);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_HW_CFG:
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
			break;
		}
		case CMD_ERASE_FLASH:
		{
			unsigned long iMode, ulAddr;

			iMode = *((unsigned long *)&g_ReadBuffer[6]);
			ulAddr = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("ERASE_FLASH,%d,%d;"), iMode, ulAddr);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_READ_MEM:
		{
			unsigned long iSize, ulOffset;

			iSize = *((unsigned long *)&g_ReadBuffer[6]);
			ulOffset = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("READ_MEM,%d,%d;"), iSize, ulOffset);
			usDataSize = (unsigned short)iSize;
			strTmp.Format(_T("0,%dBytes"), iSize);
			bReplyNull = FALSE;
			break;
		}
		case CMD_MEM_TO_QSPI:
		{
			unsigned long Addr, Size;

			Addr = *((unsigned long *)&g_ReadBuffer[6]);
			Size = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("MEM_TO_QSPI,%d,%d;"), Addr, Size);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_QSPI_TO_MEM:
		{
			unsigned long Addr, Size;

			Addr = *((unsigned long *)&g_ReadBuffer[6]);
			Size = *((unsigned long *)&g_ReadBuffer[10]);

			strLog.Format(_T("QSPI_TO_MEM,%d,%d;"), Addr, Size);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SHA204_WAKEUP:
		{
			strLog = "SHA204_WAKEUP;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SHA204_SLEEP:
		{
			strLog = "SHA204_SLEEP;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SHA204_EXECUTE_CMD:
		{
			strLog.Format(_T("SHA204_EXECUTE_CMD,%dBytes;"), usSize);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SHA204_GET_RESPONSE:
		{
			strLog = "SHA204_GET_RESPONSE;";
			usDataSize = 40;
			strTmp.Format(_T("0,%dBytes"), usDataSize);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_MOTION_INDEX_STATUS:
		{
			strLog = "GET_MOTION_INDEX_STATUS;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_CORRECT_GRID:
		{
			strLog = "GET_CORRECT_GRID;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_DNA_CODE:
		{
			strLog = "GET_DNA_CODE;";
			usDataSize = 8;
			lData[0] = 0;
			lData[1] = 0;
			strTmp.Format(_T("%d,%d"), lData[0], lData[1]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_SET_DNA_CODE:
		{
			strLog = "SET_DNA_CODE;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_ENCODER_POS_TRIGGER:
		{
			strLog = "GET_ENCODER_POS_TRIGGER;";
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
			long lIndex;

			lIndex = *((long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_ETAB_TRG_ENCODER,%d;"), lIndex);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_ETAB_REC_ENCODER:
		{
			long lIndex;

			lIndex = *((long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_ETAB_REC_ENCODER,%d;"), lIndex);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_SELECT_LIST:
		{
			strLog = "GET_SELECT_LIST;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_MOTION_ARC_GET_SIZE_EX:
		{
			strLog = "MOTION_ARC_GET_SIZE_EX;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_RECORD_MARK_TIME:
		{
			long lIndex;

			lIndex = *((long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_RECORD_MARK_TIME,%d;"), lIndex);
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_RECORD_AMOF_DIFF:
		{
			long lSetInterval;

			lSetInterval = *((long *)&g_ReadBuffer[6]);

			strLog.Format(_T("GET_RECORD_AMOF_DIFF,%d;"), lSetInterval);
			usDataSize = 36;
			strTmp.Format(_T("0,%dBytes"), usDataSize);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_NG_RECORD:
		{
			strLog = "GET_NG_RECORD;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_IPG_PD_READ:
		{
			strLog = "GET_IPG_PD_READ;";
			usDataSize = 4;
			lData[0] = 0;
			strTmp.Format(_T("%d"), lData[0]);
			bReplyNull = FALSE;
			break;
		}
		case CMD_GET_INT_FREQ_STATUS:
		{
			strLog = "GET_INT_FREQ_STATUS;";
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
			long lList;

			lList = *((long *)&g_ReadBuffer[6]);
			strLog.Format(_T("AUTO_CHANGE,%d;"), lList);
			break;
		}
		case CMD_AUTO_CHANGE_POS:
		{
			unsigned long start;

			start = *((unsigned long *)&g_ReadBuffer[6]);
			strLog.Format(_T("AUTO_CHANGE_POS,%d;"), start);
			break;
		}
		case CMD_DISABLE_LASER:
		{
			strLog = "DISABLE_LASER;";
			break;
		}
		case CMD_ENABLE_LASER:
		{
			strLog = "ENABLE_LASER;";
			break;
		}
		case CMD_EXECUTE_AT_POINTER:
		{
			unsigned long pointer;

			pointer = *((unsigned long *)&g_ReadBuffer[6]);
			strLog.Format(_T("EXECUTE_AT_POINTER,%d;"), pointer);
			break;
		}
		case CMD_GOTO_XY:
		{
			long xpos, ypos;

			xpos = *((long *)&g_ReadBuffer[6]);
			ypos = *((long *)&g_ReadBuffer[10]);
			strLog.Format(_T("GOTO_XY,%d,%d;"), xpos, ypos);
			break;
		}
		case CMD_HOME_POSITION:
		{
			long xhome, yhome;

			xhome = *((long *)&g_ReadBuffer[6]);
			yhome = *((long *)&g_ReadBuffer[10]);
			strLog.Format(_T("HOME_POSITION,%d,%d;"), xhome, yhome);
			break;
		}
		case CMD_LOAD_PROGRAM_FILE:
		{
			strLog = "LOAD_PROGRAM_FILE";
			break;
		}
		case CMD_QUIT_LOOP:
		{
			strLog = "QUIT_LOOP";
			break;
		}
		case CMD_RELEASE_WAIT:
		{
			strLog = "RELEASE_WAIT";
			break;
		}
		case CMD_RESTART_LIST:
		{
			strLog = "RESTART_LIST";
			break;
		}
		case CMD_SELECT_COR_TABLE:
		{
			unsigned long head_a, head_b;

			head_a = *((unsigned long *)&g_ReadBuffer[6]);
			head_b = *((unsigned long *)&g_ReadBuffer[10]);
			strLog.Format(_T("SELECT_COR_TABLE,%d,%d;"), head_a, head_b);
			break;
		}
		case CMD_SELECT_LIST:
		{
			unsigned long list;

			list = *((unsigned long *)&g_ReadBuffer[6]);
			strLog.Format(_T("SELECT_LIST,%d;"), list);
			break;
		}
		case CMD_SELECT_RTC:
		{
			strLog = "SELECT_RTC";
			break;
		}
		case CMD_SET_CONTROL_MODE:
		{
			unsigned long control_mode;

			control_mode = *((unsigned long *)&g_ReadBuffer[6]);
			strLog.Format(_T("SET_CONTROL_MODE,%d;"), control_mode);
			break;
		}
		case CMD_SET_DELAY_MODE:
		{
			unsigned long vapoly, directmove3d, edgelevel, MinJumpDelay, JumpLengthLimit;

			vapoly = *((unsigned long *)&g_ReadBuffer[6]);
			directmove3d = *((unsigned long *)&g_ReadBuffer[10]);
			edgelevel = *((unsigned long *)&g_ReadBuffer[14]);
			MinJumpDelay = *((unsigned long *)&g_ReadBuffer[18]);
			JumpLengthLimit = *((unsigned long *)&g_ReadBuffer[22]);
			strLog.Format(_T("SET_DELAY_MODE,%d,%d,%d,%d,%d;"), vapoly, directmove3d, edgelevel, MinJumpDelay, JumpLengthLimit);
			break;
		}
		case CMD_SET_INPUT_POINTER:
		{
			unsigned long pointer;

			pointer = *((unsigned long *)&g_ReadBuffer[6]);
			strLog.Format(_T("SET_INPUT_POINTER,%d;"), pointer);
			break;
		}
		case CMD_SET_LIST_MODE:
		{
			unsigned long mode;

			mode = *((unsigned long *)&g_ReadBuffer[6]);
			strLog.Format(_T("SET_LIST_MODE,%d;"), mode);
			break;
		}
		case CMD_SET_MAX_COUNT:
		{
			long max_count;

			max_count = *((long *)&g_ReadBuffer[6]);
			strLog.Format(_T("SET_MAX_COUNT,%d;"), max_count);
			break;
		}
		case CMD_SET_PISO_CONTROL:
		{
			unsigned long L1, L2;

			L1 = *((unsigned long *)&g_ReadBuffer[6]);
			L2 = *((unsigned long *)&g_ReadBuffer[10]);
			strLog.Format(_T("SET_PISO_CONTROL,%d,%d;"), L1, L2);
			break;
		}
		case CMD_SET_SOFTSTART_LEVEL:
		{
			unsigned long index, level;

			index = *((unsigned long *)&g_ReadBuffer[6]);
			level = *((unsigned long *)&g_ReadBuffer[10]);
			strLog.Format(_T("SET_SOFTSTART_LEVEL,%d,%d;"), index, level);
			break;
		}
		case CMD_SOFTSTART_MODE:
		{
			unsigned long mode, number, restartdelay;

			mode = *((unsigned long *)&g_ReadBuffer[6]);
			number = *((unsigned long *)&g_ReadBuffer[10]);
			restartdelay = *((unsigned long *)&g_ReadBuffer[14]);
			strLog.Format(_T("SOFTSTART_MODE,%d,%d,%d;"), mode, number, restartdelay);
			break;
		}
		case CMD_START_LOOP:
		{
			long lListNum;

			lListNum = *((long *)&g_ReadBuffer[6]);
			strLog.Format(_T("START_LOOP,%d;"), lListNum);
			break;
		}
		case CMD_WRITE_8BIT_PORT:
		{
			unsigned long Bit, bOn;

			Bit = *((unsigned long *)&g_ReadBuffer[6]);
			bOn = *((unsigned long *)&g_ReadBuffer[10]);
			strLog.Format(_T("WRITE_8BIT_PORT,%d,%d;"), Bit, bOn);
			break;
		}
		case CMD_WRITE_DA_X:
		{
			unsigned long ulData[2];

			ulData[0] = *((unsigned long *)&g_ReadBuffer[6]);
			ulData[1] = *((unsigned long *)&g_ReadBuffer[10]);
			strLog.Format(_T("WRITE_DA_X,%d,%d;"), ulData[0], ulData[1]);
			break;
		}
		case CMD_WRITE_IO_PORT:
		{
			unsigned long value;

			value = *((unsigned long *)&g_ReadBuffer[6]);
			strLog.Format(_T("WRITE_IO_PORT,%d;"), value);
			break;
		}
		case CMD_Z_OUT:
		{
			long value;

			value = *((long *)&g_ReadBuffer[6]);
			strLog.Format(_T("CMD_Z_OUT,%d;"), value);
			break;
		}
		case CMD_SET_FLYSPEED_UPDATE_TIME:
		{
			strLog = "SET_FLYSPEED_UPDATE_TIME";
			break;
		}
		case CMD_SET_PWM_VARPERIOD:
		{
			CString strData;
			long lSize;
			long lVarPeriod[10];

			lSize = *((long *)&g_ReadBuffer[6]);
			strLog = "SET_PWM_VARPERIOD";
			for(int i = 0;i < lSize;i++)
			{
				lVarPeriod[i] = *((long *)&g_ReadBuffer[10 + 4*i]);
				strData.Format(_T(",%d"), lVarPeriod[i]);
				strLog.Append(strData);
			}
			strData = ";";
			strLog.Append(strData);
			break;
		}
		case CMD_SET_STANDBY:
		{
			unsigned long half_period, pulse_width;

			half_period = *((unsigned long *)&g_ReadBuffer[6]);
			pulse_width = *((unsigned long *)&g_ReadBuffer[10]);
			strLog.Format(_T("SET_STANDBY,%d,%d;"), half_period, pulse_width);
			break;
		}
		case CMD_LASER_SIGNAL_ON:
		{
			unsigned long ulData;

			ulData = *((unsigned long *)&g_ReadBuffer[6]);
			strLog.Format(_T("LASER_SIGNAL_ON,%d;"), ulData);
			break;
		}
		case CMD_SET_LASER_FPK:
		{
			unsigned long fpk, lead_time;

			fpk = *((unsigned long *)&g_ReadBuffer[6]);
			lead_time = *((unsigned long *)&g_ReadBuffer[10]);
			strLog.Format(_T("SET_LASER_FPK,%d,%d;"), fpk, lead_time);
			break;
		}
		case CMD_SET_LASER_MODE:
		{
			unsigned long mode;

			mode = *((unsigned long *)&g_ReadBuffer[6]);
			strLog.Format(_T("SET_LASER_MODE,%d;"), mode);
			break;
		}
		case CMD_SET_MATRIX:
		{
			double dData[4];
			CString strData;

			strLog = "SET_MATRIX";
			for(int i = 0; i < 4; i++)
			{
				dData[i] = *((double *)&g_ReadBuffer[6 + sizeof(double)*i]);
				strData.Format(_T(",%.3f"), dData[i]);
				strLog.Append(strData);
			}
			strData = ";";
			strLog.Append(strData);
			break;
		}
		case CMD_SET_OFFSET:
		{
			long lReadData[2];
			CString strData;

			strLog = "SET_OFFSET";
			for(int i = 0; i < sizeof(lReadData)/sizeof(lReadData[0]); i++)
			{
				lReadData[i] = *((long *)&g_ReadBuffer[6 + sizeof(long)*i]);
				strData.Format(_T(",%d"), lReadData[i]);
				strLog.Append(strData);
			}
			strData = ";";
			strLog.Append(strData);
			break;
		}
		case CMD_MOTION_MOVETO:
		{
			long lReadData[2];
			CString strData;

			strLog = "MOTION_MOVETO";
			for(int i = 0; i < sizeof(lReadData)/sizeof(lReadData[0]); i++)
			{
				lReadData[i] = *((long *)&g_ReadBuffer[6 + sizeof(long)*i]);
				strData.Format(_T(",%d"), lReadData[i]);
				strLog.Append(strData);
			}
			strData = ";";
			strLog.Append(strData);
			break;
		}
		case CMD_SET_MOTION_PARAM1:
		{
			long lReadData[64];
			CString strData;

			strLog = "SET_MOTION_PARAM1";
			for(int i = 0; i < usSize/4; i++)
			{
				lReadData[i] = *((long *)&g_ReadBuffer[6 + sizeof(long)*i]);
				strData.Format(_T(",%d"), lReadData[i]);
				strLog.Append(strData);
			}
			strData = ";";
			strLog.Append(strData);
			break;
		}
		case CMD_SET_MOTION_PARAM2:
		{
			strLog = "SET_MOTION_PARAM2";
			break;
		}
		case CMD_SET_MOTION_PARAM3:
		{
			strLog = "SET_MOTION_PARAM3";
			break;
		}
		case CMD_SET_MOTION_XYTABLE:
		{
			strLog = "SET_MOTION_XYTABLE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_HOME:
		{
			strLog = "SET_MOTION_HOME";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_FLY_X:
		{
			strLog = "SET_FLY_X";
			bRead_double = TRUE;
			break;
		}
		case CMD_SET_FLY_Y:
		{
			strLog = "SET_FLY_Y";
			bRead_double = TRUE;
			break;
		}
		case CMD_SET_FLY_DELAY:
		{
			strLog = "SET_FLY_DELAY";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_SIMULATE_ENCODER:
		{
			strLog = "SET_SIMULATE_ENCODER";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_REBOOT:
		{
			strLog = "REBOOT";
			break;
		}
		case CMD_JOGSTART:
		{
			strLog = "JOGSTART";
			bRead_long = TRUE;
			break;
		}
		case CMD_JOGEND:
		{
			strLog = "JOGEND";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_BACKSPACE:
		{
			strLog = "SET_MOTION_BACKSPACE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_HOME_REAL:
		{
			strLog = "SET_MOTION_HOME_REAL";
			bRead_long = TRUE;
			break;
		}
		case CMD_RESET_ENCODER:
		{
			strLog = "RESET_ENCODER";
			bRead_long = TRUE;
			break;
		}
		case CMD_R05_CONFIG:
		{
			strLog = "R05_CONFIG";
			bRead_long = TRUE;
			break;
		}
		case CMD_R05_LEVEL:
		{
			strLog = "R05_LEVEL";
			bRead_long = TRUE;
			break;
		}
		case CMD_WRITE_IO_PORT_EX:
		{
			strLog = "WRITE_IO_PORT_EX";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_DSP_START:
		{
			strLog = "DSP_START";
			break;
		}
		case CMD_SET_PROGRAM_READY:
		{
			strLog = "SET_PROGRAM_READY";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_MARKING_READY:
		{
			strLog = "SET_MARKING_READY";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_MARKING_END:
		{
			strLog = "SET_MARKING_END";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SET_MARKING_END_TIME:
		{
			strLog = "SET_MARKING_END_TIME";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_RESET_PULSE_COUNT:
		{
			strLog = "RESET_PULSE_COUNT";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_LABEL:
		{
			strLog = "SET_LABEL";
			bRead_long = TRUE;
			break;
		}
		case CMD_GOTO_XYZ:
		{
			strLog = "GOTO_XYZ";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ENCODER_MODE:
		{
			strLog = "SET_ENCODER_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_ENCODER_COMP:
		{
			strLog = "SET_ENCODER_COMP";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_MOTION_XY:
		{
			strLog = "SET_MOTION_XY";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_CARD_CONTROL_STATUS:
		{
			strLog = "SET_CARD_CONTROL_STATUS";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_FLY_PREVIEW_MODE:
		{
			strLog = "SET_FLY_PREVIEW_MODE";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_IO_STARTSTOP:
		{
			strLog = "SET_IO_STARTSTOP";
			bRead_long = TRUE;
			break;
		}
		case CMD_EXEC_LIST:
		{
			strLog = "EXEC_LIST";
			bRead_short = TRUE;
			break;
		}
		case CMD_STOP_LIST:
		{
			strLog = "STOP_LIST";
			break;
		}
		case CMD_STOP_EXEC:
		{
			strLog = "STOP_EXEC";
			break;
		}
		case CMD_START_LIST:
		{
			strLog = "START_LIST";
			bRead_short = TRUE;
			break;
		}
		case CMD_MOTION_ARC_AXIS:
		{
			strLog = "MOTION_ARC_AXIS";
			bRead_long = TRUE;
			break;
		}
		case CMD_MOTION_ARC_LON_STEP:
		{
			strLog = "MOTION_ARC_LON_STEP";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_MOTION_ARC_CLR:
		{
			strLog = "MOTION_ARC_CLR";
			break;
		}
		case CMD_MOTION_ARC_START:
		{
			strLog = "MOTION_ARC_START";
			break;
		}
		case CMD_MOTION_ARC_ADD:
		{
			strLog = "MOTION_ARC_ADD";
			bRead_long = TRUE;
			break;
		}
		case CMD_ENCODER_MOTION:
		{
			strLog = "ENCODER_MOTION";
			break;
		}
		case CMD_SET_STARTSTOP_FILTER:
		{
			strLog = "SET_STARTSTOP_FILTER";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_IO_FILTER_LAG:
		{
			strLog = "SET_IO_FILTER_LAG";
			bRead_ulong = TRUE;
			break;
		}
		case CMD_SCAN_XY_ENABLE:
		{
			strLog = "SCAN_XY_ENABLE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LOAD_CORRECTION_FILE:
		{
			strLog = "LOAD_CORRECTION_FILE";
			bRead_long = TRUE;
			break;
		}
		case CMD_LOAD_CORRECTION_FILE_3D:
		{
			strLog = "LOAD_CORRECTION_FILE_3D";
			break;
		}
		case CMD_LOAD_VARPOLYDELAY:
		{
			strLog = "LOAD_VARPOLYDELAY";
			bRead_long = TRUE;
			break;
		}
		case CMD_WRITE_MEM:
		{
			unsigned long ulOffset;
			
			ulOffset = *(unsigned long *)&g_ReadBuffer[6];
			strLog.Format(_T("WRITE_MEM,%d,%dBytes;"), ulOffset, usSize-4);
			break;
		}
		case CMD_SET_LENS_TRANSFORM:
		{
			strLog = "SET_LENS_TRANSFORM";
			bRead_long = TRUE;
			break;
		}
		case CMD_SET_XY_SWAP:
		{

		}
		case CMD_SET_XYZ_REVERSE:
		{

		}
		case CMD_SET_MOTION_INDEX_DELAY:
		{

		}
		case CMD_SET_MOTION_SOFT_LIMIT:
		{

		}
		case CMD_SET_ENCODER_PARAM:
		{

		}
		case CMD_ENABLE_3D_MULTI_COR:
		{

		}
		case CMD_SET_Z_COMP_TABLE:
		{

		}
		case CMD_SET_ENCODER_POS_TRIGGER:
		{

		}
		case CMD_SET_MOTION_GANTRY:
		{

		}
		case CMD_SET_MOTION_IO:
		{

		}
		case CMD_SET_MOTION_POS_ERR:
		{

		}
		case CMD_MOTION_SERVO_ON:
		{

		}
		case CMD_SET_SCAN_PSO:
		{

		}
		case CMD_SET_ETAB_INTERVAL:
		{

		}
		case CMD_SET_PSO_FILTER:
		{

		}
		case CMD_SET_PSO_PWM2:
		{

		}
		case CMD_SET_PSO_LASER_MODE:
		{

		}
		case CMD_SET_ETAB_TRG_IN:
		{

		}
		case CMD_SET_ETAB_OUT:
		{

		}
		case CMD_SET_ENCODER_FLY_REF_MODE:
		{

		}
		case CMD_SET_MOTION_SCAN_PARAMS:
		{

		}
		case CMD_SET_DOTLINE_PARAMS:
		{

		}
		case CMD_SET_3D_Z_SCALE:
		{

		}
		case CMD_SET_SCAN_PSO_DIRECT:
		{

		}
		case CMD_MOTION_ARC_CLR_EX:
		{

		}
		case CMD_MOTION_ARC_START_EX:
		{

		}
		case CMD_MOTION_ARC_ADD_EX:
		{

		}
		case CMD_SET_ETAB_PREVIEW_MODE:
		{

		}
		case CMD_SET_NG_KEEP_DISTANCE:
		{

		}
		case CMD_SET_IPG_POWER_SETTING:
		{

		}
		case CMD_SET_NG_DISABLE_DISTANCE:
		{

		}
		case CMD_DISABLE_STOP:
		{

		}
		case CMD_LOAD_POWER_COR_FILE:
		{

		}
		case CMD_SET_POWER_COR_PARAMS:
		{

		}
		case CMD_SET_DIGITAL_POWER_OUT:
		{

		}
		case CMD_SET_POWER:
		{

		}
		case CMD_SET_MAXMIN_POWER_VALUE:
		{

		}
		case CMD_SET_ETAB_SPEED_POWER_CONTROL:
		{

		}
		case CMD_SET_SCAN_FEEDBACK_MODE:
		{

		}
		case CMD_SET_PSO_MODE:
		{

		}
		case CMD_SET_EXT_COR_TABLE:
		{

		}
		case CMD_SET_NG_RESET_POINT:
		{

		}
		case CMD_SET_ETAB_ENCODER_DIRECTION:
		{

		}
		case CMD_SET_MOTION_SCAN_FB_COMP:
		{

		}
		case CMD_SET_MOTION_SCAN_FB_COMP_DATA:
		{

		}
		case CMD_SET_NG_JUMP_CONDITION:
		{

		}
		case CMD_SET_MULTI_PATH_WOBBEL_PARAM:
		{

		}
		case CMD_LOAD_MULTI_PATH_WOBBEL_SEGMENT:
		{

		}
		case CMD_SET_AMOF_PREDICTION_TIME:
		{

		}
		case CMD_SET_NG2_PARAMS:
		{

		}
		case CMD_SET_ETAB_SPEED_FREQ_CONTROL:
		{

		}
		case CMD_SET_LASER_SIGNAL_LAG:
		{

		}
		case CMD_SET_ENCODER_JUMP_UPDATE:
		{

		}
		case CMD_SET_SCAN_CTRL_MODE:
		{

		}
		case CMD_SET_WOBBEL_EX:
		{

		}
		case CMD_SET_IPG_PD_CFG:
		{

		}
		case CMD_SET_IPG_PD_CMD:
		{

		}
		case CMD_SET_LASER_SHIFT:
		{

		}
		case CMD_SET_INT_FREQ_ALIGN_PARAMS:
		{

		}
		case CMD_SET_GOTO_SPEED:
		{

		}
		case CMD_ENABLE_IPG_POWER_MODE:
		{

		}
		case CMD_SET_IPG_DIGITAL_POWER:
		{

		}
		case CMD_SET_CUT_SCAN_RATE:
		{

		}
		case CMD_SET_ENCODER_MOTION:
		{

		}
		case CMD_SET_MOTION_MODEL_ENABLE:
		{

		}
		case CMD_SET_MOTION_MODEL_PARAMS:
		{

		}
		case CMD_SET_SCAN_CTRL_LASERON_KEEP:
		{

		}
		case CMD_SET_MOTION_SPEED_POWER_PARAMS:
		{

		}
		case CMD_SET_LASER_SIGNAL_DELAY:
		{

		}
		case CMD_SET_MOTION_ROUTE_PARAMS:
		{

		}
		case CMD_SET_MOTION_PSO_MODE:
		{

		}
		case CMD_SET_ROT_CENTER:
		{

		}
		case CMD_SET_MOTION_ENCODER:
		{

		}
		case CMD_SET_ECM_SK_CONFIG:
		{

		}
		case CMD_SET_ECM_SK_DATA:
		{

		}
		case CMD_SET_ECM_SK_DATA_EX:
		{

		}
		case CMD_SET_MOTION_COMP_PARAMS:
		{

		}
		case CMD_SET_MOTION_COMP_DATA:
		{

		}
		case CMD_SET_MOTION_ROUTE_AXIS_MAP:
		{

		}
		case CMD_SET_WAIT_ENCODER_TRG_PARAM:
		{

		}
		case CMD_SET_EXEC_LIST_JUMP_POS:
		{

		}
		case CMD_SET_LASER_SHIFT_TIME:
		{

		}
		case CMD_SET_AMOF_AXIS_MAP:
		{

		}
		case CMD_SET_SCAN_MODEL_MODE:
		{

		}
		case CMD_SET_SCAN_ACC_MODEL_PARAMS:
		{

		}
		case CMD_RING_BUFFER_COUNT_ADD:
		{

		}
		case CMD_SET_PWM_BURST_PARAMS:
		{

		}
		// case CMD_LIST_JUMP_TO:
		// {

		// }
		// case CMD_LIST_MARK_TO:
		// {

		// }
		// case CMD_LIST_JUMP_SPEED:
		// {

		// }
		// case CMD_LIST_MARK_SPEED:
		// {

		// }
		// case CMD_LIST_SET_LASER_DELAY:
		// {

		// }
		// case CMD_LIST_SET_SCANNER_DELAY:
		// {

		// }
		// case CMD_LIST_SET_LASER_TIMING:
		// {

		// }
		// case CMD_LIST_LASER_ON:
		// {

		// }
		// case CMD_LIST_LASER_SIGNAL_ON:
		// {

		// }
		// case CMD_LIST_SET_LASER_FPK:
		// {

		// }
		// case CMD_LIST_SET_STANDBY:
		// {

		// }
		// case CMD_LIST_ARC_ABS:
		// {

		// }
		// case CMD_LIST_ARC_REL:
		// {

		// }
		default:
		{
			strLog.Format(_T("CMD0x%x,%dBytes;"), usCmd, usSize);
			break;
		}
		}

		if(bRead_short)
		{
			short sReadData[512];
			CString strData;

			memset(sReadData, 0, sizeof(sReadData));
			for(int i = 0; i < usSize/2; i++)
			{
				sReadData[i] = *((short *)&g_ReadBuffer[6 + 2*i]);
				strData.Format(_T(",%d"), sReadData[i]);
				strLog.Append(strData);
			}
			strData = ";";
			strLog.Append(strData);
		}

		if(bRead_long)
		{
			long lReadData[256];
			CString strData;

			memset(lReadData, 0, sizeof(lReadData));
			for(int i = 0; i < usSize/4; i++)
			{
				lReadData[i] = *((long *)&g_ReadBuffer[6 + 4*i]);
				strData.Format(_T(",%d"), lReadData[i]);
				strLog.Append(strData);
			}
			strData = ";";
			strLog.Append(strData);
		}

		if(bRead_ulong)
		{
			unsigned long ulReadData[256];
			CString strData;

			memset(ulReadData, 0, sizeof(ulReadData));
			for(int i = 0; i < usSize/4; i++)
			{
				ulReadData[i] = *((unsigned long *)&g_ReadBuffer[6 + 4*i]);
				strData.Format(_T(",%d"), ulReadData[i]);
				strLog.Append(strData);
			}
			strData = ";";
			strLog.Append(strData);
		}

		if(bRead_double)
		{
			double dReadData[128];
			CString strData;

			memset(dReadData, 0, sizeof(dReadData));
			for(int i = 0; i < usSize/8; i++)
			{
				dReadData[i] = *((double *)&g_ReadBuffer[6 + sizeof(double)*i]);
				strData.Format(_T(",%.3f"), dReadData[i]);
				strLog.Append(strData);
			}
			strData = ";";
			strLog.Append(strData);
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
