#pragma once


#include <string>
//using namespace std;



class NetTool
{


public:

	//
	NetTool();

	//
	~NetTool();

	// 获取 转换
	static std::string FloatToString(float val);

	// 获取 转换
	static std::string IntToString(int val);
};

