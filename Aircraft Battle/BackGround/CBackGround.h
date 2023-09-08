#pragma once
#include<easyx.h>

class CBackGround {
public:
	IMAGE m_back;
	int   m_x;
	int   m_y;
public:
	CBackGround();
	~CBackGround();
public:
	void InitBackGround();
	void ShowBackGround();
	void MoveBackGround(int step);


};