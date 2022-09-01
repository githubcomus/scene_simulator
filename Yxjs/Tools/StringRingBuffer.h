#pragma once

#include <mutex>
#include <string>
//using namespace std;

class StringRingBuffer
{
public:

	struct RingInfo
	{
		std::string str = "";
		int type = 0;
	};

	//
	StringRingBuffer(int arraySize);

	//
	~StringRingBuffer();

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
	static unsigned __stdcall StringRingBufferTestThread1(void* arg);
	static unsigned __stdcall StringRingBufferTestThread2(void* arg);
	void StringRingBufferTestThreadFunc1();
	void StringRingBufferTestThreadFunc2();
	void Test1();
	void LockTest(std::string str);

	// 测试环形缓冲区
	static unsigned __stdcall StringRingBufferTestThread3(void* arg);
	static unsigned __stdcall StringRingBufferTestThread4(void* arg);
	void StringRingBufferTestThreadFunc3();
	void StringRingBufferTestThreadFunc4();
	void Test3();

	// 测试数组
	void Test2();
};

