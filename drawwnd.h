// drawwnd.h : header file
//

#include "tree.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd window

class CDrawWnd : public CWnd
{
	// Construction
public:
	CDrawWnd(BOOL bAutoDelete = TRUE);

	// Attributes
public:
	int m_nStyle;
	int m_nSpeed;

	//int m_nPos;
	//int m_nStep;
	//int m_nWidth;

public:
	void SetSpeed(int nSpeed);
	//void SetPreview(BOOL bPreviewMode = TRUE);
	void SetResolution(int nRes);
	void SetColor(COLORREF cr);
	void SetPenWidth(int nWidth);
	void SetLineStyle(int nStyle);

	// Operations
private:
	void Draw(CDC& wndDC);
	void MoveDrawing(CDC& wndDC, CDC& memDC);
	void StretchDrawing(CDC& wndDC, CDC& memDC);

	// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawWnd)
	virtual BOOL Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
private:
	enum {
		TIMER_ID = 1
	};

	BOOL	m_bAutoDelete;
	BOOL	m_bAutoStretch;

	COLORREF m_color;
	CBitmap	 m_compBitmap;

	int m_nXstart;
	int m_nYstart;
	int m_nXstep;
	int m_nYstep;
	int m_nXres;
	int m_nYres;
	int m_nXlength;
	int m_nYlength;

	CTree m_Tree;
	int m_nPoints;

	static LPCTSTR m_lpszClassName;

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
