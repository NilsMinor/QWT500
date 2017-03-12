/*==========================================================
 Copyright (C) 2005 YOKOGAWA ELECTRIC CORPORATION

    ALL RIGHTS RESERVED BY YOKOGAWA ELECTRIC CORPORATION.
THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT
WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND/OR FITNESS FOR A PARTICULAR PURPOSE.

                            YOKOGAWA ELECTRIC CORPORATION
==========================================================*/
// WT500Demo.cpp

#include "stdafx.h"
#include "WT500Demo.h"
#include "CommDialog.h"
#include "MainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWT500DemoApp

BEGIN_MESSAGE_MAP(CWT500DemoApp, CWinApp)
	//{{AFX_MSG_MAP(CWT500DemoApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWT500DemoApp instructor

CWT500DemoApp::CWT500DemoApp()
{
}

/////////////////////////////////////////////////////////////////////////////
//CWT500DemoApp object

CWT500DemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWT500DemoApp initialize

BOOL CWT500DemoApp::InitInstance()
{
	AfxEnableControlContainer();

#ifdef _AFXDLL
//	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif
	CCommDialog commDlg;
	int nResponse = commDlg.DoModal();
    if (nResponse != IDOK)
	{
		return false;
	}
	CMainDialog mainDlg;
	nResponse = mainDlg.DoModal();
	return false;
}
