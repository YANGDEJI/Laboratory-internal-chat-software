#pragma once
#include<windows.h>
#include<map>
#include<easyx.h>
using namespace std;

//游戏壳的类
class CGameFrame {
	// CGameFrame 认为 WindowProc 是我的朋友，所以可以在WindowProc 函数中直接使用私有成员
	friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	HWND m_hWnd;   //窗口句柄（唯一标识某个窗口）
private:
	bool m_isQuit;  //标识程序是否退出，false 不退出，true 程序退出

	typedef void (CGameFrame::*P_EM_MOUSE)(POINT& );       //鼠标类别处理函数的类型
	typedef void (CGameFrame::* P_EM_KEY)(WPARAM);  //键盘类别处理函数的类型
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
		union P_FUN{  //指针空间共享
			P_EM_MOUSE p_EM_MOUSE;
			P_EM_KEY p_EM_KEY;
			P_EM_CHAR p_EM_CHAR;
			P_EM_WINDOW p_EM_WINDOW;
		}p_fun;
	};


	map<UINT, MSG_FUN> m_MsgMap;  //消息映射表

public:
	CGameFrame();
	virtual ~CGameFrame();   //继承多态下，父类的析构最好为虚析构
	bool GetQuit();
	void InitMsgMap();
public:
	void InitGame(int x,int y,int width,int height,TCHAR* pTitle);
	void ShowGame();
	void RunGame();
	void CloseGame();
	void ON_WM_CLOSE(WPARAM wparam, LPARAM lparam);

	//-------------------------------------
	virtual void On_Init() = 0;  //具体游戏的初始化
	virtual void On_Paint() = 0; //具体游戏的重绘
	virtual void On_Close() = 0; //具体游戏的关闭


	virtual void ON_WM_LBUTTONDOWN(POINT& po) {};
	virtual void ON_WM_LBUTTONUP(POINT& po) {};
	virtual void ON_WM_MOUSEMOVE(POINT& po) {};
	virtual void ON_WM_KEYDOWN(WPARAM wparam) {};   //WPARAM : 按下了哪个键
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
* 使用规则：
要分享的文件：GameFrame.h  和  main.cpp 

子类（具体游戏类）继承 CGameFrame 类，父类中的
On_Init()
On_Paint()
On_Close()
纯虚函数在子类中一定要定义实现，
一般的虚函数  ON_WM_  开头的函数，按需重写，重写之后就调用具体游戏的消息处理函数了。
添加消息（在 InitMsgMap 函数中贴一条宏） 和 对应的处理函数（按照类别写参数，在添加）。


两个宏：WND_PARAM  CREATE_OBJECT 应当在具体的游戏中（必须在源文件中去使用宏）去使用，


*/