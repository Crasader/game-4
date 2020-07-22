#pragma once
#include <list>
#include <vector>
#include <string>

#include "ImgResource.h"
#include "Navigation.h"
#include "BlackWindow.h"

#define GRASS_BTN_IMG		"Image/Grass.png"
#define GRASS_BTN_INFO		"Data/grassinfo.txt"

class BrushPalette : public CDialog
{
	DECLARE_DYNAMIC(BrushPalette)

public:
	BrushPalette(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~BrushPalette();

public:
	Navigation* navigation;
	BlackWindow* blackBg;

public:
	CComboBox m_ComboBox;				// Ÿ��, �Ǽ����� �з� �޺��ڽ�

	CButton m_BtnCheckAutoTiling;		// ����Ÿ�ϸ� üũ ��ư

	CImage m_TileImage;					// �ҷ��� Ÿ�� ���� �̹���
	CImage startPoint;					// StartPoint �̹���
	CImage monster;						// StartPoint �̹���

	CButton tileSelect;					// �⺻ ���콺 ��ư
	CButton tileHeightUp;				// Ÿ�� ���� �ø��� ��ư
	CButton tileHeightDown;				// Ÿ�� ���� ������ ��ư
	CButton elevationText;

	void ReleaseButtonList(bool releaseStartPoint = false);			// ��ư ����

	void OnComboBoxChange() { AdjustBtnPosition(); AdjustStartPointPos(); }			// �޺��ڽ� ����
	void OnRadioButtonClicked();		// Ÿ�� �̹��� ��ư Ŭ��

	void LoadImgAndCreateBtns();						/* ��ư���� */
	int GetSelectedIndex() { return selectedBtnIdx; }	/* ����ڰ� ������ ��ư�� �̹��� �ε��� ���� */
	int GetSelectedKey() { return selectedBtnKey; }		/* ����ڰ� ������ ��ư�� �̹��� Ű ���� */

	bool IsAutoTiling() { return !( m_BtnCheckAutoTiling.GetCheck() ); } /* ����Ÿ�ϸ� ��ư üũ���� ���� */

	bool PlayerBtnClicked() const { return playerBtnClicked; }
	int GetSelectedPlayer() const { return selectedPlayer; }

	bool MonsterBtnClicked() const { return monsterBtnClicked; }
	int GetSelectedMonster() const { return selectedMonster; }

	void Move( int x, int y );

private: /* Define Struct, enum ... */
	
	/* �̹��� ������ �����ϴ� ����ü */
	struct BtnImgInfo 
	{
		CButton *button;		/* ���� ��ư */
		unsigned int ComboIdx;	/* �޺��ڽ� �ε��� */
		unsigned int ImgIdx;	/* �̹��� �ε��� */
		unsigned int x, y;		/* ������ x, y */
		unsigned int w, h;		/* ����, ���� */
	};

private: /* Variables */
	std::list<BtnImgInfo*> Buttons;
	std::list<CImage*> buttonsImg;
	int selectedBtnIdx;						/* ����ڰ� ������ ��ư�� �̹��� �ε��� */
	int selectedBtnKey;						/* ����ڰ� ������ ��ư�� �̹��� Ű */
	
	bool playerBtnClicked;					/* Player StartPoint ��ư�� ���ÿ��� */
	int selectedPlayer;						/* ���õ� �÷��̾� */

	bool monsterBtnClicked;					/* Player StartPoint ��ư�� ���ÿ��� */
	int selectedMonster;						/* ���õ� �÷��̾� */

	std::vector<CButton*> playerBtns;		/* Player1 ~ Player8 StartPoint��ư */
	std::list<CImage*> playerBtnsImg;

	std::vector<CButton*> monsterBtns;		/* Player1 ~ Player8 StartPoint��ư */
	std::list<CImage*> monsterBtnsImg;

private: /* Methods */
	void ReadOverallImage();								/* ��ư�� ����� �̹����� ���� ��ü �̹����� �ҷ���. */
	bool ReadImgInfo(std::list<BtnImgInfo*>&);				/* �ؽ�Ʈ ���Ϸκ��� �̹��� ������ ��� �о����. */
	bool CreateButtons(std::list<BtnImgInfo*>&);			/* ��ư �̹��� ������ �������� ���� ��ư�� ����. */

	void SplitString(std::string, std::vector<std::string>&);	/* ���� �����ڷ� �Ͽ� ���ڵ��� �ɰ�. */
	bool CheckLine(std::vector<std::string>&);					/* �̹��� ������ ����� �о�鿴���� �˻�. */
	
	void AdjustBtnPosition();
	void AdjustStartPointPos();

	void LoadStartPointImage();
	void CreateStartPointBtns();

	void LoadMonsterImage();
	void CreateMonsterBtns();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMove(int x, int y);
};
