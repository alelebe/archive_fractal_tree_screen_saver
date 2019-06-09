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
	int m_nSteps;
	//int m_nWidth;
	COLORREF m_crColor;

public:
	void SetSpeed(int nSpeed);
	void SetSteps(int nSteps);
	void SetColor(COLORREF cr);
	void SetPenWidth(int nWidth);
	void SetLineStyle(int nStyle);

	// Operations
private:
	void ResetSteps(int nSteps, COLORREF crColor);
	void ResetCoords(int nXres, int nYres);
	void Draw(CDC& wndDC);

	// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawWnd)
	virtual BOOL Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
private:
	BOOL	 m_bAutoDelete;
	BOOL	 m_bAutoStretch;
	//COLORREF m_color;
	int		 m_nXres;
	int		 m_nYres;

	struct Fractal {
		const int	sizeX, sizeY;	//bitmap size

		CTree		tree;
		CBitmap		bitmap;
		CBitmap		blackmap;
		COLORREF	color;
		int			points;			//number of generated points

		int			coordX, coordY, stepX, stepY;
		int			stretchX, stretchY;
		bool		autoScale, enabled;
		int			cxScreen, cyScreen;

		Fractal();
		void resetCoord(int nXres, int nYres);
		void draw(CDC& wndDC);
		void moveHide(CDC& wndDC);
		void moveNext();
		void moveShow(CDC& wndDC);

	private:
		struct DCHolder {
			DCHolder(CDC& dc, CBitmap& bitmap)
			:	wndDC(dc)
			{
				memoryDC.CreateCompatibleDC(&wndDC);
				pOldBitmap = memoryDC.SelectObject(&bitmap);
			}
			~DCHolder() {
				memoryDC.SelectObject(pOldBitmap);
			}
			BOOL SetPixelV(int x, int y, COLORREF crColor) {
				return memoryDC.SetPixelV(x, y, crColor);
			}
			CDC& getWndDC() {
				return wndDC;
			}
			CDC& getMemoryDC() {
				return memoryDC;
			}

		private:
			CDC&	wndDC;
			CDC		memoryDC;
			CBitmap*pOldBitmap;
		};

		void runBitBlt(DCHolder& holder, DWORD dwRop);
	};

	enum {
		NUM_OF_OBJECTS = 4
	};
	Fractal m_objs[NUM_OF_OBJECTS];
	int		m_nPoints;

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
