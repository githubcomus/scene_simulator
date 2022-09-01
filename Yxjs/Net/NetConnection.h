#pragma once

#include <memory>
#include <atomic>
#include <list>
#include <string>
//using namespace std;

class UNetMainComponent;
class NetRingBuffer;

/*------------------------------------------------------------------*/

// 协议头
#define NetTag 0xa
#define NetHeadSize 10

// 最大接收缓存
#define MaxRecvDataSize 1024*1024
#define MaxRecvMaxDataSize 1024*1024
#define MaxSendDataSize 1024*1024



class NetConnection
{
public:

	// 连接状态
	enum ConntentState
	{
		ConntentState_None,
		ConntentState_OK,
		ConntentState_ERR,
		ConntentState_Colse,
	};

	// 线程状态
	enum ThreadType
	{
		ThreadWait,
		ThreadRun,
		ThreadStoping,
		ThreadStop,
		ThreadRecvStop,
	};

	// 套接字线程数据
	struct CvSocketRecvInfo
	{
		// 线程状态
		int state;

		// 连接状态
		int conntentState;

		// socket句柄参数
		int socketHandle;

		// 端口
		int port;

		// ip
		char ip[50];

		// 上一次发送消息时间
		float lastSendTime;

		// 总的接收数据
		std::shared_ptr<char*> recvData;

		// 总的接收数据大小
		int recvDataSize;

		// 当前帧的数据大小
		int nowFrameSize;

		// 当前数据下标
		int nowDataIndex;

		// 接收数据环形缓冲区
		NetRingBuffer* ringBuffer;
	};

	// 发送线程信息
	struct CvSocketSendInfo
	{
		// 线程状态
		int state;

		// 接收数据
		std::shared_ptr<char*> sendData;

		// 接收数据大小
		int sendDataSize;

		// 发送数据锁
		std::atomic<bool> sendLock;

		// 最后发送时间
		float lastSendTime;

		// 发送数据环形缓冲区
		NetRingBuffer* ringBuffer;
	};

	// 测试线程信息
	struct CvTestFuncInfo
	{
		// 线程状态
		int state;

		// 发送计数
		int sendNumber;

		// 发送开关
		bool sendButton;
	};

	/*------------------------------------------------------------------*/

	// 是否已经启动连接服务器
	bool isConnect;

	// 套接字线程数据
	CvSocketRecvInfo cvSocketRecvInfo;

	// 发送线程信息
	CvSocketSendInfo cvSocketSendInfo;

	// 测试线程信息
	CvTestFuncInfo cvTestFuncInfo;

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UNetMainComponent> netMain;

	//
	NetConnection(UNetMainComponent* netMain_);

	//
	~NetConnection();

	// 连接服务器
	void Start(std::string ip, int port);

	// 关闭服务器
	void End(bool isExternal);

	// 网络线程 启动关闭线程
	void SocektRecvController(bool flag, bool isExternal);

	// 网络线程
	static unsigned __stdcall SocektRecvThread(void* arg);

	// 网络线程 线程逻辑
	void SocektRecvThreadFunc();

	// 网络线程 解析数据
	void AnalyticalData(char* inData, int inSize);

	// 网络线程 验证数据
	bool VerifyTheData(char* inData, int inSize, int& frameSize, int& type);

	// 网络线程 处理单个包
	void AnalyticalDataFrame();

	// 网络线程 处理单个包
	void AnalyticalDataFrameReal(int type, char* inData, int inSize);

	// 发送线程 心跳机制
	void SocektSendController(bool flag);

	// 网络线程 线程
	static unsigned __stdcall SocektSendThread(void* arg);

	// 网络线程 线程
	void SocektSendThreadFunc();

	// 发送线程 发送数据
	bool SendData(int type, char* data, int dataSize);

	// 发送线程 最外层发送协议
	void MackSendData(int type, char* inData, int inDataSize, char* outData, int& outDataSize);

	// 显示日志
	void ShowLog(char* str);

	// 业务 显示内存
	void ShowBuf(char* str, int size);

	// 测试线程
	void TestFuncController(bool flag);

	// 测试线程
	static unsigned __stdcall TestFuncThread(void* arg);

	// 测试线程
	void TestFuncThreadFunc();

	// 测试 发送一条规范的消息
	void TestSendData();

	/*------------------------------------------------------------------*/
	//
	//    // 读写
	//    bool Write(string data);
	//    bool Read(string &data);
	//    void ShowBuffer();
	//
	//private:
	//    // 变量
	//    mutex *lock1;
	//    string *pStringList;
	//    int start;
	//    int end;
	//    int readNum;
	//    int writeNum;
	//    int readSize;// 未读数量
	//
	//    // 测试锁
	//    static unsigned __stdcall NetConnectionTestThread1(void* arg);
	//    static unsigned __stdcall NetConnectionTestThread2(void* arg);
	//    void NetConnectionTestThreadFunc1();
	//    void NetConnectionTestThreadFunc2();
	//    void Test1();
	//    void LockTest(string str);
	//
	//    // 测试环形缓冲区
	//    static unsigned __stdcall NetConnectionTestThread3(void* arg);
	//    static unsigned __stdcall NetConnectionTestThread4(void* arg);
	//    void NetConnectionTestThreadFunc3();
	//    void NetConnectionTestThreadFunc4();
	//    void Test3();
	//
	//    // 测试数组
	//    void Test2();
};
