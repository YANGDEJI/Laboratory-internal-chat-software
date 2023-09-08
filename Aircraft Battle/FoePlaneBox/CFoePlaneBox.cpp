#include"CFoePlaneBox.h"
#include"CFoePlaneSma.h"
#include"CFoePlaneMid.h"
#include"CFoePlaneBig.h"
#include<typeinfo>
#include"../GameConfig/GameConfig.h"

CFoePlaneBox::CFoePlaneBox() {
}
CFoePlaneBox::~CFoePlaneBox() {

	list<CFoePlane*>::iterator ite =    m_lstFoe.begin();
	while (ite != m_lstFoe.end()) {
		if (*ite) {
			delete (*ite);
			(*ite) = nullptr;
		}
		ite++;
	}
	m_lstFoe.clear();  //���ȫ���Ľڵ�

	for (CFoePlane*& pFoe : m_lstBoomFoe) {
		if (pFoe) {
			delete pFoe;
			pFoe = nullptr;
		}
	}
	m_lstBoomFoe.clear();
}

#define SHOW_LIST(LIST)\
	for (CFoePlane*& pFoe : LIST) {\
		if (pFoe) {\
			pFoe->ShowFoe();\
		}\
	}

void CFoePlaneBox::ShowAllFoePlane() {
	//for (CFoePlane*& pFoe : m_lstFoe) {//�������������ĵ��˷ɻ� �������ʾ
	//	if (pFoe) {  
	//		pFoe->ShowFoe();
	//	}
	//}
	SHOW_LIST(m_lstFoe)
}
void CFoePlaneBox::ShowAllBoomFoePlane() {
	//for (CFoePlane*& pFoe : m_lstBoomFoe) {//�������б�ը�ĵ��˷ɻ� �������ʾ
	//	if (pFoe) {
	//		pFoe->ShowFoe();
	//	}
	//}
	SHOW_LIST(m_lstBoomFoe)
}
void CFoePlaneBox::MoveAllFoePlane() {
	list<CFoePlane*>::iterator ite = m_lstFoe.begin();
	while (ite != m_lstFoe.end()) {
		if (*ite) {

			//RTTI:����ʱ����ʶ��Run-Time Type Id��

			//typeid(���ʽ);  //�õ����ʽ��������Ϣ,�ؼ��� typeid ����������һ��ʹ��

			//*ite === CFoePlane *
			//**ite === *(CFoePlane *)  == ĳ�����������
		/*	double a = 10;
			typeid(a) == typeid(int)*/
			type_info;
			if (typeid(**ite) == typeid(CFoePlaneSma)) {
				(*ite)->MoveFoe(TIMER_FOESMA_MOVE_STEP);
			}
			else if(typeid(**ite) == typeid(CFoePlaneMid)) {
				(*ite)->MoveFoe(TIMER_FOEMID_MOVE_STEP);
			}
			else if (typeid(**ite) == typeid(CFoePlaneBig)) {
				(*ite)->MoveFoe(TIMER_FOEBIG_MOVE_STEP);
			}


			//�ж��Ƿ����
			if ((*ite)->m_y > BACKGROUND_HEIGHT) {
				delete (*ite);
				(*ite) = nullptr;


				ite = m_lstFoe.erase(ite);   //ɾ��ĳ���ڵ�
				continue;
			}
		}
		ite++;
	}
}