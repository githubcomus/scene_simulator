#include "RingBuffer.h"

#include <map>
#include <memory>
#include <list>
//#include <process.h>
//#include <windows.h>

/*------------------------------------------------------------------*/

//
RingBuffer::RingBuffer(int arraySize)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [RingBuffer::RingBuffer]"), this);
	//lock1 = new std::mutex();
	lock1 = new std::mutex();
	pStringList = new std::string[arraySize];
	start = 0;
	end = arraySize;
	readNum = 0;
	writeNum = 0;
	readSize = 0;
	//Test1();
	//Test2();
	//Test3();
}

//
RingBuffer::~RingBuffer()
{
	delete lock1;
	delete[] pStringList;
}

//
bool RingBuffer::Write(std::string data)
{
	lock1->lock();
	bool result = true;
	if (readSize == 0 && writeNum == readNum)
	{
		// 整个缓冲区已经被读完了,或者刚开始读写
		pStringList[writeNum] = data;
		if (writeNum == end - 1)
		{
			writeNum = 0;
		}
		else
		{
			writeNum++;
		}
		readSize++;
	}
	else if (readSize == end)
	{
		// 整个缓冲已经写满了
		result = false;
	}
	else if (writeNum != readNum)
	{
		// 读写指针有差距
		pStringList[writeNum] = data;
		if (writeNum == end - 1)
		{
			writeNum = 0;
		}
		else
		{
			writeNum++;
		}
		readSize++;
	}
	lock1->unlock();
	return result;
}

//
bool RingBuffer::Read(std::string& data)
{
	lock1->lock();
	bool result = false;
	if (readSize == 0 && writeNum == readNum)
	{
		// 整个缓冲区已经被读完了,或者刚开始读写
	}
	else if (readSize == end)
	{
		// 整个缓冲区已经写满了,跟下面一样的逻辑,分开只是为了调试方便.
		data = pStringList[readNum];
		if (readNum == end - 1)
		{
			readNum = 0;
		}
		else
		{
			readNum++;
		}
		readSize--;
		result = true;
	}
	else if (writeNum != readNum)
	{
		// 读写指针有差距
		data = pStringList[readNum];
		if (readNum == end - 1)
		{
			readNum = 0;
		}
		else
		{
			readNum++;
		}
		readSize--;
		result = true;
	}

	lock1->unlock();
	return result;
}

//
void RingBuffer::ShowBuffer()
{
	char temp[1024];
	sprintf_s(temp, 120, "[read:%d write:%d][ReadSize:%d] ", readNum, writeNum, readSize);
	// W可写 R待读取
	for (int i = 0; i < end; i++)
	{
		if (readNum < writeNum)
		{
			if (i >= readNum && i < writeNum)
			{
				strcat_s(temp, 1024, "R");
			}
			else
			{
				strcat_s(temp, 1024, "W");
			}
		}
		else if (readNum > writeNum)
		{
			if (i >= writeNum && i < readNum)
			{
				strcat_s(temp, 1024, "W");
			}
			else
			{
				strcat_s(temp, 1024, "R");
			}
		}
		else
		{
			// readNum == writeNum,[0,0]初始化 [3,3]全部可写/全部可读
			if (readSize == 0)
			{
				// 全部可写
				strcat_s(temp, 1024, "W");
			}
			else
			{
				// 可读
				strcat_s(temp, 1024, "R");
			}
		}
	}
	printf("[ShowBuffer] %s\n", temp);
}

/*------------------------------------------------------------------*/

// 锁测试线程
unsigned __stdcall RingBuffer::RingBufferTestThread1(void* arg)
{
	RingBuffer* gwi = (RingBuffer*)arg;
	gwi->RingBufferTestThreadFunc1();
	return 0;
}

// 锁测试线程
unsigned __stdcall RingBuffer::RingBufferTestThread2(void* arg)
{
	RingBuffer* gwi = (RingBuffer*)arg;
	gwi->RingBufferTestThreadFunc2();
	return 0;
}

// 注册读取
void RingBuffer::RingBufferTestThreadFunc1()
{
	printf("[RingBufferTestThreadFunc1] ready go \n");
	std::string str = "111111111";
	LockTest(str);

	printf("[RingBufferTestThreadFunc1] over \n");
}

// 写入
void RingBuffer::RingBufferTestThreadFunc2()
{
	//printf("[RingBufferTestThreadFunc2] ready go \n");
	//Sleep(1000 * 1);
	//std::string str = "222222222";
	//LockTest(str);


	//printf("[RingBufferTestThreadFunc2] over \n");
}

