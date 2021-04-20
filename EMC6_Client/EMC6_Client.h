
// EMC6_Client.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CEMC6_ClientApp:
// See EMC6_Client.cpp for the implementation of this class
//

class CEMC6_ClientApp : public CWinAppEx
{
public:
	CEMC6_ClientApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEMC6_ClientApp theApp;