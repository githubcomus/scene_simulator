//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include <UObject/Object.h>
//#include <string>
//
////THIRD_PARTY_INCLUDES_START
////PRAGMA_DISABLE_REGISTER_WARNINGS
////#include "Python.h"
////#include "structmember.h"
////PRAGMA_ENABLE_REGISTER_WARNINGS
////THIRD_PARTY_INCLUDES_END
//
//#include "MyPython.generated.h"
//
//UCLASS()
//class  UMyPython : public UObject
//{
//	GENERATED_BODY()
//
//public:
//
//	// 文件
//	PyObject* pModule_my_main = nullptr;
//
//	// 函数
//	PyObject* pTick = nullptr;
//
//	// 函数
//	PyObject* pInputData = nullptr;
//
//	// 函数
//	PyObject* pOutData = nullptr;
//
//	// 函数
//	PyObject* pSetActorLocation = nullptr;
//
//	// 文件是否已经加载
//	bool isLoadFile = false;
//
//	// Sets default values for this actor's properties
//	UMyPython();
//
//	void Start();
//	void End();
//	void LoadEntityManagerPyFile();
//	void TestFunc();
//
//	FString GetPyString(PyObject* pRet);
//	int GetPyDictIntValue(PyObject* pDickObject, std::string name);
//	float GetPyDictFloatValue(PyObject* pDickObject, std::string name);
//	void RunTick(double runTime, double DeltaTime);
//};
