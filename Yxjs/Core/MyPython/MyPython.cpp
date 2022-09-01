//// Fill out your copyright notice in the Description page of Project Settings.
//
//#include "MyPython.h"
//#include <Components/MeshComponent.h>
//#include <Components/SceneComponent.h>
//#include <exception>
//
//bool isInit = false;
//
//// Sets default values
//UMyPython::UMyPython()
//{
//}
//
////
//void UMyPython::Start() {
//	return;
//
//	if (isInit == false) {
//		isInit = true;
//		Py_Initialize();
//		LoadEntityManagerPyFile();
//		UE_LOG(LogTemp, Log, TEXT("[%x] [Python] [Start] 初次初始化"), this);
//	}
//	else {
//		UE_LOG(LogTemp, Log, TEXT("[%x] [Python] [Start] 重复初始化"), this);
//	}
//
//	bool flag = false;
//	if (Py_IsInitialized()) {
//		int err1 = PyRun_SimpleString("import importlib");
//		int err2 = PyRun_SimpleString("importlib.reload(my_main)");
//
//		pModule_my_main = PyImport_ImportModule("my_main");//这里是要调用的文件名
//		if (pModule_my_main != nullptr)
//		{
//			pTick = PyObject_GetAttrString(pModule_my_main, "Tick");
//			pInputData = PyObject_GetAttrString(pModule_my_main, "InputData");
//			pOutData = PyObject_GetAttrString(pModule_my_main, "OutData");
//			pSetActorLocation = PyObject_GetAttrString(pModule_my_main, "SetActorLocation");
//			flag = true;
//		}
//	}
//
//	if (flag == false) {
//		UE_LOG(LogTemp, Error, TEXT("[%x] [Python] [Start] 1"), this);
//	}
//	else {
//		int i = 0;
//	}
//}
//
////
//void UMyPython::End() {
//	return;
//	if (Py_IsInitialized()) {
//		if (pTick)Py_DECREF(pTick);
//		if (pInputData)Py_DECREF(pInputData);
//		if (pOutData)Py_DECREF(pOutData);
//		if (pModule_my_main)Py_DECREF(pModule_my_main);
//
//		try
//		{
//			//Py_Finalize();
//		}
//		catch (std::exception& e)
//		{
//			auto str = e.what();
//			UE_LOG(LogTemp, Error, TEXT("[%x] [Python] [End] 2 "), this);
//		}
//	}
//	else {
//		UE_LOG(LogTemp, Error, TEXT("[%x] [Python] [End] 3"), this);
//	}
//	//
//}
//
//// 加载目标文件(重复加载)
//void UMyPython::LoadEntityManagerPyFile() {
//	if (Py_IsInitialized() == false) {
//		return;
//	}
//
//	if (isLoadFile == false) {
//		FString path = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
//		path += TEXT("OtherData/MyPython");
//		FPaths::CollapseRelativeDirectories(path);
//		std::string path2 = (TCHAR_TO_UTF8(*path));
//		std::string path3 = "ROOT_DIR = os.path.abspath('" + path2 + "')";
//
//		int err0 = PyRun_SimpleString("# coding=utf-8");
//		int err1 = PyRun_SimpleString("import os");
//		int err2 = PyRun_SimpleString("import sys");
//		int err3 = PyRun_SimpleString(path3.c_str());
//		int err4 = PyRun_SimpleString("sys.path.append(ROOT_DIR)");
//
//		int err5 = PyRun_SimpleString("import importlib");
//		int err6 = PyRun_SimpleString("import my_main");
//		//int err5 = PyRun_SimpleString("import my_entity_manager");
//		isLoadFile = true;
//	}
//	else {
//		//int err5 = PyRun_SimpleString("import my_entity_manager");
//	}
//}
//
//// tick
//
////
//void UMyPython::TestFunc() {
//	return;
//
//	if (Py_IsInitialized() == false) {
//		return;
//	}
//
//	int err5 = PyRun_SimpleString("import test_c_call_python");
//
//	PyObject* pModule = PyImport_ImportModule("test_c_call_python");//这里是要调用的文件名
//	if (pModule != nullptr)
//	{
//		// 多个参数
//		{
//			PyObject* pFunc3 = PyObject_GetAttrString(pModule, "TestAdd");//这里是要调用的函数名
//			if (pFunc3) {
//				PyObject* args2 = Py_BuildValue("ii", 55, 66);//给python函数参数赋值
//				PyObject* pRet = PyObject_CallObject(pFunc3, args2);//调用函数
//				int res = 0;
//				PyArg_Parse(pRet, "i", &res);//转换返回类型
//
//				UE_LOG(LogTemp, Log, TEXT("[%x] [Python] [TestFunc] [res:%d] "), this, res);
//				Py_DECREF(args2);
//				Py_DECREF(pRet);
//				Py_DECREF(pFunc3);
//			}
//		}
//
//		// 多个参数
//		{
//			PyObject* pFunc3 = PyObject_GetAttrString(pModule, "TestAdd");//这里是要调用的函数名
//			if (pFunc3) {
//				PyObject* pArgs = PyTuple_New(2);
//				PyTuple_SetItem(pArgs, 0, Py_BuildValue("l", 3));
//				PyTuple_SetItem(pArgs, 1, Py_BuildValue("l", 4));
//				PyObject* pRet = PyObject_CallObject(pFunc3, pArgs);
//			}
//		}
//
//		// 多个参数
//		{
//			PyObject* pFunc3 = PyObject_GetAttrString(pModule, "TestInputValue");//这里是要调用的函数名
//			if (pFunc3) {
//				PyObject* args2 = Py_BuildValue("si", "ssrr", 3);
//				PyObject* pRet = PyObject_CallObject(pFunc3, args2);
//				FString str = GetPyString(pRet);
//				UE_LOG(LogTemp, Log, TEXT("[%x] [Python] [TestFunc] [res:%s] "), this, *str);
//				if (args2)Py_DECREF(args2);
//				if (pRet)Py_DECREF(pRet);
//			}
//			if (pFunc3)Py_DECREF(pFunc3);
//		}
//
//		// 字符串传入返回
//		{
//			PyObject* pFunc3 = PyObject_GetAttrString(pModule, "TestPrint");//这里是要调用的函数名
//			if (pFunc3) {
//				PyObject* pArgs = PyTuple_New(1);
//				PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "aabbccddee"));//0—序号 i表示创建int型变量
//				PyObject* pRet = PyObject_CallObject(pFunc3, pArgs);
//
//				FString str = GetPyString(pRet);
//				UE_LOG(LogTemp, Log, TEXT("[%x] [Python] [TestFunc] [res:%s] "), this, *str);
//				if (pArgs)Py_DECREF(pArgs);
//				if (pRet)Py_DECREF(pRet);
//			}
//			if (pFunc3)Py_DECREF(pFunc3);
//		}
//
//		// 字典传入返回字典
//		{
//			PyObject* pFunc3 = PyObject_GetAttrString(pModule, "TestDict");//这里是要调用的函数名
//			if (pFunc3) {
//				PyObject* pDict = PyDict_New(); //创建字典类型变量
//				PyDict_SetItemString(pDict, "Name", Py_BuildValue("s", "Zhangsan")); //往字典类型变量中填充数据
//				PyDict_SetItemString(pDict, "Count", Py_BuildValue("s", "3322"));
//				PyObject* pArgs = PyTuple_New(1);
//				PyTuple_SetItem(pArgs, 0, pDict);
//
//				PyObject* pRet = PyObject_CallObject(pFunc3, pArgs);
//
//				if (pArgs)Py_DECREF(pArgs);
//				if (pRet)Py_DECREF(pRet);
//			}
//			if (pFunc3)Py_DECREF(pFunc3);
//		}
//
//		// 传入多层级字典,返回多层级字典
//		{
//			PyObject* pFunc3 = PyObject_GetAttrString(pModule, "TestDict2");//这里是要调用的函数名
//			//if (pFunc3) {
//			//	PyObject* pDict = PyDict_New();
//			//	{
//			//		PyObject* pEntityInfo = PyDict_New();
//			//		PyDict_SetItemString(pEntityInfo, "id", Py_BuildValue("i", 123));
//			//		PyDict_SetItemString(pEntityInfo, "x", Py_BuildValue("d", 22.2));
//			//		PyDict_SetItemString(pEntityInfo, "y", Py_BuildValue("d", 22.2));
//			//		PyDict_SetItemString(pEntityInfo, "z", Py_BuildValue("d", 22.2));
//			//		PyDict_SetItemString(pDict, "123", Py_BuildValue("O", pEntityInfo));
//			//		if (pEntityInfo)Py_DECREF(pEntityInfo);
//			//	}
//			//	PyObject* pArgs = PyTuple_New(1);
//			//	PyTuple_SetItem(pArgs, 0, pDict);
//
//			//	PyObject* pRet = PyObject_CallObject(pFunc3, pArgs);
//
//			//	{
//			//		PyObject* pEntityInfo = PyDict_GetItemString(pRet, "123");
//			//		int id = GetPyDictIntValue(pEntityInfo, "id");
//			//		float x = GetPyDictFloatValue(pEntityInfo, "x");
//			//		if (pEntityInfo)Py_DECREF(pEntityInfo);
//
//			//		UE_LOG(LogTemp, Log, TEXT("[%x] [Python] [TestFunc] [id:%d] [x:%f] "), this, id, x);
//			//	}
//
//			//	if (pDict)Py_DECREF(pDict);
//			//	if (pArgs)Py_DECREF(pArgs);
//			//	if (pRet)Py_DECREF(pRet);
//			//}
//			if (pFunc3)Py_DECREF(pFunc3);
//		}
//
//		// 数组
//		//{
//		//	PyObject* rel = PyObject_CallObject(func, args);
//		//	int s = PyList_Size(rel);
//		//	for (int i = 0; i < s; i++)
//		//	{
//		//		std::cout << PyUnicode_AsUTF8(PyList_GetItem(rel, i));
//		//		std::cout << std::endl;
//		//	}
//		//}
//
//		//
//		Py_DECREF(pModule);
//	}
//}
//
//// py对象转字符串
//FString UMyPython::GetPyString(PyObject* pRet)
//{
//	FString str = TEXT("");
//
//	if (pRet && PyType_FastSubclass(Py_TYPE(pRet), Py_TPFLAGS_UNICODE_SUBCLASS))
//	{
//		PyObject* PyBytesObj = PyUnicode_AsUTF8String(pRet);
//		if (PyBytesObj)
//		{
//			const char* PyUtf8Buffer = PyBytes_AsString(PyBytesObj);
//			str = UTF8_TO_TCHAR(PyUtf8Buffer);
//			Py_DECREF(PyBytesObj);
//		}
//	}
//	return str;
//}
//
//// 从py字典读取数字
//int UMyPython::GetPyDictIntValue(PyObject* pDickObject, std::string name)
//{
//	int value = 0;
//	if (pDickObject) {
//		PyObject* pObject = PyDict_GetItemString(pDickObject, name.c_str());
//		if (pObject) {
//			PyArg_Parse(pObject, "i", &value);
//			Py_DECREF(pObject);
//		}
//	}
//	return value;
//}
//
//// 从py字典读取浮点数
//float UMyPython::GetPyDictFloatValue(PyObject* pDickObject, std::string name)
//{
//	double value = 0;
//	if (pDickObject) {
//		PyObject* pObject = PyDict_GetItemString(pDickObject, name.c_str());
//		if (pObject) {
//			PyArg_Parse(pObject, "d", &value);
//			Py_DECREF(pObject);
//		}
//	}
//	return value;
//}
//
//// 调用tick函数
//void UMyPython::RunTick(double runTime, double DeltaTime) {
//	if (pTick) {
//		//// 更变的数据列表
//		//PyObject* itemList = PyList_New(10);
//		//{
//		//	//PyList_Append(itemList, var);
//
//		//	//UE_LOG(LogTemp, Log, TEXT("[%x] [python] [RunTick] 输出 [%d] "), this, i);
//		//}
//
//		//PyObject* args = Py_BuildValue("dd", runTime, DeltaTime);//给python函数参数赋值
//		//PyObject* pRet = PyObject_CallObject(pTick, args);//调用函数
//
//		//if (itemList)Py_DECREF(itemList);
//
//		//// 更变的数据列表
//		//{
//		//	int size = PyList_Size(pRet);
//		//	for (int i = 0; i < size; i++)
//		//	{
//		//		PyObject* item = PyList_GetItem(pRet, i);
//		//		if (item) {
//		//			UE_LOG(LogTemp, Log, TEXT("[%x] [python] [RunTick] 输出 [%d] "), this, i);
//		//		}
//		//		if (item)Py_DECREF(item);
//		//	}
//		//}
//
//		//if (args)Py_DECREF(args);
//		//if (pRet)Py_DECREF(pRet);
//	}
//}