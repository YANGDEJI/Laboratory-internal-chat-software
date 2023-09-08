#pragma once
#include<easyx.h>
#include"../GunnerBox/CGunner.h"

class CPlayerPlane {
public:
	IMAGE m_player;
	IMAGE m_playerMask;
	int   m_x;
	int   m_y;
public:
	CPlayerPlane();
	~CPlayerPlane();
public:
	void InitPlayer();
	void ShowPlayer();
	void MovePlayer(int direct, int step);

	CGunner* SendGunner();

};


	





