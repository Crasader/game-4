// ToolBar.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ProjectH_MapEdit.h"
#include "ToolBar.h"
#include "NewMap.h"
#include "About.h"

#include <vector>

#define ProjectH_ID_ToolBar_New				1230
#define ProjectH_ID_ToolBar_Open			1231
#define ProjectH_ID_ToolBar_Save			1232
#define ProjectH_ID_ToolBar_AsSave			1233
#define ProjectH_ID_ToolBar_RadioButton		1240
#define ProjectH_ID_ToolBar_WideView		1241
#define ProjectH_ID_ToolBar_Help			1242


IMPLEMENT_DYNAMIC(ToolBar, CDialogBar)

ToolBar::ToolBar(CWnd* pParent /*=NULL*/)
{

}

ToolBar::~ToolBar()
{
	for ( std::vector< CButton* >::iterator iter = toolList.begin(); iter != toolList.end(); ++iter )
		delete (*iter);

	for ( std::vector< CImage* >::iterator iter = imgList.begin(); iter != imgList.end(); ++iter )
	{
		(*iter)->Destroy();
		delete (*iter);
	}
}

void ToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ToolBar, CDialogBar)
	ON_WM_CREATE()

	ON_UPDATE_COMMAND_UI(ProjectH_ID_ToolBar_New, ToolBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(ProjectH_ID_ToolBar_Open, ToolBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(ProjectH_ID_ToolBar_Save, ToolBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(ProjectH_ID_ToolBar_AsSave, ToolBar::OnUpdateCmdUI)

	ON_UPDATE_COMMAND_UI(ProjectH_ID_ToolBar_WideView, ToolBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(ProjectH_ID_ToolBar_Help, ToolBar::OnUpdateCmdUI)

	ON_BN_CLICKED( ProjectH_ID_ToolBar_New, NewFile )
	ON_BN_CLICKED( ProjectH_ID_ToolBar_Open, OpenFile )
	ON_BN_CLICKED( ProjectH_ID_ToolBar_Save, SaveFile )
	ON_BN_CLICKED( ProjectH_ID_ToolBar_AsSave, AsSaveFile )

	ON_BN_CLICKED( ProjectH_ID_ToolBar_RadioButton, OnButtonClicked )

	ON_BN_CLICKED( ProjectH_ID_ToolBar_WideView, ShowWideView )
	ON_BN_CLICKED( ProjectH_ID_ToolBar_Help, AboutProgram )
END_MESSAGE_MAP()

// ToolBar �޽��� ó�����Դϴ�.

int ToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	selectedTool = TEXT( "" );

	filePath = TEXT( "" );
	fileName = TEXT( "" );
	fileExt = TEXT( "" );

	OSVERSIONINFOEX osvi;
	ZeroMemory( &osvi, sizeof( OSVERSIONINFOEX ) );
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

	if( GetVersionEx( (OSVERSIONINFO *)&osvi ) )
	{
		osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
		GetVersionEx( (OSVERSIONINFO *)&osvi );
	}

	CString str( "BtnImage/BtnTool.bmp" );
	if ( osvi.dwMajorVersion == 6 )
		str.Replace( TEXT( "bmp" ), TEXT( "png" ) );
	
	toolImage.Load( str );

	int buttonWidth = 45;
	int buttonHeight = 45;
	int vSpace = 3;
	int hSpace = 5;
	int x = vSpace;
	int y = hSpace;

	// ��ư ������ ����
	std::vector< BUTTON_DATA > bDataVector;
	bDataVector.push_back( BUTTON_DATA( TEXT( "New" ),
										WS_VISIBLE | WS_CHILD | BS_BITMAP, ProjectH_ID_ToolBar_New ) );
	bDataVector.push_back( BUTTON_DATA( TEXT( "Open" ),
										WS_VISIBLE | WS_CHILD | BS_BITMAP, ProjectH_ID_ToolBar_Open ) );
	bDataVector.push_back( BUTTON_DATA( TEXT( "Save" ),
										WS_VISIBLE | WS_CHILD | BS_BITMAP, ProjectH_ID_ToolBar_Save ) );
	bDataVector.push_back( BUTTON_DATA( TEXT( "AsSave" ),
										WS_VISIBLE | WS_CHILD | BS_BITMAP, ProjectH_ID_ToolBar_AsSave ) );

	bDataVector.push_back( BUTTON_DATA( TEXT( "TOOL_SELECT" ),
										WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
										ProjectH_ID_ToolBar_RadioButton ) );
	bDataVector.push_back( BUTTON_DATA( TEXT( "TOOL_BRUSH" ),
										WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
										ProjectH_ID_ToolBar_RadioButton ) );
	bDataVector.push_back( BUTTON_DATA( TEXT( "TOOL_ERASER" ),
										WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
										ProjectH_ID_ToolBar_RadioButton ) );
	bDataVector.push_back( BUTTON_DATA( TEXT( "TOOL_ELEVATION_UP" ),
										WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
										ProjectH_ID_ToolBar_RadioButton ) );
	bDataVector.push_back( BUTTON_DATA( TEXT( "TOOL_ELEVATION_DOWN" ),
										WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
										ProjectH_ID_ToolBar_RadioButton ) );

	bDataVector.push_back( BUTTON_DATA( TEXT( "TOOL_WIDEVIEW" ),
										WS_VISIBLE | WS_CHILD | BS_BITMAP, ProjectH_ID_ToolBar_WideView ) );
	bDataVector.push_back( BUTTON_DATA( TEXT( "TOOL_HELP" ),
										WS_VISIBLE | WS_CHILD | BS_BITMAP, ProjectH_ID_ToolBar_Help ) );

	for ( int i = 0; i < 11; ++i )
	{
		if ( i == 4 || i == 9 ) x += 15;

		CImage* image = new CImage;
		CButton *temp = new CButton;
		temp->Create( bDataVector[i].name, bDataVector[i].flag,
							CRect( x, y, x + buttonWidth, y + buttonHeight ), this, bDataVector[i].groub );

		image->Create(39, 36, toolImage.GetBPP());
		HDC dc = image->GetDC();

		toolImage.StretchBlt(dc, 0, 0, 39, 36, i * 39, 0, 39, 36);
		image->ReleaseDC();
		temp->SetBitmap(*image);
		toolList.push_back(temp);
		imgList.push_back(image);

		x = x + buttonWidth + vSpace;
	}

	bDataVector.clear();

	CheckButton( TEXT( "TOOL_SELECT" ) );

	return 0;
}

void ToolBar::NewFile()
{
	NewMap dlg;

	// ������ ���� ��� �ʱ�ȭ
	filePath = TEXT( "" );
	fileName = TEXT( "" );
	fileExt = TEXT( "" );

	if ( dlg.DoModal() == IDOK )
	{
		CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();
		CChildView* view = (CChildView*) pFrame->GetView();

		view->Init( dlg.GetWidth(), dlg.GetHeight(), dlg.GetMapType() );
	}
}

void ToolBar::OpenFile()
{
	CFileDialog	dlg( true, TEXT( "phm" ), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, TEXT( "ProjectH Map Files( *.phm )|*.phm" ), this );

	if ( dlg.DoModal() == IDOK )
	{
		// ���� �� ������ ��θ� �����Ѵ�.
		filePath.Format( TEXT( "%s" ), dlg.GetPathName() );
		fileName.Format( TEXT( "%s" ), dlg.GetFileName() );
		fileExt.Format( TEXT( "%s" ), dlg.GetFileExt() );

		CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();
		CChildView* view = (CChildView*) pFrame->GetView();
		
		view->Load( filePath );
	}
}

void ToolBar::SaveFile()
{
	// ������ ���� ��ΰ� �ִ���
	if ( filePath == TEXT( "" ) )
	{
		AsSaveFile();	// ������ ���� ����
	}
	else
	{
		// ������ ���� ��ο� �ٽ� ����
		CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();
		CChildView* view = (CChildView*) pFrame->GetView();
		
		if ( view->Save( filePath ) )
		{
			CString str;
			str.Format( TEXT( "%s\n�����߽��ϴ�." ), filePath );
			AfxMessageBox( str );
		}
		else
		{
			AfxMessageBox( TEXT( "���忡 �����߽��ϴ�." ) );
		}
	}

}

void ToolBar::AsSaveFile()
{
	CFileDialog	dlg( false, TEXT( "phm" ), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT( "ProjectH Map Files( *.phm )|*.phm" ), this );

	if ( dlg.DoModal() == IDOK )
	{
		filePath.Format( TEXT( "%s" ), dlg.GetPathName() );
		fileName.Format( TEXT( "%s" ), dlg.GetFileName() );
		fileExt.Format( TEXT( "%s" ), dlg.GetFileExt() );

		CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();
		CChildView* view = (CChildView*) pFrame->GetView();
		
		if ( view->Save( filePath ) )
		{
			CString str;
			str.Format( TEXT( "%s\n�����߽��ϴ�." ), filePath );
			AfxMessageBox( str );
		}
		else
		{
			AfxMessageBox( TEXT( "���忡 �����߽��ϴ�." ) );
		}
	}
}

void ToolBar::OnButtonClicked()
{
	for ( std::vector< CButton* >::iterator iter = toolList.begin(); iter != toolList.end(); ++iter )
	{
		if ( (*iter)->GetDlgCtrlID() == ProjectH_ID_ToolBar_RadioButton && (*iter)->GetCheck() )
			(*iter)->GetWindowText( selectedTool );
	}
}

void ToolBar::CheckButton( wchar_t* c )
{
	// ��Ʈ�� �ӽú���
	CString temp;

	// ���߿� �Ű����� c �� ���� �̸��� �ִ��� ã�´�.
	for ( int i = 0; i != toolList.size(); ++i )
	{
		// ���� ������ ���� �̸��� ��Ʈ�� �ӽú����� �ְ� üũ�����Ѵ�.
		toolList[i]->GetWindowText( temp );
		toolList[i]->SetCheck( 0 );

		// �Ű����� c �� ���� ������ ���� �̸��� ���ٸ�
		if ( !( temp.Compare( c ) ) )
		{
			// ���õ� �� �̸��� ��������� �����ϰ�, üũ�����Ѵ�.
			toolList[i]->GetWindowText( selectedTool );
			toolList[i]->SetCheck( 1 );
		}
	}

	// �Ź躯�� c �� ��������� ���� Ʋ���ٸ� ���õ� ���� ���� ���̹Ƿ�
	if ( ( selectedTool.Compare( c ) ) )
	{
		// �⺻ ���� �����Ѵ�.
		toolList[4]->SetCheck( 1 );
		toolList[4]->GetWindowText( selectedTool );
	}
}

void ToolBar::ShowWideView()
{
	CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();
	CChildView* pView = (CChildView*) pFrame->GetView();

	pView->CreateWideView();
}

void ToolBar::AboutProgram()
{
	About a;
	a.DoModal();
}