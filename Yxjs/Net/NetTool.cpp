
#include "NetTool.h"



//
NetTool::NetTool()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [NetTool::NetTool]"), this);
}

//
NetTool::~NetTool()
{
}

// 获取 转换
std::string NetTool::FloatToString(float val)
{
	char temp[20];
	sprintf_s(temp, 20, "%f", val);
	std::string str = temp;
	return str;
}

// 获取 转换
std::string NetTool::IntToString(int val)
{
	char temp[20];
	sprintf_s(temp, 20, "%d", val);
	std::string str = temp;
	return str;
}