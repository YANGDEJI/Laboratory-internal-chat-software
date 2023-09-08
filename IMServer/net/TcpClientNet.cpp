#include"TcpClientNet.h"
#include"PackDef.h"
#include<process.h>
#include"TcpClientMediator.h"

TcpClientNet::TcpClientNet(INetMediator* pMediator):m_sock(INVALID_SOCKET),m_handle(NULL),m_isStop(false)
{
	m_pMediator = pMediator;
}
TcpClientNet::~TcpClientNet()
{
	UninitNet();
}
//初始化网络:加载库、创建套接字、连接服务端、创建一个接收数据的线程
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

    //2.雇帮手--创建套接字socket()
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == m_sock) {
        cout << "socket error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "socket success" << endl;
    }

    //3.连接服务器
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_DEF_TCP_SERVER_PORT);
    serverAddr.sin_addr.S_un.S_addr = inet_addr(_DEF_TCP_SERVER_IP);
    err = connect(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (SOCKET_ERROR == err) {
        cout << "connect error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "connect success" << endl;
    }

    //4.创建一个接收数据的线程
    //CreateThread和ExitThead是一起使用的，如果在线程中使用了C++运行时库的函数，
    // 例如strcpy会申请一个空间，但是使用ExitThread退出线程的时候。不会回收申请的空间，就会造成空间泄露。
    //_beginthreadex和_endtgreadex一起使用的，_endtgreadex会在结束线程的时候先回收空间，再调用ExitThread
    m_handle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, this, 0, NULL);

	return true;
}

//接收数据的线程函数
unsigned __stdcall TcpClientNet::RecvThread(void* lpVoid)
{
    TcpClientNet* pThis = (TcpClientNet*)lpVoid;
    pThis->RecvData();
    return 0;
}

//关闭网络:关闭套接字、卸载库、结束线程+回收句柄（为了回收内核对象（引用计数器为0））
void TcpClientNet::UninitNet()
{
    //1.关闭套接字
    if (m_sock&& INVALID_SOCKET != m_sock)
    {
        closesocket(m_sock);
    }
    //2.卸载库
    WSACleanup();
    //3.优雅的结束线程工作(标志位置成false，等待线程自己结束工作，如果一定时间内，线程不能结束工作，再强制杀死线程)
    m_isStop = true;
    if (m_handle) {
        if (WAIT_TIMEOUT == WaitForSingleObject(m_handle, 100)) {
            //在100ms时间内，线程没有退出，就强制杀死
            TerminateThread(m_handle, -1);
        }
        //4.回收句柄
        CloseHandle(m_handle);
        m_handle = NULL;
    }
}
//发送数据
bool TcpClientNet::SendData(long lSendIp, char* buf, int nLen)
{
    //1.校验参数
    if (!buf||nLen<=0) {
        cout << "TcpClientNet::SendData parameter error" << endl;
        return false;
    }
    //2.先发包大小
    if (send(m_sock,(char*)&nLen,sizeof(int),0)<=0) {
        return false;
    }
    //3.再发包内容
    if (send(m_sock,buf,nLen,0 ) <= 0) {
        return false;
    }
	return true;
}

//接收数据
void TcpClientNet::RecvData()
{
    int packSize = 0;
    int nRecvNum = 0;
    int offset = 0;
    while (!m_isStop) {
        //先接收包大小
        nRecvNum = recv(m_sock,(char*)&packSize,sizeof(int),0);
        if (nRecvNum > 0) {//如果接收成功
            //new一个新的空间来接收包内容
            char* packBuf = new char[packSize];
            //再接收包内容
            while (packSize) {
                nRecvNum = recv(m_sock, packBuf + offset, packSize, 0);
                offset += nRecvNum;
                packSize -= nRecvNum;
            }
            //把接收到的数据传给中介者类
            m_pMediator->DealData(m_sock, packBuf, offset);
            //offset需要清理，以备下一次使用
            offset = 0;
        }
        else {
            cout << "TcpClientNet::RecvData recv error:" << WSAGetLastError() << endl;
            break;
        }
    }
}