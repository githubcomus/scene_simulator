#include "NetConnection.h"
#include "NetMainComponent.h"
#include "NetTool.h"
#include "NetRingBuffer.h"

#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <memory>
#include <list>
#include <process.h>

//--------冲突的头文件--------//
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"

#include <Ws2tcpip.h>  //InetNtop 和InetPton 声明的文件
#include <windows.h>
#include <winsock2.h>//管道

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
//--------冲突的头文件--------//


#pragma comment(lib,"wsock32.lib")


/*------------------------------------------------------------------*/

//
NetConnection::NetConnection(UNetMainComponent* netMain_)
{
	netMain = netMain_;
	std::string str = "[NetConnection::NetConnection]";
	netMain->ShowLog(str);
	isConnect = false;
	cvSocketRecvInfo.ringBuffer = new NetRingBuffer(1024 * 1024);
	cvSocketSendInfo.ringBuffer = new NetRingBuffer(1024 * 1024);
}

//
NetConnection::~NetConnection()
{
	std::string str = "[NetConnection::~NetConnection]";
	netMain->ShowLog(str);
	netMain = nullptr;
	delete cvSocketRecvInfo.ringBuffer;
	delete cvSocketSendInfo.ringBuffer;
}

/*------------------------------------------------------------------*/

// 连接服务器
void NetConnection::Start(std::string ip, int port)
{
	if (isConnect == false)
	{
		isConnect = true;
		sprintf_s(cvSocketRecvInfo.ip, 50, "%s", ip.c_str());
		cvSocketRecvInfo.port = port;
		cvSocketRecvInfo.state = ThreadWait;
		SocektRecvController(true, true);
	}
	else
	{
		std::string str = "[NetConnection::Start] ERR ";
		netMain->ShowLog(str);
	}
}

// 关闭服务器
void NetConnection::End(bool isExternal)
{
	// 分为自身线程退出和外部线程退出
	// 自身线程退出recv时不能发closesocket消息,而是直接关闭
	if (isConnect)
	{
		SocektRecvController(false, isExternal);
		isConnect = false;
	}
	else
	{
		std::string str = "[NetConnection::End] ERR ";
		netMain->ShowLog(str);
	}
}

/*------------------------------------------------------------------*/

// 网络线程 启动关闭线程
void NetConnection::SocektRecvController(bool flag, bool isExternal)
{
	if (flag && cvSocketRecvInfo.state == ThreadWait)
	{
		_beginthreadex(nullptr, 0, SocektRecvThread, this, 0, nullptr);
	}
	// 外部线程退出
	else if (flag == false && cvSocketRecvInfo.state == ThreadRun && isExternal == true)
	{
		cvSocketRecvInfo.state = ThreadStoping;
		// 发送主动断线
		if (cvSocketRecvInfo.conntentState == ConntentState_OK)
		{
			closesocket(cvSocketRecvInfo.socketHandle);
		}
		bool flag2 = false;
		// 等待20s没有退出,错误.
		for (int i = 0; i < 200; i++)
		{
			if (cvSocketRecvInfo.state == ThreadStop)
			{
				flag2 = true;
				break;
			}
			FPlatformProcess::Sleep(0.05);
		}
		// ERR
		if (flag2 == false)
		{
			std::string str = "[SocektRecvController] ERR1 ";
			netMain->ShowLog(str);
		}
	}
	// ERR
	else
	{
		std::string str = "[SocektRecvController] ERR2 ";
		netMain->ShowLog(str);
	}
}

// 网络线程 线程
unsigned __stdcall NetConnection::SocektRecvThread(void* arg)
{
	NetConnection* gwi = (NetConnection*)arg;
	gwi->SocektRecvThreadFunc();
	return 0;
}

