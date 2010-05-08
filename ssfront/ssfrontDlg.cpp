// ssfrontDlg.cpp : implementation file
//

#include <malloc.h>
#include <errno.h>

#include "stdafx.h"
#include "ssfront.h"
#include "ssfrontDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SlideshowFrontDlg dialog

SlideshowFrontDlg::SlideshowFrontDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SlideshowFrontDlg::IDD, pParent),
	  m_prefs(HKEY_CURRENT_USER, _T("Software\\Perigee\\Slideshow"))
{
	//{{AFX_DATA_INIT(SlideshowFrontDlg)
	m_resx = 0;
	m_resy = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void SlideshowFrontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SlideshowFrontDlg)
	DDX_Control(pDX, IDC_CHECK_CUSTOM_RES, m_ckCustomRes);
	DDX_Control(pDX, IDC_EDIT_RES_Y, m_edResY);
	DDX_Control(pDX, IDC_EDIT_RES_X, m_edResX);
	DDX_Control(pDX, IDC_CHECK_READY, m_ckReady);
	DDX_Control(pDX, IDC_CMB_TRAN, m_cmbTran);
	DDX_Control(pDX, IDC_CHECK_USETRAN, m_ckUseTransitions);
	DDX_Control(pDX, IDC_ADVANCE, m_ckAdvance);
	DDX_Control(pDX, IDC_EDIT_PICFOLDER, m_edPicFolder);
	DDX_Control(pDX, IDC_EDIT_ADVANCE, m_edAdvanceTime);
	DDX_Control(pDX, IDC_CMB_SORT_ORDER, m_cmbSortOrder);
	DDX_Control(pDX, IDC_CHECK_WINDOWED, m_ckWindowed);
	DDX_Control(pDX, IDC_CHECK_SUBFOLDERS, m_ckSubfolders);
	DDX_Control(pDX, IDC_CHECK_SORT, m_ckSort);
	DDX_Control(pDX, IDC_CHECK_SHOWNAME, m_ckShowName);
	DDX_Control(pDX, IDC_CHECK_DESCENDING, m_ckSortDescending);
	DDX_Control(pDX, IDC_BTN_BROWSE, m_btnBrowse);
	DDX_Text(pDX, IDC_EDIT_RES_X, m_resx);
	DDX_Text(pDX, IDC_EDIT_RES_Y, m_resy);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(SlideshowFrontDlg, CDialog)
	//{{AFX_MSG_MAP(SlideshowFrontDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_UPDATE(IDC_EDIT_ADVANCE, OnUpdateEditAdvance)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_CHECK_USETRAN, OnCheckUsetran)
	ON_BN_CLICKED(IDC_CHECK_SORT, OnCheckSort)
	ON_BN_CLICKED(IDC_ADVANCE, OnAdvance)
	ON_BN_CLICKED(IDC_CHECK_READY, OnCheckReady)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM_RES, OnCheckCustomRes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// must match order placed in sort order combo box
static const char *sort_specs[] =
{
	"cd",
	"md",
	"n",
};

/////////////////////////////////////////////////////////////////////////////
// SlideshowFrontDlg message handlers

