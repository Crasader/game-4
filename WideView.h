#pragma once


// WideView ��ȭ �����Դϴ�.

class WideView : public CDialog
{
	DECLARE_DYNAMIC(WideView)

public:
	WideView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~WideView();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
