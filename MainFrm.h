#pragma once
#include "ChildView.h"
#include "BrushPalette.h"
#include "ToolBar.h"
#include "WideView.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:
	CChildView* GetView() { return &m_wndView; }

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // ��Ʈ�� ������ ���Ե� ����Դϴ�. ���� protected:
	CStatusBar		m_wndStatusBar;
	CChildView		m_wndView;
	ToolBar			m_DlgToolBar;
	BrushPalette*	m_DlgBrushPalette;
	WideView		m_DlgWideView;

	CPoint			pt;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDestroy();

	void ResetBrushPallete();
};


