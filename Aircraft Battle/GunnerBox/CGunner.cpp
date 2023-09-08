#include"CGunner.h"
#include"../GameConfig/GameConfig.h"

CGunner::CGunner() {
	m_x = 0;
	m_y=0;
	m_hurt = 0;
}
CGunner::~CGunner() {}
void CGunner::InitGunner(int x, int y) {
	::loadimage(&m_gun, L"./res/gunner.jpg");
	m_x = x;
	m_y = y;
	m_hurt = DEF_GUNNER_HURT;
}
void CGunner::ShowGunner() {
	::putimage(m_x, m_y,  //��ʾ��Ŀ�괰�ڵ��ĸ�λ��
		GUNNER_WIDTH, GUNNER_HEIGHT, //��ʾ��Ŀ�괰�� ��ʾ���
		&m_gun,
		GUNNER_WIDTH, 0,   //Դͼ����ʼλ��
		SRCPAINT);   //����ͼ λ�����

	::putimage(m_x, m_y,  //��ʾ��Ŀ�괰�ڵ��ĸ�λ��
		GUNNER_WIDTH, GUNNER_HEIGHT, //��ʾ��Ŀ�괰�� ��ʾ���
		&m_gun,
		0, 0,   //Դͼ����ʼλ��
		SRCAND);   //ԭͼ λ�����
}
void CGunner::MoveGunner(int step) {
	m_y -= step;
}