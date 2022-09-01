// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPythonActor.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "../EditEntityManage.h"
#include "../EditEntityManageData.h"
//#include "Yxjs/Pawn/Racing/Player/PlayerRacingPawn.h"
#include "Yxjs/Core/MyPython/MyPython.h"
#include "../Actor/EditEntityActor.h"

#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Yxjs/Tools/CommonlyObject.h"
#include "../Actor/ControllerComponent/EditEntityEditComponent.h"

/*
 *
 */

 /*/////////////////////////////////////////////////////////////*/
 // view
 /*/////////////////////////////////////////////////////////////*/

//
AMyPythonActor::AMyPythonActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	RootComponent = sceneRoot;
}

//
void AMyPythonActor::BeginPlay()
{
	Super::BeginPlay();
}

//
void AMyPythonActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

//
void AMyPythonActor::Start(AEditEntityManage* editEntityManage_)
{
	editEntityManage = editEntityManage_;
	//myPython = Cast<UBaseGameInstance>(GetGameInstance())->myPython;
	//myPython->TestFunc();
	//myPython->LoadEntityManagerPyFile();
	//PrimaryActorTick.bCanEverTick = true;
}

//
void AMyPythonActor::End()
{
}

// tick
void AMyPythonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//auto nowTime = this->GetWorld()->TimeSeconds;
	//if ((nowTime - lastTick) > 0.1 && openTick) {
	//	PyTick(0, DeltaTime);
	//	lastTick = nowTime;
	//}
}

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//// Python 加载目标py文件
//void AMyPythonActor::LoadPyFile() {
//}
//
///*------------------------------------------------------------------*/
//
//// py函数传值 传入数据,一行数据
//void AMyPythonActor::InputMakeRow(PyObject* pDict, PyTickInputInfo* pyTickInputInfo) {
//	auto tempVar = Py_BuildValue("i", 33);
//
//	PyObject* pEntityInfo = Py_BuildValue("{s:i,s:d,s:d,s:d,s:d,s:d,s:d,s:d,s:d,s:d,s:d,s:d,s:d,s:d,s:d,s:d,s:d,s:d,s:d}",
//		"entity_id", pyTickInputInfo->entity_id
//		, "w_location_x", pyTickInputInfo->w_location_x
//		, "w_location_y", pyTickInputInfo->w_location_y
//		, "w_location_z", pyTickInputInfo->w_location_z
//		, "w_rotator_pitch", pyTickInputInfo->w_rotator_pitch
//		, "w_rotator_yaw", pyTickInputInfo->w_rotator_yaw
//		, "w_rotator_roll", pyTickInputInfo->w_rotator_roll
//		, "w_scale_x", pyTickInputInfo->w_scale_x
//		, "w_scale_y", pyTickInputInfo->w_scale_y
//		, "w_scale_z", pyTickInputInfo->w_scale_z
//		, "r_location_x", pyTickInputInfo->r_location_x
//		, "r_location_y", pyTickInputInfo->r_location_y
//		, "r_location_z", pyTickInputInfo->r_location_z
//		, "r_rotator_pitch", pyTickInputInfo->r_rotator_pitch
//		, "r_rotator_yaw", pyTickInputInfo->r_rotator_yaw
//		, "r_rotator_roll", pyTickInputInfo->r_rotator_roll
//		, "r_scale_x", pyTickInputInfo->r_scale_x
//		, "r_scale_y", pyTickInputInfo->r_scale_y
//		, "r_scale_z", pyTickInputInfo->r_scale_z
//	);
//	//PyList_Append(itemList, pEntityInfo);
//	PyDict_SetItemString(pDict, std::to_string(pyTickInputInfo->entity_id).c_str(), Py_BuildValue("O", pEntityInfo));
//	Py_DECREF(pEntityInfo);
//}
//
//// py函数传值 传入数据,字典处理
//PyObject* AMyPythonActor::InputMakeDict() {
//	//PyObject* itemList = PyList_New(100);
//	//for (int i = 0; i < 100; i++) {
//	//	PyTickInputInfo pyTickInputInfo;
//	//	InputMakeRow(itemList, &pyTickInputInfo);
//	//}
//	//if (pEntityInfo)Py_DECREF(pEntityInfo);
//
//	PyObject* pDict = PyDict_New();
//
//	for (int i = 0; i < 100; i++) {
//		PyTickInputInfo pyTickInputInfo;
//		pyTickInputInfo.entity_id = i + 1000;
//		pyTickInputInfo.w_location_x = i * 100;
//		pyTickInputInfo.w_location_y = i * 100 + i * 20;
//		pyTickInputInfo.w_location_z = pyTickInputInfo.w_location_x + pyTickInputInfo.w_location_y;
//		InputMakeRow(pDict, &pyTickInputInfo);
//	}
//	return pDict;
//}
//
//// py函数传值 传出数据,字典处理
//void AMyPythonActor::OutMakeDict(PyObject* pDict) {
//	if (pDict == nullptr)return;
//	std::list<PyTickInputInfo> dataList;
//	PyObject* key, * value;
//	Py_ssize_t pos = 0;
//	while (PyDict_Next(pDict, &pos, &key, &value)) {
//		//string ckey2 = PyString_AsString(key2);
//		//tags[ckey2] = PyInt_AsLong(value2);
//		PyTickInputInfo pyTickInputInfo;
//		//OutputMakeRow(value, &pyTickInputInfo);
//		dataList.push_back(pyTickInputInfo);
//	}
//
//	// 处理数据
//}
//
//// py函数传值 传出一行数据,解析数据到结构体
//void AMyPythonActor::OutputMakeRow(PyObject* pDict, PyTickInputInfo* pyTickInputInfo) {
//	pyTickInputInfo->entity_id = myPython->GetPyDictIntValue(pDict, "entity_id");
//	pyTickInputInfo->w_location_x = myPython->GetPyDictFloatValue(pDict, "w_location_x");
//	pyTickInputInfo->w_location_y = myPython->GetPyDictFloatValue(pDict, "w_location_y");
//	pyTickInputInfo->w_location_z = myPython->GetPyDictFloatValue(pDict, "w_location_z");
//	pyTickInputInfo->w_rotator_pitch = myPython->GetPyDictFloatValue(pDict, "w_rotator_pitch");
//	pyTickInputInfo->w_rotator_yaw = myPython->GetPyDictFloatValue(pDict, "w_rotator_yaw");
//	pyTickInputInfo->w_rotator_roll = myPython->GetPyDictFloatValue(pDict, "w_rotator_roll");
//	pyTickInputInfo->w_scale_x = myPython->GetPyDictFloatValue(pDict, "w_scale_x");
//	pyTickInputInfo->w_scale_y = myPython->GetPyDictFloatValue(pDict, "w_scale_y");
//	pyTickInputInfo->w_scale_z = myPython->GetPyDictFloatValue(pDict, "w_scale_z");
//	pyTickInputInfo->r_location_x = myPython->GetPyDictFloatValue(pDict, "r_location_x");
//	pyTickInputInfo->r_location_y = myPython->GetPyDictFloatValue(pDict, "r_location_y");
//	pyTickInputInfo->r_location_z = myPython->GetPyDictFloatValue(pDict, "r_location_z");
//	pyTickInputInfo->r_rotator_pitch = myPython->GetPyDictFloatValue(pDict, "r_rotator_pitch");
//	pyTickInputInfo->r_rotator_yaw = myPython->GetPyDictFloatValue(pDict, "r_rotator_yaw");
//	pyTickInputInfo->r_rotator_roll = myPython->GetPyDictFloatValue(pDict, "r_rotator_roll");
//	pyTickInputInfo->r_scale_x = myPython->GetPyDictFloatValue(pDict, "r_scale_x");
//	pyTickInputInfo->r_scale_y = myPython->GetPyDictFloatValue(pDict, "r_scale_y");
//	pyTickInputInfo->r_scale_z = myPython->GetPyDictFloatValue(pDict, "r_scale_z");
//}
//
//// py函数传值 Python pytick
//void AMyPythonActor::PyTick2(float runTime, float DeltaTime) {
//	if (myPython.IsValid()) {
//		// 输入(新增/更新/移除)
//		{
//			//PyObject* pArgs = PyTuple_New(2);
//			//PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 3322));
//			//PyTuple_SetItem(pArgs, 1, pDict);
//			//PyObject* pRetust = PyObject_CallObject(myPython->pInputData, pArgs);
//
//			PyObject* pDict = InputMakeDict();
//			PyObject* pRetust = PyObject_CallFunction(myPython->pInputData, "iO", 33, pDict);
//			if (pDict)Py_DECREF(pDict);
//			if (pRetust)Py_DECREF(pRetust);
//		}
//
//		// tick
//		{
//			PyObject_CallFunction(myPython->pTick, "dd", runTime, DeltaTime);
//		}
//
//		// 取输出
//		{
//			PyObject* pOutData = PyObject_CallFunction(myPython->pOutData, "d", runTime);
//
//			OutMakeDict(pOutData);
//			if (pOutData)Py_DECREF(pOutData);
//		}
//	}
//}
//
///*------------------------------------------------------------------*/
//
//// pb传值
//
///*------------------------------------------------------------------*/
//
//// Python pytick
//void AMyPythonActor::PyTick3(float runTime, float DeltaTime) {
//	if (myPython.IsValid()) {
//		//	// 输入(新增/更新/移除)
//		//	{
//		//		std::string tempStr = "";
//		//		// 编码
//		//		{
//		//			Gamedata::PyTickInputInfo pyTickInputInfo;
//		//			pyTickInputInfo.mutable_transform()->mutable_location()->set_x(11);
//		//			pyTickInputInfo.mutable_transform()->mutable_location()->set_y(22);
//		//			pyTickInputInfo.mutable_transform()->mutable_location()->set_z(33);
//		//			pyTickInputInfo.set_entity_id(44);
//
//		//			tempStr = pyTickInputInfo.SerializePartialAsString();
//		//		}
//
//		//		PyObject* pbData = PyBytes_FromStringAndSize(tempStr.c_str(), tempStr.length());//inputData, inputSize
//		//		PyObject* pRetust = PyObject_CallFunction(myPython->pInputData, "iO", 33, pbData);
//
//		//		if (pRetust)Py_DECREF(pRetust);
//		//		if (pbData)Py_DECREF(pbData);
//		//	}
//
//		//	// tick
//		//	{
//		//		PyObject_CallFunction(myPython->pTick, "dd", runTime, DeltaTime);
//		//	}
//
//		//	// 取输出
//		//	{
//		//		PyObject* pOutData = PyObject_CallFunction(myPython->pOutData, "d", runTime);
//		//		if (pOutData)
//		//		{
//		//			char* bpData = PyBytes_AsString(pOutData);
//		//			int bpSize = PyBytes_Size(pOutData);
//		//			Gamedata::PyTickInputInfo pyTickInputInfo2;
//		//			bool flag = pyTickInputInfo2.ParseFromArray(bpData, bpSize);
//		//			if (flag) {
//		//				UE_LOG(LogTemp, Log, TEXT("[%x] [python] [PyTick] [x:%f] "), this, pyTickInputInfo2.transform().location().x());
//		//			}
//		//		}
//		//		if (pOutData)Py_DECREF(pOutData);
//		//	}
//	}
//}
//
//// Python pytick
//void AMyPythonActor::PyTick(float runTime, float DeltaTime) {
//	// 找一行数据执行了.
//
//	auto& actorMap = editEntityManage->actorMap;
//	for (auto node : actorMap) {
//		auto& actor = node.Value;
//		if (actor->syncInfo.has_python_info()) {
//			if (actor->syncInfo.python_info().err() == Gamedata::EntityPythonErrorType::EntityPythonErrorType_none)
//				RunEntity(runTime, actor->syncInfo.entityid());
//		}
//	}
//}
//
//// 实例执行帧
//void AMyPythonActor::RunEntity(float runTime, int entityId) {
//	auto entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(entityId));
//	if (entityActor == nullptr) {
//		return;
//	}
//
//	std::string tempStr = "";
//	// 编码
//	{
//		Gamedata::PyEntityInfo pyEntityInfo;
//		pyEntityInfo.set_entity_id(entityId);
//		pyEntityInfo.set_now_time(runTime);
//		pyEntityInfo.set_use_world_transform(true);
//		pyEntityInfo.mutable_w_transform()->CopyFrom(entityActor->observerSyncInfo.transform());
//		tempStr = pyEntityInfo.SerializePartialAsString();
//	}
//
//	PyObject* pbData = PyBytes_FromStringAndSize(tempStr.c_str(), tempStr.length());//inputData, inputSize
//	PyObject* pResult = PyObject_CallFunction(myPython->pSetActorLocation, "O", pbData);
//
//	// 执行成功
//	if (pResult)
//	{
//		char* bpData = PyBytes_AsString(pResult);
//		if (bpData) {
//			int bpSize = PyBytes_Size(pResult);
//			Gamedata::PyEntityInfo pyEntityInfo;
//			bool flag = pyEntityInfo.ParseFromArray(bpData, bpSize);
//			if (flag) {
//				if (pyEntityInfo.use_world_transform()) {
//					FTransform wTransform = UCommonlyObject::PBToUe4Transform(pyEntityInfo.mutable_w_transform());
//					entityActor->editComponent->EntityLocalMoveMain(ESelectedWorkMode::WorldTransform, wTransform.GetLocation(),
//						wTransform.GetRotation().Rotator(), wTransform.GetScale3D());
//				}
//				else {
//					FTransform rTransform = UCommonlyObject::PBToUe4Transform(pyEntityInfo.mutable_r_transform());
//					entityActor->editComponent->EntityLocalMoveMain(ESelectedWorkMode::WorldTransform, rTransform.GetLocation(),
//						rTransform.GetRotation().Rotator(), rTransform.GetScale3D());
//				}
//			}
//		}
//	}
//	// 执行失败
//	else {
//		// 将数据标记为不再可读取
//	}
//	if (pbData)Py_DECREF(pbData);
//	if (pResult)Py_DECREF(pResult);
//}
//
//// 运行python
//void AMyPythonActor::SetRunPython(bool flag) {
//	//openTick = flag;
//
//	//FString str = openTick ? TEXT("运行") : TEXT("停止");
//	//UE_LOG(LogTemp, Log, TEXT("[%x] [python] [SetRunPython] [%s] "), this, *str);
//
//	//
//	if (flag) {
//		// 保存数据
//
//		auto& actorMap = editEntityManage->actorMap;
//		//auto it = actorMap.
//		TMap<int, AEditEntityActor*>::TConstIterator iter = actorMap.CreateConstIterator();
//		//iter.Value->syncInfo.entityid();
//		//AEditEntityActor* actor = it.Value;
//		//actor->syncInfo.entityid();;
//		//auto entitiyId = it->second->syncInfo.entityid();;
//		//RunEntity(0, entitiyId);
//	}
//}