BOOL SlideshowFrontDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_edPicFolder.SetWindowText( m_prefs.get_string("path", "") );
	m_ckSubfolders.SetCheck( m_prefs.get_dword("recurse", BST_UNCHECKED) );

	m_ckSort.SetCheck( m_prefs.get_dword("sort", BST_CHECKED) );
	m_cmbSortOrder.SetCurSel( m_prefs.get_dword("sort_spec", 0) );
	m_ckSortDescending.SetCheck( m_prefs.get_dword("descending", BST_UNCHECKED) );

	m_ckShowName.SetCheck( m_prefs.get_dword("name", BST_UNCHECKED) );
	m_ckWindowed.SetCheck( m_prefs.get_dword("window", BST_UNCHECKED) );
	m_ckAdvance.SetCheck( m_prefs.get_dword("advance", BST_UNCHECKED) );
	
	m_ckUseTransitions.SetCheck( m_prefs.get_dword("tran", BST_UNCHECKED) );
	m_cmbTran.SelectString(-1, m_prefs.get_string("tran_effect", "fade") );

	m_edAdvanceTime.SetWindowText( m_prefs.get_string("advance_time", "4.0") );

	m_ckReady.SetCheck( m_prefs.get_dword("ready", BST_UNCHECKED) );

	m_ckCustomRes.SetCheck( m_prefs.get_dword("custom_res", BST_UNCHECKED) );
	m_resx = m_prefs.get_dword("resx", 800);
	m_resy = m_prefs.get_dword("resy", 600);
	UpdateData(FALSE);
	
	OnCheckUsetran();
	OnCheckSort();
	OnAdvance();
	OnCheckCustomRes();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void SlideshowFrontDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void SlideshowFrontDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR SlideshowFrontDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void SlideshowFrontDlg::OnOK() 
{
	UpdateData();

	CString path;
	m_edPicFolder.GetWindowText(path);
	
	// validate path
	if ( path.GetLength() == 0 )
	{
		MessageBox("Please specify a picture path.");
		return;
	}

	DWORD attr = GetFileAttributes(path);
	if ( attr == (DWORD)-1 )
	{
		MessageBox("The specified picture path does not exist.");
		return;
	}
	else if ( 0 == (attr & FILE_ATTRIBUTE_DIRECTORY) )
	{
		MessageBox("The specified picture path is not a directory.");
		return;
	}

	// validate advance time
	CString advance_time;
	m_edAdvanceTime.GetWindowText(advance_time);
	bool dec = false, ok = true;
	int dc = 0;
	for(int i = 0; i < advance_time.GetLength(); i++)
	{
		if ( advance_time[i] == '.' )
		{
			if ( !dec )
			{
				dec = true;
			}
			else
			{
				ok = false;
				break;
			}
		}
		else 
		{
			if ( isdigit(advance_time[i]) )
			{
				dc++;
			}
			else
			{
				ok = false;
				break;
			}
		}
	}
	ok = ok && (dc > 0);
	if ( !ok )
	{
		MessageBox("Invalid advance time setting.");
		return;
	}

	m_prefs.set_string("path", path);
	m_prefs.set_dword("recurse", m_ckSubfolders.GetCheck());

	m_prefs.set_dword("sort", m_ckSort.GetCheck());
	m_prefs.set_dword("sort_spec", m_cmbSortOrder.GetCurSel());
	m_prefs.set_dword("descending", m_ckSortDescending.GetCheck());

	m_prefs.set_dword("name", m_ckShowName.GetCheck());
	m_prefs.set_dword("window", m_ckWindowed.GetCheck());
	m_prefs.set_dword("ready", m_ckReady.GetCheck());

	m_prefs.set_dword("advance", m_ckAdvance.GetCheck());
	m_prefs.set_string("advance_time", advance_time);
	
	m_prefs.set_dword("custom_res", m_ckCustomRes.GetCheck());
	m_prefs.set_dword("resx", m_resx);	
	m_prefs.set_dword("resy", m_resy);

	CString tran_effect;
	m_cmbTran.GetLBText(m_cmbTran.GetCurSel(), tran_effect);
	m_prefs.set_dword("tran", m_ckUseTransitions.GetCheck());
	m_prefs.set_string("tran_effect", tran_effect);

	CString command_str;
	command_str.Format("slideshow \"%s\"", (LPCTSTR)path);

	if ( m_ckSubfolders.GetCheck() == BST_CHECKED )
	{
		command_str += " -recurse";
	}
	if ( m_ckSort.GetCheck() == BST_CHECKED )
	{
		command_str += " -sort ";
		command_str += sort_specs[ m_cmbSortOrder.GetCurSel() ];
		if ( m_ckSortDescending.GetCheck() )
			command_str += "-";
		else
			command_str += "+";
	}
	if ( m_ckShowName.GetCheck() == BST_CHECKED )
	{
		command_str += " -name";
	}
	if ( m_ckWindowed.GetCheck() == BST_CHECKED )
	{
		command_str += " -window";
	}
	if ( m_ckCustomRes.GetCheck() == BST_CHECKED )
	{
		CString wh;
		wh.Format(" -width %d -height %d", m_resx, m_resy);
		command_str += wh;
	}
	if ( m_ckAdvance.GetCheck() == BST_CHECKED )
	{
		command_str += " -advance ";
		command_str += advance_time;
	}
	if ( m_ckUseTransitions.GetCheck() == BST_CHECKED )
	{
		command_str += " -tran ";
		command_str += tran_effect;
	}
	if ( m_ckReady.GetCheck() == BST_CHECKED )
	{
		command_str += " -ready";
	}
	
	if ( Launch( command_str ) )
	{
		CDialog::OnOK();
	}
}

