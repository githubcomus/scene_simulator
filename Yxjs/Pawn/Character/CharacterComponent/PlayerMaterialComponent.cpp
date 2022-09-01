// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMaterialComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Core/BaseGameInstance.h"
#include "Tools/CalculationObject.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/Selected/SelectedViewActor.h"
#include "Net/NetMainComponent.h"
#include "Pawn/Character/BaseCharacter.h"

#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
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

#include <math.h>

///*------------------------------------------------------------------*/
//
//// 获取 动态获取view
//AEditEntityManage* UPlayerMaterialComponent::GetView()
//{
//    UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetOwner()->GetGameInstance());
//    //return gameInstance->editEntityManage;
//    ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//    return baseGameMode->editEntityManage;
//}
//

// 获取 获取表格数据
xlsdata::MaterialTableDefine* UPlayerMaterialComponent::GetXlsInfoWithXlsId(int32 xlsId)
{
	// 注意:传递指针会导致string内容被清楚,只能传递引用,访问源数据

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance_->zwXlsDataActorComponent->materialMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->materialMap.end())
	{
		return &gameInstance_->zwXlsDataActorComponent->materialMap[xlsId];
	}
	return nullptr;
}

/*------------------------------------------------------------------*/

// Sets default values for this component's properties
UPlayerMaterialComponent::UPlayerMaterialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPlayerMaterialComponent::BeginPlay()
{
	Super::BeginPlay();

	// this->AddToRoot();

	character = Cast<ABaseCharacter>(GetOwner());

	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerMaterialComponent::BeginPlay]   "), this);
}

// EndPlay
void UPlayerMaterialComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// this->RemoveFromRoot();

	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerMaterialComponent::EndPlay]   "), this);
}

// Called every frame
void UPlayerMaterialComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/*------------------------------------------------------------------*/

// START
void UPlayerMaterialComponent::Start()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerMaterialComponent::Start] "), this);
	character = Cast<ABaseCharacter>(GetOwner());
}

// END
void UPlayerMaterialComponent::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerMaterialComponent::End]   "), this);
}

/*------------------------------------------------------------------*/

// 指定材质
void UPlayerMaterialComponent::CallInput_SetMaterial(int xlsId)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(character->GetWorld()->GetAuthGameMode());
	auto editEntityManage = baseGameMode->editEntityManage;
	auto selectComponent = character->selectComponent;
	auto placeController = editEntityManage->controller;

	// 所有选中实例全部更新
	TArray<int32> entityIdList;
	for (auto node : selectComponent->cvMultipleInstanceInfo.selectedList)
	{
		auto syncInfo = editEntityManage->data->GetSyncInfo(node.entityId);
		//auto localInfo = editEntityManage->data->GetLocalInfo(node.entityId);
		if (syncInfo)
		{
			auto child = syncInfo->mutable_child_list(0);
			child->mutable_mesh()->set_material_id(xlsId);
			entityIdList.Add(syncInfo->entityid());
		}
	}
	if (entityIdList.Num() > 0)
	{
		placeController->NetCall_UpdateData(entityIdList);
	}
}

