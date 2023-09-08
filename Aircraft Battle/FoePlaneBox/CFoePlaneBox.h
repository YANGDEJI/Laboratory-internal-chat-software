#pragma once
#include<list>
using namespace std;
#include"CFoePlane.h"


class CFoePlaneBox {
public:
	list<CFoePlane*> m_lstFoe;
	list<CFoePlane*> m_lstBoomFoe;
public:
	CFoePlaneBox();
	~CFoePlaneBox();
public:
	void ShowAllFoePlane();
	void ShowAllBoomFoePlane();
	void MoveAllFoePlane();
};