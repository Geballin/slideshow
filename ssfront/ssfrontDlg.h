// ssfrontDlg.h : header file
//

#if !defined(AFX_SSFRONTDLG_H__72F0E30A_D07E_4050_9A9C_3A7A40CCFC55__INCLUDED_)
#define AFX_SSFRONTDLG_H__72F0E30A_D07E_4050_9A9C_3A7A40CCFC55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PersistentPref.h"

/////////////////////////////////////////////////////////////////////////////
// SlideshowFrontDlg dialog

class SlideshowFrontDlg : public CDialog
{
// Construction
public:
	SlideshowFrontDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(SlideshowFrontDlg)
	enum { IDD = IDD_SSFRONT_DIALOG };
	CButton	m_ckCustomRes;
	CEdit	m_edResY;
	CEdit	m_edResX;
	CButton	m_ckReady;
	CComboBox	m_cmbTran;
	CButton	m_ckUseTransitions;
	CButton	m_ckAdvance;
	CEdit	m_edPicFolder;
	CEdit	m_edAdvanceTime;
	CComboBox	m_cmbSortOrder;
	CButton	m_ckWindowed;
	CButton	m_ckSubfolders;
	CButton	m_ckSort;
	CButton	m_ckShowName;
	CButton	m_ckSortDescending;
	CButton	m_btnBrowse;
	int		m_resx;
	int		m_resy;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SlideshowFrontDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(SlideshowFrontDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnUpdateEditAdvance();
	afx_msg void OnBtnBrowse();
	afx_msg void OnCheckUsetran();
	afx_msg void OnCheckSort();
	afx_msg void OnAdvance();
	afx_msg void OnCheckReady();
	afx_msg void OnCheckCustomRes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool Launch(CString command_line);
	PersistentPref m_prefs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSFRONTDLG_H__72F0E30A_D07E_4050_9A9C_3A7A40CCFC55__INCLUDED_)
