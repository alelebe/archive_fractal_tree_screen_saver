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

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd

CDrawWnd::CDrawWnd(BOOL bAutoDelete)
{
        m_bAutoDelete = bAutoDelete;
        /*m_nPos = 0;
        m_nStep = 1;*/
        m_rgnLast.CreateRectRgn(0,0,0,0);

        m_nWidth = AfxGetApp()->GetProfileInt("Config", "Width", 10);
        m_nSteps = AfxGetApp()->GetProfileInt("Config", "Resolution", 10);
        m_nStyle = AfxGetApp()->GetProfileInt("Config", "Style", PS_ENDCAP_ROUND|PS_JOIN_ROUND);
        if (m_nSteps < 1)
                m_nSteps = 1;
        m_logbrush.lbStyle = m_logbrushBlack.lbStyle = BS_SOLID;
        m_logbrush.lbHatch = m_logbrushBlack.lbHatch = 0;
        m_logbrush.lbColor = RGB(
                AfxGetApp()->GetProfileInt("Config", "ColorRed", 255),
                AfxGetApp()->GetProfileInt("Config", "ColorGreen", 0),
                AfxGetApp()->GetProfileInt("Config", "ColorBlue", 0));
        m_logbrushBlack.lbColor = RGB(0, 0, 0);
}

CDrawWnd::~CDrawWnd()
{
}


BEGIN_MESSAGE_MAP(CDrawWnd, CWnd)
        //{{AFX_MSG_MAP(CDrawWnd)
        ON_WM_TIMER()
        ON_WM_PAINT()
        ON_WM_SIZE()
        ON_WM_CREATE()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDrawWnd::Draw(CDC& dc, int nWidth)
{
        dc.SetPolyFillMode(WINDING);

        CPen penblack(PS_SOLID|PS_GEOMETRIC|m_nStyle, nWidth, &m_logbrushBlack);
        CPen penred(PS_SOLID|PS_GEOMETRIC|m_nStyle, nWidth, &m_logbrush);
        CBrush brushBlack(m_logbrushBlack.lbColor);
        CBrush brushRed(m_logbrush.lbColor);
        CRgn rgnNew, rgnBlack;

        CPen* pPen = dc.SelectObject(&penred);

        dc.BeginPath();

        CPoint pt(m_nXstart, m_nYstart);
        CTreeWnd tree(dc, pt, m_nScale);
        tree.Render();

        dc.EndPath();
        dc.WidenPath();
        rgnNew.CreateFromPath(&dc);

        rgnBlack.CreateRectRgn(0,0,0,0);
        rgnBlack.CombineRgn(&m_rgnLast, &rgnNew, RGN_DIFF);
        dc.FillRgn(&rgnBlack, &brushBlack);
        dc.FillRgn(&rgnNew, &brushRed);

        dc.SelectObject(pPen);
        m_rgnLast.DeleteObject();
        m_rgnLast.Attach(rgnNew.Detach());
}

void CDrawWnd::SetSpeed(int nSpeed)
{
        KillTimer(1);
        VERIFY(SetTimer(1, 50+500-nSpeed*5, NULL) != 0);
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
        if (nWidth != m_nWidth)
        {
                CRect rect;
                m_nWidth = nWidth;
                Invalidate();
                UpdateWindow();
        }
}

void CDrawWnd::SetColor(COLORREF cr)
{
        m_logbrush.lbColor = cr;
}

void CDrawWnd::SetLineStyle(int nStyle)
{
        if (nStyle != m_nStyle)
        {
                m_nStyle = nStyle;
                Invalidate();
                UpdateWindow();
        }
}

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd message handlers

void CDrawWnd::OnTimer(UINT_PTR nIDEvent)
{
        if (nIDEvent == 1)
        {
                /*m_nPos += m_nStep;
                if (m_nPos > m_nSteps || m_nPos < 0)
                {
                        m_nStep = -m_nStep;
                        m_nPos += m_nStep;
                }*/
                CClientDC dc(this);
                CRect rect;
                GetClientRect(&rect);
                int nWidth = m_nWidth * rect.Width() + ::GetSystemMetrics(SM_CXSCREEN)/2;
                nWidth = nWidth/::GetSystemMetrics(SM_CXSCREEN);
                Draw(dc, nWidth);
        }
        else
                CWnd::OnTimer(nIDEvent);
}

void CDrawWnd::OnPaint()
{
        CPaintDC dc(this); // device context for painting
        m_rgnLast.DeleteObject();
        m_rgnLast.CreateRectRgn(0,0,0,0);
        CBrush brush(RGB(0,0,0));
        CRect rect;
        GetClientRect(rect);
        dc.FillRect(&rect, &brush);
        int nWidth = m_nWidth * rect.Width() + ::GetSystemMetrics(SM_CXSCREEN)/2;
        nWidth = nWidth/::GetSystemMetrics(SM_CXSCREEN);
        Draw(dc, nWidth);
        // Do not call CWnd::OnPaint() for painting messages
}

void CDrawWnd::OnSize(UINT nType, int cx, int cy)
{
        CWnd::OnSize(nType, cx, cy);

        /*m_nScale = 0xFFFF*cx/640;
        m_nHeight = cy;*/

        //m_nXres = cx;
        //m_nYres = cy;

        m_nScale  = min(cx, cy);
        m_nXstart = cx/2;
        m_nYstart = cy;
}

int CDrawWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
        if (CWnd::OnCreate(lpCreateStruct) == -1)
                return -1;

        int nSpeed = AfxGetApp()->GetProfileInt("Config", "Speed", 1);
        if (nSpeed < 0)
                nSpeed = 0;
        SetSpeed(nSpeed);

        return 0;
}

BOOL CDrawWnd::Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect,
        CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    // Register a class with no cursor
        if (m_lpszClassName == NULL)
        {
        m_lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,
                        ::LoadCursor(AfxGetResourceHandle(),
                        MAKEINTRESOURCE(IDC_NULLCURSOR)));
        }

        // TODO: Add your specialized code here and/or call the base class
        return CreateEx(dwExStyle, m_lpszClassName, "", dwStyle,
                rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                pParentWnd->GetSafeHwnd(), NULL, NULL );
}

void CDrawWnd::PostNcDestroy()
{
        if (m_bAutoDelete)
                delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CTreeWnd

CTreeWnd::CTreeWnd(CDC& dc, CPoint& pt, int scale) :
   dc(dc), pt(pt), scale(scale)
{
}

CTreeWnd::~CTreeWnd()
{
}

void CTreeWnd::put(double x, double y)
{
   dc.SetPixelV(pt.x - x*scale, pt.y - y*scale, RGB(0x0, 0xFF, 0x0));
}

