// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectedViewActor.h"
#include "FunctionalModule/Selected/SelectedDataComponent.h"
#include "FunctionalModule/Selected/SelectedControllerComponent.h"
#include "Tools/CommonlyObject.h"
#include "Core/BaseGameInstance.h"
#include "Tools/CalculationObject.h"
#include "GameMode/BaseGameMode.h"

#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceConstant.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Engine/Selection.h"

/*
view
*/

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

// 构造
ASelectedViewActor::ASelectedViewActor()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	RootComponent = sceneRoot;

	PlaneMesh();
	CreateAxis();
	CreateSpinMesh();
	CreateZoomMesh();
	LoadMaterial();

	my2TempMesh0 = CreateTempMesh(0);
	my2TempMesh1 = CreateTempMesh(1);
	my2TempMesh2 = CreateTempMesh(2);
	my2TempMesh3 = CreateTempMesh(3);
	my2TempMesh4 = CreateTempMesh(4);
	my2TempMesh5 = CreateTempMesh(5);
	showXyzComponentList.Add(my2TempMesh0);
	showXyzComponentList.Add(my2TempMesh1);
	showXyzComponentList.Add(my2TempMesh2);
	showXyzComponentList.Add(my2TempMesh3);
	showXyzComponentList.Add(my2TempMesh4);
	showXyzComponentList.Add(my2TempMesh5);

	data = CreateDefaultSubobject<USelectedDataComponent>(TEXT("USelectedDataComponent"));

	controller = CreateDefaultSubobject<USelectedControllerComponent>(TEXT("USelectedControllerComponent"));

}

// 构造 平面网格
void ASelectedViewActor::PlaneMesh()
{
	// 中间网格
	myPlaneMove = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NameMyPlaneMove"));
	myPlaneMove->SetGenerateOverlapEvents(false);
	myPlaneMove->SetCollisionObjectType(SelectedInstance);
	myPlaneMove->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	myPlaneMove->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	myPlaneMove->SetEnableGravity(false);
	myPlaneMove->SetupAttachment(sceneRoot);
	myPlaneMove->SetRelativeLocation(FVector(0, 0, 0));
	myPlaneMove->SetRelativeRotation(FRotator(0, 0, 0));
	myPlaneMove->SetHiddenInGame(false);
	myPlaneMove->SetRenderCustomDepth(false);
	myPlaneMove->SetCustomDepthStencilValue(2);
	myPlaneMove->SetCastShadow(false);
}

// 构造 临时网格
UStaticMeshComponent* ASelectedViewActor::CreateTempMesh(int index)
{
	// 中间网格
	UStaticMeshComponent* component = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("My2TempMesh_%d"), index));
	component->SetGenerateOverlapEvents(false);
	component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	component->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	component->SetEnableGravity(false);
	component->AttachToComponent(myLocationX, FAttachmentTransformRules::KeepRelativeTransform);
	component->SetRelativeLocation(FVector(0, 0, 0));
	component->SetRelativeRotation(FRotator(0, 0, 0));
	component->SetHiddenInGame(false);
	component->SetCastShadow(false);
	return component;
}