// 网络线程 线程
void NetConnection::SocektRecvThreadFunc()
{
	ShowLog("[SocektRecvThreadFunc] start ");

	cvSocketRecvInfo.state = ThreadRun;
	cvSocketRecvInfo.recvData = std::make_shared<char*>(new char[MaxRecvMaxDataSize]);
	memset(*cvSocketRecvInfo.recvData, 0, MaxRecvMaxDataSize);
	cvSocketRecvInfo.recvDataSize = 0;

	std::shared_ptr<char*> recvData = std::make_shared<char*>(new char[MaxRecvDataSize]);

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(struct sockaddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(cvSocketRecvInfo.port);
	InetPtonA(AF_INET, cvSocketRecvInfo.ip, &serverAddr.sin_addr.s_addr);

	std::string str = "[SocektRecvThreadFunc] 准备连接 " + netMain->GetTimeStr();
	ShowLog((char*)str.c_str());

	// 初始化网络
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		ShowLog("[SocektRecvThreadFunc] WSAStartup err ");
		WSACleanup();
		cvSocketRecvInfo.conntentState = ConntentState_None;
		cvSocketRecvInfo.state = ThreadStop;
		if (netMain.IsValid())
		{
			netMain->Callback_ConnectFailed();
		}
		return;
	}

	// 初始化套接字
	if ((cvSocketRecvInfo.socketHandle = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		ShowLog("[SocektRecvThreadFunc]  socket err ");
		closesocket(cvSocketRecvInfo.socketHandle);
		WSACleanup();
		cvSocketRecvInfo.conntentState = ConntentState_None;
		cvSocketRecvInfo.state = ThreadStop;

		if (netMain.IsValid())
		{
			netMain->Callback_ConnectFailed();
		}
		return;
	}

	// 连接服务端
	if (connect(cvSocketRecvInfo.socketHandle, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)) == -1)
	{
		ShowLog("[SocektRecvThreadFunc] connect err ");
		closesocket(cvSocketRecvInfo.socketHandle);
		WSACleanup();
		cvSocketRecvInfo.conntentState = ConntentState_None;
		cvSocketRecvInfo.state = ThreadStop;

		if (netMain.IsValid())
		{
			netMain->Callback_ConnectFailed();
		}
		return;
	}

	// 设置超时时长,超过这个时间没有发送数据则阻塞会自动退出.
	int netTimeout = 30 * 1000;
	setsockopt(cvSocketRecvInfo.socketHandle, SOL_SOCKET, SO_RCVTIMEO, (char*)&netTimeout, sizeof(int));

	cvSocketRecvInfo.conntentState = ConntentState_OK;

	std::string str1 = "[SocektRecvThreadFunc] 连接成功 " + netMain->GetTimeStr();
	ShowLog((char*)str1.c_str());

	// 启动接收线程
	cvSocketSendInfo.state = ThreadWait;
	SocektSendController(true);
	// 发起一个登陆请求
	netMain->Callback_ConnectSuccess();

	int recvErr = 0;
	while (true)
	{
		recvErr = recv(cvSocketRecvInfo.socketHandle, *recvData, MaxRecvDataSize, 0);

		// 正常接收
		if (recvErr > 0)
		{
			AnalyticalData(*recvData, recvErr);
			if (recvErr < MaxRecvDataSize) memset(*recvData, 0, recvErr + 1);
			else memset(*recvData, 0, MaxRecvDataSize);

			// 接收到退出消息
			if (cvSocketRecvInfo.state == ThreadRecvStop)
			{
				break;
			}
		}
		// 正常断开
		else if (recvErr == 0)
		{
			break;
		}
		// 连接出错
		else if (recvErr < 0)
		{
			int err2 = WSAGetLastError();
			if (err2 == WSAETIMEDOUT || err2 == EWOULDBLOCK || err2 == WSAEWOULDBLOCK || err2 == WSAEINTR)
			{
				continue;
			}
			//跳出接收循环
			if (netMain.IsValid())
			{
				netMain->Callback_ConnectFailed();
			}
			break;
		}
	}

	SocektSendController(false);

	closesocket(cvSocketRecvInfo.socketHandle);
	WSACleanup();

	cvSocketRecvInfo.conntentState = ConntentState_None;
	cvSocketRecvInfo.state = ThreadStop;

	ShowLog("[SocektRecvThreadFunc] end ");
}

// 网络线程 解析缓冲区数据
void NetConnection::AnalyticalData(char* inData, int inSize)
{
	// 总缓冲区
	memcpy(*cvSocketRecvInfo.recvData + cvSocketRecvInfo.recvDataSize, inData, inSize);
	cvSocketRecvInfo.recvDataSize += inSize;
	cvSocketRecvInfo.nowDataIndex = 0;

	//{
	//    string str2 = "[AnalyticalData] 解析 --------------------------------   inSize " + NetTool::IntToString(inSize)
	//                  + " recvDataSize " + NetTool::IntToString(cvSocketRecvInfo.recvDataSize);
	//    ShowLog((char*)str2.c_str());
	//}
	if (netMain.IsValid() == false)return;
	AnalyticalDataFrame();
}

