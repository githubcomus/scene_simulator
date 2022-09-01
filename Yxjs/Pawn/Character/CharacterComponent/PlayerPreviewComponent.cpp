// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPreviewComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerPlaceComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerMaterialComponent.h"
#include "Tools/CalculationObject.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/Selected/SelectedViewActor.h"
#include "Net/NetMainComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "UI/BaseHud.h"
#include "OnlineResources/OnlineResourcesComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Pawn/Character/Actor/PreviewActor.h"
#include "FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityEditComponent.h"
#include "PlayerEditEntityComponent.h"
#include "GameMode/Component/BaseGameModeController.h"
#include "FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "Core/BaseGameInstance.h"
#include "Core/SoundEffect/MySoundEffect.h"

#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Runtime/Engine/Classes/Engine/GameViewportClient.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include <math.h>
#include "../../../Core/GameInstanceStaticFunc.h"
#include "../../../UI/HUDStaticFunc.h"

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

// Sets default values for this component's properties
UPlayerPreviewComponent::UPlayerPreviewComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPlayerPreviewComponent::BeginPlay()
{
	Super::BeginPlay();
}

// EndPlay
void UPlayerPreviewComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// START
void UPlayerPreviewComponent::Start()
{
	character = Cast<ABaseCharacter>(GetOwner());

	FTransform transform;
	transform.SetLocation(FVector::ZeroVector);
	transform.SetRotation(FRotator::ZeroRotator.Quaternion());
	transform.SetScale3D(FVector(1, 1, 1));
	previewActor = Cast<APreviewActor>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), APreviewActor::StaticClass(), transform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (previewActor)
	{
		previewActor->SetOwner(GetOwner());
		UGameplayStatics::FinishSpawningActor(previewActor, transform);
	}
	previewActor->Start();
	previewActor->SetActorHiddenInGame(true);
}

// END
void UPlayerPreviewComponent::End()
{
	if (previewActor)
	{
		previewActor->End();
		previewActor->Destroy();
		previewActor = nullptr;
	}
}

// 开关预览
void UPlayerPreviewComponent::OpenClosePerview(bool open, Gamedata::EntityInfo* syncInfo, bool faceOneself_)
{
	if (previewActor == nullptr)return;
	auto playerController = character->myData->playerController;
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;

	if (open)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [预览] [OpenClosePerview] 进入预览  "), this);

		//AEditEntityActor* entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(syncInfo->entityid()));
		//if (entityActor == nullptr) { return; }

		faceOneself = faceOneself_;
		isTick = true;

		Gamedata::EntityInfo syncEntityInfo;
		syncEntityInfo.CopyFrom(*syncInfo);
		syncEntityInfo.set_entityid(PreviewEntityID);
		auto location = character->GetActorLocation() + playerController->GetControlRotation().Vector() * 1000;
		auto location2 = syncEntityInfo.mutable_transform()->mutable_location();
		location2->set_x(location.X);
		location2->set_y(location.Y);
		location2->set_z(location.Z);
		character->myData->gameMode->controller->NetCallback_EntityEnterView(&syncEntityInfo);

		{
			//lineLen = 8000;
			//
			//UStaticMesh* mesh = Cast<UStaticMesh>(it->handle->GetLoadedAsset());
			//if (mesh) {
			//	//it->transform.SetLocation(mesh->ExtendedBounds.Origin);
			//	//it->transform.SetScale3D(mesh->ExtendedBounds.BoxExtent);

			//}
		}

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [预览] [OpenClosePerview] 退出预览 "), this);

		isTick = false;

		AEditEntityActor* entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(PreviewEntityID));
		if (entityActor == nullptr) { return; }

		editEntityManage->controller->NetCall_NewData(entityActor->syncInfo);

		character->myData->gameMode->controller->NetCallback_EntityLeaveView(PreviewEntityID);
		UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_ui_买);
		UHUDStaticFunc::PushMessage(this, FString::Printf(TEXT("放置(%s)"), *entityActor->GetMyName()));
	}
}

// tick Called every frame
void UPlayerPreviewComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MouseMove();
}

// 鼠标xy输入
void UPlayerPreviewComponent::MouseMove()
{
	if (isTick)
	{

		auto playerController = Cast<APlayerController>(character->GetController());
		FVector2D mousePostion2;
		playerController->GetMousePosition(mousePostion2.X,
			mousePostion2.Y);
		auto location = character->GetActorLocation();
		auto rotator = character->GetActorRotation();
		if (mousePostion2.Equals(mousePostion) == false
			|| location.Equals(characterLocation) == false
			|| rotator.Equals(characterRotator) == false)
		{
			mousePostion = mousePostion2;
			characterLocation = location;
			characterRotator = rotator;
			Frame();
		}

	}
}

// wasd输入
void UPlayerPreviewComponent::PawnMove()
{
	Frame();
}

// 帧
void UPlayerPreviewComponent::Frame()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [预览] [Frame] 帧  %f"), this, GetWorld()->TimeSeconds);

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto playerController = character->myData->playerController;

	AEditEntityActor* entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(PreviewEntityID));
	if (entityActor == nullptr) { return; }

	auto lineLen = 8000;

	FVector startLocation;
	FRotator mouseRotator;
	FVector endLocation;
	FVector mouseLocation;
	{
		FVector mouseRotator_;
		playerController->DeprojectMousePositionToWorld(mouseLocation, mouseRotator_);
		mouseRotator = mouseRotator_.Rotation();
		startLocation = mouseLocation;
		FRotator r1 = mouseRotator;
		FVector m1 = r1.Vector() * lineLen;
		endLocation = mouseLocation + m1;
	}
	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation, endLocation, FLinearColor::Red, 10, 1);

	FCollisionQueryParams traceParams(FName(TEXT("test")), false, GetOwner());

	TArray<struct FHitResult> hitResultList;
	TArray<AActor*> hitActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	objectTypes.Add(UEngineTypes::ConvertToObjectType(BuildObject));

	bool bBlocked = GetOwner()->GetWorld()->LineTraceMultiByObjectType(hitResultList, startLocation, endLocation,
		objectTypes, traceParams);

	bool isHit = false;
	if (bBlocked)
	{
		for (auto node : hitResultList)
		{
			if (node.GetActor() != nullptr && node.GetActor() != entityActor)
			{
				if (Cast<UBoxComponent>(node.GetComponent()) == false)
				{
					endLocation = node.ImpactPoint;
					isHit = true;
					break;
				}
			}
		}
	}

	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation, endLocation, FLinearColor::Red, 10, 1);

	// 没有击中任何物体,放置在镜头前10m处
	if (isHit == false)
	{
		endLocation = mouseLocation + mouseRotator.Vector() * 1000;
	}

	// 资源文件夹
	if (faceOneself)
	{
		auto scale = entityActor->syncInfo.mutable_transform()->scale();;
		FVector scale2 = FVector(scale.x(), scale.y(), scale.z());
		FRotator lookat = (character->GetActorLocation() - endLocation).Rotation();
		FRotator tempR(0, playerController->GetControlRotation().Yaw + 90, 0);
		entityActor->editComponent->EntityLocalMoveMain(ESelectedWorkMode::WorldTransform, endLocation,
			tempR, scale2);
	}
	// 收纳盒
	else
	{
		entityActor->editComponent->EntityLocalMoveMain(ESelectedWorkMode::WorldLocation, endLocation,
			FRotator::ZeroRotator, FVector::OneVector);
	}

}