// 构造 创建坐标轴
void ASelectedViewActor::CreateAxis()
{
	// x pitch
	myLocationX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NameMyLocationX"));
	myLocationX->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	myLocationX->bReceivesDecals = true;
	myLocationX->SetGenerateOverlapEvents(false);
	myLocationX->SetCollisionObjectType(SelectedInstance);
	myLocationX->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	myLocationX->SetCollisionResponseToAllChannels(ECR_Ignore);
	myLocationX->SetEnableGravity(false);
	myLocationX->SetupAttachment(sceneRoot);
	myLocationX->SetRelativeLocation(FVector(0, 0, 0));
	myLocationX->SetRelativeRotation(FRotator(0, 0, 0));
	myLocationX->SetRenderCustomDepth(false);
	myLocationX->SetCustomDepthStencilValue(2);
	myLocationX->SetCastShadow(false);

	// y yaw
	myLocationY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NameMyLocationY"));
	myLocationY->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	myLocationY->bReceivesDecals = true;
	myLocationY->SetGenerateOverlapEvents(false);
	myLocationY->SetCollisionObjectType(SelectedInstance);
	myLocationY->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	myLocationY->SetCollisionResponseToAllChannels(ECR_Ignore);
	myLocationY->SetEnableGravity(false);
	myLocationY->SetupAttachment(sceneRoot);
	myLocationY->SetHiddenInGame(false);
	myLocationY->SetRelativeLocation(FVector(0, 0, 0));
	myLocationY->SetRelativeRotation(FRotator(0, 90, 0));
	myLocationY->SetRenderCustomDepth(false);
	myLocationY->SetCustomDepthStencilValue(2);
	myLocationY->SetCastShadow(false);

	// z roll
	myLocationZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NameMyLocationZ"));
	myLocationZ->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	myLocationZ->bReceivesDecals = true;
	myLocationZ->SetGenerateOverlapEvents(false);
	myLocationZ->SetCollisionObjectType(SelectedInstance);
	myLocationZ->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	myLocationZ->SetCollisionResponseToAllChannels(ECR_Ignore);
	myLocationZ->SetEnableGravity(false);
	myLocationZ->SetupAttachment(sceneRoot);
	myLocationZ->SetHiddenInGame(false);
	myLocationZ->SetRelativeLocation(FVector(0, 0, 0));
	myLocationZ->SetRelativeRotation(FRotator(90, 0, 0));
	myLocationZ->SetRenderCustomDepth(false);
	myLocationZ->SetCustomDepthStencilValue(2);
	myLocationZ->SetCastShadow(false);

	// 模型&材质
	FString meshPath = TEXT("StaticMesh'/Game/Resources/Mesh/Selected/local_arrow.local_arrow'");

	UStaticMesh* mesh = LoadObject<UStaticMesh>(NULL, *meshPath);

	if (mesh)
	{
		myLocationX->SetStaticMesh(mesh);
		myLocationY->SetStaticMesh(mesh);
		myLocationZ->SetStaticMesh(mesh);
	}
}

// 构造 创建旋转网格
void ASelectedViewActor::CreateSpinMesh()
{
	// x pitch
	myRotatorPitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NameMyRotatorPitch"));
	myRotatorPitch->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	myRotatorPitch->bReceivesDecals = true;
	myRotatorPitch->SetGenerateOverlapEvents(false);
	myRotatorPitch->SetCollisionObjectType(SelectedInstance);
	myRotatorPitch->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	myRotatorPitch->SetCollisionResponseToAllChannels(ECR_Ignore);
	myRotatorPitch->SetEnableGravity(false);
	myRotatorPitch->SetupAttachment(sceneRoot);
	myRotatorPitch->SetRelativeLocation(FVector(0, 0, 0));
	myRotatorPitch->SetRelativeRotation(FRotator(0, 0, 90));
	myRotatorPitch->SetRenderCustomDepth(false);
	myRotatorPitch->SetCustomDepthStencilValue(2);
	myRotatorPitch->SetCastShadow(false);

	// y yaw
	myRotatorYaw = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NameMyRotatorYaw"));
	myRotatorYaw->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	myRotatorYaw->bReceivesDecals = true;
	myRotatorYaw->SetGenerateOverlapEvents(false);
	myRotatorYaw->SetCollisionObjectType(SelectedInstance);
	myRotatorYaw->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	myRotatorYaw->SetCollisionResponseToAllChannels(ECR_Ignore);
	myRotatorYaw->SetEnableGravity(false);
	myRotatorYaw->SetupAttachment(sceneRoot);
	myRotatorYaw->SetHiddenInGame(false);
	myRotatorYaw->SetRelativeLocation(FVector(0, 0, 0));
	myRotatorYaw->SetRelativeRotation(FRotator(0, 0, 0));
	myRotatorYaw->SetRenderCustomDepth(false);
	myRotatorYaw->SetCustomDepthStencilValue(2);
	myRotatorYaw->SetCastShadow(false);

	// z roll
	myRotatorRoll = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NameMyRotatorRoll"));
	myRotatorRoll->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	myRotatorRoll->bReceivesDecals = true;
	myRotatorRoll->SetGenerateOverlapEvents(false);
	myRotatorRoll->SetCollisionObjectType(SelectedInstance);
	myRotatorRoll->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	myRotatorRoll->SetCollisionResponseToAllChannels(ECR_Ignore);
	myRotatorRoll->SetEnableGravity(false);
	myRotatorRoll->SetupAttachment(sceneRoot);
	myRotatorRoll->SetHiddenInGame(false);
	myRotatorRoll->SetRelativeLocation(FVector(0, 0, 0));
	myRotatorRoll->SetRelativeRotation(FRotator(90, 0, 0));
	myRotatorRoll->SetRenderCustomDepth(false);
	myRotatorRoll->SetCustomDepthStencilValue(2);
	myRotatorRoll->SetCastShadow(false);

	// 模型&材质
	FString meshPath = TEXT("StaticMesh'/Game/Resources/Mesh/Selected/rotation_ring.rotation_ring'");

	UStaticMesh* mesh = LoadObject<UStaticMesh>(NULL, *meshPath);
	//UE_LOG(LogTemp, Log, TEXT("[%x] [CreateSpinMesh] [%x]  [%x] [%x] [%x] [%x] "),
	//       this, mesh, rotatorPitchMaterial, rotatorYawMaterial, rotatorRollMaterial, rotatorSelectMaterial);

	if (mesh)
	{
		myRotatorPitch->SetStaticMesh(mesh);
		myRotatorYaw->SetStaticMesh(mesh);
		myRotatorRoll->SetStaticMesh(mesh);
	}
}

