#include"CFoePlaneMid.h"
#include"../GameConfig/GameConfig.h"
#include"../PlaneApp/CPlaneApp.h"

CFoePlaneMid::CFoePlaneMid() {
}
CFoePlaneMid::~CFoePlaneMid() {
}

void CFoePlaneMid::InitFoe() {
	/*
	m_foe;
	m_x;
	m_y;
	m_blood;
	m_showId
	*/
	::loadimage(&m_foe, L"./res/foeplanemid.jpg");
	m_x = CPlaneApp::m_rand % (BACKGROUND_WIDTH - FOEMID_WIDTH + 1);  //设定随机值
	m_y = -FOEMID_HEIGHT;

	m_blood = DEF_FOEMID_BLOOD;  //初始化血量

	m_showId = 3;

}
void CFoePlaneMid::ShowFoe() {
	::putimage(m_x, m_y, FOEMID_WIDTH, FOEMID_HEIGHT,
		&m_foe, FOEMID_WIDTH, (3 - m_showId) * FOEMID_HEIGHT, SRCPAINT);

	::putimage(m_x, m_y, FOEMID_WIDTH, FOEMID_HEIGHT,
		&m_foe, 0, (3 - m_showId) * FOEMID_HEIGHT, SRCAND);
}
bool CFoePlaneMid::IsHitPlayer(CPlayerPlane& player) {
	int x1 = player.m_x + PLAYER_WIDTH / 2;
	if (m_x <= x1 && x1 <= (m_x + FOEMID_WIDTH) &&
		m_y <= player.m_y && player.m_y <= (m_y + FOEMID_HEIGHT)
		) {
		return true;
	}

	int y1 = player.m_y + PLAYER_HEIGHT / 2;
	if (m_x <= player.m_x && player.m_x <= (m_x + FOEMID_WIDTH) &&
		m_y <= y1 && y1 <= (m_y + FOEMID_HEIGHT)
		) {
		return true;
	}

	int x2 = player.m_x + PLAYER_WIDTH;
	if (m_x <= x2 && x2 <= (m_x + FOEMID_WIDTH) &&
		m_y <= y1 && y1 <= (m_y + FOEMID_HEIGHT)
		) {
		return true;
	}


	return false;
}
bool CFoePlaneMid::IsHitGunner(CGunner* pGun) {
	int x = pGun->m_x + GUNNER_WIDTH / 2;

	if (m_x <= x && x <= (m_x + FOEMID_WIDTH) &&
		m_y <= pGun->m_y && pGun->m_y <= (m_y + FOEMID_HEIGHT)
		) {
		return true;
	}
	return false;
}