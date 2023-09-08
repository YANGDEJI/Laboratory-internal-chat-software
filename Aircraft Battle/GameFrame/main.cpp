#include"GameFrame.h"
#include<easyx.h>
#include<Windowsx.h>


/*
int WND_POS_X = 400;
int WND_POS_Y = 100;
int WND_WIDTH = 500;
int WND_HEIGHT = 500;
TCHAR* WND_TITLE = (TCHAR*)L"����";

CGameFrame* CreateObject() {
		return new ������Ϸ��;
	}

*/

CGameFrame* pGameFrame = nullptr;  
int main() {
	//����Ϸ�����������ھ������Ϸ�ж���
	extern int WND_POS_X ;
	extern int WND_POS_Y ;
	extern int WND_WIDTH ;
	extern int WND_HEIGHT ;
	extern TCHAR* WND_TITLE;

	CGameFrame* CreateObject();  //����

	pGameFrame = CreateObject();
	pGameFrame->InitGame(WND_POS_X, WND_POS_Y, WND_WIDTH, WND_HEIGHT, WND_TITLE);  //���ֽ���ƥ��WCHAR* Ӧ����L
	//pGameFrame->RunGame();
	pGameFrame->ShowGame();  //���ڴ�����ϣ��Ȼ���һ��


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
	m_isQuit = false;  //false ���˳���true �����˳�
}
CGameFrame::~CGameFrame() {

}

////3. ��Ϸ���У����ڵĻص�����
// hwnd����ʶ�ĸ�����
//uMsg: ��Ϣid
// wParam��lParam  ,uMsg ��ϢЯ������
LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) {

	//if (uMsg == WM_LBUTTONDOWN) {  //���������������
	//		//��ȡ����㣬�����������Ϸ������
	//	POINT po = { 0 };
	//	////po.x = msg.x;
	//	////po.y = msg.y;
	//	
	//	//��ȡ��������
	//	po.x =  GET_X_LPARAM(lParam);
	//	po.y = GET_Y_LPARAM(lParam);


	//	pGameFrame->ON_WM_LBUTTONDOWN(po);

	//}
	//else if (uMsg == WM_CLOSE) {  //����Ǵ��ڹر�
	//	pGameFrame->CloseGame();
	//}

	//WM_KEYDOWN;

	//---------------------------------
	if (pGameFrame->m_MsgMap.count(uMsg)) {  //�ж�ĳ����Ϣ�Ƿ���ڣ�map ��keyֵ�Ƿ���ڣ�
		switch (pGameFrame->m_MsgMap[uMsg].MSG_TYPE)  //��ȡ��Ӧ����Ϣ���
		{
		case EM_MOUSE:
		{
			POINT po = { 0 };
			////po.x = msg.x;
			////po.y = msg.y;

			//��ȡ��������
			po.x = GET_X_LPARAM(lParam);
			po.y = GET_Y_LPARAM(lParam);
			(pGameFrame->*(pGameFrame->m_MsgMap[uMsg].p_fun.p_EM_MOUSE))(po);  //������Ϣ��Ӧ�Ĵ�����
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
		//����Ϣ�����ˣ��ܴ����������Ҫ���»��ƣ���֤���ڼ�ʱˢ��
		pGameFrame->ShowGame();
	}

	if (uMsg == WM_CLOSE) {  //����Ǵ��ڹر�
		pGameFrame->ON_WM_CLOSE(wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);  //window Ĭ�ϴ�����
}

#define INIT_MSGMAP(MSG_ID,MSG_TYPEE)\
	m_MsgMap[MSG_ID].MSG_TYPE = MSG_TYPEE;\
	m_MsgMap[MSG_ID].p_fun.p_##MSG_TYPEE = &CGameFrame::ON_##MSG_ID;


//1. ��ʼ��
void CGameFrame::InitGame(int x, int y, int width, int height, TCHAR* pTitle) {
	m_hWnd = ::initgraph(width, height);  //��������
	::MoveWindow(m_hWnd, x, y, width, height, true);   //�ƶ�����
	::SetWindowText(m_hWnd, pTitle);   //���ô��ڱ���

	::setbkcolor(RGB(255, 255, 255));   //���ñ���ɫ
	::cleardevice();    //ʹ���õı���ɫ ������Ч

	//�����Ϣӳ���
	InitMsgMap();

	//Ϊ�������ûص�����������ȫ������Ϣ
	::SetWindowLong(m_hWnd,  //�����ĸ����ڵ�����
		GWL_WNDPROC,   //����ʲô���ԣ����ڻص����ԣ�
		(LONG)&WindowProc);  //���Զ�Ӧ��ֵ���ص������ĵ�ַ��


	//������Ϸ�ĳ�ʼ��
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
		
	//INIT_MSGMAP(WM_CLOSE, EM_WINDOW)  //����Ϣӳ������Ƴ�WM_CLOSE����ΪҪ�����������ػ��г�ͻ
}

//2. �ػ溯��
void CGameFrame::ShowGame(){
	::BeginBatchDraw();  //��ʼ������ʾ
	::cleardevice();   //�����һ�εĻ�ͼ�ۼ�

	//---������Ϸ���ػ�-----------------------------------------
	On_Paint();

	//--------------------------------------------
	::EndBatchDraw();  //���������ػ�
}
//3. ��Ϸ����  = delete
void CGameFrame::RunGame() {

	while (1) {//���ϵĻ�ȡ��Ϣ
		ExMessage msg = {0};
		::getmessage(&msg);  //��ȡ����������Ϣ
		if (msg.message == WM_LBUTTONDOWN) {  //���������������
			//��ȡ����㣬�����������Ϸ������
			POINT po = { msg.x,msg.y };
			//po.x = msg.x;
			//po.y = msg.y;
			ON_WM_LBUTTONDOWN(po);
		
		}
		else if (msg.message == WM_CLOSE) {  //����Ǵ��ڹر�
			CloseGame();
		}
	}
}
//4. �رմ���
void CGameFrame::CloseGame() {
	On_Close();   //���þ�����Ϸ�Ĺر�
	::closegraph();  //�رմ���
	m_isQuit = true;
}

bool CGameFrame::GetQuit() {
	return m_isQuit;
}

void CGameFrame::ON_WM_CLOSE(WPARAM wparam, LPARAM lparam) {
	CloseGame();  //���ùرյ�
}

//----------------------------------------------------

