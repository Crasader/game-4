// ProjectH_MapEdit.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//


#include "stdafx.h"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


#include "afxwinappex.h"
#include "ProjectH_MapEdit.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProjectH_MapEditApp

BEGIN_MESSAGE_MAP(CProjectH_MapEditApp, CWinApp)
	ON_COMMAND( ID_ProjectH_Acc_New, &CProjectH_MapEditApp::New )
	ON_COMMAND( ID_ProjectH_Acc_Open, &CProjectH_MapEditApp::Open )
	ON_COMMAND( ID_ProjectH_Acc_Save, &CProjectH_MapEditApp::Save )
	ON_COMMAND( ID_ProjectH_Acc_AsSave, &CProjectH_MapEditApp::AsSave )
	ON_COMMAND( ID_ProjectH_Acc_Select, &CProjectH_MapEditApp::Select )
	ON_COMMAND( ID_ProjectH_Acc_Brush, &CProjectH_MapEditApp::Brush )
	ON_COMMAND( ID_ProjectH_Acc_Eraser, &CProjectH_MapEditApp::Eraser )
	ON_COMMAND( ID_ProjectH_Acc_ElevationUp, &CProjectH_MapEditApp::ElevationUp )
	ON_COMMAND( ID_ProjectH_Acc_ElevationDown, &CProjectH_MapEditApp::Elevationdown )
	ON_COMMAND( ID_ProjectH_Acc_SHBrushPalette, &CProjectH_MapEditApp::SHBrushPalette )
	ON_COMMAND( ID_ProjectH_Acc_Help, &CProjectH_MapEditApp::Help )
	ON_COMMAND( ID_ProjectH_Acc_WideView, &CProjectH_MapEditApp::WideView )
	ON_COMMAND( ID_ProjectH_Acc_MiniMapOnOff, &CProjectH_MapEditApp::MinimapOnOff )
END_MESSAGE_MAP()


// CProjectH_MapEditApp ����

CProjectH_MapEditApp::CProjectH_MapEditApp()
{

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.

	

}

// ������ CProjectH_MapEditApp ��ü�Դϴ�.

CProjectH_MapEditApp theApp;


// CProjectH_MapEditApp �ʱ�ȭ

BOOL CProjectH_MapEditApp::InitInstance()
{
	/*_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );*/
	//_CrtSetBreakAlloc(494);
	//_CrtSetBreakAlloc(4454);

	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	// �� â�� ����� ���� �� �ڵ忡���� �� ������ â ��ü��
	// ���� ���� �̸� ���� ���α׷��� �� â ��ü�� �����մϴ�.
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// �������� ����� ���ҽ��� �Բ� �ε��մϴ�.
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);






	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	return TRUE;
}


// CProjectH_MapEditApp �޽��� ó����




// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CProjectH_MapEditApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CProjectH_MapEditApp �޽��� ó����




BOOL CProjectH_MapEditApp::OnIdle(LONG lCount)
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if ( directXComplete )
	{
		pFrame->GetView()->RedrawWindow();
		return true;
	}

	return CWinApp::OnIdle(lCount);
}

void CProjectH_MapEditApp::New()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.NewFile();
}

void CProjectH_MapEditApp::Open()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.OpenFile();
}

void CProjectH_MapEditApp::Save()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.SaveFile();
}

void CProjectH_MapEditApp::AsSave()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.AsSaveFile();
}

void CProjectH_MapEditApp::Select()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_SELECT" ) );
}

void CProjectH_MapEditApp::Brush()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_BRUSH" ) );
}

void CProjectH_MapEditApp::Eraser()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_ERASER" ) );
}

void CProjectH_MapEditApp::ElevationUp()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_ELEVATION_UP" ) );
}

void CProjectH_MapEditApp::Elevationdown()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_ELEVATION_DOWN" ) );
}

void CProjectH_MapEditApp::SHBrushPalette()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	BOOL b = pFrame->m_DlgBrushPalette->IsWindowVisible();
	pFrame->m_DlgBrushPalette->ShowWindow( !b );
	
	CChildView* view = (CChildView*) pFrame->GetView();
	view->SetFocus();
}

void CProjectH_MapEditApp::Help()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.AboutProgram();
}

void CProjectH_MapEditApp::WideView()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChildView* view = (CChildView*) pFrame->GetView();
	view->CreateWideView();
}

int CProjectH_MapEditApp::Run()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CWinApp::Run();
}

void CProjectH_MapEditApp::MinimapOnOff()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChildView* view = (CChildView*) pFrame->GetView();
	view->MiniMapOnOff();
}