// 测试锁
void RingBuffer::Test1()
{
	//_beginthreadex(NULL, 0, RingBufferTestThread1, this, 0, NULL);
	//_beginthreadex(NULL, 0, RingBufferTestThread2, this, 0, NULL);
}

// 测试锁
void RingBuffer::LockTest(std::string str)
{
	//printf("[LockTest] [%s] a   \n", str.c_str());
	//lock1->lock();

	//printf("[LockTest] [%s] b   \n", str.c_str());
	//Sleep(3000);

	//lock1->unlock();
	//printf("[LockTest] [%s] c   \n", str.c_str());
}

// 测试动态数组
void RingBuffer::Test2()
{
	// list插入指定位置会将原来的数据往下挤压
	if (false)
	{
		int arraySize = 33;

		char temp[128];
		std::list<std::string> list1(arraySize);
		for (int i = 0; i < 32; i++)
		{
			sprintf_s(temp, 128, "ttttt %d", i);
			std::string temp2 = temp;
			std::list<std::string>::iterator itor = list1.begin();
			std::advance(itor, i);
			list1.insert(itor, temp2);
		}
		for (int i = 13; i < 22; i++)
		{
			sprintf_s(temp, 128, "aaaaa %d", i);
			std::string temp2 = temp;
			std::list<std::string>::iterator itor = list1.begin();
			std::advance(itor, i);
			list1.insert(itor, temp2);
		}

		//printf(" [size] %d %d\n",(int)list1.size(),(int)list1.max_size());

		int listNum = 0;
		std::string* str1 = NULL;
		std::list<std::string>::iterator itor;
		for (itor = list1.begin(); itor != list1.end(); ++itor)
		{
			str1 = &(*itor);

			//printf("[%d] %s\n", listNum, (*itor).c_str());
			printf("[%d] %s\n", listNum, str1->c_str());
			listNum++;
		}
	}

	if (false)
	{
		// array的.list / 数组 / map./ new[] 4种方式.
		std::list<std::string> list1;
		std::map<int, std::string> list2;
		std::string list3[100];
		std::string* list4 = new std::string[100];
	}
	if (true)
	{
		int arraySize = 100;
		std::string* list1 = new std::string[arraySize];
		char temp[128];
		for (int i = 0; i < arraySize; i++)
		{
			sprintf_s(temp, 128, "ttttt %d", i);
			list1[i] = temp;
		}

		for (int i = 0; i < arraySize; i++)
		{
			printf("-- [%d] %s\n", i, list1[i].c_str());
		}
	}
}


// 环形缓冲测试线程
unsigned __stdcall RingBuffer::RingBufferTestThread3(void* arg)
{
	RingBuffer* gwi = (RingBuffer*)arg;
	gwi->RingBufferTestThreadFunc3();
	return 0;
}

// 环形缓冲测试线程
unsigned __stdcall RingBuffer::RingBufferTestThread4(void* arg)
{
	RingBuffer* gwi = (RingBuffer*)arg;
	gwi->RingBufferTestThreadFunc4();
	return 0;
}

// 读取
void RingBuffer::RingBufferTestThreadFunc3()
{
	//printf("[RingBufferTestThreadFunc3] start \n");
	//int i = 0;
	//while (true) {
	//    ShowBuffer();
	//    std::string str;
	//    bool result=Read(str);
	//    ShowBuffer();
	//    if(result==false)
	//        printf("[read] [%d] [result:%d]  [read:%d write : %d] [ReadSize:%d] \n", i, result, readNum, writeNum, readSize);

	//    i++;
	//    Sleep(200);
	//}

	//printf("[RingBufferTestThreadFunc3] over \n");
}

// 写入
void RingBuffer::RingBufferTestThreadFunc4()
{
	//printf("[RingBufferTestThreadFunc4] start \n");
	//int i = 0;
	//while (true) {
	//    bool result=Write("asdfasf");
	//    if (result == false) {
	//        printf("[write] [%d] [result:%d]  [read:%d write : %d] [ReadSize:%d] \n", i, result, readNum, writeNum, readSize);
	//    }
	//    i++;
	//    Sleep(500);
	//}

	//printf("[RingBufferTestThreadFunc4] over \n");
}

// 测试环形缓冲区
void RingBuffer::Test3()
{
	//_beginthreadex(NULL, 0, RingBufferTestThread3, this, 0, NULL);
	//_beginthreadex(NULL, 0, RingBufferTestThread4, this, 0, NULL);

	//// 写入前后缓存变化
	//ShowBuffer();
	//bool result = Write("asdfasf");
	//printf("[write] [result:%d]\n", result);
	//ShowBuffer();
}
