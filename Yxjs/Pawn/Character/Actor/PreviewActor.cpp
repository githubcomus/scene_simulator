// Fill out your copyright notice in the Description page of Project Settings.

#include "PreviewActor.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "UI/BaseHud.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshView.h"

#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/HierarchicalInstancedStaticMeshComponent.h"
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
#include "Yxjs/Tools/CalculationObject.h"

// 功能模块
// view

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

//
APreviewActor::APreviewActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	RootComponent = sceneRoot;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("box"));
	boxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	boxComponent->SetBoxExtent(FVector(1, 1, 1));
	boxComponent->SetGenerateOverlapEvents(false);
	boxComponent->SetCollisionObjectType(BuildObject);
	boxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComponent->SetEnableGravity(false);
	boxComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	boxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	boxComponent->SetHiddenInGame(true);
	boxComponent->SetRelativeScale3D(FVector(100, 100, 100));

	//arrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("arrowComponent"));
	//arrowComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//arrowComponent->SetHiddenInGame(false);
	//arrowComponent->SetRelativeScale3D(FVector(3, 1, 1));
}

//
void APreviewActor::BeginPlay()
{
	Super::BeginPlay();
}

//
void APreviewActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// START
void APreviewActor::Start()
{
	CheckDefaultData();
}

// END
void APreviewActor::End()
{
}

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

// 检查必须的数据
bool APreviewActor::CheckDefaultData()
{
	bool checkTwice = false;
	if (gameInstance.IsValid() == false || gameMode.IsValid() == false
		|| playerController.IsValid() == false || hud.IsValid() == false)
	{
		gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
		gameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController.IsValid())hud = Cast<ABaseHUD>(playerController->GetHUD());

		checkTwice = true;
	}

	bool flag = true;
	if (checkTwice)
	{
		if (gameInstance.IsValid() == false || gameMode.IsValid() == false
			|| playerController.IsValid() == false || hud.IsValid() == false)
		{
			flag = false;
		}
	}

	return flag;
}

// 获取 当前放置系统视图
TWeakObjectPtr< AEditEntityManage> APreviewActor::GetEditEntityManage()
{
	if (gameMode.IsValid())
	{
		return gameMode->editEntityManage;
	}
	return nullptr;
}

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

// 开始预览
void APreviewActor::StartPerview(int xlsId, bool isMerge)
{
	SetActorHiddenInGame(false);

	auto editEntityManage = GetEditEntityManage();
	if (editEntityManage == nullptr)return;

	if (isMerge)
	{
		//	UE_LOG(LogTemp, Log, TEXT("[%x] [合并实例] [Start] [entityId:%d] [xlsId:%d] "), this, syncInfo->entityid(), syncInfo->xlsid());

		if (auto entityMeshMasterInfo = editEntityManage->data->GetMergeInfoWithXlsId(xlsId))
		{
			for (auto i = 0; i < entityMeshMasterInfo->child_size(); i++)
			{
				auto entityMeshChildInfo = entityMeshMasterInfo->mutable_child(i);
				auto childXlsInfo = editEntityManage->data->GetXlsInfoWithXlsId(entityMeshChildInfo->xlsid());

				ChildMeshInfo childMeshInfo;
				childMeshInfo.childRTransform = UCommonlyObject::PBToUe4Transform(entityMeshChildInfo->mutable_transform());

				FTransform wTransform = UCalculationObject::RelativeToWorldTransform(
					GetActorTransform(), childMeshInfo.childRTransform);

				FString meshPath = UTF8_TO_TCHAR(childXlsInfo->MeshPath_.c_str());
				FTempSceneStaticInfo tempInfo;
				tempInfo.openCollision = ECollisionEnabled::Type::NoCollision;
				tempInfo.entityId = 0;
				tempInfo.color = FVector::ZeroVector;
				tempInfo.highlight = false;
				tempInfo.materialPath = TEXT("");
				childMeshInfo.meshIstanceId = gameMode->sceneStaticMeshView->CreateInstance(ASceneStaticMeshView::ESceneActorType::ActorType_Item,
					meshPath, wTransform, false, tempInfo);

				meshIDList.push_back(childMeshInfo);
			}
		}
	}
	else
	{
	}
}

// 更新预览
void APreviewActor::UpdatePerview()
{
	for (auto childMeshInfo : meshIDList)
	{
		FTransform wTransform = UCalculationObject::RelativeToWorldTransform(
			GetActorTransform(), childMeshInfo.childRTransform);


		gameMode->sceneStaticMeshView->UpdateInstance(childMeshInfo.meshIstanceId, wTransform);
	}
}

// 结束预览
void APreviewActor::EndPerview()
{
	SetActorHiddenInGame(true);

	for (auto node : meshIDList)
	{
		gameMode->sceneStaticMeshView->DestroyInstance(node.meshIstanceId, false);
	}
	meshIDList.clear();
}