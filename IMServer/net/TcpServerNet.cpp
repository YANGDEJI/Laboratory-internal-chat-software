#include"TcpServerNet.h"
#include"TcpServerMediator.h"
#include"PackDef.h"
#include<process.h>

TcpServerNet::TcpServerNet(INetMediator* pMediator): m_sockListen(INVALID_SOCKET),  m_isStop(false)
{
	m_pMediator = pMediator;
}
TcpServerNet::~TcpServerNet()
{
    UninitNet();
}
//初始化网络:加载库、创建套接字、绑定ip地址、监听、创建一个接受连接的线程
bool TcpServerNet::InitNet() 
{
    //1.选项目--加载库WSAStartup()
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        return false;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        return false;
    }
    else {
        printf("The Winsock 2.2 dll was found okay\n");
    }
    //2.雇帮手--创建套接字socket()
    m_sockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == m_sockListen) {
        cout << "socket error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "socket success" << endl;
    }

    //3.选地址--绑定ip地址bind()
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_DEF_TCP_SERVER_PORT);//htons--转换成网络字节序，也就是转换成大端存储方式
    //十进制4等分字符串类型的ip地址，转换成ulong类型的ip地址，使用inet_addr()
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;//服务端需要绑定所以网卡
    err = bind(m_sockListen, (sockaddr*)&serverAddr, sizeof(serverAddr));

    if (SOCKET_ERROR == err) {
        cout << "bind error:" << WSAGetLastError() << endl;
        return false; 
    }
    else {
        cout << "bind success" << endl;
    }

    //4.监听
    err = listen(m_sockListen, 10);
    if (SOCKET_ERROR == err) {
        cout << "listen error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "listen success" << endl;
    }

	//5.创建一个接收连接的线程
    //CreateThread和ExitThead是一起使用的，如果在线程中使用了C++运行时库的函数，
    // 例如strcpy会申请一个空间，但是使用ExitThread退出线程的时候。不会回收申请的空间，就会造成空间泄露。
    //_beginthreadex和_endtgreadex一起使用的，_endtgreadex会在结束线程的时候先回收空间，再调用ExitThread
    HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &AcceptThread, this, 0, NULL);
    if (handle) {
        m_listThreadHandle.push_back(handle);
    }
	return true;
}

//接受连接的线程函数:循环等待接受客户端的连接
unsigned __stdcall TcpServerNet::AcceptThread(void* lpVoid) 
{
    TcpServerNet* pThis = (TcpServerNet*)lpVoid;
    sockaddr_in addrClient;
    int addrClientSize = sizeof(addrClient);//传入传出参数
    while (!pThis->m_isStop) {
        //接受连接
        SOCKET sockClient = accept(pThis->m_sockListen,(sockaddr*)&addrClient,&addrClientSize);
        //打印连接成功的客户端的ip地址（只有打印了ip地址，才说明客户端和服务端连接成功了）
        cout << "client ip:" << inet_ntoa(addrClient.sin_addr) << "connect success" << endl;

        //创建一个连接成功的客户端对应的接受数据的线程
        unsigned int threadId = 0;
        HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, pThis, 0, &threadId);

        //把这个客户端对应的socket放到map中管理
        pThis->m_mapThreadIdToSocket[threadId] = sockClient;
        //把线程的句柄放在list里面管理
        if (handle) {
            pThis->m_listThreadHandle.push_back(handle);
        }
    }
    return 0;
}

//接收数据的线程函数
unsigned __stdcall TcpServerNet::RecvThread(void* lpVoid)
{
    TcpServerNet* pThis = (TcpServerNet*)lpVoid;
    pThis->RecvData();
    return 0;
}

//关闭网络:退出线程、关闭句柄、关闭socket、卸载库
void TcpServerNet::UninitNet()
{
    //1.退出线程
    m_isStop = true;
    for (auto ite = m_listThreadHandle.begin(); ite != m_listThreadHandle.end();){
        if (*ite) {
            if (WAIT_TIMEOUT == WaitForSingleObject(*ite, 100)) {
                //在100ms时间内，线程没有退出，就强制杀死
                TerminateThread(*ite, -1);
            }
            //2.回收句柄
            CloseHandle(*ite);
            *ite = NULL;
        }
    ite = m_listThreadHandle.erase(ite); //移除无效节点，返回值是下一个有效节点
}

    //3.关闭socket
    if (m_sockListen && INVALID_SOCKET != m_sockListen) {
        closesocket(m_sockListen);
    }
    for (auto ite = m_mapThreadIdToSocket.begin(); ite != m_mapThreadIdToSocket.end();) {
        if (ite->second && INVALID_SOCKET != ite->second) {
            closesocket(ite->second);
        }
        ite = m_mapThreadIdToSocket.erase(ite);
    }
    //4.卸载库
    WSACleanup();
}
//发送数据
bool TcpServerNet::SendData(long lSendIp, char* buf, int nLen)
{
    //1.校验参数
    if (!buf || nLen <= 0) {
        cout << "TcpSeverNet::SendData parameter error" << endl;
        return false;
    }
    //2.先发包大小
    if (send(lSendIp, (char*)&nLen, sizeof(int), 0) <= 0) {
        return false;
    }
    //3.再发包内容
    if (send(lSendIp, buf, nLen, 0) <= 0) {
        return false;
    }
	return true;
}

//接收数据
void TcpServerNet::RecvData()
{
    //因为线程创建即运行，但把socket存入map还需要一点时间，所以线程中先休眠一会
    Sleep(100);
    //1.获取当前线程对应的socket
    //获取线程id
    unsigned int threadId = GetCurrentThreadId();
    SOCKET socket = m_mapThreadIdToSocket[threadId];
    //判断socket的合法性
    if (!socket || INVALID_SOCKET == socket) {
        cout << "socket 不合法" << endl;
        return;
    }
    int packSize = 0;
    int nRecvNum = 0;
    int offset = 0;
    while (!m_isStop) {
        //先接收包大小
        nRecvNum = recv(socket, (char*)&packSize, sizeof(int), 0);
        if (nRecvNum > 0) {//如果接收成功
            //new一个新的空间来接收包内容
            char* packBuf = new char[packSize];
            //再接收包内容
            while (packSize) {
                nRecvNum = recv(socket, packBuf + offset, packSize, 0);
                offset += nRecvNum;
                packSize -= nRecvNum;
            }
            //把接收到的数据传给中介者类
            m_pMediator->DealData(socket, packBuf, offset);
            //offset需要清理，以备下一次使用
            offset = 0;
        }
        else if (10054 == WSAGetLastError()) {
            break;
        }
        else {
            cout << "TcpSeverNet::RecvData recv error:" << WSAGetLastError() << endl;
            break;
        }
    }
}