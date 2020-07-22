#pragma once
#include "afxwin.h"


// NewMap ��ȭ �����Դϴ�.

class NewMap : public CDialog
{
	DECLARE_DYNAMIC(NewMap)

public:
	NewMap(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~NewMap();


	int widthVal;
	int heightVal;
	int typeVal;

	int GetWidth() { return widthVal; }
	int GetHeight() { return heightVal; }
	int GetMapType() { return typeVal; }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = NEWMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox width;
	CComboBox height;
	CComboBox mapType;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
