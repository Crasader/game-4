#pragma once
#include "map.h"

class CChildView : public CWnd
{
// �����Դϴ�.
public:
	CChildView();

// Ư���Դϴ�.
public:
	CPoint selectedTilePx;		// ���õ� Ÿ�� ���� �׸� �̹��� ����
	CPoint selectedTileArr;		// ���õ� Ÿ�� �迭 �ε��� ��

	float ratio;
	bool miniMap;

	SCROLLINFO vScrollInfo;		// ���� ��ũ�� ����
	SCROLLINFO hScrollInfo;		// ���� ��ũ�� ����

	CRect clientRect;			// Ŭ���̾�Ʈ ���� ũ��
	RECT rectangle;				// �̴ϸ� �簢�� ũ��

	bool drag;					// �巡��������
	CPoint dragStart;			// �巡�� ���� ��ġ

	ProjectH::Map* map;			// �� Ŭ���� �ν��Ͻ�

// �۾��Դϴ�.
public:
	bool Init( int width = 128, int height = 128, int type = 0, char* filePath = NULL );
	void Release();

	void Render();						// ����ȭ�� �׸��� �Լ�
	void RenderAll();					// �̴Ϻ� �׸��� �Լ�
	void CreateWideView();
	void SetPosition( int x, int y );
	void SetWindowSize();
	void SetScrollBar();
	void SetMiniMapRect();

	void MiniMapOnOff() { miniMap = !miniMap; }

	void OnMousePosition();
	void OnMouse( UINT nFlags, CPoint point, bool lButtonDown = false );
	void ViewDrag( CPoint& point );
	void Select() {}
	void Brush();
	void Eraser() { map->ClearDoodad(); }
	void ElevationUP();
	void ElevationDown();

	bool Save( CString& str );
	void Load( CString& str );

private:
	int LimitHScrollPos( int sx );
	int LimitVScrollPos( int sy );

// �������Դϴ�.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CChildView();

	// ������ �޽��� �� �Լ�
protected:
//	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) { return false; }	//	return CWnd::OnEraseBkgnd(pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

