// VC_Example.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CVC_ExampleApp:
// �� Ŭ������ ������ ���ؼ��� VC_Example.cpp�� �����Ͻʽÿ�.
//

class CVC_ExampleApp : public CWinApp
{
public:
	CVC_ExampleApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CVC_ExampleApp theApp;