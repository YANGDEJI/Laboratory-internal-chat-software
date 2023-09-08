#include"CPlaneApp.h"
#include"../GameConfig/GameConfig.h"
#include<time.h>
#include"../FoePlaneBox/CFoePlaneBig.h"
#include"../FoePlaneBox/CFoePlaneMid.h"
#include"../FoePlaneBox/CFoePlaneSma.h"
#include<string>
using namespace std;


WND_PARAM(WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)
CREATE_OBJECT(CPlaneApp)



int CPlaneApp::m_rand = 0;


CPlaneApp::CPlaneApp() {
	m_score = 0;
}
CPlaneApp::~CPlaneApp() {

}
void CPlaneApp::On_Init() {

	srand((size_t)time(NULL));   //设定随机种子


	m_back.InitBackGround();

	m_player.InitPlayer();

	::loadimage(&m_scoreBoard, L"./res/cardboard.png");  //加载分数板图片


	SetTimer();
}

//设定定时器
void CPlaneApp::SetTimer() {
	::SetTimer(m_hWnd, TIMER_BACK_MOVE_ID, TIMER_BACK_MOVE_INTERVAL, nullptr);

	//定时检测方向键是否按下
	::SetTimer(m_hWnd, TIMER_CHECK_DIRECTION_ID, TIMER_CHECK_DIRECTION_INTERVAL, nullptr);

	//炮弹定时创建 移动
	::SetTimer(m_hWnd, TIMER_SEND_GUNNER_ID, TIMER_SEND_GUNNER_INTERVAL, nullptr);
	::SetTimer(m_hWnd, TIMER_GUNNER_MOVE_ID, TIMER_GUNNER_MOVE_INTERVAL, nullptr);
	
	
	//定时创建三种敌人飞机
	::SetTimer(m_hWnd, TIMER_CREATE_FOEBIG_ID, TIMER_CREATE_FOEBIG_INTERVAL, nullptr);
	::SetTimer(m_hWnd, TIMER_CREATE_FOEMID_ID, TIMER_CREATE_FOEMID_INTERVAL, nullptr);
	::SetTimer(m_hWnd, TIMER_CREATE_FOESMA_ID, TIMER_CREATE_FOESMA_INTERVAL, nullptr);


	//所有的敌人飞机定时移动
	::SetTimer(m_hWnd, TIMER_FOE_MOVE_ID, TIMER_FOE_MOVE_INTERVAL, nullptr);
		
	//定时检测是否碰撞（敌人飞机和 玩家飞机   敌人飞机 和 炮弹）
	::SetTimer(m_hWnd, TIMER_CHECK_HIT_ID, TIMER_CHECK_HIT_INTERVAL, nullptr);
	

	::SetTimer(m_hWnd, TIMER_CHANGE_PIC_ID, TIMER_CHANGE_PIC_INTERVAL, nullptr);
	
}


void CPlaneApp::On_Paint() {
	m_rand = rand();  //创建随机数字


	//显示背景图
	m_back.ShowBackGround();

	//显示玩家飞机
	m_player.ShowPlayer();

	//显示所有炮弹
	m_gunBox.ShowAllGunner();

	//显示所有的正常敌人飞机
	m_foeBox.ShowAllFoePlane();

	//显示所有爆炸的敌人飞机
	m_foeBox.ShowAllBoomFoePlane();


	//显示分数
	ShowScore();
}
void CPlaneApp::On_Close() {

}

