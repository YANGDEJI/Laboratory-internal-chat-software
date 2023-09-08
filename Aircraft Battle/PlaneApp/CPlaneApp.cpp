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

	srand((size_t)time(NULL));   //�趨�������


	m_back.InitBackGround();

	m_player.InitPlayer();

	::loadimage(&m_scoreBoard, L"./res/cardboard.png");  //���ط�����ͼƬ


	SetTimer();
}

//�趨��ʱ��
void CPlaneApp::SetTimer() {
	::SetTimer(m_hWnd, TIMER_BACK_MOVE_ID, TIMER_BACK_MOVE_INTERVAL, nullptr);

	//��ʱ��ⷽ����Ƿ���
	::SetTimer(m_hWnd, TIMER_CHECK_DIRECTION_ID, TIMER_CHECK_DIRECTION_INTERVAL, nullptr);

	//�ڵ���ʱ���� �ƶ�
	::SetTimer(m_hWnd, TIMER_SEND_GUNNER_ID, TIMER_SEND_GUNNER_INTERVAL, nullptr);
	::SetTimer(m_hWnd, TIMER_GUNNER_MOVE_ID, TIMER_GUNNER_MOVE_INTERVAL, nullptr);
	
	
	//��ʱ�������ֵ��˷ɻ�
	::SetTimer(m_hWnd, TIMER_CREATE_FOEBIG_ID, TIMER_CREATE_FOEBIG_INTERVAL, nullptr);
	::SetTimer(m_hWnd, TIMER_CREATE_FOEMID_ID, TIMER_CREATE_FOEMID_INTERVAL, nullptr);
	::SetTimer(m_hWnd, TIMER_CREATE_FOESMA_ID, TIMER_CREATE_FOESMA_INTERVAL, nullptr);


	//���еĵ��˷ɻ���ʱ�ƶ�
	::SetTimer(m_hWnd, TIMER_FOE_MOVE_ID, TIMER_FOE_MOVE_INTERVAL, nullptr);
		
	//��ʱ����Ƿ���ײ�����˷ɻ��� ��ҷɻ�   ���˷ɻ� �� �ڵ���
	::SetTimer(m_hWnd, TIMER_CHECK_HIT_ID, TIMER_CHECK_HIT_INTERVAL, nullptr);
	

	::SetTimer(m_hWnd, TIMER_CHANGE_PIC_ID, TIMER_CHANGE_PIC_INTERVAL, nullptr);
	
}


void CPlaneApp::On_Paint() {
	m_rand = rand();  //�����������


	//��ʾ����ͼ
	m_back.ShowBackGround();

	//��ʾ��ҷɻ�
	m_player.ShowPlayer();

	//��ʾ�����ڵ�
	m_gunBox.ShowAllGunner();

	//��ʾ���е��������˷ɻ�
	m_foeBox.ShowAllFoePlane();

	//��ʾ���б�ը�ĵ��˷ɻ�
	m_foeBox.ShowAllBoomFoePlane();


	//��ʾ����
	ShowScore();
}
void CPlaneApp::On_Close() {

}

