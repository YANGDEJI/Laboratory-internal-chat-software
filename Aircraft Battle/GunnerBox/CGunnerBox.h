#pragma once
#include<list>
using namespace std;
#include"CGunner.h"

class CGunnerBox {
public:
	list<CGunner*> m_lst;
public:
	CGunnerBox();
	~CGunnerBox();
public:
	void ShowAllGunner();
	void MoveAllGunner(int step);
};