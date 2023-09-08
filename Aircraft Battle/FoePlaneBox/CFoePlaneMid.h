#pragma once

#include"CFoePlane.h"

class CFoePlaneMid :public CFoePlane {
public:
	CFoePlaneMid();
	~CFoePlaneMid();
public:
	virtual void InitFoe();
	virtual void ShowFoe();
	virtual bool IsHitPlayer(CPlayerPlane& player);
	virtual bool IsHitGunner(CGunner* pGun);
};