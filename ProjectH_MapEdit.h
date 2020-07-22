
// ProjectH_MapEdit.h : ProjectH_MapEdit ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CProjectH_MapEditApp:
// �� Ŭ������ ������ ���ؼ��� ProjectH_MapEdit.cpp�� �����Ͻʽÿ�.
//

class CProjectH_MapEditApp : public CWinApp
{
public:
	CProjectH_MapEditApp();

	afx_msg void New();
	afx_msg void Open();
	afx_msg void Save();
	afx_msg void AsSave();
	afx_msg void Select();
	afx_msg void Brush();
	afx_msg void Eraser();
	afx_msg void ElevationUp();
	afx_msg void Elevationdown();
	afx_msg void SHBrushPalette();
	afx_msg void WideView();
	afx_msg void Help();
	afx_msg void MinimapOnOff();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
};

extern CProjectH_MapEditApp theApp;
