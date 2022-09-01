#pragma once


#include <mutex>
#include <string>
//using namespace std;





class RingBuffer
{


public:

	//
	RingBuffer(int arraySize);

	//
	~RingBuffer();

	// 读写
	bool Write(std::string data);
	bool Read(std::string& data);
	void ShowBuffer();

private:
	// 变量
	std::mutex* lock1;
	std::string* pStringList;
	int start;
	int end;
	int readNum;
	int writeNum;
	int readSize;// 未读数量

	// 测试锁
	static unsigned __stdcall RingBufferTestThread1(void* arg);
	static unsigned __stdcall RingBufferTestThread2(void* arg);
	void RingBufferTestThreadFunc1();
	void RingBufferTestThreadFunc2();
	void Test1();
	void LockTest(std::string str);

	// 测试环形缓冲区
	static unsigned __stdcall RingBufferTestThread3(void* arg);
	static unsigned __stdcall RingBufferTestThread4(void* arg);
	void RingBufferTestThreadFunc3();
	void RingBufferTestThreadFunc4();
	void Test3();

	// 测试数组
	void Test2();

};

