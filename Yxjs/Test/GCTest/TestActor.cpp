// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "TestChildActor.h"
#include "TestChildObject.h"


#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


/*
view
*/


/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = true;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	sceneRoot->SetMobility(EComponentMobility::Movable);
	RootComponent = sceneRoot;

	sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UStaticMeshComponent"));
	sphereMesh->SetMobility(EComponentMobility::Movable);
	sphereMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	sphereMesh->SetHiddenInGame(false);
	sphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

//
void ATestActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::BeginPlay] "), this);
	//Start();


	// TestFunc();
	// actorList.Add(actor);
	//actorList2.push_back(actor);
	//TWeakObjectPtr<ATestChildActor> actor2 = actor;
	//actorList3.push_back(actor);

	// TransformTest();
}

//
void ATestActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::BeginPlay]   "), this);
	//End();
}

// tick
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TestTick();
}

/*------------------------------------------------------------------*/

// START
void ATestActor::Start(UPlayerRangedWeaponComponent* playerRangedWeaponComponent_)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::Start] "), this);
}

// END
void ATestActor::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::End]   "), this);
}

/*------------------------------------------------------------------*/

// GC test
void ATestActor::TestTick()
{
	// 1s
	auto nowTime = GetWorld()->TimeSeconds;
	if (nowTime - testInfo.lastTimeTestTick < 1)
	{
		return;
	}
	testInfo.lastTimeTestTick = nowTime;

}

/*------------------------------------------------------------------*/

// 相对坐标转世界坐标
FTransform ATestActor::GetRToWorldTransform(FTransform originWTransform, FTransform rTransform)
{
	// 相对坐标转世界坐标
	FVector worldLocation = UKismetMathLibrary::TransformLocation(originWTransform
	                                                              , rTransform.GetLocation());
	// 相对旋转转世界旋转
	FRotator worldRotator = UKismetMathLibrary::TransformRotation(originWTransform,
	                                                              rTransform.Rotator());

	FTransform wTransform;
	wTransform.SetLocation(worldLocation);
	wTransform.SetRotation(worldRotator.Quaternion());
	wTransform.SetScale3D(rTransform.GetScale3D());

	return wTransform;
}

// transfrom test
void ATestActor::TransformTest()
{
	FTransform t1;
	t1.SetLocation(FVector(100, 0, 0));
	t1.SetRotation(FRotator(0, 0, 0).Quaternion());
	t1.SetScale3D(FVector(1, 1, 1));


	FTransform t2;
	t2.SetLocation(FVector(200, 100, 0));
	t2.SetRotation(FRotator(20, 0, 0).Quaternion());
	t2.SetScale3D(FVector(1, 1, 1));


	auto t3 = t1 * t2;

	auto t4 = GetRToWorldTransform(t1, t2);

	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), cameraV, end, FColor(0, 255, 255), time, 1.0f);
	UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), t1.GetLocation(), 2, 2, FRotator::ZeroRotator, FLinearColor::Red,
	                                       600, 1);
	UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), t2.GetLocation(), 4, 4, FRotator::ZeroRotator,
	                                       FLinearColor::White, 600, 1);
	UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), t3.GetLocation(), 6, 6, FRotator::ZeroRotator,
	                                       FLinearColor::Yellow, 600, 1);
	UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), t4.GetLocation(), 8, 8, FRotator::ZeroRotator,
	                                       FLinearColor::Black, 600, 1);
}

/*------------------------------------------------------------------*/

// 测试 按键
void ATestActor::TestEnterKey(int type)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::TestEnterKey] [type:%d] "), this, type);

	
	if (type == 1)
	{
		Test_ActorSpawn();
	}
	else if (type == 2)
	{
		Test_ActorDestroy();
	}
	else if (type == 3)
	{
		Test_ActorGC();
	}
	else if (type == 4)
	{
		Test_ActorReadAgain();
	}
	//
	// if (type == 1)
	// {
	// 	Test_ObjectSpawn();
	// }
	// else if (type == 2)
	// {
	// 	Test_ObjectDestroy();
	// }
	// else if (type == 3)
	// {
	// 	Test_ObjectGC();
	// }
	// else if (type == 4)
	// {
	// 	Test_ObjectReadAgain();
	// }
}

/*------------------------------------------------------------------*/

// actor 对象释放需要destroy再gc

// actor GC test
AActor* ATestActor::Test_ActorSpawn2(FVector location, int number)
{
	FTransform t;
	t.SetLocation(location);
	t.SetRotation(FRotator::ZeroRotator.Quaternion());
	t.SetScale3D(FVector(1, 1, 1));
	auto actor = Cast<ATestChildActor>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ATestChildActor::StaticClass(), t, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (actor)
	{
		actor->number = number;
		UGameplayStatics::FinishSpawningActor(actor, t);
		actor->Start();
		return actor;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::TestFunc] ERR "), this);
		return nullptr;
	}
}

