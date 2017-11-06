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

// mdvwView.cpp : implementation of the CmdvwView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "mdvw.h"
#endif

#include "mdvwDoc.h"
#include "mdvwView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CmdvwView

IMPLEMENT_DYNCREATE(CmdvwView, CHtmlView)

BEGIN_MESSAGE_MAP(CmdvwView, CHtmlView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_COMMAND(IDM_NAV_GOBACK, &CmdvwView::OnGoBack)
	ON_COMMAND(IDM_NAV_GOFOREWARD, &CmdvwView::OnGoForward)
	ON_COMMAND(IDM_NAV_GOHOME, &CmdvwView::OnGoHome)
END_MESSAGE_MAP()

// CmdvwView construction/destruction

CmdvwView::CmdvwView()
{
	// TODO: add construction code here

}

CmdvwView::~CmdvwView()
{
}

BOOL CmdvwView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

void CmdvwView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	CmdvwDoc* doc = reinterpret_cast<CmdvwDoc*>(GetDocument());

	if (doc != NULL && lstrlen(doc->GetPathName()) > 0) {
		Navigate2(doc->GetPathName());
	}
}


// CmdvwView printing


void CmdvwView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}


// CmdvwView diagnostics

#ifdef _DEBUG
void CmdvwView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CmdvwView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CmdvwDoc* CmdvwView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmdvwDoc)));
	return (CmdvwDoc*)m_pDocument;
}
#endif //_DEBUG


// CmdvwView message handlers

void CmdvwView::OnGoBack()
{
	CHtmlView::GoBack();
}

void CmdvwView::OnGoForward()
{
	CHtmlView::GoForward();
}

void CmdvwView::OnGoHome()
{
	CHtmlView::GoHome();
}
