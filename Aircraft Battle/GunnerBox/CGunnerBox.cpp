#include"CGunnerBox.h"
#include"../GameConfig/GameConfig.h"


CGunnerBox::CGunnerBox() {
}
CGunnerBox::~CGunnerBox() {
	list<CGunner*>::iterator ite = m_lst.begin();
	while (ite != m_lst.end()) {
		if (*ite) {  //指针不为空
			
			//回收炮弹空间
			delete (*ite);
			(*ite) = nullptr;
		}
		ite++;
	}
	m_lst.clear();  //清空全部的节点
}
void CGunnerBox::ShowAllGunner() {
	for (CGunner* pGun : m_lst) {
		pGun ? pGun->ShowGunner() : (void)0;
	}
}
void CGunnerBox::MoveAllGunner(int step) {
	list<CGunner*>::iterator ite = m_lst.begin();
	while (ite != m_lst.end()) {
		if (*ite) {
			(*ite)->MoveGunner(step);
			
			if ((*ite)->m_y<=-GUNNER_HEIGHT)//是否出上边界
			{
				delete (*ite);   //删除出界的炮弹
				(*ite) = nullptr;

				ite = m_lst.erase(ite);   //删除节点
				continue;
			}
		}
		ite++;
	}
}