bool SlideshowFrontDlg::Launch(CString command_line)
{
	bool res = true;

	// Create pipes and redirect standard in and out for the child process
	SECURITY_ATTRIBUTES saAttr; 
 
	// Set the bInheritHandle flag so pipe handles are inherited. 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
 
	HANDLE hChildStderrRd, hChildStderrWr,
			hChildStdoutRd, hChildStdoutWr,
			hChildStdinRd, hChildStdinWr;
   
	// Create pipes for the child process's STDERR, STDOUT, STDIN. 
	CreatePipe(&hChildStderrRd, &hChildStderrWr, &saAttr, 0);
	CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0);
	CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0);

	// Set up members of STARTUPINFO structure. 
	STARTUPINFO siStartInfo; 

	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.hStdError = hChildStderrWr;  
	siStartInfo.hStdInput = hChildStdinRd;  
	siStartInfo.hStdOutput = hChildStdoutWr; 
	siStartInfo.dwFlags = STARTF_USESTDHANDLES;

	PROCESS_INFORMATION piInfo;

	// Start the process
	char *windows_sucks = (char *)malloc(command_line.GetLength() + 1);
	strcpy(windows_sucks, (LPCTSTR)command_line);
	bool process_created = !!CreateProcess(0, windows_sucks, &saAttr, &saAttr, TRUE, 
		DETACHED_PROCESS, NULL, NULL, &siStartInfo, &piInfo);
	free(windows_sucks);

	CloseHandle(hChildStderrWr);
	CloseHandle(hChildStdinRd);
	CloseHandle(hChildStdoutWr);

	if ( process_created )
	{
		// Pipe in data from the application's stderr 
		CString output;

		OVERLAPPED o;
		o.Offset=0;
		o.OffsetHigh=0;
		DWORD cnt, dwExitCode;
		BOOL retval; 
		char c;
		do
		{
			if(!GetExitCodeProcess(piInfo.hProcess, &dwExitCode)) break;
			if(dwExitCode != STILL_ACTIVE) break;

			retval = ReadFile(hChildStderrRd, &c, 1, &cnt, NULL);
			if (retval == 0 && cnt == 0)
				break;
			
			output += c;
		}
		while( c != '\n' );

		// Wait for the process to terminate
		WaitForSingleObject(piInfo.hProcess, INFINITE);

		// Display the error message if the return code is nonzero
		// ARRRGH: SDL ignores what main() returns and always returns zero.
		// So we'll just print whatever happened to show up on stderr, if anything.
		output.TrimLeft();
		if ( output.GetLength() > 0 )
		{
			res = false;
			MessageBox(output);
		}

		// Close handle to thread and process
		CloseHandle(piInfo.hThread);
		CloseHandle(piInfo.hProcess);
	}
	else
	{
		MessageBox("Failed to launch slideshow.exe");
		res = false;
	}

	// Clean up -- Close remaining pipes
	CloseHandle(hChildStderrRd);
	CloseHandle(hChildStdinWr);
	CloseHandle(hChildStdoutRd);

	return res;
}

void SlideshowFrontDlg::OnUpdateEditAdvance() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

void SlideshowFrontDlg::OnBtnBrowse() 
{
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	TCHAR display_name[MAX_PATH];

	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot  = NULL;
	bi.pszDisplayName = display_name;
	bi.lpszTitle = "Select a picture path";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;

	LPITEMIDLIST poodle = SHBrowseForFolder(&bi);

	if ( poodle )
	{
		TCHAR path[MAX_PATH];
		if ( SHGetPathFromIDList(poodle, path) )
		{
			m_edPicFolder.SetWindowText(path);
		}

		// clean up the shell's mess
		IMalloc *shell_malloc;
		if ( NOERROR == SHGetMalloc(&shell_malloc) )
		{
			shell_malloc->Free(poodle);
			shell_malloc->Release();
		}
	}
}

void SlideshowFrontDlg::OnCheckUsetran() 
{
	if ( BST_CHECKED == m_ckUseTransitions.GetCheck() )
	{
		m_cmbTran.EnableWindow( TRUE );
	}
	else
	{
		m_cmbTran.EnableWindow( FALSE );
	}
}

void SlideshowFrontDlg::OnCheckSort() 
{
	BOOL enable = (BST_CHECKED == m_ckSort.GetCheck());
	m_cmbSortOrder.EnableWindow(enable);
	m_ckSortDescending.EnableWindow(enable);
}

void SlideshowFrontDlg::OnAdvance() 
{
	BOOL enable = (BST_CHECKED == m_ckAdvance.GetCheck());
	m_edAdvanceTime.EnableWindow(enable);
}

void SlideshowFrontDlg::OnCheckReady() 
{
}

void SlideshowFrontDlg::OnCheckCustomRes() 
{
	BOOL enable = (BST_CHECKED == m_ckCustomRes.GetCheck());
	m_edResX.EnableWindow(enable);
	m_edResY.EnableWindow(enable);
}
