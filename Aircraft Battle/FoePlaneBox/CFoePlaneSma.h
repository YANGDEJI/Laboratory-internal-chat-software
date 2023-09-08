#pragma once


#include"CFoePlane.h"

class CFoePlaneSma :public CFoePlane {
public:
	CFoePlaneSma();
	~CFoePlaneSma();
public:
	virtual void InitFoe();
	virtual void ShowFoe();
	virtual bool IsHitPlayer(CPlayerPlane& player);
	virtual bool IsHitGunner(CGunner* pGun);
};