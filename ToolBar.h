#pragma once

#include <vector>

class ToolBar : public CDialogBar
{
	DECLARE_DYNAMIC(ToolBar)

public:
	struct BUTTON_DATA
	{
		BUTTON_DATA( CString _name, int _flag, int _groub ) :
		name( _name ), flag( _flag ), groub( _groub ) {}

		CString name;
		int flag;
		int groub;
	};

public:
	ToolBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ToolBar();

public:
	std::vector< CButton* > toolList;
	std::vector< CImage* > imgList;
	CImage toolImage;

	CString filePath;
	CString fileName;
	CString fileExt;

	CString selectedTool;

	void CheckButton( wchar_t* c );

	void OnButtonClicked();
	void NewFile();
	void OpenFile();
	void SaveFile();
	void AsSaveFile();
	void ShowWideView();
	void AboutProgram();

	CString GetSelectTool() { return selectedTool; }

	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOGBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