//
//// 测试 生成测试箭头
//UArrowComponent* UPlayerMaterialComponent::CreateTestArrow(FString name,FTransform t)
//{
//    auto arrow = NewObject<UArrowComponent>(character, *name);
//
//    t.SetScale3D(FVector(1,4,0.2f));
//
//    arrow->RegisterComponent();
//    arrow->AttachToComponent(character->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
//    arrow->SetCollisionObjectType(ECC_WorldStatic);
//    arrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//    arrow->SetWorldTransform(t);
//    arrow->SetHiddenInGame(false);
//    return arrow;
//}
//
//// 测试 销毁测试箭头
//void UPlayerMaterialComponent::DestroyTestArrow(UArrowComponent* arrow)
//{
//    if (arrow)
//    {
//        if (arrow->IsRegistered())
//        {
//            arrow->UnregisterComponent();
//        }
//        arrow->DestroyComponent();
//    }
//}
//
//// 测试 更新箭头
//void UPlayerMaterialComponent::UpdateTestArrow(FString name, FTransform t)
//{
//    if (arrowMap.Find(name))
//    {
//        DestroyTestArrow(arrowMap[name]);
//        arrowMap.Remove(name);
//    }
//    auto arrow = CreateTestArrow(name, t);
//    arrowMap.Add(name, arrow);
//}
//
///*------------------------------------------------------------------*/
//
//// 生成一个实例实例
//void UPlayerMaterialComponent::SpawnBuildInstance()
//{
//
//    //ACharacter* character = Cast<ACharacter>(GetOwner());
//    //if (character == nullptr)return;
//    ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(character->GetWorld()->GetAuthGameMode());
//    if (baseGameMode == nullptr)return;
//    auto data = baseGameMode->editEntityManage->data;
//    auto gameInstance = baseGameMode->gameInstance;
//
//    auto location = GetHitLocation();
//    FTransform t;
//    t.SetLocation(location);
//    t.SetRotation(FRotator::ZeroRotator.Quaternion());
//    t.SetScale3D(FVector(1, 1, 1));
//    int32 xlsId = 2022000;
//
//    Gamedata::EntityInfo syncInfo;
//    syncInfo.entityId = 0;
//    syncInfo.playerId = baseGameMode->GetLocalPlayerId();
//    syncInfo.operate = NetEntitySync::EntityAdd;
//    syncInfo.type = NetEntityResourcesType::EntityMesh;
//    syncInfo.xlsId = xlsId;
//    syncInfo.materialId = 0;
//    syncInfo.masterId = 0;
//    syncInfo.spawnWTransform = t;
//
//    GetView()->controller->NetCall_NewData(syncInfo);
//}
//
//// 获取射线检测的位置
//FVector UPlayerMaterialComponent::GetHitLocation()
//{
//    TArray< FHitResult> outHits;
//    FVector start;
//    FVector end;
//
//    //auto character = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
//    auto playerController = GetWorld()->GetFirstPlayerController();
//    UActorComponent* camera = character->GetComponentByClass(UCameraComponent::StaticClass());
//    UCameraComponent* cameraComponent = Cast<UCameraComponent>(camera);
//    start = cameraComponent->GetComponentLocation();
//    FRotator cameraR = cameraComponent->GetComponentRotation();
//    end = start + cameraR.Vector() * 800;
//
//    //UKismetSystemLibrary::DrawDebugLine(GetWorld(), start, end, FColor(0, 255, 255), 5, 1.0f);
//
//    FCollisionObjectQueryParams objectQueryParams;
//    FCollisionQueryParams params;
//    objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
//    FVector hitLocation;
//    GetWorld()->LineTraceMultiByObjectType(outHits, start, end, objectQueryParams, params);
//    bool checkFlag = false;
//    for (auto node : outHits)
//    {
//        auto component = node.GetComponent();
//        auto component2 = Cast<UActorComponent>(component);
//        UBoxComponent* buildBox = Cast<UBoxComponent>(component);
//        ECollisionChannel collisionChannel = component->GetCollisionObjectType();
//
//        if (collisionChannel == ECollisionChannel::ECC_WorldStatic)
//        {
//            hitLocation = node.ImpactPoint;
//            checkFlag = true;
//            break;
//        }
//    }
//    if (checkFlag == false)
//    {
//        hitLocation = end;
//    }
//
//    return hitLocation;
//}
//
//// 获取玩家摄像头旋转
//FRotator UPlayerMaterialComponent::GetPlayerCameraRotator()
//{
//    //auto character = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
//    UActorComponent* camera = character->GetComponentByClass(UCameraComponent::StaticClass());
//    UCameraComponent* cameraComponent = Cast<UCameraComponent>(camera);
//    FRotator cameraR = cameraComponent->GetComponentRotation();
//    return cameraR;
//}
//
///*------------------------------------------------------------------*/
//
//// 主从设置
//void UPlayerMaterialComponent::SetMasterNode()
//{
//    // 多选主从设置
//}
//
///*------------------------------------------------------------------*/