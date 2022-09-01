// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerMoveTimelineObject.h"
#include "UI/BaseHud.h"
#include "PlayerPlaceComponent.generated.h"

class UPlaceControllerMoveTimelineObject;

class ABaseCharacter;
class AEditEntityManage;
class ACharacter;
class UArrowComponent;
class UBoxComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlayerPlaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 左键按下记录信息(旋转模式)
	struct CvMouseLeftInfo
	{
		// 当前时间轴总时间
		float totalTime = 0;

		// 鼠标左键按下的时间
		float enterTime = 0;

		// 上次保存记录的时间
		float lastTimeSetEditLightRotator = 0;

		// 上次保存记录的数值
		FVector lastTimeMove = FVector::ZeroVector;
	};

	// 左键按下记录信息(位移模式)
	struct CvMoveInfo
	{
		// 上次记录时间
		float lastTime = 0;
	};

	// 激光UI信息
	struct CvLaserUIInfo
	{
		// 播放
		bool isPlay = false;

		// 播放时间轴
		int timelineIndex = 0;

		// UI点击开始播放的时间(时间轴)
		float startPlayUITime = 0;

		// UI点击开始播放的时间(系统时间)
		float startPlaySystemTime = 0;

		// 当前帧播放到的时间(时间轴 四舍五入取key值)
		float nowPlayTime = 0;
	};

	/*------------------------------------------------------------------*/

	//
	//
	TWeakObjectPtr<ABaseCharacter> view = nullptr;

	// 旋转信息
	CvMouseLeftInfo cmfi;

	// 位移信息
	CvMoveInfo cmi;

	// 激光ui数据
	CvLaserUIInfo clui;

	/*------------------------------------------------------------------*/

	//Sets default values for this component's properties
	UPlayerPlaceComponent();

	//Called when the game starts
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	// START
	void Start();

	// END
	void End();

	//
	TWeakObjectPtr<AEditEntityManage> GetEditEntityManage();

	//
	UArrowComponent* CreateTestArrow(FString name, FTransform t);

	//
	void DestroyTestArrow(UArrowComponent* arrow);

	//
	void UpdateTestArrow(FString name, FTransform t);

	void SpawnMeshInstance(int32 xlsId, bool isMerge, FTransform transform);
	FVector GetHitLocation();
	FRotator GetPlayerCameraRotator();
	bool GetHitPoint(bool hitPlace, FVector& result);
	void SpawnImageInstance(FString url);
	void SetMasterNode();
	TWeakObjectPtr<UPlaceControllerMoveTimelineObject> GetMoveTimelineController();
	ABaseHUD* GetHUD();
	int GetSelect();
	void Input_EnterEdit();
	bool CanEnterEditMoveTimeline();
	void Input_LeaveEdit();
	void Input_WriteMove(bool isMast = false);
	void Input_WriteStartEnd(bool isEnter);
	void Input_EnterKeyWriteMove(float writeTime = -1);
	void Input_WriteKey(bool isOpen);
	void Input_OpenTimeline(bool flag);
	//void Input_SpwanEntity();
	UFUNCTION(BlueprintCallable)
		void UICallback_TimelinePlay(bool flag);
	void TimelinePlay(bool flag);
	void UI_UpdataLaserUI(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, bool isMast);
	UFUNCTION(BlueprintCallable)
		void UICallback_Play(bool isStart);
	void TickTimeline();

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/
};
