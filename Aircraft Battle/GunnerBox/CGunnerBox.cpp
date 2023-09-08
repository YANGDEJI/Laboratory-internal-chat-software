#include"CGunnerBox.h"
#include"../GameConfig/GameConfig.h"


CGunnerBox::CGunnerBox() {
}
CGunnerBox::~CGunnerBox() {
	list<CGunner*>::iterator ite = m_lst.begin();
	while (ite != m_lst.end()) {
		if (*ite) {  //ָ�벻Ϊ��
			
			//�����ڵ��ռ�
			delete (*ite);
			(*ite) = nullptr;
		}
		ite++;
	}
	m_lst.clear();  //���ȫ���Ľڵ�
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
			
			if ((*ite)->m_y<=-GUNNER_HEIGHT)//�Ƿ���ϱ߽�
			{
				delete (*ite);   //ɾ��������ڵ�
				(*ite) = nullptr;

				ite = m_lst.erase(ite);   //ɾ���ڵ�
				continue;
			}
		}
		ite++;
	}
}