// 构造 创建缩放网格
void ASelectedViewActor::CreateZoomMesh()
{
	// x pitch
	myScaleX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NameMyScaleX"));
	myScaleX->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	myScaleX->bReceivesDecals = true;
	myScaleX->SetGenerateOverlapEvents(false);
	myScaleX->SetCollisionObjectType(SelectedInstance);
	myScaleX->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	myScaleX->SetCollisionResponseToAllChannels(ECR_Ignore);
	myScaleX->SetEnableGravity(false);
	myScaleX->SetupAttachment(sceneRoot);
	myScaleX->SetRelativeLocation(FVector(0, 0, 0));
	myScaleX->SetRelativeRotation(FRotator(0, 0, 0));
	myScaleX->SetRenderCustomDepth(false);
	myScaleX->SetCustomDepthStencilValue(2);
	myScaleX->SetCastShadow(false);

	// y yaw
	myScaleY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NameMyScaleY"));
	myScaleY->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	myScaleY->bReceivesDecals = true;
	myScaleY->SetGenerateOverlapEvents(false);
	myScaleY->SetCollisionObjectType(SelectedInstance);
	myScaleY->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	myScaleY->SetCollisionResponseToAllChannels(ECR_Ignore);
	myScaleY->SetEnableGravity(false);
	myScaleY->SetupAttachment(sceneRoot);
	myScaleY->SetHiddenInGame(false);
	myScaleY->SetRelativeLocation(FVector(0, 0, 0));
	myScaleY->SetRelativeRotation(FRotator(0, 90, 0));
	myScaleY->SetRenderCustomDepth(false);
	myScaleY->SetCustomDepthStencilValue(2);
	myScaleY->SetCastShadow(false);

	// z roll
	myScaleZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NameMyScaleZ"));
	myScaleZ->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	myScaleZ->bReceivesDecals = true;
	myScaleZ->SetGenerateOverlapEvents(false);
	myScaleZ->SetCollisionObjectType(SelectedInstance);
	myScaleZ->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	myScaleZ->SetCollisionResponseToAllChannels(ECR_Ignore);
	myScaleZ->SetEnableGravity(false);
	myScaleZ->SetupAttachment(sceneRoot);
	myScaleZ->SetHiddenInGame(false);
	myScaleZ->SetRelativeLocation(FVector(0, 0, 0));
	myScaleZ->SetRelativeRotation(FRotator(90, 0, 0));
	myScaleZ->SetRenderCustomDepth(false);
	myScaleZ->SetCustomDepthStencilValue(2);
	myScaleZ->SetCastShadow(false);

	// 模型&材质
	FString meshPath = TEXT("StaticMesh'/Game/Resources/Mesh/Selected/spin_box.spin_box'");

	UStaticMesh* mesh = LoadObject<UStaticMesh>(NULL, *meshPath);

	if (mesh)
	{
		myScaleX->SetStaticMesh(mesh);
		myScaleY->SetStaticMesh(mesh);
		myScaleZ->SetStaticMesh(mesh);
	}
}

