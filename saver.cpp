// Saver.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "saver.h"
#include "drawwnd.h"
#include "saverdlg.h"
#include "saverwnd.h"
#include <string>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaverApp

BEGIN_MESSAGE_MAP(CSaverApp, CWinApp)
	//{{AFX_MSG_MAP(CSaverApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

TCHAR szConfig[]="Config";

/////////////////////////////////////////////////////////////////////////////
// CSaverApp construction

CSaverApp::CSaverApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSaverApp object

CSaverApp theApp;

BOOL MatchOption(LPTSTR lpsz, LPTSTR lpszOption)
{
	if (lpsz[0] == '-' || lpsz[0] == '/')
		lpsz++;
	if (lstrcmpi(lpsz, lpszOption) == 0)
		return TRUE;
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CSaverApp initialization

BOOL CSaverApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	SetRegistryKey("Fractal Tree Screen Savers");

	if (__argc == 1 || MatchOption(__argv[1], "c"))
		DoConfig();
	else if (MatchOption(__argv[1], "p"))
	{
		CWnd* pParent = CWnd::FromHandle((HWND)std::stoull(__argv[2]));
		ASSERT(pParent != NULL);
		CDrawWnd* pWnd = new CDrawWnd();
		CRect rect;
		pParent->GetClientRect(&rect);
		pWnd->Create(NULL, WS_VISIBLE|WS_CHILD, rect, pParent, NULL);
		m_pMainWnd = pWnd;
		return TRUE;
	}
	else if (MatchOption(__argv[1], "s"))
	{
		CSaverWnd* pWnd = new CSaverWnd;
		pWnd->Create();
		m_pMainWnd = pWnd;
		return TRUE;
	}
	return FALSE;
}

void CSaverApp::DoConfig()
{
	CSaverDlg dlg;
	dlg.m_nWidth = GetProfileInt(szConfig, "Width", 10);
	int nStyle = GetProfileInt(szConfig, "Style", 0);
	dlg.m_nCapStyle = 0;
	if ((nStyle & PS_ENDCAP_MASK) == PS_ENDCAP_SQUARE)
		dlg.m_nCapStyle = 1;
	else if ((nStyle & PS_ENDCAP_MASK) == PS_ENDCAP_FLAT)
		dlg.m_nCapStyle = 2;
	dlg.m_nJoinStyle = 0;
	if ((nStyle & PS_JOIN_MASK) == PS_JOIN_BEVEL)
		dlg.m_nJoinStyle = 1;
	else if ((nStyle & PS_JOIN_MASK) == PS_JOIN_MITER)
		dlg.m_nJoinStyle = 2;

	dlg.m_nResolution = GetProfileInt(szConfig, "Resolution", 10);
	dlg.m_nSpeed = GetProfileInt(szConfig, "Speed", 100);
	dlg.m_color = RGB(GetProfileInt(szConfig, "ColorRed", 0),
		GetProfileInt(szConfig, "ColorGreen", 255),
		GetProfileInt(szConfig, "ColorBlue", 0));
	m_pMainWnd = &dlg;
	if (dlg.DoModal() == IDOK)
	{
		WriteProfileInt(szConfig, "ColorRed", GetRValue(dlg.m_color));
		WriteProfileInt(szConfig, "ColorGreen", GetGValue(dlg.m_color));
		WriteProfileInt(szConfig, "ColorBlue", GetBValue(dlg.m_color));
		WriteProfileInt(szConfig, "Width", dlg.m_nWidth);
		int nStyle = 0;
		if (dlg.m_nCapStyle == 0)
			nStyle = PS_ENDCAP_ROUND;
		else if (dlg.m_nCapStyle == 1)
			nStyle = PS_ENDCAP_SQUARE;
		else if (dlg.m_nCapStyle == 2)
			nStyle = PS_ENDCAP_FLAT;
		if (dlg.m_nJoinStyle == 0)
			nStyle |= PS_JOIN_ROUND;
		else if (dlg.m_nJoinStyle == 1)
			nStyle |= PS_JOIN_BEVEL;
		else if (dlg.m_nJoinStyle == 2)
			nStyle |= PS_JOIN_MITER;
		WriteProfileInt(szConfig, "Style", nStyle);
		WriteProfileInt(szConfig, "Resolution", dlg.m_nResolution);
		WriteProfileInt(szConfig, "Speed", dlg.m_nSpeed);
	}
}
