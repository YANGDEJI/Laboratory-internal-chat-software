#include"CFoePlaneBig.h"
#include"../GameConfig/GameConfig.h"
#include"../PlaneApp/CPlaneApp.h"


CFoePlaneBig::CFoePlaneBig() {
}
CFoePlaneBig::~CFoePlaneBig() {
}

void CFoePlaneBig::InitFoe() {
	/*
	m_foe;
	m_x;
	m_y;
	m_blood;
	m_showId
	*/
	::loadimage(&m_foe,L"./res/foeplanebig.jpg");
	//m_x = rand() % (BACKGROUND_WIDTH - FOEBIG_WIDTH + 1);  //设定随机值
	m_x = CPlaneApp::m_rand % (BACKGROUND_WIDTH - FOEBIG_WIDTH + 1);  //设定随机值
	

	m_y = -FOEBIG_HEIGHT;

	m_blood = DEF_FOEBIG_BLOOD;  //初始化血量

	m_showId = 4;

}
void CFoePlaneBig::ShowFoe() {
	::putimage(m_x, m_y, FOEBIG_WIDTH, FOEBIG_HEIGHT,
		&m_foe, FOEBIG_WIDTH,(4 - m_showId)* FOEBIG_HEIGHT,SRCPAINT); 

	::putimage(m_x, m_y, FOEBIG_WIDTH, FOEBIG_HEIGHT,
		&m_foe, 0, (4 - m_showId) * FOEBIG_HEIGHT, SRCAND);
}
bool CFoePlaneBig::IsHitPlayer(CPlayerPlane& player) {
	int x1 = player.m_x + PLAYER_WIDTH / 2;
	if (m_x <= x1 && x1 <= (m_x + FOEBIG_WIDTH) &&
		m_y <= player.m_y && player.m_y <= (m_y + FOEBIG_HEIGHT)
		) {
		return true;
	}

	int y1 = player.m_y + PLAYER_HEIGHT / 2;
	if (m_x <= player.m_x && player.m_x <= (m_x + FOEBIG_WIDTH) &&
		m_y <= y1 && y1 <= (m_y + FOEBIG_HEIGHT)
		) {
		return true;
	}

	int x2 = player.m_x + PLAYER_WIDTH;
	if (m_x <= x2 && x2 <= (m_x + FOEBIG_WIDTH) &&
		m_y <= y1 && y1 <= (m_y + FOEBIG_HEIGHT)
		) {
		return true;
	}
	return false;
}
bool CFoePlaneBig::IsHitGunner(CGunner* pGun) {
	int x = pGun->m_x + GUNNER_WIDTH / 2;

	if (m_x <= x  && x<=  (m_x+ FOEBIG_WIDTH)&& 
		m_y <= pGun->m_y&& pGun->m_y<= (m_y+ FOEBIG_HEIGHT)
		) {
		return true;
	}
	return false;
}