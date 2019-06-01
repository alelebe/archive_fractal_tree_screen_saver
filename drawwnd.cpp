// drawwnd.cpp : implementation file
//

#include "stdafx.h"
#include "saver.h"
#include "drawwnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

LPCTSTR CDrawWnd::m_lpszClassName = NULL;

enum {
	START_X = 220,
	START_Y = 360,
	MAX_X = START_X * 31 / 20,
	MAX_Y = START_Y + 11,
};

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd

CDrawWnd::CDrawWnd(BOOL bAutoDelete)
{
	m_bAutoDelete = bAutoDelete;
	m_nXres = m_nYres = 0;
	m_nPoints = 0;

	/*m_nPos = 0;
	m_nStep = 1;*/

	m_nStyle = AfxGetApp()->GetProfileInt("Config", "Style", PS_ENDCAP_ROUND | PS_JOIN_ROUND);
	//m_nWidth = AfxGetApp()->GetProfileInt("Config", "Width", 10);
	
	int nSteps = AfxGetApp()->GetProfileInt("Config", "Steps", 1);
	if (nSteps <= 1)
		nSteps = 25;
	m_nXstep = m_nYstep = nSteps;

	m_nSpeed = AfxGetApp()->GetProfileInt("Config", "Speed", 100);
	if (m_nSpeed < 1)
		m_nSpeed = 100;
	
	m_color = RGB(
		AfxGetApp()->GetProfileInt("Config", "ColorRed", 0),
		AfxGetApp()->GetProfileInt("Config", "ColorGreen", 255),
		AfxGetApp()->GetProfileInt("Config", "ColorBlue", 0));

	CDC dc;
	dc.Attach(::GetDC(NULL));
	m_compBitmap.CreateCompatibleBitmap(&dc, MAX_X, MAX_Y);
}

BEGIN_MESSAGE_MAP(CDrawWnd, CWnd)
        //{{AFX_MSG_MAP(CDrawWnd)
        ON_WM_TIMER()
        ON_WM_PAINT()
        ON_WM_SIZE()
        ON_WM_CREATE()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDrawWnd::Draw(CDC& wndDC)
{
	CDC memDC;
	memDC.CreateCompatibleDC(&wndDC);
	CBitmap *pOldBitmap = memDC.SelectObject(&m_compBitmap);

	m_Tree.Render( [&](int x, int y, int z) {
		int px = START_X - (x - z);
		int py = START_Y - y;
		m_nPoints++;
		COLORREF color = (m_nPoints / 15000) % 2 ? RGB(0, 0, 0) : m_color;

		wndDC.SetPixelV(px, py, color);
		memDC.SetPixelV(px, py, color);
		wndDC.BitBlt(m_nXstart, m_nYstart, MAX_X, MAX_Y, &memDC, 0, 0, SRCPAINT);
	});

	MoveDrawing(wndDC, memDC);
	memDC.SelectObject(pOldBitmap);
}

void CDrawWnd::MoveDrawing(CDC &wndDC, CDC &memDC)
{
	int oldX = m_nXstart;
	int oldY = m_nYstart;

	m_nXstart += m_nXstep;
	m_nYstart += m_nYstep;

	if (m_nXstart + MAX_X >= m_nWidth) {
		m_nXstep = -m_nXstep;
		//m_nXstart = m_nWidth - MAX_X;
	}
	else if (m_nXstart <= 0) {
		m_nXstep = -m_nXstep;
		//m_nXstart = 0;
	}

	if (m_nYstart + MAX_Y >= m_nHeight) {
		m_nYstep = -m_nYstep;
		//m_nYstart = m_nHeight - MAX_Y;
	}
	else if (m_nYstart <= 0) {
		m_nYstep = -m_nYstep;
		//m_nYstart = 0;
	}

	wndDC.BitBlt(oldX, oldY, MAX_X, MAX_Y, &memDC, 0, 0, SRCINVERT);
	wndDC.BitBlt(m_nXstart, m_nYstart, MAX_X, MAX_Y, &memDC, 0, 0, SRCPAINT);
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
	m_color = cr;
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
	
	CBrush brush(RGB(0, 0, 0));
	CRect rect;
	GetClientRect(rect);
	dc.FillRect(&rect, &brush);
	//int nWidth = m_nWidth * rect.Width() + ::GetSystemMetrics(SM_CXSCREEN)/2;
	//nWidth = nWidth/::GetSystemMetrics(SM_CXSCREEN);

	//CBrush frameBrush(m_color);
	//CRect frameRect(m_nXstart, m_nYstart, m_nXstart + MAX_X, m_nYstart + MAX_Y);
	//dc.FrameRect(&frameRect, &frameBrush);

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

	m_nXstart = (m_nWidth - MAX_X) / 2;
	m_nYstart = (m_nHeight - MAX_Y) / 2;

	//m_nXres = cx;
	//m_nYres = cy;
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
