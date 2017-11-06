// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// mdvw.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "mdvw.h"
#include "MainFrm.h"

#include "mdvwDoc.h"
#include "mdvwView.h"
#include "trans.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmdvwApp

BEGIN_MESSAGE_MAP(CmdvwApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CmdvwApp::OnAppAbout)
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CmdvwApp::OnFileOpen)
	// Standard print setup command
	//ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CmdvwApp construction

CmdvwApp::CmdvwApp() : _newnm(0)
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("mdvw.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance


	_newnm = new TCHAR[MAX_PATH];
}

CmdvwApp::~CmdvwApp()
{
	if (_newnm) delete[] _newnm;
}

// The one and only CmdvwApp object

CmdvwApp theApp;


// CmdvwApp initialization

BOOL CmdvwApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("chenxiang"));
	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CmdvwDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CmdvwView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo; ParseCommandLine(cmdInfo);
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
	if (cmdInfo.m_strFileName.GetLength() > 0) {
		if (trans::isMD(cmdInfo.m_strFileName) && _newnm) {
			if (trans::tempfile(_newnm, trans::pathtitle(cmdInfo.m_strFileName))) {
				trans::trans_start(cmdInfo.m_strFileName, _newnm);
				cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
				cmdInfo.m_strFileName = _newnm;
			}
		}
		else {
			cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
		}
	}
	else {
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
		cmdInfo.m_strFileName = _T("mdvw.html");
	}

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

int CmdvwApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CmdvwApp message handlers

// App command to run the dialog
void CmdvwApp::OnAppAbout()
{
	ShellAbout(m_pMainWnd->m_hWnd, _T("About..#MarkDown Viewer"), _T("Copyright(C)2017 Hsiang Chen"), LoadIcon(IDR_MAINFRAME));
}

// CmdvwApp customization load/save methods

void CmdvwApp::PreLoadState()
{
}

void CmdvwApp::LoadCustomState()
{
}

void CmdvwApp::SaveCustomState()
{
}

// CmdvwApp message handlers

//
//
//CDocument* CmdvwApp::OpenDocumentFile(LPCTSTR lpFileName, BOOL bAddMRU)
//{
//	AfxMessageBox(lpFileName);
//
//	return CWinAppEx::OpenDocumentFile(lpFileName, bAddMRU);
//}