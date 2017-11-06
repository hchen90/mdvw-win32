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

// mdvwView.h : interface of the CmdvwView class
//

#pragma once


class CmdvwView : public CHtmlView
{
protected: // create from serialization only
	CmdvwView();
	DECLARE_DYNCREATE(CmdvwView)

// Attributes
public:
	CmdvwDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CmdvwView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnGoBack();
	afx_msg void OnGoForward();
	afx_msg void OnGoHome();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in mdvwView.cpp
inline CmdvwDoc* CmdvwView::GetDocument() const
   { return reinterpret_cast<CmdvwDoc*>(m_pDocument); }
#endif

