// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Protobuf/game.pb.h"
#include "PlayerTimelineComponent.generated.h"

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class AMyPythonActor;
class UPlayerSelectedComponent;
class ABaseHUD;
class ABaseCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YXJS_API UPlayerTimelineComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 常用指针
	TWeakObjectPtr<UBaseGameInstance> gameInstance;

	// 常用指针
	TWeakObjectPtr<APlayerController> playerController;

	// 常用指针
	TWeakObjectPtr<ABaseGameMode> gameMode;

	// 常用指针
	TWeakObjectPtr<ABaseHUD> hud;

	//
	TWeakObjectPtr<UPlayerSelectedComponent> selectComponent = nullptr;

	//
	TWeakObjectPtr<ABaseCharacter> view = nullptr;

	/*------------------------------------------------------------------*/

	//// 时间轴控制数据
	//struct CvTimelineInfo {
	//	bool isTimeline = false;	// 从时间轴切换到character
	//	int entityId = 0;			// 时间轴id
	//};
	//CvTimelineInfo timelineInfo;

	/*------------------------------------------------------------------*/

	bool CheckTargetData();
	bool CheckDefaultData();
	UFUNCTION(BlueprintCallable)
		bool CanOpenUI();
	UPlayerTimelineComponent();
	virtual void BeginPlay() override;

	void Call_SetUIData(Gamedata::EntityInfo* entityInfo);
	//void SaveChangeDate();
	bool SwitchEditEntityStaus();
	//void SetChildUI_TimelineData(bool write);
	//void NetCallback_UseTimelineData();
	void Start();
	void End();
	void Call_EnterEditTimelineStaus(bool isEnter);
};