void CPlaneApp::ON_WM_TIMER(WPARAM wparam, LPARAM lparam) {

	if (wparam == TIMER_BACK_MOVE_ID) {
		m_back.MoveBackGround(TIMER_BACK_MOVE_STEP);
	}
	if (TIMER_CHECK_DIRECTION_ID == wparam) {
		if (::GetAsyncKeyState(VK_LEFT)) {  //��ȡ VK_LEFT ������״̬������ǰ��·��ط�0
			m_player.MovePlayer(VK_LEFT, TIMER_PLAYER_MOVE_STEP);
		}
		if (::GetAsyncKeyState(VK_UP)) {  //��ȡ VK_UP ������״̬������ǰ��·��ط�0
			m_player.MovePlayer(VK_UP, TIMER_PLAYER_MOVE_STEP);
		}
		if (::GetAsyncKeyState(VK_RIGHT)) {  //��ȡ VK_RIGHT ������״̬������ǰ��·��ط�0
			m_player.MovePlayer(VK_RIGHT, TIMER_PLAYER_MOVE_STEP);
		}
		if (::GetAsyncKeyState(VK_DOWN)) {  //��ȡ VK_DOWN ������״̬������ǰ��·��ط�0
			m_player.MovePlayer(VK_DOWN, TIMER_PLAYER_MOVE_STEP);
		}
	}

	if (TIMER_SEND_GUNNER_ID == wparam) {  //�����ڵ�
		CGunner* pGun = m_player.SendGunner();
		//�������һ���ڵ���
		m_gunBox.m_lst.push_back(pGun);  //��ӵ��ڵ�������
	}

	if (TIMER_GUNNER_MOVE_ID == wparam) {   //�ƶ������ڵ�
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

	if (TIMER_FOE_MOVE_ID == wparam) {  //�������͵ĵ��˷ɻ���ʱ�ƶ�
		m_foeBox.MoveAllFoePlane();
	}

	//��ʱ����Ƿ���ײ�����˷ɻ��� ��ҷɻ�   ���˷ɻ� �� �ڵ���
	if (TIMER_CHECK_HIT_ID == wparam) {

		bool isBoom = false;   //û�б�ը

		list<CFoePlane*>::iterator iteFoe = m_foeBox.m_lstFoe.begin();
		while (iteFoe != m_foeBox.m_lstFoe.end()) {
			isBoom = false;  //ÿ��Ĭ�϶���û��ը
			//�ж��Ƿ����ҷɻ���ײ
			if ( (*iteFoe)->IsHitPlayer(m_player)  ) {
				GameOver();
				return;
			}

			//�����ڵ����ж��Ƿ���ڵ���ײ
			list<CGunner*>::iterator iteGun =  m_gunBox.m_lst.begin();
			while (iteGun != m_gunBox.m_lst.end()) {
			
				//ȡ��ÿһ���ڵ��͵��˷ɻ��ж��Ƿ���ײ
				if ((*iteFoe)->IsHitGunner(*iteGun)) {
					
					//�����ڵ�
					delete (*iteGun);
					(*iteGun) = nullptr;

					//��������ڵ�
					iteGun = m_gunBox.m_lst.erase(iteGun);

					//���˷ɻ���Ѫ
					(*iteFoe)->m_blood -= DEF_GUNNER_HURT;
					if ((*iteFoe)->m_blood <= 0) {

						m_score += 1;  //�ӷ�

						isBoom = true;
						m_foeBox.m_lstBoomFoe.push_back(*iteFoe);  //��ӵ���ը������
						iteFoe = m_foeBox.m_lstFoe.erase(iteFoe);  //ɾ��������
						break;
					}
					continue;
				}
				iteGun++;
			}
			if(!isBoom)  //���û�б�ը��iteFoe ����++
				iteFoe++;
		}
	}


	if (TIMER_CHANGE_PIC_ID == wparam) {
		list<CFoePlane* >::iterator ite = m_foeBox.m_lstBoomFoe.begin();
		while (ite != m_foeBox.m_lstBoomFoe.end()) {
			
			(*ite)->m_showId--;  //�л�ͼ

			if ((*ite)->m_showId < 0) {  //�ж��Ƿ���ʾ���
				delete (*ite);    //���ձ�ը�ķɻ�
				(*ite) = nullptr;

				ite = m_foeBox.m_lstBoomFoe.erase(ite);   //���սڵ�
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


	//������ʾ�ĵ�����
	::MessageBox(m_hWnd,L"GameOver!",L"��ʾ",MB_OK);

	//�ó����������˳�,Ͷ��һ�����ڹرյ���Ϣ
	::PostMessage(m_hWnd, WM_CLOSE,0,0);
}


void CPlaneApp::ShowScore() {
	//����ʾһ��������
	::putimage(0, 0, &m_scoreBoard);


	wchar_t buf[5] = { 0 };
	_itow_s(m_score, buf,10);

	wstring str = L"����Ϊ��";
	str += buf;

	RECT rect = {0,0,100,50};

	::settextcolor(RGB(255, 0, 0));
	::setbkmode(TRANSPARENT);   //�������ֱ���͸��

	::drawtext(str.c_str(),&rect, DT_CENTER);   //������ʾ����
}