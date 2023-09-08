#pragma once
// ../ Ѱ���ϲ�Ŀ¼  ��  ./  ͬ��Ŀ¼
#include"../GameFrame/GameFrame.h"
#include"../BackGround/CBackGround.h"
#include"../PlayerPlane/CPlayerPlane.h"
#include"../GunnerBox/CGunnerBox.h"
#include"../FoePlaneBox/CFoePlaneBox.h"

class CPlaneApp:public CGameFrame {
public:
	CBackGround m_back;
	CPlayerPlane m_player;
	CGunnerBox m_gunBox;

	CFoePlaneBox m_foeBox;  //���˷ɻ�����

	int m_score;
	IMAGE m_scoreBoard;

	static int m_rand;

public:
	CPlaneApp();
	~CPlaneApp();
public:
	virtual void On_Init();
	virtual void On_Paint();
	virtual void On_Close();

	virtual void ON_WM_TIMER(WPARAM wparam, LPARAM lparam);
	virtual void ON_WM_KEYDOWN(WPARAM wparam);  //��������

	void SetTimer();
	void GameOver();
	void ShowScore();
};