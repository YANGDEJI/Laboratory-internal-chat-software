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
	m_lstFoe.clear();  //清空全部的节点

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
	//for (CFoePlane*& pFoe : m_lstFoe) {//遍历所有正常的敌人飞机 ，逐个显示
	//	if (pFoe) {  
	//		pFoe->ShowFoe();
	//	}
	//}
	SHOW_LIST(m_lstFoe)
}
void CFoePlaneBox::ShowAllBoomFoePlane() {
	//for (CFoePlane*& pFoe : m_lstBoomFoe) {//遍历所有爆炸的敌人飞机 ，逐个显示
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

			//RTTI:运行时类型识别（Run-Time Type Id）

			//typeid(表达式);  //得到表达式的类型信息,关键字 typeid ，可以像函数一样使用

			//*ite === CFoePlane *
			//**ite === *(CFoePlane *)  == 某个具体的子类
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


			//判断是否出界
			if ((*ite)->m_y > BACKGROUND_HEIGHT) {
				delete (*ite);
				(*ite) = nullptr;


				ite = m_lstFoe.erase(ite);   //删除某个节点
				continue;
			}
		}
		ite++;
	}
}