#include"CBackGround.h"
#include"../GameConfig/GameConfig.h"


CBackGround::CBackGround() {
	m_x = 0;
	m_y = 0;
}
CBackGround::~CBackGround() {

}
void CBackGround::InitBackGround() {
	::loadimage(&m_back,L"./res/����.jpg");  //���·��������ڹ����ļ�
	m_x = 0;
	m_y =-BACKGROUND_HEIGHT;
}
void CBackGround::ShowBackGround() {
	::putimage(m_x, m_y, &m_back);  //��ͼ
}
void CBackGround::MoveBackGround(int step) {
	m_y += step;
	if (m_y >= 0) {  //�ж��ٽ�����
		m_y = -BACKGROUND_HEIGHT;
	}
}