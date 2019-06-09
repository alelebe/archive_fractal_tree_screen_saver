// drawwnd.cpp : implementation file
//

#include "stdafx.h"
#include "saver.h"
#include "drawwnd.h"
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

namespace {
	static const int TIMER_ID = 1;
}

LPCTSTR CDrawWnd::m_lpszClassName = NULL;

enum {
	START_X = 220,
	START_Y = 360,
	SIZE_X = START_X * 31 / 20,
	SIZE_Y = START_Y + 11,
};

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd::Fractal

CDrawWnd::Fractal::Fractal()
:	sizeX(SIZE_X), sizeY(SIZE_Y),
	color(RGB(255,255,255)),	//white
	points(0),
	coordX(0), coordY(0), stepX(0), stepY(0),
	stretchX(sizeX), stretchY(sizeY),
	autoScale(false), enabled(false),
	cxScreen(0), cyScreen(0)
{
}

void CDrawWnd::Fractal::resetCoord(int nXres, int nYres)
{
	enabled = true;		//enable fractal in start position...
	cxScreen = nXres;
	cyScreen = nYres;
	autoScale = (nXres < sizeX || nYres < sizeY);

	double ratio = 1.;
	if (autoScale) {
		double xRatio = double(nXres) / sizeX;
		double yRatio = double(nYres) / sizeY;

		if (xRatio < yRatio)		ratio = xRatio;
		else if (yRatio < xRatio)	ratio = yRatio;
	}

	stretchX = static_cast<int>(ratio * sizeX);
	stretchY = static_cast<int>(ratio * sizeY);
	
	//place in centre
	coordX = (nXres - stretchX) / 2;
	coordY = (nYres - stretchY) / 2;

	CDC dc;
	dc.Attach(::GetDC(NULL));
	bitmap.CreateCompatibleBitmap(&dc, sizeX, sizeY);
	blackmap.CreateCompatibleBitmap(&dc, sizeX, sizeY);
}

void CDrawWnd::Fractal::runBitBlt(DCHolder& memDCholder, DWORD dwRop)
{
	CDC& wndDC = memDCholder.getWndDC();
	CDC& memoryDC = memDCholder.getMemoryDC();

	if (autoScale) {
		wndDC.SetStretchBltMode(WHITEONBLACK);
		wndDC.StretchBlt(coordX, coordY, stretchX, stretchY, &memoryDC, 0, 0, sizeX, sizeY, dwRop);
	}
	else {
		wndDC.BitBlt(coordX, coordY, sizeX, sizeY, &memoryDC, 0, 0, dwRop);
	}
}

void CDrawWnd::Fractal::draw(CDC& wndDC)
{
	if (!enabled)
		return;

	DCHolder memDCholder(wndDC, bitmap);
	DCHolder blackDCholder(wndDC, blackmap);

	tree.Render(50, [&](int x, int y, int z) {
		int px = START_X - (x - z);
		int py = START_Y - y;
		points++;

		memDCholder.SetPixelV(px, py, color);
		blackDCholder.SetPixelV(px, py, RGB(0, 0, 0));

		runBitBlt(memDCholder, SRCPAINT);
	});
}

void CDrawWnd::Fractal::moveHide(CDC& wndDC)
{
	if (!enabled || autoScale)
		return;

	DCHolder blackDCholder(wndDC, blackmap);
	runBitBlt(blackDCholder, SRCCOPY);
}

void CDrawWnd::Fractal::moveShow(CDC& wndDC)
{
	if (!enabled || autoScale)
		return;

	DCHolder memDCholder(wndDC, bitmap);
	runBitBlt(memDCholder, SRCPAINT);
}

