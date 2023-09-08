#include"CBackGround.h"
#include"../GameConfig/GameConfig.h"


CBackGround::CBackGround() {
	m_x = 0;
	m_y = 0;
}
CBackGround::~CBackGround() {

}
void CBackGround::InitBackGround() {
	::loadimage(&m_back,L"./res/背景.jpg");  //相对路径，相对于工程文件
	m_x = 0;
	m_y =-BACKGROUND_HEIGHT;
}
void CBackGround::ShowBackGround() {
	::putimage(m_x, m_y, &m_back);  //贴图
}
void CBackGround::MoveBackGround(int step) {
	m_y += step;
	if (m_y >= 0) {  //判断临界条件
		m_y = -BACKGROUND_HEIGHT;
	}
}