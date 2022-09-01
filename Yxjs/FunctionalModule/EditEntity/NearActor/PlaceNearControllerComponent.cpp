// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceNearControllerComponent.h"
#include "PlaceNearDataComponent.h"
#include "PlaceNearActorView.h"
//#include "Yxjs/Pawn/Racing/Player/PlayerRacingPawn.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshComponent.h"

#include "GameMode/BaseGameMode.h"
#include "Core/BaseGameInstance.h"
#include "UI/BaseHud.h"
#include "GameFramework/PlayerController.h"

#include "Json/Public/Serialization/JsonSerializer.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/Controller.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "Tools/CommonlyObject.h"
#include "EngineMinimal.h"
#include "Engine/Engine.h"
#include <AIModule/Classes/AIController.h>
#include "GameFramework/CharacterMovementComponent.h"
#include <../Plugins/Runtime/PhysXVehicles/Source/PhysXVehicles/Public/WheeledVehicleMovementComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Engine/EngineTypes.h>
#include "PlaceNearEntity.h"
#include "../Actor/EditEntityActor.h"

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values for this component's properties
UPlaceNearControllerComponent::UPlaceNearControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceNearControllerComponent]  "),this);
}

// Called when the game starts
void UPlaceNearControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

//
void UPlaceNearControllerComponent::Start(UPlaceNearDataComponent* data_, APlaceNearActorView* view_)
{
	view = view_;
	data = data_;

	data->CheckDefaultData();
}

//
void UPlaceNearControllerComponent::End()
{
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Called every frame
void UPlaceNearControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//auto nowTime = GetWorld()->TimeSeconds;
	//if (nowTime < (lastTimeTick + 0.5))
	//{
	//	return;
	//}
	//lastTimeTick = nowTime;

	//auto playerRacingPawn = Cast<APlayerRacingPawn>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	//if (playerRacingPawn == nullptr)return;

	//std::list<int>tempActorNameList;
	//{
	//	FVector start = playerRacingPawn->GetActorLocation();
	//	FVector end = start + FVector::UpVector * 5;
	//	auto rotator = playerRacingPawn->GetActorRotation();
	//	auto boxExten = FVector(1000, 1000, 1000);

	//	TArray<FHitResult> hitResultList;
	//	TArray<AActor*>hitActors;
	//	TArray<TEnumAsByte<EObjectTypeQuery> >  ObjectTypes;
	//	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(BuildObject));
	//	bool bBlocked = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), start, end, boxExten, rotator, ObjectTypes, false, hitActors,
	//		EDrawDebugTrace::Type::None, hitResultList, true, FLinearColor::Red, FLinearColor::Green, 1);

	//	for (int i = 0; i < hitResultList.Num(); i++)
	//	{
	//		if (hitResultList[i].GetActor()) {
	//			auto box = Cast<UBoxComponent>(hitResultList[i].GetComponent());
	//			if (box) {
	//				int entityId = 0;
	//				int32 selectIndex = 0;
	//				if (view->editEntityManage->data->GetEntityIdWithBox(box, entityId, selectIndex)) {
	//					tempActorNameList.push_back(entityId);
	//				}
	//			}
	//		}
	//	}
	//}

	//int count1 = 0, count2 = 0;
	//std::list<int> addList;
	//std::list<int> removeList;
	//DataComparison(actorNameList, tempActorNameList, addList, removeList);

	//for (auto it = removeList.begin(); it != removeList.end(); ++it)
	//{
	//	auto node = *it;
	//	//UE_LOG(LogTemp, Log, TEXT("[%x] [周围实例] [TickComponent]  销毁 [node:%d] "), this, node);
	//	DestroyActor(node);
	//	count1++;
	//}

	//for (auto it = addList.begin(); it != addList.end(); ++it)
	//{
	//	auto node = *it;
	//	//UE_LOG(LogTemp, Log, TEXT("[%x] [周围实例] [TickComponent]  生成 [node:%d] "), this, node);
	//	SpawnActor(node);
	//	count2++;
	//}

	//if (count1 != 0 || count2 != 0) {
	//	actorNameList.clear();
	//	for (auto it = tempActorNameList.begin(); it != tempActorNameList.end(); ++it)actorNameList.push_back(*it);
	//}
}

// 数据对比
void UPlaceNearControllerComponent::DataComparison(std::list<int>& beforeList, std::list<int>& nowList, std::list<int>& addList, std::list<int>& removeList)
{
	// 移除
	{
		std::list<int> list2;
		for (auto it = beforeList.begin(); it != beforeList.end(); ++it)removeList.push_back(*it);
		for (auto it = nowList.begin(); it != nowList.end(); ++it)list2.push_back(*it);

		for (auto it = list2.begin(); it != list2.end(); ++it)
		{
			removeList.remove(*it);
		}
	}

	// 加入
	{
		std::list<int> list1;
		for (auto it = beforeList.begin(); it != beforeList.end(); ++it)list1.push_back(*it);
		for (auto it = nowList.begin(); it != nowList.end(); ++it)addList.push_back(*it);
		for (auto it = list1.begin(); it != list1.end(); ++it)
		{
			auto node = *it;
			addList.remove(node);
		}
	}
}

// 生存替换actor
void UPlaceNearControllerComponent::SpawnActor(int entityId)
{
	if (actorMap.Find(entityId))
	{
		return;
	}

	//auto localInfo = view->editEntityManage->data->GetLocalInfo(entityId);
	//if (localInfo == nullptr)return;

	auto entityActor = view->editEntityManage->data->GetEntityActor(entityId);
	if (entityActor == nullptr)return;

	auto meshComponent = view->editEntityManage->data->GetEntityMeshComponent(entityActor->childComponentList[0]->meshIstanceId);
	if (meshComponent == nullptr)return;
	auto staticMesh = meshComponent->GetStaticMesh();
	if (staticMesh == nullptr)return;

	FTransform t = entityActor->childComponentList[0]->meshInfo.wTransform;
	t.SetScale3D(t.GetScale3D() * 0.5);
	APlaceNearEntity* actor = Cast<APlaceNearEntity>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), APlaceNearEntity::StaticClass(), t, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (actor)
	{
		UGameplayStatics::FinishSpawningActor(actor, t);
	}
	actor->Start(view.Get(), staticMesh);

	actorMap.Add(entityId, actor);
}

// 销毁替换actor
void UPlaceNearControllerComponent::DestroyActor(int entityId)
{
	if (actorMap.Find(entityId) == false)
	{
		return;
	}
	auto& node = actorMap[entityId];
	node->End();
	node->Destroy();
	actorMap.Remove(entityId);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/
