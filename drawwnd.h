// drawwnd.h : header file
//

#include "tree.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd window

class CDrawWnd : public CWnd, public CTree
{
	// Construction
public:
	CDrawWnd(BOOL bAutoDelete = TRUE);

	// Attributes
public:
	//CRgn m_rgnLast;
	int m_nWidth;
	int m_nSteps;
	int m_nStyle;
	int m_nSpeed;

	//int m_nPos;
	//int m_nStep;

	//int m_nXres;
	//int m_nYres;
	COLORREF m_Color;

private:
	int m_nScale;
	int m_nXstart;
	int m_nYstart;

	//LOGBRUSH m_logbrush;
	//LOGBRUSH m_logbrushBlack;
	static LPCTSTR m_lpszClassName;

public:
	void SetSpeed(int nSpeed);
	void SetResolution(int nRes);
	void SetColor(COLORREF cr);
	void SetPenWidth(int nWidth);
	void SetLineStyle(int nStyle);

	// Operations
private:
	void Draw(CDC& dc);
	virtual void tree_placePoint(int x, int y, int z);

	// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawWnd)
	virtual BOOL Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawWnd();

private:
	enum {
		TIMER_ID = 1
	};

	BOOL	m_bAutoDelete;
	HDC		m_hDC;

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrawWnd)
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
