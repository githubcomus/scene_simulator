// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Import/CharacterInfo.h"
#include "Pawn/Character/BaseCharacterInfo.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "PlaceNearControllerComponent.generated.h"

class UPlaceNearDataComponent;
class APlaceNearActorView;
class APlaceNearEntity;

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class ABaseHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlaceNearControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:


	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 数据组件
	TWeakObjectPtr<UPlaceNearDataComponent> data;

	// 视图
	TWeakObjectPtr < APlaceNearActorView> view;

	//
	float lastTimeTick = 0;

	// 替换列表
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<int, APlaceNearEntity*> actorMap;

	// 周围实例
	std::list<int>actorNameList;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// Sets default values for this component's properties
	UPlaceNearControllerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DataComparison(std::list<int>& beforeList, std::list<int>& nowList, std::list<int>& addList, std::list<int>& removeList);
	void SpawnActor(int entity);
	void DestroyActor(int entity);
	//
	void Start(UPlaceNearDataComponent* data_, APlaceNearActorView* view_);

	//
	void End();

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/
};