// 网络线程 处理单个包
void NetConnection::AnalyticalDataFrame()
{
	char* inData = *cvSocketRecvInfo.recvData + cvSocketRecvInfo.nowDataIndex;
	int inSize = cvSocketRecvInfo.recvDataSize - cvSocketRecvInfo.nowDataIndex;

	// 单帧
	int nowFrameSize = 0;
	int type;
	bool falg = VerifyTheData(inData, inSize, nowFrameSize, type);
	if (falg && nowFrameSize == inSize)
	{
		//{
		//    string str2 = "[AnalyticalDataFrame] 单帧 nowFrameSize:" + NetTool::IntToString(nowFrameSize)
		//                  + " inSize:" + NetTool::IntToString(inSize)
		//                  + " 游标:" + NetTool::IntToString(cvSocketRecvInfo.nowDataIndex)
		//                  + " /" + NetTool::IntToString(cvSocketRecvInfo.recvDataSize)
		//                  ;
		//    ShowLog((char*)str2.c_str());
		//}

		AnalyticalDataFrameReal(type, inData, inSize);
		memset(inData, 0, inSize);
		cvSocketRecvInfo.recvDataSize = 0;
	}
	// 多帧
	else if (falg && nowFrameSize < inSize)
	{
		//{
		//    string str2 = "[AnalyticalDataFrame] 多帧 nowFrameSize:" + NetTool::IntToString(nowFrameSize)
		//                  + " inSize:" + NetTool::IntToString(inSize)
		//                  + " 游标:" + NetTool::IntToString(cvSocketRecvInfo.nowDataIndex)
		//                  + " /" + NetTool::IntToString(cvSocketRecvInfo.recvDataSize);
		//    ShowLog((char*)str2.c_str());
		//}

		AnalyticalDataFrameReal(type, inData, nowFrameSize);
		memset(inData, 0, nowFrameSize);
		cvSocketRecvInfo.nowDataIndex += nowFrameSize;

		AnalyticalDataFrame();
	}
	// 半帧
	else if (falg && nowFrameSize > inSize)
	{
		//{
		//    string str2 = "[AnalyticalDataFrame] 半帧 nowFrameSize:" + NetTool::IntToString(nowFrameSize)
		//                  + " inSize:" + NetTool::IntToString(inSize)
		//                  + " 游标:" + NetTool::IntToString(cvSocketRecvInfo.nowDataIndex)
		//                  + " /" + NetTool::IntToString(cvSocketRecvInfo.recvDataSize);
		//    ShowLog((char*)str2.c_str());
		//}

		if (cvSocketRecvInfo.nowDataIndex != 0)
		{
			memcpy(*cvSocketRecvInfo.recvData, inData, inSize);
			cvSocketRecvInfo.recvDataSize = inSize;
		}
	}
	// 错误帧
	else if (falg == false)
	{
		{
			std::string str2 = "[AnalyticalDataFrame] ERR 错误帧 nowFrameSize:" + NetTool::IntToString(nowFrameSize) +
				" inSize:" + NetTool::IntToString(inSize);
			ShowLog((char*)str2.c_str());
			ShowBuf(inData, inSize);
		}

		// 重新搜索tag
		// 干掉所有垃圾数据
		memset(inData, 0, inSize);
		cvSocketRecvInfo.recvDataSize = 0;
	}
}

// 网络线程 处理单个包
void NetConnection::AnalyticalDataFrameReal(int type, char* inData, int inSize)
{
	//string str2 = " [消息] [AnalyticalDataFrameReal] type:" + std::to_string(type) + " inSize:" + NetTool::IntToString(inSize); // +"  [" + (inData + NetHeadSize) + "]";
	//ShowLog((char*)str2.c_str());

	char* data_ = (inData + NetHeadSize);
	int dataSize = inSize - NetHeadSize;

	// ShowBuf(data_, dataSize);

	netMain->NetCallback_MsgBuf(type, data_, dataSize);
}

/*------------------------------------------------------------------*/

// 发送线程 心跳机制
void NetConnection::SocektSendController(bool flag)
{
	if (flag && cvSocketSendInfo.state == ThreadWait)
	{
		cvSocketSendInfo.sendData = std::make_shared<char*>(new char[MaxRecvMaxDataSize]);
		memset(*cvSocketSendInfo.sendData.get(), 0, MaxSendDataSize);
		cvSocketSendInfo.sendDataSize = 0;
		cvSocketSendInfo.sendLock = false;
		cvSocketSendInfo.lastSendTime = 0;

		cvSocketSendInfo.state = ThreadRun;

		_beginthreadex(nullptr, 0, SocektSendThread, this, 0, nullptr);
	}
	else if (flag == false && cvSocketSendInfo.state == ThreadRun)
	{
		cvSocketSendInfo.state = ThreadStoping;

		bool flag2 = false;
		// 等待20s没有退出,错误.
		for (int i = 0; i < 100; i++)
		{
			if (cvSocketSendInfo.state == ThreadStop)
			{
				flag2 = true;
				break;
			}
			FPlatformProcess::Sleep(0.05);
		}
		// ERR
		if (flag2 == false)
		{
			std::string str = "[SocektSendController] ERR1 ";
			netMain->ShowLog(str);
		}
	}
	// ERR
	else
	{
		std::string str = "[SocektSendController] ERR2 ";
		netMain->ShowLog(str);
	}
}

