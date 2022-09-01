// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "TestActor.generated.h"


class UPlayerRangedWeaponComponent;
class ATestChildActor;

class UBoxComponent;
class ABuildManageActor;
class UMaterial;
class UPlaceStaticMeshComponent;
class USceneComponent;
class UArrowComponent;
class USplineComponent;
class USplineMeshComponent;
class UStaticMesh;


// 普通结构体
struct CvST1
{
	UPROPERTY()
	AActor* actor;

	UPROPERTY()
	UObject* object;
};

// UE4结构体
USTRUCT()
struct FCvST3
{
	GENERATED_BODY()

		UPROPERTY()
		AActor* actor = nullptr;

	UPROPERTY()
		UObject* object = nullptr;
};

// UE4结构体
USTRUCT()
struct FCvST2
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* actor=nullptr;

	UPROPERTY()
	UObject* object = nullptr;

	// gc后指针无法置空
	FCvST3 cv3;

	// gc后指针正常置空
	UPROPERTY()
	FCvST3 cv4;
};



UCLASS()
class ATestActor : public AActor
{
	GENERATED_BODY()

public:

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* sceneRoot;

	// 终点mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* sphereMesh;

	/*------------------------------------------------------------------*/

	// 
	// TArray<AActor*> actorList;

	// 观察者模式
	std::vector<TWeakObjectPtr<AActor>> actorList3;

	// gc后指针正常置空
	UPROPERTY()
	TArray<AActor*> actorList4;

	// gc后指针无法置空
	TArray<CvST1> actorList5;

	// gc后指针无法制空
	TArray<FCvST2> actorList6;

	// gc后指针正常置空
	UPROPERTY()
	TArray<FCvST2> actorList7;

	/*------------------------------------------------------------------*/

	// 测试类型
	int testObjType = 1;
	
	// 观察者模式
	std::vector<TWeakObjectPtr<UObject>> objectList3;

	// 正常置空
	UPROPERTY()
	TArray<UObject*> objectList4;

	// 无法影响引用计数器
	TArray<CvST1> objectList5;

	// 无法影响引用计数器
	TArray<FCvST2> objectList6;

	// 正常置空
	UPROPERTY()
	TArray<FCvST2> objectList7;

	/*------------------------------------------------------------------*/

	//
	struct TestInfo
	{
		//
		float lastTimeTestTick = 0;

		//
		bool runOnce = false;
	};

	//
	TestInfo testInfo;

	/*------------------------------------------------------------------*/

	// Sets default values for this actor's properties
	ATestActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// EndPlay
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// PostInitializeComponents
	virtual void PostInitializeComponents() override;

	/*------------------------------------------------------------------*/

	// START
	void Start(UPlayerRangedWeaponComponent* playerRangedWeaponComponent_);

	// END
	void End();

	// void TestFunc();
	AActor* Test_ActorSpawn2(FVector location, int number);
	void TestTick();
	FTransform GetRToWorldTransform(FTransform originWTransform, FTransform rTransform);
	void TransformTest();
	UFUNCTION(BlueprintCallable)
	void TestEnterKey(int type);
	void Test_ActorSpawn();
	void Test_ActorDestroy();
	void Test_ActorGC();
	void Test_ActorReadAgain();
	void Test_ObjectSpawn();
	void Test_ObjectDestroy();
	void Test_ObjectGC();
	void Test_ObjectReadAgain();

	/*------------------------------------------------------------------*/
};
