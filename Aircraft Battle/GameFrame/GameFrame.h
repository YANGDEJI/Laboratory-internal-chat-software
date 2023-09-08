#pragma once
#include<windows.h>
#include<map>
#include<easyx.h>
using namespace std;

//��Ϸ�ǵ���
class CGameFrame {
	// CGameFrame ��Ϊ WindowProc ���ҵ����ѣ����Կ�����WindowProc ������ֱ��ʹ��˽�г�Ա
	friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	HWND m_hWnd;   //���ھ����Ψһ��ʶĳ�����ڣ�
private:
	bool m_isQuit;  //��ʶ�����Ƿ��˳���false ���˳���true �����˳�

	typedef void (CGameFrame::*P_EM_MOUSE)(POINT& );       //����������������
	typedef void (CGameFrame::* P_EM_KEY)(WPARAM);  //�����������������
	typedef void (CGameFrame::*P_EM_CHAR)(WPARAM);
	typedef void (CGameFrame::* P_EM_WINDOW)(WPARAM, LPARAM);

	/*
	#define EM_MOUSE	1
	#define EM_KEY		2
	#define EM_CHAR		4
	#define EM_WINDOW	8
	*/
	struct MSG_FUN {
		int MSG_TYPE;
		union P_FUN{  //ָ��ռ乲��
			P_EM_MOUSE p_EM_MOUSE;
			P_EM_KEY p_EM_KEY;
			P_EM_CHAR p_EM_CHAR;
			P_EM_WINDOW p_EM_WINDOW;
		}p_fun;
	};


	map<UINT, MSG_FUN> m_MsgMap;  //��Ϣӳ���

public:
	CGameFrame();
	virtual ~CGameFrame();   //�̳ж�̬�£�������������Ϊ������
	bool GetQuit();
	void InitMsgMap();
public:
	void InitGame(int x,int y,int width,int height,TCHAR* pTitle);
	void ShowGame();
	void RunGame();
	void CloseGame();
	void ON_WM_CLOSE(WPARAM wparam, LPARAM lparam);

	//-------------------------------------
	virtual void On_Init() = 0;  //������Ϸ�ĳ�ʼ��
	virtual void On_Paint() = 0; //������Ϸ���ػ�
	virtual void On_Close() = 0; //������Ϸ�Ĺر�


	virtual void ON_WM_LBUTTONDOWN(POINT& po) {};
	virtual void ON_WM_LBUTTONUP(POINT& po) {};
	virtual void ON_WM_MOUSEMOVE(POINT& po) {};
	virtual void ON_WM_KEYDOWN(WPARAM wparam) {};   //WPARAM : �������ĸ���
	virtual void ON_WM_CHAR(WPARAM wparam) {};
	virtual void ON_WM_TIMER(WPARAM wparam, LPARAM lparam) {};
	
};


#define WND_PARAM(WND_X,WND_Y,WND_W,WND_H,WND_T)\
int WND_POS_X = WND_X;\
int WND_POS_Y = WND_Y;\
int WND_WIDTH = WND_W;\
int WND_HEIGHT = WND_H;\
TCHAR* WND_TITLE = (TCHAR*)WND_T;



#define CREATE_OBJECT(OBJ_CLASS)\
CGameFrame* CreateObject() {\
	return new OBJ_CLASS;\
}


/*
* ʹ�ù���
Ҫ������ļ���GameFrame.h  ��  main.cpp 

���ࣨ������Ϸ�ࣩ�̳� CGameFrame �࣬�����е�
On_Init()
On_Paint()
On_Close()
���麯����������һ��Ҫ����ʵ�֣�
һ����麯��  ON_WM_  ��ͷ�ĺ�����������д����д֮��͵��þ�����Ϸ����Ϣ�������ˡ�
�����Ϣ���� InitMsgMap ��������һ���꣩ �� ��Ӧ�Ĵ��������������д����������ӣ���


�����꣺WND_PARAM  CREATE_OBJECT Ӧ���ھ������Ϸ�У�������Դ�ļ���ȥʹ�ú꣩ȥʹ�ã�


*/