// 构造 加载材质
void ASelectedViewActor::LoadMaterial()
{
	FString xPath = TEXT("MaterialInstanceConstant'/Game/Resources/Material/Selected/MI_Ghost_Select1.MI_Ghost_Select1'");
	FString yPath = TEXT("MaterialInstanceConstant'/Game/Resources/Material/Selected/MI_Ghost_Select2.MI_Ghost_Select2'");
	FString zPath = TEXT("MaterialInstanceConstant'/Game/Resources/Material/Selected/MI_Ghost_Select3.MI_Ghost_Select3'");
	FString rotatorSelectPath = TEXT("MaterialInstanceConstant'/Game/Resources/Material/Selected/MI_Ghost_Select4.MI_Ghost_Select4'");
	rotatorPitchMaterial = LoadObject<UMaterialInstanceConstant>(NULL, *xPath);
	rotatorYawMaterial = LoadObject<UMaterialInstanceConstant>(NULL, *yPath);
	rotatorRollMaterial = LoadObject<UMaterialInstanceConstant>(NULL, *zPath);
	rotatorSelectMaterial = LoadObject<UMaterialInstanceConstant>(NULL, *rotatorSelectPath);
	if (rotatorPitchMaterial
		&& rotatorYawMaterial
		&& rotatorRollMaterial
		&& rotatorSelectMaterial)
	{
		myLocationX->SetMaterial(0, rotatorPitchMaterial);
		myLocationY->SetMaterial(0, rotatorYawMaterial);
		myLocationZ->SetMaterial(0, rotatorRollMaterial);

		myRotatorPitch->SetMaterial(0, rotatorPitchMaterial);
		myRotatorYaw->SetMaterial(0, rotatorYawMaterial);
		myRotatorRoll->SetMaterial(0, rotatorRollMaterial);

		myScaleX->SetMaterial(0, rotatorPitchMaterial);
		myScaleY->SetMaterial(0, rotatorYawMaterial);
		myScaleZ->SetMaterial(0, rotatorRollMaterial);
	}
}

//
void ASelectedViewActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//
void ASelectedViewActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	Start();
}

//
void ASelectedViewActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	End();
}

// START
void ASelectedViewActor::Start()
{
	// 注册到GI
	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	data->Start(controller, this);
	controller->Start(data, this);

	//auto componentList = GetComponentsByClass(UMeshComponent::StaticClass());
	//for (auto node : componentList) {
	//	auto name = node->GetName();
	//	if (name.Contains(TEXT("Cube"))) {
	//		auto mesh = Cast<UMeshComponent>(node);
	//		if (mesh) {
	//			showXyzComponentList.Add(mesh);
	//		}
	//	}
	//}


	UE_LOG(LogTemp, Log, TEXT("[%x] [ASelectedViewActor::Start] "), this);
}

// END
void ASelectedViewActor::End()
{
	data->End();
	controller->End();

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//baseGameMode->selectedViewActor = nullptr;

	rotatorPitchMaterial->RemoveFromRoot();
	rotatorPitchMaterial = nullptr;
	rotatorYawMaterial->RemoveFromRoot();
	rotatorYawMaterial = nullptr;
	rotatorRollMaterial->RemoveFromRoot();
	rotatorRollMaterial = nullptr;
	rotatorSelectMaterial->RemoveFromRoot();
	rotatorSelectMaterial = nullptr;

	UE_LOG(LogTemp, Log, TEXT("[%x] [ASelectedViewActor::End]   "), this);
}