// actor 生成actor
void ATestActor::Test_ActorSpawn()
{
	for (int i = 0; i < 2; i++)
	{
		FVector location(200 * i, 200 * i, 250);
		auto actor = Test_ActorSpawn2(location, i);
		// actorList.Add(actor);
		actorList3.push_back(actor);
		actorList4.Add(actor);
		{
			CvST1 cv;
			cv.actor = actor;
			actorList5.Add(cv);
		}
		{
			FCvST2 cv;
			cv.actor = actor;
			actorList6.Add(cv);
		}
		{
			FCvST2 cv;
			cv.actor = actor;
			cv.cv3.actor = actor;
			cv.cv4.actor = actor;
			actorList7.Add(cv);
		}
	}
}

// actor 销毁
void ATestActor::Test_ActorDestroy()
{
	// Destroy执行endplay
	for (auto node : actorList3)
	{
		node->Destroy();
	}
}

// actor 强制GC
void ATestActor::Test_ActorGC()
{
	// gc后执行析构
	UKismetSystemLibrary::CollectGarbage();
}

// actor 再访问
void ATestActor::Test_ActorReadAgain()
{
	for (auto node : actorList3)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::Test_ReadAgain] a [node:%x] "), this, node.Get());
	}
	for (auto node : actorList4)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::Test_ReadAgain] b [node:%x] "), this, node);
	}

	for (auto node : actorList5)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::Test_ReadAgain] c [node:%x] "), this, node.actor);
	}

	for (auto node : actorList6)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::Test_ReadAgain] d [node:%x] "), this, node.actor);
	}
	for (auto node : actorList7)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::Test_ReadAgain] e [node:%x] [cv3:%x] [cv4:%x]"), this, node.actor,node.cv3.actor, node.cv4.actor);
	}

	// destroy后强制gc前可以访问,强制gc后访问奔溃
	// for (auto node : actorList)
	// {
	//     Cast<ATestChildActor>(node)->Test();
	// }
}

/*------------------------------------------------------------------*/

// uobject 对象释放需要共享指针主动置空再gc

// uobject 生成uobject
void ATestActor::Test_ObjectSpawn()
{
	for (int i = 0; i < 2; i++)
	{

		auto object = NewObject<UTestChildObject>();
		
		objectList3.push_back(object);

		if(testObjType==1)
		{
			{
				objectList4.Add(object);
			}
			
			// {
			// 	FCvST2 cv;
			// 	cv.object = object;
			// 	objectList7.Add(cv);
			// }
		}
		else if (testObjType == 2)
		{
			{
				CvST1 cv;
				cv.object = object;
				objectList5.Add(cv);
			}
			{
				objectList4.Add(object);
			}
			// {
			// 	FCvST2 cv;
			// 	cv.object = object;
			// 	objectList7.Add(cv);
			// }
		}
		else if (testObjType == 3)
		{
			{
				FCvST2 cv;
				cv.object = object;
				objectList6.Add(cv);
			}
			{
				objectList4.Add(object);
			}
			// {
			// 	FCvST2 cv;
			// 	cv.object = object;
			// 	objectList7.Add(cv);
			// }
		}
		else if (testObjType == 4)
		{
			{
				FCvST2 cv;
				cv.object = object;
				objectList7.Add(cv);
			}
			// {
			// 	FCvST2 cv;
			// 	cv.object = object;
			// 	objectList6.Add(cv);
			// }
			// {
			// 	CvST1 cv;
			// 	cv.object = object;
			// 	objectList5.Add(cv);
			// }
			{

				objectList4.Add(object);
			}
		}
	}
}

// actor 销毁
void ATestActor::Test_ObjectDestroy()
{

	if (testObjType == 1)
	{
		for (int i = 0; i < objectList4.Num(); i++)
		{
			objectList4[i] = nullptr;
		}
	}
	else if (testObjType == 2)
	{
		for (int i = 0; i < objectList5.Num(); i++)
		{
			objectList5[i].object = nullptr;
		}
	}
	else if (testObjType == 3)
	{
		for (int i = 0; i < objectList6.Num(); i++)
		{
			objectList6[i].object = nullptr;
		}
	}
	else if (testObjType == 4)
	{
		for (int i = 0; i < objectList7.Num(); i++)
		{
			objectList7[i].object = nullptr;
		}
	}
}

// actor 强制GC
void ATestActor::Test_ObjectGC()
{
	// gc后执行析构
	UKismetSystemLibrary::CollectGarbage();
}

// actor 再访问
void ATestActor::Test_ObjectReadAgain()
{
	for (auto node : objectList3)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::Test_ObjectReadAgain] a [node:%x] "), this, node.Get());
	}
	
	// for (auto node : actorList4)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::Test_ReadAgain] b [node:%x] "), this, node);
	// }
	// for (auto node : actorList5)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::Test_ReadAgain] c [node:%x] "), this, node.actor);
	// }
	//
	// for (auto node : actorList6)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::Test_ReadAgain] d [node:%x] "), this, node.actor);
	// }
	// for (auto node : actorList7)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestActor::Test_ReadAgain] e [node:%x] "), this, node.actor);
	// }

	// destroy后强制gc前可以访问,强制gc后访问奔溃
	// for (auto node : actorList)
	// {
	//     Cast<ATestChildActor>(node)->Test();
	// }
}

/*------------------------------------------------------------------*/