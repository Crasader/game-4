#pragma once
#include "afxwin.h"


// About ��ȭ �����Դϴ�.

class About : public CDialog
{
	DECLARE_DYNAMIC(About)

public:
	About(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~About();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CEdit thanksTo;
	virtual BOOL OnInitDialog();
	CEdit comment;
};