// tick
void ASelectedViewActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (controller)
	{
	}
}

// 设置 还原所有坐标轴箭头颜色
void ASelectedViewActor::RestoreAxisArrowColor()
{
	if (data->changeAxisArrowColorType == ESelectedMoveStrategy::LocationX)
	{
		myLocationX->SetMaterial(0, rotatorPitchMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::LocationY)
	{
		myLocationY->SetMaterial(0, rotatorYawMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::LocationZ)
	{
		myLocationZ->SetMaterial(0, rotatorRollMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::RotatorPitch)
	{
		myRotatorPitch->SetMaterial(0, rotatorPitchMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::RotatorYaw)
	{
		myRotatorYaw->SetMaterial(0, rotatorYawMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::RotatorRoll)
	{
		myRotatorRoll->SetMaterial(0, rotatorRollMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::ScaleX)
	{
		myScaleX->SetMaterial(0, rotatorPitchMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::ScaleY)
	{
		myScaleY->SetMaterial(0, rotatorYawMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::ScaleZ)
	{
		myScaleZ->SetMaterial(0, rotatorRollMaterial);
	}
	//UE_LOG(LogTemp, Log, TEXT("[%x] [RestoreAxisArrowColor] 还原 [%f] [%d]"), this, GetWorld()->TimeSeconds, data->changeAxisArrowColorType);
}

// 设置 改变坐标轴箭头颜色
void ASelectedViewActor::ChangeAxisArrowColor()
{
	if (data->changeAxisArrowColorType == ESelectedMoveStrategy::LocationX)
	{
		myLocationX->SetMaterial(0, rotatorSelectMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::LocationY)
	{
		myLocationY->SetMaterial(0, rotatorSelectMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::LocationZ)
	{
		myLocationZ->SetMaterial(0, rotatorSelectMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::RotatorPitch)
	{
		myRotatorPitch->SetMaterial(0, rotatorSelectMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::RotatorYaw)
	{
		myRotatorYaw->SetMaterial(0, rotatorSelectMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::RotatorRoll)
	{
		myRotatorRoll->SetMaterial(0, rotatorSelectMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::ScaleX)
	{
		myScaleX->SetMaterial(0, rotatorSelectMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::ScaleY)
	{
		myScaleY->SetMaterial(0, rotatorSelectMaterial);
	}
	else if (data->changeAxisArrowColorType == ESelectedMoveStrategy::ScaleZ)
	{
		myScaleZ->SetMaterial(0, rotatorSelectMaterial);
	}
	//UE_LOG(LogTemp, Log, TEXT("[%x] [ChangeAxisArrowColor] 改变 [%f] [%d]"), this, GetWorld()->TimeSeconds, data->changeAxisArrowColorType);
}

// 设置 工作模式
void ASelectedViewActor::SetWorkMode()
{
	//if (data->selectedWorkMode == ESelectedWorkMode::None)
	//{
	//	ShowHideAuxiliaryLine(true);
	//	myPlaneMove->SetHiddenInGame(true);
	//	myLocationX->SetHiddenInGame(true);
	//	myLocationY->SetHiddenInGame(true);
	//	myLocationZ->SetHiddenInGame(true);
	//	myRotatorPitch->SetHiddenInGame(true);
	//	myRotatorYaw->SetHiddenInGame(true);
	//	myRotatorRoll->SetHiddenInGame(true);
	//	myScaleX->SetHiddenInGame(true);
	//	myScaleY->SetHiddenInGame(true);
	//	myScaleZ->SetHiddenInGame(true);
	//}
	//else 
	if (data->selectedWorkMode == ESelectedWorkMode::WorldLocation
		|| data->selectedWorkMode == ESelectedWorkMode::RelativelyLocation)
	{
		ShowHideAuxiliaryLine(false);
		myPlaneMove->SetHiddenInGame(false);
		myLocationX->SetHiddenInGame(false);
		myLocationY->SetHiddenInGame(false);
		myLocationZ->SetHiddenInGame(false);
		myRotatorPitch->SetHiddenInGame(true);
		myRotatorYaw->SetHiddenInGame(true);
		myRotatorRoll->SetHiddenInGame(true);
		myScaleX->SetHiddenInGame(true);
		myScaleY->SetHiddenInGame(true);
		myScaleZ->SetHiddenInGame(true);
	}
	else if (data->selectedWorkMode == ESelectedWorkMode::WorldRotator
		|| data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
	{
		ShowHideAuxiliaryLine(true);
		myPlaneMove->SetHiddenInGame(true);
		myLocationX->SetHiddenInGame(true);
		myLocationY->SetHiddenInGame(true);
		myLocationZ->SetHiddenInGame(true);
		myRotatorPitch->SetHiddenInGame(false);
		myRotatorYaw->SetHiddenInGame(false);
		myRotatorRoll->SetHiddenInGame(false);
		myScaleX->SetHiddenInGame(true);
		myScaleY->SetHiddenInGame(true);
		myScaleZ->SetHiddenInGame(true);
	}
	else if (data->selectedWorkMode == ESelectedWorkMode::Scale)
	{
		ShowHideAuxiliaryLine(true);
		myPlaneMove->SetHiddenInGame(true);
		myLocationX->SetHiddenInGame(true);
		myLocationY->SetHiddenInGame(true);
		myLocationZ->SetHiddenInGame(true);
		myRotatorPitch->SetHiddenInGame(true);
		myRotatorYaw->SetHiddenInGame(true);
		myRotatorRoll->SetHiddenInGame(true);
		myScaleX->SetHiddenInGame(false);
		myScaleY->SetHiddenInGame(false);
		myScaleZ->SetHiddenInGame(false);
	}
}

// 设置 只显示当前选中策略
void ASelectedViewActor::SetShowSelectedMoveStrategySingle(ESelectedMoveStrategy selectedMoveStrategy_)
{
	if (data->selectedWorkMode == ESelectedWorkMode::WorldLocation
		|| data->selectedWorkMode == ESelectedWorkMode::RelativelyLocation)
	{
		myPlaneMove->SetHiddenInGame(true);
		myLocationX->SetHiddenInGame(true);
		myLocationY->SetHiddenInGame(true);
		myLocationZ->SetHiddenInGame(true);
		ShowHideAuxiliaryLine(true);
		if (selectedMoveStrategy_ == ESelectedMoveStrategy::LocationX)
		{
			myLocationX->SetHiddenInGame(false);
		}
		else if (selectedMoveStrategy_ == ESelectedMoveStrategy::LocationY)
		{
			myLocationY->SetHiddenInGame(false);
		}
		else if (selectedMoveStrategy_ == ESelectedMoveStrategy::LocationZ)
		{
			myLocationZ->SetHiddenInGame(false);
		}
		else if (selectedMoveStrategy_ == ESelectedMoveStrategy::planeMove)
		{
			myPlaneMove->SetHiddenInGame(false);
		}
	}
	else if (data->selectedWorkMode == ESelectedWorkMode::WorldRotator
		|| data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
	{
		myRotatorPitch->SetHiddenInGame(true);
		myRotatorYaw->SetHiddenInGame(true);
		myRotatorRoll->SetHiddenInGame(true);
		if (selectedMoveStrategy_ == ESelectedMoveStrategy::RotatorPitch)
		{
			myRotatorPitch->SetHiddenInGame(false);
		}
		else if (selectedMoveStrategy_ == ESelectedMoveStrategy::RotatorYaw)
		{
			myRotatorYaw->SetHiddenInGame(false);
		}
		else if (selectedMoveStrategy_ == ESelectedMoveStrategy::RotatorRoll)
		{
			myRotatorRoll->SetHiddenInGame(false);
		}
	}
	else if (data->selectedWorkMode == ESelectedWorkMode::Scale)
	{
		myScaleX->SetHiddenInGame(true);
		myScaleY->SetHiddenInGame(true);
		myScaleZ->SetHiddenInGame(true);
		if (selectedMoveStrategy_ == ESelectedMoveStrategy::ScaleX)
		{
			myScaleX->SetHiddenInGame(false);
		}
		else if (selectedMoveStrategy_ == ESelectedMoveStrategy::ScaleY)
		{
			myScaleY->SetHiddenInGame(false);
		}
		else if (selectedMoveStrategy_ == ESelectedMoveStrategy::ScaleZ)
		{
			myScaleZ->SetHiddenInGame(false);
		}
	}
	// else if (data->selectedWorkMode == ESelectedWorkMode::PlaneMove)
	// {
	//     planeMove->SetHiddenInGame(false);
	//     locationX2->SetHiddenInGame(true);
	//     locationY2->SetHiddenInGame(true);
	//     locationZ2->SetHiddenInGame(true);
	// }
}

// 设置 还原当前工作模式显示
void ASelectedViewActor::SetShowSelectedMoveStrategyAll()
{
	if (data->selectedWorkMode == ESelectedWorkMode::WorldLocation
		|| data->selectedWorkMode == ESelectedWorkMode::RelativelyLocation)
	{
		myPlaneMove->SetHiddenInGame(false);
		myLocationX->SetHiddenInGame(false);
		myLocationY->SetHiddenInGame(false);
		myLocationZ->SetHiddenInGame(false);
		ShowHideAuxiliaryLine(false);
	}
	else if (data->selectedWorkMode == ESelectedWorkMode::WorldRotator
		|| data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
	{
		myRotatorPitch->SetHiddenInGame(false);
		myRotatorYaw->SetHiddenInGame(false);
		myRotatorRoll->SetHiddenInGame(false);
	}
	else if (data->selectedWorkMode == ESelectedWorkMode::Scale)
	{
		myScaleX->SetHiddenInGame(false);
		myScaleY->SetHiddenInGame(false);
		myScaleZ->SetHiddenInGame(false);
	}
	// else if (data->selectedWorkMode == ESelectedWorkMode::PlaneMove)
	// {
	//     planeMove->SetHiddenInGame(false);
	//     locationX2->SetHiddenInGame(false);
	//     locationY2->SetHiddenInGame(false);
	//     locationZ2->SetHiddenInGame(false);
	// }
}

// 显示隐藏辅助线
void ASelectedViewActor::ShowHideAuxiliaryLine(bool isShow)
{
	for (auto node : showXyzComponentList)
	{
		if (isShow)
		{
			node->SetHiddenInGame(isShow);
		}
		else
		{
			node->SetHiddenInGame(isShow);
		}
	}

}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

// 测试 将辅助面xy调整到人对其的面
void ASelectedViewActor::RollFaceXY(ACharacter* character)
{
	if (character == nullptr)return;

	auto characterLocation = character->GetActorLocation();
	auto location = GetActorLocation();

	float lenght = (characterLocation - location).Size();

	//FVector scale3D(1, 1, 1);
	//scale3D = scale3D * (lenght / 10000);

	//sceneRoot->SetWorldScale3D(scale3D);

	//auto xLocation = rayAssistBoxX->GetComponentLocation();
	//FRotator r1(90, 0, 0);
	//auto xDownLocaton = xLocation + r1.Vector() * 10;
	//auto line1 = characterLocation - xLocation;
	//auto line2= xDownLocaton - xLocation;
	//float angle = UCalculationObject::GetVectorAngle(line1, line2);

	//FRotator r2(0, 0, 0);
	//r2.Roll = angle;
	//rayAssistBoxX->SetWorldRotation(r2);

	//UE_LOG(LogTemp, Log, TEXT("[%x] [ASelectedViewActor::RollFaceXY] [%s] "), this, *scale3D.ToString());
	//UBoxComponent* rayAssistBoxX;
	//	UBoxComponent* rayAssistBoxY;
}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/