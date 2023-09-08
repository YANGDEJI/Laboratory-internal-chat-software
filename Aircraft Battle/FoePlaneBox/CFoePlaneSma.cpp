
#include"CFoePlaneSma.h"
#include"../GameConfig/GameConfig.h"
#include"../PlaneApp/CPlaneApp.h"

CFoePlaneSma::CFoePlaneSma() {
}
CFoePlaneSma::~CFoePlaneSma() {
}

void CFoePlaneSma::InitFoe() {
	/*
	m_foe;
	m_x;
	m_y;
	m_blood;
	m_showId
	*/
	::loadimage(&m_foe, L"./res/foeplanesma.jpg");
	m_x = CPlaneApp::m_rand % (BACKGROUND_WIDTH - FOESMA_WIDTH + 1);  //设定随机值
	m_y = -FOESMA_HEIGHT;

	m_blood = DEF_FOESMA_BLOOD;  //初始化血量

	m_showId = 2;

}
void CFoePlaneSma::ShowFoe() {
	::putimage(m_x, m_y, FOESMA_WIDTH, FOESMA_HEIGHT,
		&m_foe, FOESMA_WIDTH, (2 - m_showId) * FOESMA_HEIGHT, SRCPAINT);

	::putimage(m_x, m_y, FOESMA_WIDTH, FOESMA_HEIGHT,
		&m_foe, 0, (2 - m_showId) * FOESMA_HEIGHT, SRCAND);
}
bool CFoePlaneSma::IsHitPlayer(CPlayerPlane& player) {
	int x1 = player.m_x + PLAYER_WIDTH / 2;
	if (m_x <= x1 && x1 <= (m_x + FOESMA_WIDTH) &&
		m_y <= player.m_y && player.m_y <= (m_y + FOESMA_HEIGHT)
		) {
		return true;
	}

	int y1 = player.m_y + PLAYER_HEIGHT / 2;
	if (m_x <= player.m_x && player.m_x <= (m_x + FOESMA_WIDTH) &&
		m_y <= y1 && y1 <= (m_y + FOESMA_HEIGHT)
		) {
		return true;
	}

	int x2 = player.m_x + PLAYER_WIDTH;
	if (m_x <= x2 && x2 <= (m_x + FOESMA_WIDTH) &&
		m_y <= y1 && y1 <= (m_y + FOESMA_HEIGHT)
		) {
		return true;
	}

	return false;
}
bool CFoePlaneSma::IsHitGunner(CGunner* pGun) {
	int x = pGun->m_x + GUNNER_WIDTH / 2;

	if (m_x <= x && x <= (m_x + FOESMA_WIDTH) &&
		m_y <= pGun->m_y && pGun->m_y <= (m_y + FOESMA_HEIGHT)
		) {
		return true;
	}
	return false;
}