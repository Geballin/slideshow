// ssfront.h : main header file for the SSFRONT application
//

#if !defined(AFX_SSFRONT_H__A25B2FB9_2D48_47DC_9879_84D1DA26774E__INCLUDED_)
#define AFX_SSFRONT_H__A25B2FB9_2D48_47DC_9879_84D1DA26774E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// SlideshowFrontApp:
// See ssfront.cpp for the implementation of this class
//

class SlideshowFrontApp : public CWinApp
{
public:
	SlideshowFrontApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SlideshowFrontApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(SlideshowFrontApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSFRONT_H__A25B2FB9_2D48_47DC_9879_84D1DA26774E__INCLUDED_)
