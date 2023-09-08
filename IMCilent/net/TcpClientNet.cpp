#include"TcpClientNet.h"
#include"packDef.h"
#include"TcpClientMediator.h" 
#include<process.h>

TcpClientNet::TcpClientNet(INetMediator* pMediator) :m_isStop(false), m_sock(INVALID_SOCKET), m_handle(NULL)
{
    m_pMediator = pMediator;
}
TcpClientNet::~TcpClientNet()
{
    UninitNet();
}
//初始化网络:加载库、创建套接字、连接服务端、创建接收数据的线程
bool TcpClientNet::InitNet()
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

    //2.创建套接字socket()
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == m_sock) {
        cout << "socket error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "socket success" << endl;
    }

    //3.连接服务端
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_DEF_UDP_SERVER_PORT);//htons--转换成网络字节序，也就是转换成大端存储方式 //十进制4等分字符串类型的ip地址，转换成ulong类型的ip地址，使用inet_addr()
    serverAddr.sin_addr.S_un.S_addr = inet_addr(_DEF_TCP_SERVER_IP);
    err = connect(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (SOCKET_ERROR == err) {
        cout << "bind error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "bind success" << endl;
    }

    //4.创建接收数据的线程(只接收来自服务端的线程)
    m_handle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, this, 0, NULL);//（安全等级，堆栈大小，接收线程，内容，创建状态（0创建就启动），线程ID）

    return true;
}

//接收数据的线程函数
unsigned __stdcall TcpClientNet::RecvThread(void* lpVoid)
{
    TcpClientNet* pThis = (TcpClientNet*)lpVoid;
    pThis->RecvData();
    return 0;
}

//关闭网络：结束线程工作（通过成员变量标志位）、关闭句柄（回收内核对象）、关闭套接字、卸载库
void TcpClientNet::UninitNet()
{
    //1.结束线程工作
    m_isStop = true;

    //2.回收句柄

    if (m_handle) {
        if (WAIT_TIMEOUT == WaitForSingleObject(m_handle, 500)) {
            TerminateThread(m_handle, -1);
        }
        CloseHandle(m_handle);
        m_handle = NULL;
    }
    //3.关闭套接字
    if (!m_sock && INVALID_SOCKET != m_sock) {
        closesocket(m_sock);
    }

    //4.卸载库
    WSACleanup();
}
//发送数据
bool TcpClientNet::SendData(long lSendIp, char* buf, int nLen)
{
    //1.判断参数的合法性
    if (!buf || nLen <= 0){
        cout << __func__ << "parameters error:" << endl;
        return false;
    }
    //2.先发包大小，
    if (send(m_sock, (char*)&nLen, sizeof(int), 0) <= 0) {
        cout << "TcpClientNet::SendLen error" << WSAGetLastError() << endl;
        return false;
    }
    //3.再发包内容
    if (send(m_sock, buf, nLen, 0) <= 0) {
        cout << "TcpClientNet::SendData error" << WSAGetLastError() << endl;
        return false;
    }
    return true;
}

//接收数据
void TcpClientNet::RecvData()
{
    int nRecvNum = 0;
    int nPackSize = 0;
    int nOffset = 0;
    while (!m_isStop) {
        //先接收包大小
        nRecvNum = recv(m_sock, (char*)&nPackSize, sizeof(int), 0);
        if (nRecvNum > 0) {
            //再接包内容
            char* packBuf = new char[nPackSize];
            while (nPackSize > 0) {
                nRecvNum = recv(m_sock, packBuf + nOffset, nPackSize, 0);
                if (nRecvNum > 0) {
                    cout << "data recive success" << endl;
                    nOffset += nRecvNum;
                    nPackSize -= nRecvNum;
                }
                else {
                    cout << "接收数据失败" << endl;
                    return;
                }
            }
            //接收一次完整的包，传给中介者类
            m_pMediator->DealData(m_sock, packBuf, nOffset);
        }
    }
}


