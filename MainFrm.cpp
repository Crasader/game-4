// MainFrm.cpp : CMainFrame Ŭ������ ����
//
#include "stdafx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_MOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
	m_DlgBrushPalette->DestroyWindow();
	m_DlgToolBar.DestroyWindow();
	m_wndView.DestroyWindow();
	m_wndStatusBar.DestroyWindow();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if ( !m_DlgToolBar.Create( this, IDD_DIALOGBAR, CBRS_TOP, IDD_DIALOGBAR ) )
	{
		TRACE0("���̾�α� ���ٸ� ������ ���߽��ϴ�.\n");
		return -1;
	}
	m_DlgToolBar.EnableDocking( CBRS_ALIGN_TOP );
	ShowControlBar( &m_DlgToolBar, TRUE, FALSE );

	m_DlgBrushPalette = 0;

	if ( !m_DlgWideView.Create( IDD_DIALOG2, this ) )
	{
		TRACE0("���̵� �並 ������ ���߽��ϴ�.\n");
		return -1;
	}

	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("�� â�� ������ ���߽��ϴ�.\n");
		return -1;
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.hMenu = NULL;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// �� â���� ��Ŀ���� �̵��մϴ�.
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// �信�� ù° ũ���� �ش� ��ɿ� ��Ÿ������ �մϴ�.
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// �׷��� ������ �⺻ ó���մϴ�.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
void CMainFrame::OnMove(int x, int y)
{
	CFrameWnd::OnMove(x, y);

	m_DlgBrushPalette->Move( pt.x - x, pt.y - y );

	pt.x = x;
	pt.y = y;
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
}

void CMainFrame::ResetBrushPallete()
{
	delete m_DlgBrushPalette;

	m_DlgBrushPalette = new BrushPalette();

	if ( !m_DlgBrushPalette->Create( IDD_DIALOG1, this ) )
		TRACE0("�귯�� �ȷ�Ʈ�� ������ ���߽��ϴ�.\n");

	// �귯�� �ȷ�Ʈ ũ�� ����
	CRect windowRt;
	m_DlgBrushPalette->GetWindowRect( &windowRt );

	CRect clientRt;
	m_DlgBrushPalette->GetClientRect( &clientRt );

	CRect frameRt;
	GetWindowRect( &frameRt );

	CRect rt;
	rt.left = frameRt.left + 20;
	rt.top = frameRt.top + 80;
	rt.right = rt.left + windowRt.right - windowRt.left - clientRt.right + 210;
	rt.bottom = rt.top + windowRt.bottom - windowRt.top;

	m_DlgBrushPalette->MoveWindow( rt );

	pt.x = frameRt.left;
	pt.y = frameRt.top;
}