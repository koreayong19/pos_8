// VC_Example.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CVC_ExampleApp:
// See VC_Example_Eng.cpp for the implementation of this class
//

class CVC_ExampleApp : public CWinApp
{
public:
	CVC_ExampleApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CVC_ExampleApp theApp;