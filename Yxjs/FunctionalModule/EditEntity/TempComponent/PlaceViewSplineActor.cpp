// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceViewSplineActor.h"


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


/*
view
*/


/*------------------------------------------------------------------*/

//
APlaceViewSplineActor::APlaceViewSplineActor()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	sceneRoot->SetMobility(EComponentMobility::Movable);
	RootComponent = sceneRoot;

	splineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("USplineComponent"));
	splineComponent->SetMobility(EComponentMobility::Movable);
	splineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	splineComponent->SetHiddenInGame(false);
	splineComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

//
void APlaceViewSplineActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

//
void APlaceViewSplineActor::BeginPlay()
{
	Super::BeginPlay();
	//Start();
}

//
void APlaceViewSplineActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//End();
}

// tick
void APlaceViewSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*------------------------------------------------------------------*/

// START
void APlaceViewSplineActor::Start(UPlayerRangedWeaponComponent* playerRangedWeaponComponent_)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [APlaceViewSplineActor::Start] "), this);

	this->AddToRoot();

	FString meshPath = TEXT("StaticMesh'/Game/Res/RangedWeapon/mode/yuanzhu_01.yuanzhu_01'");
	FString meshPath2 = TEXT("StaticMesh'/Game/Res/RangedWeapon/mode/yuanzhu_02.yuanzhu_02'");

	splineComponent->RemoveSplinePoint(1);

	for (int i = 0; i < maxPointCount; i++)
	{
		auto v1 = FVector(maxPointCount * i, maxPointCount * i, maxPointCount * i + 50);
		if (i < 3)
		{
			CreateSplineMeshComponent(v1, meshPath);
		}
		else
		{
			CreateSplineMeshComponent(v1, meshPath2);
		}
	}
}

// END
void APlaceViewSplineActor::End()
{
	// 销毁所有mesh
	int index = 0;
	for (auto node : splineMeshList)
	{
		//splineComponent->RemoveSplinePoint(index);
		//index++;
		CloseSplineMeshComponent(node);
	}
	splineMeshList.Reset(0);

	this->RemoveFromRoot();

	UE_LOG(LogTemp, Log, TEXT("[%x] [APlaceViewSplineActor::End]   "), this);
}

/*------------------------------------------------------------------*/

// 资源 加载资源
bool APlaceViewSplineActor::LoadRes(FString meshPath, USceneComponent* meshComponent)
{
	FSoftObjectPath meshObj;
	meshObj.SetPath(meshPath);
	ri.resPathMap.Add(meshComponent, meshObj);
	//加载资源
	int32 var = 334455;
	TSharedPtr<FStreamableHandle> handle = UAssetManager::GetStreamableManager().RequestAsyncLoad(ri.resPathMap[meshComponent],
		FStreamableDelegate::CreateUObject(this, &APlaceViewSplineActor::CallBackLoadRes, var, meshComponent), 0, true);
	ri.resHandleMap.Add(meshComponent, handle);
	return true;
}

// 资源 加载完成回调
void APlaceViewSplineActor::CallBackLoadRes(int32 var, USceneComponent* component)
{
	if (this && component && ri.resHandleMap.Find(component))
	{
		TSharedPtr<FStreamableHandle> handle = ri.resHandleMap[component];
		UObject* obj = handle->GetLoadedAsset();

		if (obj)
		{
			UStaticMesh* mesh = Cast<UStaticMesh>(obj);
			auto meshComponent_ = Cast<UStaticMeshComponent>(component);

			if (mesh && meshComponent_)
			{
				bool flag = meshComponent_->SetStaticMesh(mesh);
			}
		}
	}
}

// 资源 卸载资源
void APlaceViewSplineActor::UnloadRes(USceneComponent* meshComponent)
{
	if (ri.resHandleMap.Find(meshComponent))
	{
		UAssetManager::GetStreamableManager().Unload(ri.resPathMap[meshComponent]);
		ri.resPathMap.Remove(meshComponent);
		ri.resHandleMap.Remove(meshComponent);
	}
}

/*------------------------------------------------------------------*/

