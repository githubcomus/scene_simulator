#include "StringRingBuffer.h"

#include <map>
#include <memory>
#include <list>
//#include <process.h>
//#include <windows.h>

/*------------------------------------------------------------------*/

//
StringRingBuffer::StringRingBuffer(int arraySize)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [StringRingBuffer::StringRingBuffer]"), this);
	//lock1 = new std::mutex();
	lock1 = new std::mutex();
	pStringList = new RingInfo[arraySize];
	start = 0;
	end = arraySize;
	readNum = 0;
	writeNum = 0;
	readSize = 0;

	// for(int i=0;i< arraySize;i++)
	// {
	// 	pStringList[i].data = (char*)malloc(100*1024);
	// }
}

//
StringRingBuffer::~StringRingBuffer()
{
	// for (int i = 0; i < end; i++)
	// {
	// 	free(pStringList[i].data );
	// }
	delete lock1;
	delete[] pStringList;
}

//
bool StringRingBuffer::Write(RingInfo& data)
{
	lock1->lock();
	bool result = true;
	if (readSize == 0 && writeNum == readNum)
	{
		// 整个缓冲区已经被读完了,或者刚开始读写

		pStringList[writeNum].str = data.str;
		pStringList[writeNum].type = data.type;

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
		pStringList[writeNum].str = data.str;
		pStringList[writeNum].type = data.type;

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
bool StringRingBuffer::Read(RingInfo& data)
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
		data.type = pStringList[readNum].type;
		data.str = pStringList[readNum].str;

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
		data.type = pStringList[readNum].type;
		data.str = pStringList[readNum].str;

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
void StringRingBuffer::ShowBuffer()
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
unsigned __stdcall StringRingBuffer::StringRingBufferTestThread1(void* arg)
{
	StringRingBuffer* gwi = (StringRingBuffer*)arg;
	gwi->StringRingBufferTestThreadFunc1();
	return 0;
}

// 锁测试线程
unsigned __stdcall StringRingBuffer::StringRingBufferTestThread2(void* arg)
{
	StringRingBuffer* gwi = (StringRingBuffer*)arg;
	gwi->StringRingBufferTestThreadFunc2();
	return 0;
}

// 注册读取
void StringRingBuffer::StringRingBufferTestThreadFunc1()
{
	printf("[StringRingBufferTestThreadFunc1] ready go \n");
	std::string str = "111111111";
	LockTest(str);

	printf("[StringRingBufferTestThreadFunc1] over \n");
}

// 写入
void StringRingBuffer::StringRingBufferTestThreadFunc2()
{
	//printf("[StringRingBufferTestThreadFunc2] ready go \n");
	//Sleep(1000 * 1);
	//std::string str = "222222222";
	//LockTest(str);

	//printf("[StringRingBufferTestThreadFunc2] over \n");
}

// 测试锁
void StringRingBuffer::Test1()
{
	//_beginthreadex(NULL, 0, StringRingBufferTestThread1, this, 0, NULL);
	//_beginthreadex(NULL, 0, StringRingBufferTestThread2, this, 0, NULL);
}

// 测试锁
void StringRingBuffer::LockTest(std::string str)
{
	//printf("[LockTest] [%s] a   \n", str.c_str());
	//lock1->lock();

	//printf("[LockTest] [%s] b   \n", str.c_str());
	//Sleep(3000);

	//lock1->unlock();
	//printf("[LockTest] [%s] c   \n", str.c_str());
}

// 测试动态数组
void StringRingBuffer::Test2()
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
unsigned __stdcall StringRingBuffer::StringRingBufferTestThread3(void* arg)
{
	StringRingBuffer* gwi = (StringRingBuffer*)arg;
	gwi->StringRingBufferTestThreadFunc3();
	return 0;
}

// 环形缓冲测试线程
unsigned __stdcall StringRingBuffer::StringRingBufferTestThread4(void* arg)
{
	StringRingBuffer* gwi = (StringRingBuffer*)arg;
	gwi->StringRingBufferTestThreadFunc4();
	return 0;
}

// 读取
void StringRingBuffer::StringRingBufferTestThreadFunc3()
{
	//printf("[StringRingBufferTestThreadFunc3] start \n");
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

	//printf("[StringRingBufferTestThreadFunc3] over \n");
}

// 写入
void StringRingBuffer::StringRingBufferTestThreadFunc4()
{
	//printf("[StringRingBufferTestThreadFunc4] start \n");
	//int i = 0;
	//while (true) {
	//    bool result=Write("asdfasf");
	//    if (result == false) {
	//        printf("[write] [%d] [result:%d]  [read:%d write : %d] [ReadSize:%d] \n", i, result, readNum, writeNum, readSize);
	//    }
	//    i++;
	//    Sleep(500);
	//}

	//printf("[StringRingBufferTestThreadFunc4] over \n");
}

// 测试环形缓冲区
void StringRingBuffer::Test3()
{
	//_beginthreadex(NULL, 0, StringRingBufferTestThread3, this, 0, NULL);
	//_beginthreadex(NULL, 0, StringRingBufferTestThread4, this, 0, NULL);

	//// 写入前后缓存变化
	//ShowBuffer();
	//bool result = Write("asdfasf");
	//printf("[write] [result:%d]\n", result);
	//ShowBuffer();
}