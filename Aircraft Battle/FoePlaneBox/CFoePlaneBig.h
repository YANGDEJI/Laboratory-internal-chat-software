#pragma once
#include"CFoePlane.h"

class CFoePlaneBig:public CFoePlane{
public:
	CFoePlaneBig();
	~CFoePlaneBig();
public:
	virtual void InitFoe();
	virtual void ShowFoe();
	virtual bool IsHitPlayer(CPlayerPlane& player);
	virtual bool IsHitGunner(CGunner* pGun);
};