#include"TcpServerNet.h"
#include"packDef.h"
#include"TcpServerMediator.h" 
#include<process.h>

TcpServerNet::TcpServerNet(INetMediator* pMediator):m_isStop(false)
{
	m_pMediator = pMediator;
}
TcpServerNet::~TcpServerNet()
{
	UninitNet();
}
	//初始化网络:加载库、创建套接字、绑定ip地址、申请广播权限（有限广播地址）、创建接收数据的线程（在线程里一直等待接收数据）
bool TcpServerNet::InitNet()
{
    cout << __func__ <<  endl;
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

    /* Confirm that the WinSock DLL supports 2.2.*/
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */

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
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == m_sock) {
        cout << "socket error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "socket success" << endl;
    }

    //3.选地址--绑定ip地址bind()
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_DEF_UDP_SERVER_PORT);//htons--转换成网络字节序，也就是转换成大端存储方式
    //十进制4等分字符串类型的ip地址，转换成ulong类型的ip地址，使用inet_addr()
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;//服务端需要绑定所有网卡
    err = bind(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (SOCKET_ERROR == err) {
        cout << "bind error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "bind success" << endl;
    }

    //4.监听
    listen(m_sock, 10);
    if (err == SOCKET_ERROR) {
        cout << "listen error:" << WSAGetLastError() << endl;
        UninitNet();
        return false;
    }
    else {
        cout << "listen success" << endl;
    }

    //5.接收连接(需要接收多个)--阻塞函数、一直等待连接、需要多线程
    HANDLE handel = (HANDLE)_beginthreadex(NULL, 0, &AcceptThread, this, 0, 0);  

    return true;
}

//接收连接的线程函数
unsigned __stdcall TcpServerNet::AcceptThread(void* lpVoid) {
    TcpServerNet* pThis = (TcpServerNet*)lpVoid;
    sockaddr_in CilentAddr;
    int CilentAddrSize = sizeof(CilentAddr);
    while (1) {
        //6.创建接收数据的线程
        SOCKET sock = accept(pThis->m_sock, (sockaddr*)&CilentAddr, &CilentAddrSize);
        //打印客户端ip
        cout << "客户端连接成功：ip" << inet_ntoa(CilentAddr.sin_addr) << endl;
        unsigned int threadID;
        HANDLE m_handle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, pThis, 0, &threadID);

        if (m_handle) {
            pThis->m_handlelist.push_back(m_handle);
        }
        //把socket和线程保持到map中
        pThis->m_threadidToSocket[threadID] = sock;
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

//

//关闭网络：结束线程工作（通过成员变量标志位）、关闭句柄（回收内核对象）、关闭套接字、卸载库
void TcpServerNet::UninitNet()
{
    cout << __func__ <<  endl;
    //1.结束线程工作
    m_isStop = true;
    
    //2.回收句柄
    list<HANDLE>::iterator ite = m_handlelist.begin();
    while (ite != m_handlelist.end())
    {
        if (*ite) {
            if (WAIT_TIMEOUT == WaitForSingleObject(*ite, 500)) {
                TerminateThread(*ite, -1);
            }
            CloseHandle(*ite);
            *ite = NULL;
        }
        ite = m_handlelist.erase(ite);
    }
    //3.关闭套接字
    if (!m_sock && INVALID_SOCKET != m_sock) {
        closesocket(m_sock);
    }
    for (auto ite = m_threadidToSocket.begin(); ite != m_threadidToSocket.end();) {
        if (ite->second && INVALID_SOCKET != ite->second) {
            closesocket(m_sock);
        }
        //把无效的节点从map中移除
        ite = m_threadidToSocket.erase(ite);
    }
    //4.卸载库
    WSACleanup();
}
//发送数据
bool TcpServerNet::SendData(long lSendIp, char* buf, int nLen)
{
    cout << __func__ <<  endl;
    //1.判断参数的合法性
    if (!buf || nLen <= 0)return false;
    //2.先发包大小，
    if (send(lSendIp, (char*)&nLen, sizeof(nLen), 0) < 0) {
        cout << "TcpServerNet::SendLen error" << WSAGetLastError() << endl;
        return false;
    }
    //3.再发包内容
    if (send(lSendIp,buf,nLen,0) < 0) {
        cout << "TcpServerNet::SendData error" << WSAGetLastError() << endl;
        return false;
    }
	return true;
}

//接收数据
void TcpServerNet::RecvData()
{
    cout << __func__ <<  endl;
    //1.休眠一会,把socket放入map需要时间
    Sleep(100);

    int nRecvNum = 0;
    int nPackSize = 0;
    int nOffset = 0;
    //获取线程ID并依据线程ID从map中获取sock
    SOCKET sock = m_threadidToSocket[GetCurrentThreadId()];
    if (!sock || sock == INVALID_SOCKET) {
        cout << __func__ << "socket error" << endl;
    }
    while (!m_isStop) {
        //先接收包大小
        nRecvNum = recv(sock, (char*)&nPackSize, sizeof(int), 0);
        if (nRecvNum > 0) {
            //再接包内容
            char* packBuf = new char[nPackSize];
            while (nPackSize > 0) {
                nRecvNum = recv(sock, packBuf + nOffset, nPackSize, 0);
                if (nRecvNum > 0) {
                    cout << "接收数据成功" << endl;
                    nPackSize -= nRecvNum;
                    nOffset += nRecvNum;
                }
                else {
                    cout << "接收数据失败" << endl;
                    return;
                }
            }
            //接收一次完整的包，传给中介者类
            m_pMediator->DealData(sock, packBuf, nOffset);
        }
    }
}