// 网络线程 线程
unsigned __stdcall NetConnection::SocektSendThread(void* arg)
{
	NetConnection* gwi = (NetConnection*)arg;
	gwi->SocektSendThreadFunc();
	return 0;
}

// 网络线程 线程
void NetConnection::SocektSendThreadFunc()
{
	ShowLog("[SocektSendThreadFunc] start ");

	NetRingBuffer::RingInfo sendData;
	sendData.data = (char*)malloc(1024 * 100);

	while (cvSocketSendInfo.state == ThreadRun)
	{
		if (cvSocketRecvInfo.conntentState == ConntentState_OK && netMain.IsValid())
		{
			auto nowTime = netMain->GetTime();
			// 发送数据
			if (cvSocketSendInfo.ringBuffer->Read(sendData))
			{
				SendData(sendData.type, sendData.data, sendData.dataSize);
			}
			// 发送心跳
			else
			{
				if (nowTime - cvSocketSendInfo.lastSendTime > 30)
				{
					std::string str2 = "心跳";
					SendData(9999, (char*)str2.c_str(), str2.length());
				}
				else
				{
					Sleep(10);
				}
			}
		}
		else
		{
			Sleep(10);
		}
	}

	free(sendData.data);
	cvSocketSendInfo.state = ThreadStop;

	ShowLog("[SocektSendThreadFunc] end ");
}

// 网络线程 发送数据
bool NetConnection::SendData(int type, char* data, int dataSize)
{
	bool flag = false;
	if (cvSocketSendInfo.sendLock == false)
	{
		cvSocketSendInfo.sendLock = true;
		MackSendData(type, data, dataSize, *cvSocketSendInfo.sendData.get(), cvSocketSendInfo.sendDataSize);
		if (cvSocketRecvInfo.conntentState == ConntentState_OK)
		{
			if (send(cvSocketRecvInfo.socketHandle, *cvSocketSendInfo.sendData.get(), cvSocketSendInfo.sendDataSize,
				0) == -1)
			{
				ShowLog("[SendData] ERR-------------------------ERR ");
				std::string sendStr = data;
				ShowLog((char*)sendStr.c_str());
				ShowLog("[SendData] ERR-------------------------ERR ");
			}
			else
			{
				//string sendStr = data;
				//ShowLog((char*)sendStr.c_str());
				flag = true;
			}
		}
		cvSocketSendInfo.sendLock = false;
		cvSocketSendInfo.lastSendTime = netMain->GetTime();
	}
	return flag;
}

/*------------------------------------------------------------------*/

// 外层协议 编码
void NetConnection::MackSendData(int type, char* inData, int inDataSize, char* outData, int& outDataSize)
{
	// 发送协议
	// |TAG     |Type				|dataSize           |data |
	// |0xa 0xa |0x00 0x00 0x00 0x03|0x00 0x00 0x00 0x03|a b c|
	memset(outData, 0, MaxSendDataSize);

	// tag
	outData[0] = NetTag;
	outData[1] = NetTag;

	// type
	{
		char numberBuf[4];
		memcpy(numberBuf, &type, sizeof(int));
		outData[2] = numberBuf[3];
		outData[3] = numberBuf[2];
		outData[4] = numberBuf[1];
		outData[5] = numberBuf[0];
	}

	// lenght
	{
		char numberBuf[4];
		memcpy(numberBuf, &inDataSize, sizeof(int));
		outData[6] = numberBuf[3];
		outData[7] = numberBuf[2];
		outData[8] = numberBuf[1];
		outData[9] = numberBuf[0];
	}

	// data
	memcpy(outData + NetHeadSize, inData, inDataSize);
	outDataSize = inDataSize + NetHeadSize;
}

