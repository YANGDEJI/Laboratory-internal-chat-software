#include<iostream>
#include<Windows.h>
#include"CKernel.h"

using namespace std;

int main()
{
	CKernel kernel;
	if (!kernel.startServer()) {
		cout << "服务器打开网络失败" << endl;
		return 1;
	}
	else {
		cout << "服务端打开成功" << endl;
	}
	while (1) {
		Sleep(5000);
		cout << "server running" << endl;
	}


	return 0;
}