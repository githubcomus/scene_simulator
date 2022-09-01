// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <list>
#include <map>
#include <vector>
#include "Import/GameModeInfo.h"
#include "Net/NetInfo.h"
#include "Protobuf/game.pb.h"
#include "OverallSituationView.generated.h"

class UBoxComponent;
class UMaterial;
class UBoxComponent;
class USceneComponent;
class USceneStaticMeshComponent;
class APawnLineActor;
class ABaseCharacter;
class AEditEntityManage;

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class ABaseHUD;

//using namespace std;

UCLASS()
class  AOverallSituationView : public AActor
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* sceneRoot;

	/*------------------------------------------------------------------*/

	// 注册进来的天气actor
	TWeakObjectPtr<AActor> skyActor;

	// 注册进来的天气actor
	TWeakObjectPtr<AActor> weatherActor;

	// 注册进来的天气actor
	TWeakObjectPtr<AActor> skyControllerActor;

	// 常用指针
	//TWeakObjectPtr<ACharacter> character;

	// 常用指针
	TWeakObjectPtr<UBaseGameInstance> gameInstance;

	// 常用指针
	TWeakObjectPtr<APlayerController> playerController;

	// 常用指针
	TWeakObjectPtr<ABaseGameMode> gameMode;

	// 常用指针
	TWeakObjectPtr<ABaseHUD> hud;

	// 同步数据
	Gamedata::OverallSituationDataResponse syncInfo;

	/*------------------------------------------------------------------*/

	UPROPERTY()
		TMap<int, APawnLineActor*> lineActorMap;

	/*------------------------------------------------------------------*/

	bool CheckTargetData();
	bool CheckDefaultData();
	TWeakObjectPtr< AEditEntityManage> GetEditEntityManage();
	// Sets default values for this actor's properties
	AOverallSituationView();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//
	void Start();

	//
	void End();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NetCall_ChangeSkySet(int time, int weather, int cloud_coverageoverall, int cloud_speed, int moon_color);
	void NetCall_Place_Merge_GetAllActor();
	void NetCall_Place_Merge_AddActor(std::vector<Gamedata::EntityInfo*>nodeEntityIdList);
	void NetCall_Place_Merge_UpdateActor(int entityId);
	void NetCall_Place_Merge_RemoveActor(int entityId);
	void NetCallback_ChangeSkySet(Gamedata::OverallSituationSkyInfo* info);
	void Sky_Set(Gamedata::OverallSituationSkyInfo* info);
	void NetCallback_get_all_actor(::google::protobuf::Map< ::google::protobuf::int32, ::Gamedata::EntityMeshMasterInfo >* info);
	void NetCallback_add_actor(Gamedata::EntityMeshMasterInfo* info);
	void NetCallback_actor_update(Gamedata::EntityMeshMasterInfo* info);
	void NetCallback_remove_actor(int entityId);
	//void UI_MergeActor_AddLine(Gamedata::EntityMeshMasterInfo* info);
	void NetCallback_BP_OverallSituationData_Change(Gamedata::OverallSituationDataResponse& info);
	virtual void PostInitializeComponents() override;
};
