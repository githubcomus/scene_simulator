// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "PlayerSelectedComponent.generated.h"

class UEditEntityManageController;
class ASelectedViewActor;
class ABaseCharacter;
class UTimelinePanelUserWidget;
class UEditEntityMainUserWidget;

class ACharacter;
class UArrowComponent;
class UBoxComponent;

/*------------------------------------------------------------------*/

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlayerSelectedComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// 操作类型
	enum Operation_Type
	{
		Operation_Type_Update = 1, // 更新
		Operation_Type_Copy = 2, // 复制
		Operation_Type_Crop = 3, // 裁剪
		Operation_Type_paste = 4,// 粘贴
		Operation_Type_Delete = 5, // 删除
		Operation_Type_None = 6, //none

		// type=1 寻找首次移动落点
		Operation_Type_GetLineResponset_find_first,
		// type=2 寻找移动策略
		Operation_Type_GetLineResponset_find_move_type,
		// type=3 寻找选中实例(第一次找box,第二次找mesh)
		Operation_Type_GetLineResponset_find_entity,
		// type=4 寻找移动位置
		Operation_Type_GetLineResponset_finde_move_location
	};

	/*------------------------------------------------------------------*/

	// 箭头平移数据
	struct CvSelectMoveInfo
	{
		// (select箭头)映射到屏幕移动方向的变换
		FTransform mouseArrowTransform;

		// 屏幕出发到达(select箭头)射线所在平面的变换
		FTransform lineFaceTransform;

		// 选中策略坐标轴变换,当时的变化(恒定在远处的点)不能实时获取
		FTransform componentTransform;
	};

	// 旋转数据
	struct SelectSpinInfo
	{
	};

	// 缩放数据
	struct SelectZoomInfo
	{
	};

	// 移动策略数据
	struct CvMoveStrategyInfo
	{
		// 当前移动策略
		ESelectedMoveStrategy selectedMoveStrategy;

		// 左键按下
		bool isLeftPressed;

		// 按下时屏幕坐标轴位置
		FVector shootStartLocation;;

		// 按下时射中坐标轴位置
		FVector shootEndLocation;

		// 按下时射中坐标轴旋转
		FRotator shootRotator;

		// 按下时视图的缩放比
		float enterScale;
	};

	// 复制数据
	struct CvCopyInfo
	{
		// 是否复制
		bool isCopy;

		// 是否裁剪
		bool isCrop;

		// 选中实例列表(最后一个为主实例)
		TArray<int32> selectedList;
	};

	/*------------------------------------------------------------------*/

	// 镜头前的盒体
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* camearBox;

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<ABaseCharacter> character = nullptr;

	// 移动策略数据
	CvMoveStrategyInfo cvMoveStrategyInfo;

	// 多选数据
	CvMultipleInstanceInfo cvMultipleInstanceInfo;

	// 箭头平移数据
	CvSelectMoveInfo cvSelectMoveInfo;

	// 旋转数据
	SelectSpinInfo selectSpinInfo;

	// 缩放数据
	SelectZoomInfo selectZoomInfo;

	// 复制数据
	CvCopyInfo cvCopyInfo;

	// 鼠标射线面箭头
	TMap<FString, UArrowComponent*> arrowMap;

	// 上一次鼠标在屏幕的位置
	FVector2D lastTimeMousePosion;

	// 上一次tick时间
	float lastTimeTick = 0;

	/*------------------------------------------------------------------*/

	UPlayerSelectedComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Start();

	void End();

	TWeakObjectPtr<ASelectedViewActor> GetSelectedView();

	UEditEntityManageController* GetPlaceController();

	bool GetMultipleChoiceIndex(int32 entityId, int32& arrayIndex);
	int GetSelectMasterInstance();

	bool IsSelectEntity();

	bool GetHitBoxLocation(FVector start, FVector end, FVector& result);

	void GetLineResponset(Operation_Type type, TArray<struct FHitResult>& hitResultList, FVector& startLocation, FRotator& mouseRotator);
	bool GetHitBoxWithMesh(int32 entityId, int32 selectIndex, FVector hitBoxLocation, FVector& hitLocation);

	bool GetPlaceInstance(int32& entityId, int32& selectIndex);

	bool GetSelectedInstance(ESelectedMoveStrategy& selectedMoveStrategy_);

	bool GetPanAxisTransform();

	FTransform GetTransformWithLine(bool isX, FVector location, FVector line1, FVector line2);

	bool GetPanAxisRealData(FVector& move);

	void SetMultiplePanAxis();
	bool GetPlaneMoveRealData(FVector& move);
	FRotator GetSpinRealData();

	void SetMultipleSpinAxis();

	float GetZoomRealData();

	void SetSingeZoom();
	void Input_Base_IsMultipleChoice(bool flag);
	void Input_Base_LeftPressed();
	void Input_Base_LeftReleased();
	void Input_SetWorkMode(ESelectedWorkMode selectedWorkMode_);
	void Input_Base_MiddleMouseButton();
	void Input_Timeline_IsMultipleChoice(bool isMultipleChoice_);
	void Input_Timeline_MouseLeftButtonPressed();
	void Input_Timeline_MouseLeftButtonReleased();
	bool IsSelectAxis();
	void GetSelectInstanceList(std::vector<int>& selectList);
	bool TimelineIsSelectKeyFrame(int& timeKey, int& entityId, int& uniqueMark);
	TWeakObjectPtr<UTimelinePanelUserWidget> GetTimelineUI();
	TWeakObjectPtr < UEditEntityMainUserWidget> GetEditEntityMainUI();
	void Input_Timeline_MouseLeftButtonEnterMove();
	void Input_Timeline_NextFunction(int var);

	void SetScaleAxis();

	void NetCall_CopyInstance();

	void NetCall_CropInstance();

	void NetCall_PasteInstance();
	void NetCall_DeletedInstance();
	void Input_Base_HighlightArrow();
	void Input_Base_LeftMove();

	void NetCall_EntityUpdate(bool mast);

	void  SelectAxis();
	void AddSelected(int32 entityId);
	void RemoveSelected(int32 entityId);

	void SelectInstance(int32 entityId);
	void SetSelectHidden(bool isShow = false);
	void JustLoseMaster();
	void Call_SelectEntity(int entityId);
	void NetCall_Lock();
	void SetSelectMesh(bool isShow, int32 entityId);
	void UICall_SelectToSetUI();
	void UICall_ChoseMaterial();
	void OperationResultsSwitch(Operation_Type type, bool mast = true);

	void RemoveAllSelected();

	UArrowComponent* CreateTestArrow(FString name, FTransform t);

	void DestroyTestArrow(UArrowComponent* arrow);

	void UpdateTestArrow(FString name, FTransform t);

	/*------------------------------------------------------------------*/
};