void CPlaneApp::ON_WM_TIMER(WPARAM wparam, LPARAM lparam) {

	if (wparam == TIMER_BACK_MOVE_ID) {
		m_back.MoveBackGround(TIMER_BACK_MOVE_STEP);
	}
	if (TIMER_CHECK_DIRECTION_ID == wparam) {
		if (::GetAsyncKeyState(VK_LEFT)) {  //获取 VK_LEFT 按键的状态，如果是按下返回非0
			m_player.MovePlayer(VK_LEFT, TIMER_PLAYER_MOVE_STEP);
		}
		if (::GetAsyncKeyState(VK_UP)) {  //获取 VK_UP 按键的状态，如果是按下返回非0
			m_player.MovePlayer(VK_UP, TIMER_PLAYER_MOVE_STEP);
		}
		if (::GetAsyncKeyState(VK_RIGHT)) {  //获取 VK_RIGHT 按键的状态，如果是按下返回非0
			m_player.MovePlayer(VK_RIGHT, TIMER_PLAYER_MOVE_STEP);
		}
		if (::GetAsyncKeyState(VK_DOWN)) {  //获取 VK_DOWN 按键的状态，如果是按下返回非0
			m_player.MovePlayer(VK_DOWN, TIMER_PLAYER_MOVE_STEP);
		}
	}

	if (TIMER_SEND_GUNNER_ID == wparam) {  //发射炮弹
		CGunner* pGun = m_player.SendGunner();
		//盒子添加一个炮弹；
		m_gunBox.m_lst.push_back(pGun);  //添加到炮弹盒子中
	}

	if (TIMER_GUNNER_MOVE_ID == wparam) {   //移动所有炮弹
		m_gunBox.MoveAllGunner(TIMER_GUNNER_MOVE_STEP);
	}

#define CREATE_FOE(FOE)\
	CFoePlane* pFoe = new FOE;\
	pFoe->InitFoe();\
	m_foeBox.m_lstFoe.push_back(pFoe);
	
	if (TIMER_CREATE_FOEBIG_ID == wparam) {
		
		/*CFoePlane* pFoe = new CFoePlaneBig;
		pFoe->InitFoe();
		m_foeBox.m_lstFoe.push_back(pFoe);*/

		CREATE_FOE(CFoePlaneBig)
	}
	if (TIMER_CREATE_FOEMID_ID == wparam) {
		CREATE_FOE(CFoePlaneMid)
	}
	if (TIMER_CREATE_FOESMA_ID == wparam) {
		CREATE_FOE(CFoePlaneSma)
	}
#undef CREATE_FOE

	if (TIMER_FOE_MOVE_ID == wparam) {  //所有类型的敌人飞机定时移动
		m_foeBox.MoveAllFoePlane();
	}

	//定时检测是否碰撞（敌人飞机和 玩家飞机   敌人飞机 和 炮弹）
	if (TIMER_CHECK_HIT_ID == wparam) {

		bool isBoom = false;   //没有爆炸

		list<CFoePlane*>::iterator iteFoe = m_foeBox.m_lstFoe.begin();
		while (iteFoe != m_foeBox.m_lstFoe.end()) {
			isBoom = false;  //每次默认都是没爆炸
			//判断是否和玩家飞机相撞
			if ( (*iteFoe)->IsHitPlayer(m_player)  ) {
				GameOver();
				return;
			}

			//遍历炮弹，判断是否和炮弹相撞
			list<CGunner*>::iterator iteGun =  m_gunBox.m_lst.begin();
			while (iteGun != m_gunBox.m_lst.end()) {
			
				//取出每一个炮弹和敌人飞机判断是否相撞
				if ((*iteFoe)->IsHitGunner(*iteGun)) {
					
					//回收炮弹
					delete (*iteGun);
					(*iteGun) = nullptr;

					//回收链表节点
					iteGun = m_gunBox.m_lst.erase(iteGun);

					//敌人飞机掉血
					(*iteFoe)->m_blood -= DEF_GUNNER_HURT;
					if ((*iteFoe)->m_blood <= 0) {

						m_score += 1;  //加分

						isBoom = true;
						m_foeBox.m_lstBoomFoe.push_back(*iteFoe);  //添加到爆炸链表中
						iteFoe = m_foeBox.m_lstFoe.erase(iteFoe);  //删除正常的
						break;
					}
					continue;
				}
				iteGun++;
			}
			if(!isBoom)  //如果没有爆炸，iteFoe 正常++
				iteFoe++;
		}
	}


	if (TIMER_CHANGE_PIC_ID == wparam) {
		list<CFoePlane* >::iterator ite = m_foeBox.m_lstBoomFoe.begin();
		while (ite != m_foeBox.m_lstBoomFoe.end()) {
			
			(*ite)->m_showId--;  //切换图

			if ((*ite)->m_showId < 0) {  //判断是否显示完毕
				delete (*ite);    //回收爆炸的飞机
				(*ite) = nullptr;

				ite = m_foeBox.m_lstBoomFoe.erase(ite);   //回收节点
				continue;
			}
			ite++;

		}
	}
}

void CPlaneApp::ON_WM_KEYDOWN(WPARAM wparam) {
	//m_player.MovePlayer(wparam, TIMER_PLAYER_MOVE_STEP);
}


void CPlaneApp::GameOver() {
	//::KillTimer(m_hWnd, TIMER_BACK_MOVE_ID);

	::KillTimer(m_hWnd, TIMER_CHECK_DIRECTION_ID);

	::KillTimer(m_hWnd, TIMER_SEND_GUNNER_ID);
	::KillTimer(m_hWnd, TIMER_GUNNER_MOVE_ID);


	::KillTimer(m_hWnd, TIMER_CREATE_FOEBIG_ID);
	::KillTimer(m_hWnd, TIMER_CREATE_FOEMID_ID);
	::KillTimer(m_hWnd, TIMER_CREATE_FOESMA_ID);


	::KillTimer(m_hWnd, TIMER_FOE_MOVE_ID);

	::KillTimer(m_hWnd, TIMER_CHECK_HIT_ID);

	::KillTimer(m_hWnd, TIMER_CHANGE_PIC_ID);


	//弹出提示的弹出框
	::MessageBox(m_hWnd,L"GameOver!",L"提示",MB_OK);

	//让程序主动的退出,投递一个窗口关闭的消息
	::PostMessage(m_hWnd, WM_CLOSE,0,0);
}


void CPlaneApp::ShowScore() {
	//先显示一个分数板
	::putimage(0, 0, &m_scoreBoard);


	wchar_t buf[5] = { 0 };
	_itow_s(m_score, buf,10);

	wstring str = L"分数为：";
	str += buf;

	RECT rect = {0,0,100,50};

	::settextcolor(RGB(255, 0, 0));
	::setbkmode(TRANSPARENT);   //设置文字背景透明

	::drawtext(str.c_str(),&rect, DT_CENTER);   //居中显示文字
}