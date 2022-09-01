// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <list>
#include <map>
#include <vector>
#include "Protobuf/my_python.pb.h"

//THIRD_PARTY_INCLUDES_START
//PRAGMA_DISABLE_REGISTER_WARNINGS
//#include "Python.h"
//#include "structmember.h"
//PRAGMA_ENABLE_REGISTER_WARNINGS
//THIRD_PARTY_INCLUDES_END

#include "MyPythonActor.generated.h"

class UPlaceNearControllerComponent;
class UPlaceNearDataComponent;

class UBoxComponent;
class UMaterial;
class UBoxComponent;
class USceneComponent;
class USceneStaticMeshComponent;
class APawnLineActor;

class AEditEntityManage;
//class UMyPython;

//using namespace std;

UCLASS()
class  AMyPythonActor : public AActor
{
	GENERATED_BODY()

public:

	// 传入数据结构
	struct PyTickInputInfo
	{
		int entity_id = 0;

		float w_location_x = 0;
		float w_location_y = 0;
		float w_location_z = 0;
		float w_rotator_pitch = 0;
		float w_rotator_yaw = 0;
		float w_rotator_roll = 0;
		float w_scale_x = 0;
		float w_scale_y = 0;
		float w_scale_z = 0;

		float r_location_x = 0;
		float r_location_y = 0;
		float r_location_z = 0;
		float r_rotator_pitch = 0;
		float r_rotator_yaw = 0;
		float r_rotator_roll = 0;
		float r_scale_x = 0;
		float r_scale_y = 0;
		float r_scale_z = 0;
	};

	// 传出数据结构
	struct PyTickOutInfo
	{
	};

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* sceneRoot;

	//
	TWeakObjectPtr<AEditEntityManage> editEntityManage;

	//
	//TWeakObjectPtr<UMyPython> myPython;

	//
	float lastTick = 0;

	// 是否运行
	bool openTick = false;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// Sets default values for this actor's properties
	AMyPythonActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//
	void Start(AEditEntityManage* editEntityManage_);

	//
	void End();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//void LoadPyFile();
	//void InputMakeRow(PyObject* pDict, PyTickInputInfo* pyTickInputInfo);
	//PyObject* InputMakeDict();
	//void OutMakeDict(PyObject* pDict);
	//void OutputMakeRow(PyObject* pDict, PyTickInputInfo* pyTickInputInfo);
	//void PyTick2(float runTime, float DeltaTime);
	//void PyTick3(float runTime, float DeltaTime);
	//void PyTick(float runTime, float DeltaTime);
	//void RunEntity(float runTime, int entityId);
	//void SetRunPython(bool flag);
};
