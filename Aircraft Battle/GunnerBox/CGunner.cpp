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
	::putimage(m_x, m_y,  //显示到目标窗口的哪个位置
		GUNNER_WIDTH, GUNNER_HEIGHT, //显示到目标窗口 显示多大
		&m_gun,
		GUNNER_WIDTH, 0,   //源图的起始位置
		SRCPAINT);   //屏蔽图 位或操作

	::putimage(m_x, m_y,  //显示到目标窗口的哪个位置
		GUNNER_WIDTH, GUNNER_HEIGHT, //显示到目标窗口 显示多大
		&m_gun,
		0, 0,   //源图的起始位置
		SRCAND);   //原图 位与操作
}
void CGunner::MoveGunner(int step) {
	m_y -= step;
}