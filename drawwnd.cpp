// drawwnd.cpp : implementation file
//

#include "stdafx.h"
#include "Saver.h"
#include "drawwnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

LPCTSTR CDrawWnd::m_lpszClassName = NULL;

enum {
	START_X = 220,
	START_Y = 345,
	MAX_X = START_X * 31 / 20,
	MAX_Y = START_Y + 5,
};

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd

CDrawWnd::CDrawWnd(BOOL bAutoDelete)
{
	m_bAutoDelete = bAutoDelete;

	/*m_nPos = 0;
	m_nStep = 1;*/
	//m_rgnLast.CreateRectRgn(0, 0, 0, 0);

	m_nStyle = AfxGetApp()->GetProfileInt("Config", "Style", PS_ENDCAP_ROUND | PS_JOIN_ROUND);
	//m_nWidth = AfxGetApp()->GetProfileInt("Config", "Width", 10);
	m_nSteps = AfxGetApp()->GetProfileInt("Config", "Steps", 5);
	if (m_nSteps < 1)
		m_nSteps = 5;
	m_nSpeed = AfxGetApp()->GetProfileInt("Config", "Speed", 1);
	if (m_nSpeed < 0)
		m_nSpeed = 1;
	
	m_Color = RGB(
		AfxGetApp()->GetProfileInt("Config", "ColorRed", 0),
		AfxGetApp()->GetProfileInt("Config", "ColorGreen", 255),
		AfxGetApp()->GetProfileInt("Config", "ColorBlue", 0));

	//m_logbrush.lbStyle = m_logbrushBlack.lbStyle = BS_SOLID;
	//m_logbrush.lbHatch = m_logbrushBlack.lbHatch = 0;
	//m_logbrush.lbColor = RGB(
	//        AfxGetApp()->GetProfileInt("Config", "ColorRed", 255),
	//        AfxGetApp()->GetProfileInt("Config", "ColorGreen", 0),
	//        AfxGetApp()->GetProfileInt("Config", "ColorBlue", 0));
	//m_logbrushBlack.lbColor = RGB(0, 0, 0);
}

BEGIN_MESSAGE_MAP(CDrawWnd, CWnd)
        //{{AFX_MSG_MAP(CDrawWnd)
        ON_WM_TIMER()
        ON_WM_PAINT()
        ON_WM_SIZE()
        ON_WM_CREATE()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDrawWnd::Draw(CDC& dc)
{
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap memBM;
	memBM.CreateCompatibleBitmap(&dc, MAX_X, MAX_Y);
	memDC.SelectObject(memBM);

	BITMAP bitmap;
	memBM.GetBitmap(&bitmap);
	memDC.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, &dc, m_nXstart, m_nYstart, SRCCOPY);

	CRect rect(m_nXstart, m_nYstart, m_nXstart + bitmap.bmWidth, m_nYstart + bitmap.bmHeight);
	CBrush brush(RGB(0, 0, 0));
	dc.FillRect(&rect, &brush);

	MoveNext();

	m_Tree.Render( [&](int x, int y, int z) {
		//int px = m_nXstart - (double(x - z) / double(START_X)) * m_nScale;
		//int py = m_nYstart - double(y) / double(START_Y) * m_nScale;
		memDC.SetPixelV(START_X - (x - z), START_Y - y, m_Color);
		dc.BitBlt(m_nXstart, m_nYstart, bitmap.bmWidth, bitmap.bmHeight, &memDC, 0, 0, SRCCOPY);
	});

	memBM.DeleteObject();
	memDC.DeleteDC();
}

void CDrawWnd::MoveNext()
{
	m_nXstart += m_nSteps;
	m_nYstart += m_nSteps;

	if (m_nXstart + MAX_X >= m_nWidth) {
		m_nXstart = 0;
	}
	if (m_nYstart + MAX_Y >= m_nHeight) {
		m_nYstart = 0;
	}
}

void CDrawWnd::SetSpeed(int nSpeed)
{
	KillTimer(TIMER_ID);
	m_nSpeed = nSpeed;
	VERIFY(SetTimer(TIMER_ID, 500 + 500 - m_nSpeed * 5, NULL) != 0);
}

void CDrawWnd::SetResolution(int nRes)
{
	/*nRes = (nRes < 1) ? 1 : nRes;
	if (nRes != m_nSteps)
	{
			m_nSteps = nRes;
			if (m_nPos > m_nSteps)
					m_nPos = m_nSteps;
			Invalidate();
			UpdateWindow();
	}*/
}

void CDrawWnd::SetPenWidth(int nWidth)
{
	/*if (nWidth != m_nWidth)
	{
		CRect rect;
		m_nWidth = nWidth;
		Invalidate();
		UpdateWindow();
	}*/
}

void CDrawWnd::SetColor(COLORREF cr)
{
	m_Color = cr;
	//m_logbrush.lbColor = cr;
}

void CDrawWnd::SetLineStyle(int nStyle)
{
	/*if (nStyle != m_nStyle)
	{
		m_nStyle = nStyle;
		Invalidate();
		UpdateWindow();
	}*/
}

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd message handlers

void CDrawWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID)
	{
		/*m_nPos += m_nStep;
		if (m_nPos > m_nSteps || m_nPos < 0)
		{
				m_nStep = -m_nStep;
				m_nPos += m_nStep;
		}*/
		CClientDC dc(this);
		/*CRect rect;
		GetClientRect(&rect);
		int nWidth = m_nWidth * rect.Width() + ::GetSystemMetrics(SM_CXSCREEN)/2;
		nWidth = nWidth/::GetSystemMetrics(SM_CXSCREEN);*/
		Draw(dc);
	}
	else
		CWnd::OnTimer(nIDEvent);
}

void CDrawWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	/*m_rgnLast.DeleteObject();
	m_rgnLast.CreateRectRgn(0,0,0,0);*/
	CBrush brush(RGB(0, 0, 0));
	CRect rect;
	GetClientRect(rect);
	dc.FillRect(&rect, &brush);
	//int nWidth = m_nWidth * rect.Width() + ::GetSystemMetrics(SM_CXSCREEN)/2;
	//nWidth = nWidth/::GetSystemMetrics(SM_CXSCREEN);

	//CBrush frameBrush(RGB(64, 64, 64));
	//CRect frameRect(0, 0, MAX_X, MAX_Y);
	//dc.FrameRect(&rect, &frameBrush);

	Draw(dc);
	// Do not call CWnd::OnPaint() for painting messages
}

void CDrawWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);
	m_nWidth = rect.Width();
	m_nHeight = rect.Height();

	/*m_nScale = 0xFFFF*cx/640;
	m_nHeight = cy;*/

	//m_nXres = cx;
	//m_nYres = cy;

	m_nScale = min(cx, cy);
	m_nXstart = 0;
	m_nYstart = 0;
}

int CDrawWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetSpeed(m_nSpeed);
	return 0;
}

BOOL CDrawWnd::Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect,
        CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// Register a class with no cursor
	if (m_lpszClassName == NULL)
	{
		m_lpszClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
			::LoadCursor(AfxGetResourceHandle(),
				MAKEINTRESOURCE(IDC_NULLCURSOR)));
	}

	// TODO: Add your specialized code here and/or call the base class
	return CreateEx(dwExStyle, m_lpszClassName, "", dwStyle,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(), NULL, NULL);
}

void CDrawWnd::PostNcDestroy()
{
	if (m_bAutoDelete)
		delete this;
}
