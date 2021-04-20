
// EMC6_Server.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CEMC6_ServerApp:
// See EMC6_Server.cpp for the implementation of this class
//

class CEMC6_ServerApp : public CWinAppEx
{
public:
	CEMC6_ServerApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEMC6_ServerApp theApp;