void CDrawWnd::Fractal::moveNext()
{
	if (!enabled || autoScale)
		return;

	coordX += stepX;
	coordY += stepY;

	if (coordX + sizeX >= cxScreen || coordX <= 0) {
		coordX -= stepX;
		stepX  = -stepX;
		coordX += stepX;
	}	
	if (coordY + sizeY >= cyScreen || coordY <= 0) {
		coordY -= stepY;
		stepY  = -stepY;
		coordY += stepY;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd

CDrawWnd::CDrawWnd(BOOL bAutoDelete)
{
	m_bAutoDelete = bAutoDelete;
	m_bAutoStretch = FALSE;

	m_nXres = m_nYres = 0;
	m_nPoints = 0;

	//m_nPos = 0;	
	m_nStyle = AfxGetApp()->GetProfileInt("Config", "Style", PS_ENDCAP_ROUND | PS_JOIN_ROUND);
	//m_nWidth = AfxGetApp()->GetProfileInt("Config", "Width", 10);
	
	int nSteps = AfxGetApp()->GetProfileInt("Config", "Steps", 1);
	if (nSteps <= 1)
		nSteps = 50;

	m_nSpeed = AfxGetApp()->GetProfileInt("Config", "Speed", 100);
	if (m_nSpeed < 1)
		m_nSpeed = 100;

	const COLORREF crColor = RGB(
		AfxGetApp()->GetProfileInt("Config", "ColorRed", 0),
		AfxGetApp()->GetProfileInt("Config", "ColorGreen", 255),
		AfxGetApp()->GetProfileInt("Config", "ColorBlue", 0));

	ResetSteps(nSteps, crColor);
}

BEGIN_MESSAGE_MAP(CDrawWnd, CWnd)
        //{{AFX_MSG_MAP(CDrawWnd)
        ON_WM_TIMER()
        ON_WM_PAINT()
        ON_WM_SIZE()
        ON_WM_CREATE()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDrawWnd::ResetSteps(int nSteps, COLORREF crColor)
{
	m_nSteps = nSteps;
	m_crColor = crColor;

	//step & directions
	m_objs[0].stepX = m_nSteps;
	m_objs[0].stepY = m_nSteps;
	m_objs[1].stepX = -m_nSteps;
	m_objs[1].stepY = m_nSteps;
	m_objs[2].stepX = -m_nSteps;
	m_objs[2].stepY = -m_nSteps;
	m_objs[3].stepX = m_nSteps;
	m_objs[3].stepY = -m_nSteps;

	for (auto& obj : m_objs) {
		obj.color = crColor;
	}
}

void CDrawWnd::ResetCoords(int nXres, int nYres)
{
	m_nXres = nXres;
	m_nYres = nYres;

	for (auto& o : m_objs) {
		o.resetCoord(m_nXres, m_nYres);
		if (o.autoScale)
			break;
	}
}

void CDrawWnd::Draw(CDC& wndDC)
{
	for (auto& o : m_objs) {
		o.draw(wndDC);
	}
	for (auto& o : m_objs) {
		o.moveHide(wndDC);
	}
	for (auto& o : m_objs) {
		o.moveNext();
	}
	for (auto& o : m_objs) {
		o.moveShow(wndDC);
	}
}

void CDrawWnd::SetSpeed(int nSpeed)
{
	KillTimer(TIMER_ID);
	
	m_nSpeed = std::min(100, nSpeed);
	VERIFY(SetTimer(TIMER_ID, 250 + 500 - m_nSpeed * 5, NULL) != 0);
}

void CDrawWnd::SetSteps(int nSteps)
{
	nSteps = (nSteps < 1) ? 1 : nSteps;
	if (nSteps != m_nSteps)
	{
		ResetSteps(nSteps, m_crColor);
		Invalidate();
		UpdateWindow();
	}
}

void CDrawWnd::SetPenWidth(int nWidth)
{
	//if (nWidth != m_nWidth)
	//{
	//	CRect rect;
	//	m_nWidth = nWidth;
	//	Invalidate();
	//	UpdateWindow();
	//}
}

void CDrawWnd::SetColor(COLORREF cr)
{
	for (auto& obj : m_objs) {
		obj.color = cr;
	}
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

	//CBrush frameBrush(m_color);
	//CRect frameRect(m_nXstart, m_nYstart, m_nXstart + SIZE_X, m_nYstart + SIZE_Y);
	//dc.FrameRect(&frameRect, &frameBrush);

	Draw(dc);
	// Do not call CWnd::OnPaint() for painting messages
}

void CDrawWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	//m_nXres = cx;
	//m_nYres = cy;
	CRect rect;
	GetClientRect(&rect);
	ResetCoords(rect.Width(), rect.Height());
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
