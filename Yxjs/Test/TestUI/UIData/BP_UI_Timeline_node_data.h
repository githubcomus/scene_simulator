// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "BP_UI_Timeline_node_line_data.h"
#include "BP_UI_Timeline_node_data.generated.h"


class UTestScrollBox2;
class UBP_UI_Timeline_node_block_data;

// [实例时间轴]节点数据
USTRUCT(BlueprintType)
struct FPointInfo
{
	GENERATED_BODY()

public:

	// 位移x
	float x = 0;

	// 实例id
	int entityId = 0;

	// 所在索引
	int timelineIndex = 0;

	// 时间
	float time = 0;

	// keytime
	int keyTime = 0;

	// map value
	FVector move;
};

// 区块数据
USTRUCT(BlueprintType)
struct FBlockInfo
{
	GENERATED_BODY()

public:

	// 区块对应UI指针
	TWeakObjectPtr<UBP_UI_Timeline_node_block_data> block = nullptr;

	// 区块索引
	int blockIndex = 0;

	// 时间
	int start = 0;

	//
	int end = 0;

	// map key是entityid,value是需要显示的数据
	TMap<int32, TArray<FPointInfo>> entityInfoMap;

	// 是否属于扩展
	bool itBelongsToExtension = false;
};

/**
 * 时间轴滚动条
 */
UCLASS(BlueprintType)
class YXJS_API UBP_UI_Timeline_node_data : public UUserWidget
{
	GENERATED_BODY()
public:

	/*------------------------------------------------------------------*/

	// 时间轴数据
	struct CvTimelineInfo
	{

		//########################//

		/*固定数值*/

		// 区块有多少秒钟
		int blockSeconds = 10;

		// 区块有多少x轴像素
		int blockXLenght = 2000;

		// 单独一个格子需要补全的秒钟(0-9)表示10秒的区间,所以长度计算的时候要补1s
		int timeItemLenght = 1;

		//########################//

		/*同步数据-变量*/

		// 需要分配多少个区块
		int blockCount = 0;

		// 最小时间
		float minTime = 0;

		// 最大时间
		float maxTime = 0;

		//########################//

		/*玩家操作-变量*/

		// 关键帧时间
		float frameKeyTime = 0;

		// 视野时间(取当前视野中央的时间节点)
		float oldViewTime = 5;

		// 视图更新
		bool isViewUpdate = false;

		// 滚动条最终百分比
		float srollNumber = 0;
	};

	/*------------------------------------------------------------------*/

	// 联网数据
	TMap<int32, Gamedata::EntityInfo> syncEntityInfoMap;

	// 场景时间轴排序
	TArray<int> entitySortList;

	// 横向滚动条
	TWeakObjectPtr<UTestScrollBox2> testScrollBox2 = nullptr;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_Timeline_node_block_C;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_Timeline_node_Line_C;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_Timeline_node_point_C;


	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	// 选中显示的时间轴
	int selectTimelineIndex = 0;

	// 时间轴数据
	CvTimelineInfo ctli;

	// 区块数据
	TArray< FBlockInfo> blockList;


	/*------------------------------------------------------------------*/


	virtual void NativeOnInitialized();
	//virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeDestruct();

	UFUNCTION(BlueprintCallable)
		void TestData();
	void Init();
	void InitPointNodeData();
	void MultiplePointWrite(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	FBlockInfo* GetViewBlockInfo();
	FBlockInfo* GetLeftViewBlockInfo();
	FBlockInfo* GetRightViewBlockInfo();
	float GetViewTime();
	int GetBlockIndexWithViewTime(float viewTime);
	void GetBlockData();
	void GetPointData();
	Gamedata::EntityInfo* GetSyncInfo(int entityId);
	void WriteBlockToScroll();
	void CleanUpDisplayBlock(FBlockInfo* blockInfo);
	void InitBlockView();
	// void WriteLineToBlock(int entityId, int lineNumber);
	void WritePointToLine(FBlockInfo* blockInfo, int entityId,
		UBP_UI_Timeline_node_line_data* bP_UI_Timeline_node_line_data, int lineNumber);
	void LeftExtensionNull();
	void RightExtensionNull(int oldBlockIndex, int newBlockIndex);
	void DeleteRightExtensionBlock();
	void SetLineOffsetView(float srollNumber_);
	void WriteLineToBlock(FBlockInfo* blockInfo);
	void WriteBlockToScroll_UserScrolled(int oldBlockIndex, int newBlockIndex);
	UFUNCTION()
		void Callback_UserScrolled(float value);
	void SetLineOffsetView();
	UFUNCTION()
		void Callback_MouseWheel(float value);
	void WritePointToLine();
	void GetPointData_InBlock(int entityId, float start, float end, TArray<FPointInfo>& pointList);
	void GetLeftBlockData();
	void GetRightBlockData();
	void DeleteLeftBlockData();
	void DeleteRightBlockData();
};
