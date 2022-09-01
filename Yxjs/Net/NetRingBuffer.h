#pragma once


#include <mutex>
#include <string>
//using namespace std;





class NetRingBuffer
{


public:

	struct RingInfo
	{
		// string str;
		char* data = nullptr;
		int dataSize = 0;
		int type = 0;
	};

	//
	NetRingBuffer(int arraySize);

	//
	~NetRingBuffer();

	// 读写
	bool Write(RingInfo& data);
	bool Read(RingInfo& data);
	void ShowBuffer();

private:
	// 变量
	std::mutex* lock1;
	RingInfo* pStringList;
	int start;
	int end;
	int readNum;
	int writeNum;
	int readSize;// 未读数量

	// 测试锁
	static unsigned __stdcall NetRingBufferTestThread1(void* arg);
	static unsigned __stdcall NetRingBufferTestThread2(void* arg);
	void NetRingBufferTestThreadFunc1();
	void NetRingBufferTestThreadFunc2();
	void Test1();
	void LockTest(std::string str);

	// 测试环形缓冲区
	static unsigned __stdcall NetRingBufferTestThread3(void* arg);
	static unsigned __stdcall NetRingBufferTestThread4(void* arg);
	void NetRingBufferTestThreadFunc3();
	void NetRingBufferTestThreadFunc4();
	void Test3();

	// 测试数组
	void Test2();

};

