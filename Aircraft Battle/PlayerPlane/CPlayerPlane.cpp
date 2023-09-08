#include"CPlayerPlane.h"
#include"../GameConfig/GameConfig.h"

CPlayerPlane::CPlayerPlane() {
	m_x = 0;
	m_y = 0;
}
CPlayerPlane::~CPlayerPlane() {

}
void CPlayerPlane::InitPlayer() {
	::loadimage(&m_player,L"./res/playerplane.jpg");
	::loadimage(&m_playerMask, L"./res/playerplane-mask.jpg");   //����ͼ
	m_x = (BACKGROUND_WIDTH - PLAYER_WIDTH)/2;
	m_y = BACKGROUND_HEIGHT - PLAYER_HEIGHT;
}
void CPlayerPlane::ShowPlayer() {
	::putimage(m_x, m_y,&m_playerMask,SRCPAINT);  //����ͼ�� ���� �� λ�����
	::putimage(m_x, m_y, &m_player, SRCAND);     //��һ���Ľ�� �� ԭͼ�� λ����� 
}
void CPlayerPlane::MovePlayer(int direct, int step) {
	switch (direct)
	{
	case VK_UP:
	{
		if (m_y - step >= 0) {
			m_y -= step;
		}
		else {
			m_y = 0;
		}
	}
		break;
	case VK_DOWN:
	{
		if (m_y + step <= (BACKGROUND_HEIGHT - PLAYER_HEIGHT)) {
			m_y += step;
		}
		else {
			m_y = BACKGROUND_HEIGHT - PLAYER_HEIGHT;
		}
	}
	break;
	case VK_LEFT:
	{
		if (m_x - step >= 0) {
			m_x -= step;
		}
		else {
			m_x = 0;
		}
	}
	break;
	case VK_RIGHT:
	{
		if (m_x + step <= BACKGROUND_WIDTH - PLAYER_WIDTH) {
			m_x += step;
		}
		else {
			m_x = BACKGROUND_WIDTH - PLAYER_WIDTH;
		}
	}
	break;

	}
}


CGunner* CPlayerPlane::SendGunner() {

	CGunner* pGun = new CGunner;
	int x = m_x + (PLAYER_WIDTH - GUNNER_WIDTH) / 2;
	int y = m_y - GUNNER_HEIGHT;
	pGun->InitGunner(x,y);   //��ʼ���ڵ�

	return pGun;
}