// 样条线网格组件 创建
void APlaceViewSplineActor::CreateSplineMeshComponent(FVector location, FString meshPath)
{
	int meshIndex = splineMeshList.Num();
	auto pointIndex = splineComponent->GetNumberOfSplinePoints();
	FSplinePoint p(pointIndex, location);
	splineComponent->AddPoint(p, true);

	// 网格组件创建
	auto meshName1 = FString::Printf(TEXT("SplineMesh_%d"), meshIndex);
	USplineMeshComponent* meshComponent = nullptr;

	meshComponent = NewObject<USplineMeshComponent>(this, *meshName1);
	meshComponent->RegisterComponent();
	meshComponent->SetMobility(EComponentMobility::Type::Movable);
	bool flag = meshComponent->AttachToComponent(splineComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// 设置组件参数
	meshComponent->bReceivesDecals = false;
	meshComponent->CastShadow = false;

	meshComponent->SetGenerateOverlapEvents(false);
	meshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	meshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	meshComponent->SetHiddenInGame(false);
	//bool flag2 = meshComponent->SetStaticMesh(obj);
	LoadRes(meshPath, meshComponent);

	// 刷新当前mesh
	{
		FVector v1, v2, v3, v4;
		splineComponent->GetLocationAndTangentAtSplinePoint(pointIndex - 1, v1, v2, ESplineCoordinateSpace::Local);
		splineComponent->GetLocationAndTangentAtSplinePoint(pointIndex, v3, v4, ESplineCoordinateSpace::Local);
		meshComponent->SetStartAndEnd(v1, v2, v3, v4);
	}
	splineMeshList.Add(meshComponent);
	//UE_LOG(LogTemp, Log, TEXT("[%x] [CreateSplineMeshComponent] A [meshIndex:%d--%d] [pointIndex:%d] "), this, meshIndex, splineMeshList.Num()-1, pointIndex);


	// 刷新上一个mesh
	if (meshIndex >= 1)
	{
		auto previousIndex = meshIndex - 1;
		auto previousPointIndex = pointIndex - 1;
		auto previousSplineMesh = splineMeshList[previousIndex];
		FVector v1, v2, v3, v4;
		splineComponent->GetLocationAndTangentAtSplinePoint(previousPointIndex - 1, v1, v2, ESplineCoordinateSpace::Local);
		splineComponent->GetLocationAndTangentAtSplinePoint(previousPointIndex, v3, v4, ESplineCoordinateSpace::Local);
		previousSplineMesh->SetStartAndEnd(v1, v2, v3, v4);
		//previousSplineMesh->UpdateMesh();

		//UE_LOG(LogTemp, Log, TEXT("[%x] [CreateSplineMeshComponent] B [meshIndex:%d] [pointIndex:%d] "), this, previousIndex, previousPointIndex);
	}


}

// 样条线网格组件 销毁
void APlaceViewSplineActor::CloseSplineMeshComponent(USplineMeshComponent* meshComponent)
{
	if (meshComponent)
	{
		bool isReg = false;
		if (meshComponent->IsRegistered())
		{
			meshComponent->UnregisterComponent();
			isReg = true;
		}
		meshComponent->DestroyComponent();

		//UE_LOG(LogTemp, Log, TEXT("[%x] [CloseMeshComponent] a [name:%s] [isReg:%d]"), this, *key, isReg);
	}
}

// 抛物线 更新网格位置
void APlaceViewSplineActor::UpdateSplineMesh(int meshIndex)
{
	auto pointIndex = meshIndex - 1;
	USplineMeshComponent* splineMesh = splineMeshList[meshIndex];
	FVector v1, v2, v3, v4;
	splineComponent->GetLocationAndTangentAtSplinePoint(pointIndex - 1, v1, v2, ESplineCoordinateSpace::Local);
	splineComponent->GetLocationAndTangentAtSplinePoint(pointIndex, v3, v4, ESplineCoordinateSpace::Local);
	splineMesh->SetStartAndEnd(v1, v2, v3, v4);
	//splineMesh->UpdateMesh();
}

/*------------------------------------------------------------------*/

// 设置 重置样条线节点与网格
void APlaceViewSplineActor::ResetLinePoint(TArray<FVector>& pointList)
{
	auto count = splineComponent->GetNumberOfSplinePoints();
	if (count != pointList.Num())
	{
		return;
	}
	splineComponent->SetSplineWorldPoints(pointList);

	for (int meshIndex = 0; meshIndex < count - 1; meshIndex++)
	{
		UpdateSplineMesh(meshIndex);
	}

	auto hitLocation = pointList[pointList.Num() - 1];
}

/*------------------------------------------------------------------*/

// 测试 增加样条线节点
void APlaceViewSplineActor::TestAddPoint()
{
	// ACharacter* character = Cast<ACharacter>(playerRangedWeaponComponent->GetOwner());
	// auto characterLocation = character->GetActorLocation();
	// //auto index = splineComponent->GetNumberOfSplinePoints() ;
	// //FSplinePoint p(index, characterLocation);
	// //splineComponent->AddPoint(p,true);
	//
	//
	// UE_LOG(LogTemp, Log, TEXT("[%x] [APlaceViewSplineActor::TestAddPoint] 增加样条线节点 "), this);
	// //CreateSplineMeshComponent(characterLocation, staticMeshObject);

}

/*------------------------------------------------------------------*/
