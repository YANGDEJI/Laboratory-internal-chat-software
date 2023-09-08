#include"UdpNet.h"
#include"packDef.h"
#include"UdpMediator.h" 
#include<process.h>

UdpNet::UdpNet(INetMediator* pMediator):m_handle(0),m_sock(INVALID_SOCKET),m_isStop(false)
{
	m_pMediator = pMediator;
}
UdpNet::~UdpNet() 
{
	UninitNet();
}
	//初始化网络:加载库、创建套接字、绑定ip地址、申请广播权限（有限广播地址）、创建接收数据的线程（在线程里一直等待接收数据）
bool UdpNet::InitNet()
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
    m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;//服务端需要绑定所以网卡
    err = bind(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

    if (SOCKET_ERROR == err) {
        cout << "bind error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "bind success" << endl;
    }

//4.申请广播权限
    bool bval = true;
    setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (char*)&bval, sizeof(bval));

    //5.创建接收数据的线程
    //CreateThread和ExitThead是一起使用的，如果在线程中使用了C++运行时库的函数，
    // 例如strcpy会申请一个空间，但是使用ExitThread退出线程的时候。不会回收申请的空间，就会造成空间泄露。
    //_beginthreadex和_endtgreadex一起使用的，_endtgreadex会在结束线程的时候先回收空间，再调用ExitThread
    m_handle=(HANDLE)_beginthreadex(NULL, 0,&RecvThread, this,0, NULL);

	return true;
}

//接收数据的线程函数
unsigned __stdcall UdpNet::RecvThread(void* lpVoid)
{
    UdpNet* pThis = (UdpNet*)lpVoid;
    pThis->RecvData();
    return 0;
}

//关闭网络：结束线程工作（通过成员变量标志位）、关闭句柄（回收内核对象）、关闭套接字、卸载库
void UdpNet::UninitNet()
{
    //1.结束线程工作
    m_isStop = true;
    
    //2.回收句柄
    if (m_handle) {
        if (WAIT_TIMEOUT == WaitForSingleObject(m_handle,500)) {
            TerminateThread(m_handle, -1);
        }
        CloseHandle(m_handle);
        m_handle = NULL;
    }
    //3.关闭套接字
    if (m_sock && INVALID_SOCKET != m_sock) {
        closesocket(m_sock);
    }
    //4.卸载库
    WSACleanup();
}
//发送数据
bool UdpNet::SendData(long lSendIp, char* buf, int nLen)
{
    sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(_DEF_UDP_SERVER_PORT);
    sockAddr.sin_addr.S_un.S_addr = lSendIp;
    if (sendto(m_sock,buf,nLen,0,(sockaddr*)&sockAddr,sizeof(sockAddr)) <= 0) {
        cout << "UdpNet::SendData error" << WSAGetLastError() << endl;
        return false;
    }
	return true;
}

//接收数据
void UdpNet::RecvData() 
{
    int nRecvNum = 0;
    char recvBuf[4096] = "";
    sockaddr_in sockAddrClient;
    int sockAddrClientSize = sizeof(sockAddrClient);
    while (!m_isStop) {
        nRecvNum = recvfrom(m_sock,recvBuf,sizeof(recvBuf),0,(sockaddr*)&sockAddrClient,&sockAddrClientSize);
        if (nRecvNum > 0) {
            //new一个新的空间，拷贝收到的数据到新的空间，把新的空间传递给中介者类
            char* packBuf = new char[nRecvNum];
            memcpy(packBuf,recvBuf, nRecvNum);
            //将接收到的数据传递给中介者类
            m_pMediator->DealData(sockAddrClient.sin_addr.S_un.S_addr, packBuf, nRecvNum);
        }
    }
}
