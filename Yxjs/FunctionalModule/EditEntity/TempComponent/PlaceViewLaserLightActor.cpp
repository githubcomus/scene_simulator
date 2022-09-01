// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceViewLaserLightActor.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "Tools/CommonlyObject.h"

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
#include "Runtime/Engine/Classes/Materials/MaterialInstanceConstant.h"
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
#include "Components/SpotLightComponent.h"
#include "Tools/CalculationObject.h"

/*
灯光视图
*/

/*------------------------------------------------------------------*/

//
APlaceViewLaserLightActor::APlaceViewLaserLightActor()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	sceneRoot->SetMobility(EComponentMobility::Movable);
	RootComponent = sceneRoot;

	bottomLayerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bottomLayerMeshComponent"));
	bottomLayerMeshComponent->SetMobility(EComponentMobility::Movable);
	bottomLayerMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	bottomLayerMeshComponent->SetHiddenInGame(false);
	bottomLayerMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bottomLayerMeshComponent->SetRelativeLocation(FVector(0, 0, -300));

	lightBulbMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("lightBulbMeshComponent"));
	lightBulbMeshComponent->SetMobility(EComponentMobility::Movable);
	lightBulbMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	lightBulbMeshComponent->SetHiddenInGame(false);
	lightBulbMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// spotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("USpotLightComponent"));
	// spotLight->SetMobility(EComponentMobility::Movable);
	// spotLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	// spotLight->SetHiddenInGame(false);

	lightMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("lightMeshComponent"));
	lightMeshComponent->SetMobility(EComponentMobility::Movable);
	lightMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	lightMeshComponent->SetHiddenInGame(false);
	lightMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

//
void APlaceViewLaserLightActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//
void APlaceViewLaserLightActor::BeginPlay()
{
	Super::BeginPlay();
}

//
void APlaceViewLaserLightActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// tick
void APlaceViewLaserLightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*------------------------------------------------------------------*/

// START
void APlaceViewLaserLightActor::Start(AEditEntityManage* view, Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [ ::Start] [entityId:%d] "), this, syncInfo->entityid());

	FString meshPath = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");

	LoadRes(true, meshPath, ELoadResourcesType::LoadRes_Mesh3, bottomLayerMeshComponent);
	LoadRes(true, meshPath, ELoadResourcesType::LoadRes_Mesh3, lightBulbMeshComponent);

	FString meshPath2 = TEXT("StaticMesh'/Game/Yxjs/Res/light/Mesh_TestLight2.Mesh_TestLight2'");
	LoadRes(true, meshPath2, ELoadResourcesType::LoadRes_Mesh4_light, lightMeshComponent);

	// // 强度
	// spotLight->SetIntensity(20000);
	// // 衰减半径
	// spotLight->SetAttenuationRadius(900);
	// // 锥体内部角度
	// spotLight->SetInnerConeAngle(13);
	// // 锥体外部角度
	// spotLight->SetOuterConeAngle(24);
	// // 体积散射强度
	// spotLight->SetVolumetricScatteringIntensity(2500);
	// // 颜色
	// spotLight->SetLightColor(FLinearColor(148, 0, 255));
}

// END
void APlaceViewLaserLightActor::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [APlaceViewLaserLightActor::End]   "), this);
}

/*------------------------------------------------------------------*/

// 服从controller控制和view的总控制
void APlaceViewLaserLightActor::TickMove(int time)
{
	// bool isOpen = 2;							// 激光开关
	// EntityLightColorType colorType = 3;		// 当前颜色
	// float brightness = 4;						// 亮度

	// 旋转
	// auto rotator = info->rotation();
	// auto rotator2 = UCommonlyObject::PBToUe4Rotator(&rotator);
	// auto move = info->move();
	// auto newMove = UCommonlyObject::PBToUe4Vector(&move);
	// auto rotator2 = newMove.Rotation();
	//
	// UE_LOG(LogTemp, Log, TEXT("[%x] [APlaceViewLaserLightActor::TickMove] [time:%d] [rotator:%s]  "), this, time,
	//        *rotator2.ToString());
	//
	// // SetActorRotation(rotator2);;
	// RealRelativeRotator(rotator2);
}

// 正式旋转
void APlaceViewLaserLightActor::RealRelativeRotator(FRotator& rotator)
{
	lightBulbMeshComponent->SetRelativeRotation(rotator);
	// spotLight->SetRelativeRotation(rotator);
	lightMeshComponent->SetRelativeRotation(rotator);
}

/*------------------------------------------------------------------*/

// 加载资源
void APlaceViewLaserLightActor::LoadRes(bool satrt, FString path, ELoadResourcesType type, UObject* obj)
{
	if (satrt)
	{
		if (resMap.Find(obj))
		{
			resMap.Remove(obj);
		}
		FSoftObjectPath softObj;
		softObj.SetPath(path);

		CvResourcesInfo cvResourcesInfo;
		cvResourcesInfo.path = path;
		cvResourcesInfo.handle = UAssetManager::GetStreamableManager().RequestAsyncLoad(softObj,
			FStreamableDelegate::
			CreateUObject(
				this,
				&APlaceViewLaserLightActor
				::
				LoadRes, false, path, type,
				obj), 0, false);

		resMap.Add(obj, cvResourcesInfo);
	}
	else
	{
		if (resMap.Find(obj) == false)
		{
			return;
		}

		// 如果当前path和map的path不相符合,那么不加载
		if (path.Equals(resMap[obj].path) == false)
		{
			return;
		}

		auto handle = resMap[obj].handle;
		UObject* resObj = handle->GetLoadedAsset();

		// 加载灯座静态网格
		if (type == ELoadResourcesType::LoadRes_Mesh3)
		{
			// 获取数据
			auto component = Cast<UStaticMeshComponent>(obj);
			UStaticMesh* mesh = Cast<UStaticMesh>(resObj);
			if (component && mesh)
			{
				component->SetStaticMesh(mesh);
			}
		}
		// 加载灯光束静态网格
		else if (type == ELoadResourcesType::LoadRes_Mesh4_light)
		{
			auto component = Cast<UStaticMeshComponent>(obj);
			UStaticMesh* mesh = Cast<UStaticMesh>(resObj);
			if (component && mesh)
			{
				component->SetStaticMesh(mesh);
			}
		}
		resMap.Remove(obj);
	}
}

/*------------------------------------------------------------------*/

// 激光 更新
void APlaceViewLaserLightActor::LaserLightUpdate(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	// 位置
	auto t2 = syncInfo->transform();
	auto spawnWTransform = UCommonlyObject::PBToUe4Transform(&t2);
	auto newWtransform = spawnWTransform;
	newWtransform.SetScale3D(FVector(1, 1, 1));
	SetActorTransform(newWtransform);

	// 灯光调制
	auto scale = spawnWTransform.GetScale3D();

	// // 强度
	// // spotLight->SetIntensity(20000);
	// // 衰减半径
	// spotLight->SetAttenuationRadius(1000 * scale.X);
	// // 锥体内部角度
	// spotLight->SetInnerConeAngle(10 * scale.Y);
	// // 锥体外部角度
	// spotLight->SetOuterConeAngle(10 * scale.Z);
}

// 鼠标晃动
void APlaceViewLaserLightActor::SetPreviewRotator(FRotator rotator)
{
	auto rRotator = UCalculationObject::WorldToRelativeTransform(GetTransform(), rotator);

	RealRelativeRotator(rRotator);
}

/*------------------------------------------------------------------*/