// 外层协议 解码
bool NetConnection::VerifyTheData(char* inData, int inSize, int& frameSize, int& type)
{
	// 接收协议
	// |dataSize           |TAG    |Type				|data |
	// |0x00 0x00 0x00 0x07|0xa 0xa|0x00 0x00 0x00 0x03 |a b c|

	if (inData[4] == NetTag && inData[5] == NetTag)
	{
		{
			int dataSize = 0;
			char sizeBuf[4];
			sizeBuf[0] = inData[3];
			sizeBuf[1] = inData[2];
			sizeBuf[2] = inData[1];
			sizeBuf[3] = inData[0];
			memcpy(&dataSize, sizeBuf, 4);
			frameSize = dataSize + 4;
		}
		{
			char sizeBuf[4];
			sizeBuf[0] = inData[9];
			sizeBuf[1] = inData[8];
			sizeBuf[2] = inData[7];
			sizeBuf[3] = inData[6];
			memcpy(&type, sizeBuf, 4);
		}
		return true;
	}
	return false;
}

/*------------------------------------------------------------------*/

// 业务 显示日志
void NetConnection::ShowLog(char* str)
{
	std::string str2 = str;
	netMain->ShowLog(str2);
}

// 业务 显示内存
void NetConnection::ShowBuf(char* data, int size)
{
	std::string str;
	char temp1[10];
	for (int i = 0; i < size; i++)
	{
		auto var = data[i];
		memset(temp1, 0, 10);
		sprintf_s(temp1, 10, ",%2x", var);
		str = str + temp1;
	}
	str = "[" + str + "]";
	ShowLog((char*)str.c_str());
}

/*------------------------------------------------------------------*/

// 测试线程
void NetConnection::TestFuncController(bool flag)
{
	// 启动
	if (flag && cvTestFuncInfo.state == ThreadWait)
	{
		cvTestFuncInfo.state = ThreadRun;
		cvTestFuncInfo.sendNumber = 0;
		cvTestFuncInfo.sendButton = false;

		_beginthreadex(nullptr, 0, TestFuncThread, this, 0, nullptr);
	}
	// 关闭
	else if (flag == false && cvTestFuncInfo.state == ThreadRun)
	{
		cvTestFuncInfo.state = ThreadStoping;

		bool flag2 = false;
		// 等待20s没有退出,错误.
		for (int i = 0; i < 100; i++)
		{
			if (cvTestFuncInfo.state == ThreadStop)
			{
				flag2 = true;
				break;
			}
			FPlatformProcess::Sleep(0.05);
		}
		// ERR
		if (flag2 == false)
		{
			std::string str = "[TestFuncController] ERR1 ";
			netMain->ShowLog(str);
		}
	}
	// 错误
	else
	{
		std::string str = "[TestFuncController] ERR2 ";
		netMain->ShowLog(str);
	}
}

// 测试线程
unsigned __stdcall NetConnection::TestFuncThread(void* arg)
{
	NetConnection* gwi = (NetConnection*)arg;
	gwi->TestFuncThreadFunc();
	return 0;
}

// 测试线程
void NetConnection::TestFuncThreadFunc()
{
	ShowLog("[TestFuncThreadFunc] start ");

	int tempVar = 0;
	// 等待5秒,50次查询过是否退出
	int tempVar2 = 5 * 10;

	while (cvTestFuncInfo.state == ThreadRun)
	{
		if (cvTestFuncInfo.sendButton)
		{
			std::string str = "[TestFuncThreadFunc] 发送开始 nmuber:" + NetTool::IntToString(cvTestFuncInfo.sendNumber);
			ShowLog((char*)str.c_str());

			if (cvSocketRecvInfo.conntentState == ConntentState_OK)
			{
				for (int i = 0; i < 300; i++)
				{
					std::string str2 = "aabbaaccddeessr__[" + NetTool::IntToString(cvTestFuncInfo.sendNumber) + "]__";
					SendData(0x01, (char*)str2.c_str(), str2.length());
					cvTestFuncInfo.sendNumber++;
				}
			}

			cvTestFuncInfo.sendButton = false;
			std::string str3 = "[TestFuncThreadFunc] 发送结束 nmuber:" + NetTool::IntToString(cvTestFuncInfo.sendNumber);
			ShowLog((char*)str3.c_str());
		}

		// 退出时快速响应主线程
		//for (tempVar = 0; tempVar < tempVar2; tempVar++)
		//{
		//    if (cvTestFuncInfo.state != ThreadType::ThreadRun)
		//    {
		//        break;
		//    }
		//    Sleep(100);
		//}
		Sleep(100);
	}

	cvTestFuncInfo.state = ThreadStop;

	ShowLog("[TestFuncThreadFunc] end ");
}

// 测试 发送一条规范的消息
void NetConnection::TestSendData()
{
	// 发送超过缓存
	cvTestFuncInfo.sendButton = true;
	//char* str = "aabbaaccddeessr";
	//SendData(str, strlen(str));
}

/*------------------------------------------------------------------*/
