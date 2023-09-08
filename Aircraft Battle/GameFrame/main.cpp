#include"GameFrame.h"
#include<easyx.h>
#include<Windowsx.h>


/*
int WND_POS_X = 400;
int WND_POS_Y = 100;
int WND_WIDTH = 500;
int WND_HEIGHT = 500;
TCHAR* WND_TITLE = (TCHAR*)L"测试";

CGameFrame* CreateObject() {
		return new 具体游戏类;
	}

*/

CGameFrame* pGameFrame = nullptr;  
int main() {
	//在游戏壳中声明，在具体的游戏中定义
	extern int WND_POS_X ;
	extern int WND_POS_Y ;
	extern int WND_WIDTH ;
	extern int WND_HEIGHT ;
	extern TCHAR* WND_TITLE;

	CGameFrame* CreateObject();  //声明

	pGameFrame = CreateObject();
	pGameFrame->InitGame(WND_POS_X, WND_POS_Y, WND_WIDTH, WND_HEIGHT, WND_TITLE);  //宽字节下匹配WCHAR* 应当加L
	//pGameFrame->RunGame();
	pGameFrame->ShowGame();  //窗口创建完毕，先绘制一下


	while (!pGameFrame->GetQuit()) {
		
		Sleep(100);
	}



	if (pGameFrame) {
		delete pGameFrame;
		pGameFrame = nullptr;
	}
	return 0;
}


CGameFrame::CGameFrame() {
	m_isQuit = false;  //false 不退出，true 程序退出
}
CGameFrame::~CGameFrame() {

}

////3. 游戏运行，窗口的回调函数
// hwnd：标识哪个窗口
//uMsg: 消息id
// wParam，lParam  ,uMsg 消息携带参数
LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) {

	//if (uMsg == WM_LBUTTONDOWN) {  //如果是鼠标左键按下
	//		//获取坐标点，传到具体的游戏处理中
	//	POINT po = { 0 };
	//	////po.x = msg.x;
	//	////po.y = msg.y;
	//	
	//	//获取鼠标点坐标
	//	po.x =  GET_X_LPARAM(lParam);
	//	po.y = GET_Y_LPARAM(lParam);


	//	pGameFrame->ON_WM_LBUTTONDOWN(po);

	//}
	//else if (uMsg == WM_CLOSE) {  //如果是窗口关闭
	//	pGameFrame->CloseGame();
	//}

	//WM_KEYDOWN;

	//---------------------------------
	if (pGameFrame->m_MsgMap.count(uMsg)) {  //判断某个消息是否存在（map 的key值是否存在）
		switch (pGameFrame->m_MsgMap[uMsg].MSG_TYPE)  //获取对应的消息类别
		{
		case EM_MOUSE:
		{
			POINT po = { 0 };
			////po.x = msg.x;
			////po.y = msg.y;

			//获取鼠标点坐标
			po.x = GET_X_LPARAM(lParam);
			po.y = GET_Y_LPARAM(lParam);
			(pGameFrame->*(pGameFrame->m_MsgMap[uMsg].p_fun.p_EM_MOUSE))(po);  //调用消息对应的处理函数
		}
			break;
		case EM_KEY:
		{

			(pGameFrame->*(pGameFrame->m_MsgMap[uMsg].p_fun.p_EM_KEY))(wParam);
		}
		break;
		case EM_CHAR:
		{
			//WM_CHAR;
			(pGameFrame->*(pGameFrame->m_MsgMap[uMsg].p_fun.p_EM_CHAR))(wParam);
		}
		break;
		case EM_WINDOW:
		{
			(pGameFrame->*(pGameFrame->m_MsgMap[uMsg].p_fun.p_EM_WINDOW))(wParam, lParam);
		}
		break;
		}
		//有消息发生了，能处理，处理后需要重新绘制，保证窗口及时刷新
		pGameFrame->ShowGame();
	}

	if (uMsg == WM_CLOSE) {  //如果是窗口关闭
		pGameFrame->ON_WM_CLOSE(wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);  //window 默认处理函数
}

#define INIT_MSGMAP(MSG_ID,MSG_TYPEE)\
	m_MsgMap[MSG_ID].MSG_TYPE = MSG_TYPEE;\
	m_MsgMap[MSG_ID].p_fun.p_##MSG_TYPEE = &CGameFrame::ON_##MSG_ID;


//1. 初始化
void CGameFrame::InitGame(int x, int y, int width, int height, TCHAR* pTitle) {
	m_hWnd = ::initgraph(width, height);  //创建窗口
	::MoveWindow(m_hWnd, x, y, width, height, true);   //移动窗口
	::SetWindowText(m_hWnd, pTitle);   //设置窗口标题

	::setbkcolor(RGB(255, 255, 255));   //设置背景色
	::cleardevice();    //使设置的背景色 立即生效

	//添加消息映射表
	InitMsgMap();

	//为窗口设置回调函数，接收全量的消息
	::SetWindowLong(m_hWnd,  //设置哪个窗口的属性
		GWL_WNDPROC,   //设置什么属性（窗口回调属性）
		(LONG)&WindowProc);  //属性对应的值（回调函数的地址）


	//具体游戏的初始化
	On_Init();
}

void CGameFrame::InitMsgMap() {
	/*m_MsgMap[WM_LBUTTONDOWN].MSG_TYPE = EM_MOUSE;
	m_MsgMap[WM_LBUTTONDOWN].p_fun.p_EM_MOUSE = &CGameFrame::ON_WM_LBUTTONDOWN;*/
	INIT_MSGMAP(WM_LBUTTONDOWN, EM_MOUSE)
	INIT_MSGMAP(WM_KEYDOWN, EM_KEY)
	INIT_MSGMAP(WM_CHAR, EM_CHAR)
	INIT_MSGMAP(WM_LBUTTONUP, EM_MOUSE)
	INIT_MSGMAP(WM_MOUSEMOVE, EM_MOUSE)
	INIT_MSGMAP(WM_TIMER, EM_WINDOW)
		
	//INIT_MSGMAP(WM_CLOSE, EM_WINDOW)  //从消息映射表中移除WM_CLOSE，因为要单独处理，和重绘有冲突
}

//2. 重绘函数
void CGameFrame::ShowGame(){
	::BeginBatchDraw();  //开始批量显示
	::cleardevice();   //清除上一次的绘图痕迹

	//---具体游戏的重绘-----------------------------------------
	On_Paint();

	//--------------------------------------------
	::EndBatchDraw();  //结束批量重绘
}
//3. 游戏运行  = delete
void CGameFrame::RunGame() {

	while (1) {//不断的获取消息
		ExMessage msg = {0};
		::getmessage(&msg);  //获取所有类别的消息
		if (msg.message == WM_LBUTTONDOWN) {  //如果是鼠标左键按下
			//获取坐标点，传到具体的游戏处理中
			POINT po = { msg.x,msg.y };
			//po.x = msg.x;
			//po.y = msg.y;
			ON_WM_LBUTTONDOWN(po);
		
		}
		else if (msg.message == WM_CLOSE) {  //如果是窗口关闭
			CloseGame();
		}
	}
}
//4. 关闭窗口
void CGameFrame::CloseGame() {
	On_Close();   //调用具体游戏的关闭
	::closegraph();  //关闭窗口
	m_isQuit = true;
}

bool CGameFrame::GetQuit() {
	return m_isQuit;
}

void CGameFrame::ON_WM_CLOSE(WPARAM wparam, LPARAM lparam) {
	CloseGame();  //调用关闭的
}

//----------------------------------------------------

