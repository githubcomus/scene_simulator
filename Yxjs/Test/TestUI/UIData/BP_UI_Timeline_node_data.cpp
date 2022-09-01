// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_UI_Timeline_node_data.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Tools/CommonlyObject.h"
#include "Test/TestUI/UIView/TestScrollBox2.h"
#include "BP_UI_Timeline_node_block_data.h"
#include "BP_UI_Timeline_node_point_data.h"
#include "BP_UI_Timeline_node_line_data.h"

#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ScrollBox.h"
#include  "UObject\ScriptDelegates.h"

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// data
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

// 获取 视野时间
float UBP_UI_Timeline_node_data::GetViewTime()
{
	if (testScrollBox2.IsValid() == false)return 0;

	// 滚动条位置确认当前时间

	float lenght = abs(ctli.minTime) + abs(ctli.maxTime);
	float offset = testScrollBox2->GetScrollOffset() / testScrollBox2->GetScrollOffsetOfEnd();
	float var = lenght * offset;
	return ctli.minTime + var;
}

// 获取 当前视野时间所在区块
int UBP_UI_Timeline_node_data::GetBlockIndexWithViewTime(float viewTime)
{
	// 最左和最后的区块索引(区分正负)

	int farLeftBlockIndex = ctli.minTime / ctli.blockSeconds;
	int farRightBlockIndex = ctli.maxTime / ctli.blockSeconds;
	auto tempIndex = viewTime / ctli.blockSeconds;

	return tempIndex - farLeftBlockIndex;
}

// 获取 视野区块数据
FBlockInfo* UBP_UI_Timeline_node_data::GetViewBlockInfo()
{
	int blockIndexWithViewTime = GetBlockIndexWithViewTime(ctli.oldViewTime);

	if (blockIndexWithViewTime < 0 || blockList.Num() <= blockIndexWithViewTime)
	{
		return nullptr;
	}
	return &blockList[blockIndexWithViewTime];
}

// 获取 视野左区块数据
FBlockInfo* UBP_UI_Timeline_node_data::GetLeftViewBlockInfo()
{
	int blockIndexWithViewTime = GetBlockIndexWithViewTime(ctli.oldViewTime);
	blockIndexWithViewTime--;
	if (blockIndexWithViewTime < 0 || blockList.Num() <= blockIndexWithViewTime)
	{
		return nullptr;
	}
	return &blockList[blockIndexWithViewTime];
}

// 获取 视野右区块数据
FBlockInfo* UBP_UI_Timeline_node_data::GetRightViewBlockInfo()
{
	int blockIndexWithViewTime = GetBlockIndexWithViewTime(ctli.oldViewTime);
	blockIndexWithViewTime++;
	if (blockIndexWithViewTime < 0 || blockList.Num() <= blockIndexWithViewTime)
	{
		return nullptr;
	}
	return &blockList[blockIndexWithViewTime];
}

// 获取 同步数据
Gamedata::EntityInfo* UBP_UI_Timeline_node_data::GetSyncInfo(int entityId)
{
	if (syncEntityInfoMap.Find(entityId))
	{
		Gamedata::EntityInfo* syncInfo = &syncEntityInfoMap[entityId];
		return syncInfo;
	}
	return nullptr;
}

// 新增 区块数据
void UBP_UI_Timeline_node_data::GetBlockData()
{
	//// 实例最大最小范围
	//ctli.minTime = ctli.maxTime = 0;
	//// 实例的最大最小时间
	//{
	//	for (auto& entityId : entitySortList)
	//	{
	//		Gamedata::EntityInfo* syncInfo = GetSyncInfo(entityId);
	//		if (syncInfo == nullptr)
	//		{
	//			continue;
	//		}
	//		//if (syncInfo->movetimeline_size() > selectTimelineIndex)
	//		{
	//			auto& movetimeline = syncInfo->movetimeline();
	//			auto& nodemap = movetimeline.pointmap();
	//			std::vector<int> timelimeKeyList;
	//			for (auto node : nodemap)
	//			{
	//				auto key = node.first;
	//				timelimeKeyList.push_back(key);
	//			}
	//			if (timelimeKeyList.size() > 1)
	//			{
	//				std::sort(timelimeKeyList.begin(), timelimeKeyList.end());
	//				float tempMin = timelimeKeyList[0] / 1000.0f;
	//				float tempMax = timelimeKeyList[timelimeKeyList.size() - 1] / 1000.0f;
	//				if (tempMin < ctli.minTime)
	//				{
	//					ctli.minTime = tempMin;
	//				}
	//				if (tempMax > ctli.maxTime)
	//				{
	//					ctli.maxTime = tempMax;
	//				}
	//			}
	//		}
	//	}
	//}

	//UE_LOG(LogTemp, Log,
	//	TEXT(
	//		"[%x] [UBP_UI_Timeline_node_data::GetBlockData] 1 [ctli.minTime:%f] [ctli.maxTime:%f]  "
	//	), this, ctli.minTime, ctli.maxTime);

	//// 视野时间
	//{
	//	if (ctli.oldViewTime < ctli.minTime)
	//	{
	//		ctli.minTime = ctli.oldViewTime;
	//	}
	//	if (ctli.oldViewTime > ctli.maxTime)
	//	{
	//		ctli.maxTime = ctli.oldViewTime;
	//	}
	//}

	//// 关键帧时间
	//{
	//	if (ctli.frameKeyTime < ctli.minTime)
	//	{
	//		ctli.minTime = ctli.frameKeyTime;
	//	}
	//	if (ctli.frameKeyTime > ctli.maxTime)
	//	{
	//		ctli.maxTime = ctli.frameKeyTime;
	//	}
	//}

	//// 左右多出一个区块作为缓冲
	//{
	//	ctli.minTime = ctli.minTime - ctli.blockSeconds;
	//	ctli.maxTime = ctli.maxTime + ctli.blockSeconds;
	//}


	//// 综合的最大最小区间长度
	//int lenght = abs(ctli.minTime) + abs(ctli.maxTime);

	//// 区块重置
	//{
	//	ctli.blockCount = lenght / ctli.blockSeconds;
	//	if ((lenght % ctli.blockSeconds) > 0)
	//	{
	//		ctli.blockCount++;
	//	}


	//	blockList.Reset();
	//	for (int i = 0; i < ctli.blockCount; i++)
	//	{
	//		FBlockInfo node;
	//		node.blockIndex = i;
	//		blockList.Add(node);
	//	}
	//}

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::GetBlockData] 3 [len:%d] [ctli.blockCount:%d]  "), this,
	//	lenght, ctli.blockCount);

	//// 区块内部开始结束点设置
	//{
	//	// 最左和最后的区块索引(区分正负)
	//	int farLeftBlockIndex = ctli.minTime / ctli.blockSeconds;
	//	int farRightBlockIndex = ctli.maxTime / ctli.blockSeconds;

	//	UE_LOG(LogTemp, Log,
	//		TEXT(
	//			"[%x] [UBP_UI_Timeline_node_data::GetBlockData] 2 时间节点 [ctli.minTime:%f] [ctli.maxTime:%f] 区块索引 [farLeftBlockIndex:%d] [farRightBlockIndex:%d] "
	//		), this, ctli.minTime, ctli.maxTime, farLeftBlockIndex, farRightBlockIndex);

	//	// 最左侧区块的开始时间
	//	float farLeftBlockStartTime = farLeftBlockIndex * ctli.blockSeconds;
	//	for (int i = 0; i < ctli.blockCount; i++)
	//	{
	//		auto blockInfo = &blockList[i];
	//		blockInfo->start = farLeftBlockStartTime + ctli.blockSeconds * i;
	//		blockInfo->end = blockInfo->start + ctli.blockSeconds - ctli.timeItemLenght;
	//		blockInfo->itBelongsToExtension = false;

	//		UE_LOG(LogTemp, Log,
	//			TEXT(
	//				"[%x] [UBP_UI_Timeline_node_data::GetBlockData]  [index:%d] [start:%d] [end:%d]   "
	//			), this, i, blockInfo->start, blockInfo->end);
	//	}
	//}
}

// 新增 点数据
void UBP_UI_Timeline_node_data::GetPointData()
{
	// 所有实例
	for (auto& entityId : entitySortList)
	{
		// 所有区块
		for (auto& blockInfo : blockList)
		{
			TArray<FPointInfo> pointList;
			GetPointData_InBlock(entityId, blockInfo.start, blockInfo.end, pointList);
			blockInfo.entityInfoMap.Add(entityId, pointList);
		}
	}
}

// 新增 点数据
void UBP_UI_Timeline_node_data::GetPointData_InBlock(int entityId, float start, float end,
	TArray<FPointInfo>& pointList)
{
	//Gamedata::EntityInfo* syncInfo = GetSyncInfo(entityId);
	//if (syncInfo == nullptr)return;

	////if (syncInfo->movetimeline_size() > selectTimelineIndex)
	//{
	//	const Gamedata::EntityMoveTimeline& movetimeline = syncInfo->movetimeline();
	//	if (movetimeline.pointmap_size() > 0)
	//	{
	//		auto& nodemap = movetimeline.pointmap();
	//		std::vector<int> timelimeKeyList;
	//		for (auto node : nodemap)
	//		{
	//			auto key = node.first;
	//			timelimeKeyList.push_back(key);

	//			// UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::GetPointData_InBlock]  [time:%d]"), this, keytime);
	//		}

	//		if (timelimeKeyList.size() > 1)
	//		{
	//			std::sort(timelimeKeyList.begin(), timelimeKeyList.end());
	//		}
	//		float tempMin = timelimeKeyList[0] / 1000.0f;
	//		float tempMax = timelimeKeyList[timelimeKeyList.size() - 1] / 1000.0f;

	//		for (int i = 0; i < timelimeKeyList.size(); i++)
	//		{
	//			int keyTime = timelimeKeyList[i];
	//			float time = keyTime / 1000.0f;

	//			// UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::GetPointData_InBlock] [entityId:%d] [start:%f -- end:%f] [time:%f]"), this, entityId, start, end, time);


	//			if (start <= time && time <= end)
	//			{
	//				const Gamedata::FVector* entityMoveFrameKey = &nodemap.find(keyTime)->second.location();
	//				FPointInfo pointInfo;
	//				pointInfo.entityId = entityId;
	//				pointInfo.time = time;
	//				pointInfo.keyTime = keyTime;
	//				pointInfo.move = UCommonlyObject::PBToUe4Vector(entityMoveFrameKey);
	//				pointInfo.timelineIndex = i;
	//				// x 位移
	//				pointInfo.x = ((float)(time - start) / (end - start + ctli.timeItemLenght)) * ctli.blockXLenght;

	//				pointList.Add(pointInfo);

	//				UE_LOG(LogTemp, Log,
	//					TEXT(
	//						"[%x] [UBP_UI_Timeline_node_data::GetPointData_InBlock] [entityId:%d] [start:%f -- end:%f] [time:%f] [timelineIndex:%d] [x:%f]"
	//					), this, entityId, start, end, time, pointInfo.timelineIndex, pointInfo.x);
	//			}
	//		}
	//	}
	//}
}

// 新增 左侧增加一个区块数据
void UBP_UI_Timeline_node_data::GetLeftBlockData()
{
	// 左右多出一个区块作为缓冲
	ctli.minTime = ctli.minTime - ctli.blockSeconds;

	ctli.blockCount++;

	// 区块重置
	{
		blockList.Reset();
		for (int i = 0; i < ctli.blockCount; i++)
		{
			FBlockInfo node;
			node.blockIndex = i;
			blockList.Add(node);
		}
	}

	// 区块内部开始结束点设置
	{
		// 最左和最后的区块索引(区分正负)
		int farLeftBlockIndex = ctli.minTime / ctli.blockSeconds;
		int farRightBlockIndex = ctli.maxTime / ctli.blockSeconds;


		// 最左侧区块的开始时间
		float farLeftBlockStartTime = farLeftBlockIndex * ctli.blockSeconds;
		for (int i = 0; i < ctli.blockCount; i++)
		{
			auto blockInfo = &blockList[i];
			blockInfo->start = farLeftBlockStartTime + ctli.blockSeconds * i;
			blockInfo->end = blockInfo->start + ctli.blockSeconds - ctli.timeItemLenght;
			if (i == 0)
			{
				blockInfo->itBelongsToExtension = true;
			}
			else
			{
				blockInfo->itBelongsToExtension = false;
			}
		}
	}
}

// 新增 左侧增加一个区块数据
void UBP_UI_Timeline_node_data::GetRightBlockData()
{
	// 左右多出一个区块作为缓冲
	{
		// ctli.minTime = ctli.minTime - ctli.blockSeconds;
		ctli.maxTime = ctli.maxTime + ctli.blockSeconds;
	}

	ctli.blockCount++;

	FBlockInfo node;
	node.blockIndex = ctli.blockCount - 1;
	blockList.Add(node);


	// 区块内部开始结束点设置

	// 最左和最后的区块索引(区分正负)
	int farLeftBlockIndex = ctli.minTime / ctli.blockSeconds;
	int farRightBlockIndex = ctli.maxTime / ctli.blockSeconds;

	UE_LOG(LogTemp, Log,
		TEXT(
			"[%x] [UBP_UI_Timeline_node_data::GetRightBlockData] 2 时间节点 [ctli.minTime:%f] [ctli.maxTime:%f] 区块索引 [farLeftBlockIndex:%d] [farRightBlockIndex:%d] "
		), this, ctli.minTime, ctli.maxTime, farLeftBlockIndex, farRightBlockIndex);

	// 最左侧区块的开始时间
	float farLeftBlockStartTime = farLeftBlockIndex * ctli.blockSeconds;

	auto blockInfo = &blockList[node.blockIndex];
	blockInfo->start = farLeftBlockStartTime + ctli.blockSeconds * blockInfo->blockIndex;
	blockInfo->end = blockInfo->start + ctli.blockSeconds - ctli.timeItemLenght;
	blockInfo->itBelongsToExtension = true;


	UE_LOG(LogTemp, Log,
		TEXT(
			"[%x] [UBP_UI_Timeline_node_data::GetRightBlockData]  [index:%d] [start:%d] [end:%d]   "
		), this, blockInfo->blockIndex, blockInfo->start, blockInfo->end);
}

// 移除 左区块数据
void UBP_UI_Timeline_node_data::DeleteLeftBlockData()
{

}

// 移除 右区块数据
void UBP_UI_Timeline_node_data::DeleteRightBlockData()
{
	// std::list<FBlockInfo*> delList;

	for (int i = 0; i < blockList.Num(); i++)
	{
		auto node = &blockList[i];
		if (node->itBelongsToExtension)
		{
			blockList.RemoveAt(i);

			ctli.blockCount--;
			ctli.maxTime -= ctli.blockSeconds;


			UE_LOG(LogTemp, Log,
				TEXT(
					"[%x] [UBP_UI_Timeline_node_data::DeleteRightBlockData] 移除右区块数据 [blockCount:%d] [minTime:%d] [maxTime:%d]   "
				), this, ctli.blockCount, ctli.minTime, ctli.maxTime);
		}
	}

}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// controller
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UBP_UI_Timeline_node_data::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::NativeOnInitialized] "), this);
}

////
//void UBP_UI_Timeline_node_data::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//	UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::NativePreConstruct] "), this);
//}

//
void UBP_UI_Timeline_node_data::NativeConstruct()
{
	Super::NativeConstruct();
	testScrollBox2 = Cast<UTestScrollBox2>(GetWidgetFromName(TEXT("TestScrollBox2_114")));
	UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::NativeConstruct] [testScrollBox2:%x]"), this,
		testScrollBox2.Get());


	// 绑定鼠标拖拽
	{
		FScriptDelegate scriptDelegate;
		scriptDelegate.BindUFunction(this, FName("Callback_UserScrolled"));
		testScrollBox2->OnUserScrolled.AddUnique(scriptDelegate);
	}
	// 绑定鼠标滚动
	{
		FScriptDelegate scriptDelegate;
		scriptDelegate.BindUFunction(this, FName("Callback_MouseWheel"));
		testScrollBox2->onMouseWheelEvent.AddUnique(scriptDelegate);
	}

	Init();
}

//
void UBP_UI_Timeline_node_data::NativeDestruct()
{
	Super::NativeDestruct();
	UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::NativeDestruct] "), this);
}

/*------------------------------------------------------------------*/

// 测试 多点数据写入
void UBP_UI_Timeline_node_data::InitPointNodeData()
{
}

// 测试 写入单个实例的一条时间轴
void UBP_UI_Timeline_node_data::MultiplePointWrite(Gamedata::EntityInfo* syncInfo,
	FLocalEntityInfo* localInfo)
{
	// if(syncInfo->movetimeline_size()==0)
	// {
	// 	return;
	// }
	//
	// // 当前选定显示的时间轴
	// int scenestimelineindex = -1;
	// for(auto i=0;i< syncInfo->movetimeline_size();i++)
	// {
	// 	auto& movetimeline = syncInfo->movetimeline(i);
	// 	if(movetimeline.scenestimelineindex()== selectTimelineIndex)
	// 	{
	// 		scenestimelineindex = selectTimelineIndex;
	// 		break;
	// 	}
	// }
	// if(scenestimelineindex!=-1)
	// {
	// 	return;
	// }
	//
	// if(scenesTimelineInfoList.Num()< scenestimelineindex)
	// {
	// 	return;
	// }
	//
	// auto& movetimeline = syncInfo->movetimeline(scenestimelineindex);
	// auto& nodemap = movetimeline.nodemap();
	//
	// auto scenesTimeline = &scenesTimelineInfoList[scenestimelineindex];
	//
	// if(scenesTimeline->entityInfoMap.Find(syncInfo->entityid())==nullptr)
	// {
	// 	FMoveTimelineInfo moveTimelineInfo;;
	// 	scenesTimeline->entityInfoMap.Add(syncInfo->entityid(), moveTimelineInfo);
	// }
	//
	// auto moveTimelineInfo = &scenesTimeline->entityInfoMap[movetimeline.entitylistindex()];
	// moveTimelineInfo->pointInfoList.Reset();
	//
	//
	// // 实例关键帧排序
	// std::vector<int> timelimeKeyList;
	// for (auto node : movetimeline.nodemap())
	// {
	// 	auto key = node.first;
	// 	timelimeKeyList.push_back(key);
	// }
	// std::sort(timelimeKeyList.begin(), timelimeKeyList.end());
	//
	// // 读取数据到ui的数据结构里
	// for(int i=0;i< timelimeKeyList.size();i++)
	// {
	// 	int key = timelimeKeyList[i];
	// 	auto& node = nodemap.find(key)->second;
	// 	;
	// 	FPointInfo pointInfo;
	// 	pointInfo.move = FVector(node.move().x(), node.move().y(), node.move().z());
	// 	moveTimelineInfo->pointInfoList.Add(pointInfo);
	// }
}

/*------------------------------------------------------------------*/

// 测试 数据
void UBP_UI_Timeline_node_data::TestData()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::TestData] a [time:%f]"), this, GetWorld()->TimeSeconds);

	//for (int h = 0; h < 20; h++)
	//{
	//	int offsetValue = h * 5;
	//	int entityId = h;
	//	Gamedata::EntityInfo syncInfo;

	//	auto movetimeline = syncInfo.mutable_movetimeline();
	//	FString timelineName = "the0";
	//	std::string str(TCHAR_TO_UTF8(*timelineName));
	//	movetimeline->set_name(str);
	//	movetimeline->set_entitymovetimelineendofplay(Gamedata::EntityMoveTimelineEndOfPlay_ShutDown);
	//	movetimeline->set_isreflection(false);
	//	movetimeline->set_size(1);
	//	movetimeline->set_lenght(40000);
	//	movetimeline->set_playlenght(10);
	//	movetimeline->set_ismove(true);
	//	movetimeline->set_moveyaw(true);
	//	movetimeline->set_scenestimelineindex(0);
	//	movetimeline->set_entitylistindex(0);


	//	for (int i = 0; i < 130; i++)
	//	{
	//		int timeKey2 = (offsetValue + i) * 1000;
	//		// Gamedata::EntityMoveFrameKey entityTimeInfo;
	//		// entityTimeInfo.set_ishide(false);
	//		// entityTimeInfo.set_colortype(Gamedata::EntityMoveTimelineColorType::EntityMoveTimelineColorType_Blue);
	//		Gamedata::FVector location;
	//		location.set_x(10 * i);
	//		location.set_y(10 * i);
	//		location.set_z(10 * i);

	//		// auto nodemap = movetimeline->mutable_locationmap();
	//		// nodemap->insert({timeKey2, location });
	//	}

	//	syncEntityInfoMap.Add(entityId, syncInfo);
	//	entitySortList.Add(entityId);
	//}


	//UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::TestData] b [time:%f]"), this, GetWorld()->TimeSeconds);
	//GetBlockData();

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::TestData] c [time:%f]"), this, GetWorld()->TimeSeconds);
	//GetPointData();

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::TestData] d [time:%f]"), this, GetWorld()->TimeSeconds);
	//InitBlockView();

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::TestData] e [time:%f]"), this, GetWorld()->TimeSeconds);
	//WriteBlockToScroll();
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::TestData] f [time:%f]"), this, GetWorld()->TimeSeconds);
}

/*------------------------------------------------------------------*/

// 初始化
void UBP_UI_Timeline_node_data::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	{
		FString bpPath = TEXT(
			"WidgetBlueprint'/Game/Test/UI/TestTimeline/BP_UI_Timeline_node_block.BP_UI_Timeline_node_block_C'");
		class_BP_UI_Timeline_node_block_C = LoadClass<UBP_UI_Timeline_node_block_data>(this, *bpPath);
		if (class_BP_UI_Timeline_node_block_C == nullptr)
		{
			return;
		}
	}
	{
		FString bpPath = TEXT(
			"WidgetBlueprint'/Game/Test/UI/TestTimeline/BP_UI_Timeline_node_line.BP_UI_Timeline_node_line_C'");
		class_BP_UI_Timeline_node_Line_C = LoadClass<UBP_UI_Timeline_node_line_data>(this, *bpPath);
		if (class_BP_UI_Timeline_node_Line_C == nullptr)
		{
			return;
		}
	}
	{
		FString bpPath = TEXT(
			"WidgetBlueprint'/Game/Test/UI/TestTimeline/BP_UI_Timeline_node_point.BP_UI_Timeline_node_point_C'");
		class_BP_UI_Timeline_node_point_C = LoadClass<UBP_UI_Timeline_node_point_data>(this, *bpPath);
		if (class_BP_UI_Timeline_node_point_C == nullptr)
		{
			return;
		}
	}


	// 空数据构建区块节点
	// {
	// 	FScenesTimelineInfo node;
	// 	node.index = 0;
	// 	scenesTimelineInfoList.Add(node);
	// }
	// TestData();
}

// 移除显示区块内的行
void UBP_UI_Timeline_node_data::CleanUpDisplayBlock(FBlockInfo* blockInfo)
{
	if (blockInfo == nullptr || blockInfo->block == nullptr)return;

	auto count = blockInfo->block->scrollBox->GetChildrenCount();
	for (int i = 0; i < count; i++)
	{
		blockInfo->block->scrollBox->RemoveChildAt(0);
	}
}

// 初始化区块视图
void UBP_UI_Timeline_node_data::InitBlockView()
{
	if (testScrollBox2.IsValid() == false)return;

	ctli.isViewUpdate = true;


	int childCount = testScrollBox2->GetChildrenCount();

	// 增加区块
	if (ctli.blockCount > childCount)
	{
		auto count = ctli.blockCount - childCount;
		for (int i = 0; i < count; i++)
		{
			auto bP_UI_Timeline_node_block = Cast<UBP_UI_Timeline_node_block_data>(
				CreateWidget(GetWorld(), class_BP_UI_Timeline_node_block_C));
			testScrollBox2->AddChild(bP_UI_Timeline_node_block);
		}
	}
	// 移除区块
	else if (ctli.blockCount < childCount)
	{
		auto count = childCount - ctli.blockCount;
		for (int i = 0; i < count; i++)
		{
			testScrollBox2->RemoveChildAt(0);
		}
	}

	// 初始化区块UI界面
	for (int i = 0; i < ctli.blockCount; i++)
	{
		auto blockInfo = &blockList[i];
		blockInfo->block = Cast<UBP_UI_Timeline_node_block_data>(testScrollBox2->GetChildAt(blockInfo->blockIndex));
		blockInfo->block->SetTimeString(blockInfo->start, blockInfo->end);
	}

	ctli.isViewUpdate = false;
}

// 将区块写入滚动条 完全写入 
void UBP_UI_Timeline_node_data::WriteBlockToScroll()
{
	if (testScrollBox2.IsValid() == false)return;

	ctli.isViewUpdate = true;

	// 当前区块指针
	auto leftBlockInfo = GetLeftViewBlockInfo();
	auto viewBlockInfo = GetViewBlockInfo();
	auto rightBlockInfo = GetRightViewBlockInfo();

	// 清空原来的区块数据
	CleanUpDisplayBlock(leftBlockInfo);
	CleanUpDisplayBlock(viewBlockInfo);
	CleanUpDisplayBlock(rightBlockInfo);

	// 以区块为单位
	WriteLineToBlock(leftBlockInfo);
	WriteLineToBlock(viewBlockInfo);
	WriteLineToBlock(rightBlockInfo);

	ctli.isViewUpdate = false;
}

// 将区块写入滚动条 部分写入 
void UBP_UI_Timeline_node_data::WriteBlockToScroll_UserScrolled(int oldBlockIndex, int newBlockIndex)
{
	if (testScrollBox2.IsValid() == false)return;

	ctli.isViewUpdate = true;

	// 老左中右
	TArray<int> oldList;
	if (oldBlockIndex > 0)
		oldList.Add(oldBlockIndex - 1);
	oldList.Add(oldBlockIndex);
	if (oldBlockIndex + 1 < blockList.Num())
		oldList.Add(oldBlockIndex + 1);

	// 新左中右
	TArray<int> newList;
	if (newBlockIndex > 0)
		newList.Add(newBlockIndex - 1);
	newList.Add(newBlockIndex);
	if (newBlockIndex + 1 < blockList.Num())
		newList.Add(newBlockIndex + 1);

	FString str1 = TEXT("");
	if (oldList.Num() == 3 && newList.Num() == 3)
		str1 = FString::Printf(
			TEXT(" [old:%d %d %d] --> [new:%d %d %d] 移除 "), oldList[0], oldList[1], oldList[2], newList[0], newList[1],
			newList[2]);

	// 移除
	{
		// 交集
		TArray<int> list1;
		for (int i = 0; i < oldList.Num(); i++)
		{
			if (newList.Find(oldList[i]) != INDEX_NONE)
			{
				list1.Add(oldList[i]);
			}
		}

		// 不在交集之中
		for (int i = 0; i < oldList.Num(); i++)
		{
			if (list1.Find(oldList[i]) == INDEX_NONE)
			{
				auto blockInfo = &blockList[oldList[i]];
				CleanUpDisplayBlock(blockInfo);
				str1 = str1 + FString::Printf(TEXT("[:%d]"), blockInfo->blockIndex);
			}
		}
	}

	str1 = str1 + TEXT("新增 ");

	// 新增
	{
		for (int i = 0; i < newList.Num(); i++)
		{
			if (oldList.Find(newList[i]) == INDEX_NONE)
			{
				auto blockInfo = &blockList[newList[i]];
				WriteLineToBlock(blockInfo);
				str1 = str1 + FString::Printf(TEXT("[:%d]"), blockInfo->blockIndex);
			}
		}
	}

	// 保持不变
	{
	}

	UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::WriteBlockToScroll_UserScrolled] %s "), this, *str1);

	ctli.isViewUpdate = false;
}

// 以区块为单位 将行写入区块
void UBP_UI_Timeline_node_data::WriteLineToBlock(FBlockInfo* blockInfo)
{
	if (blockInfo == nullptr || blockInfo->block == nullptr)return;

	// 所有实例数据写入区块
	for (int lineNumber = 0; lineNumber < entitySortList.Num(); lineNumber++)
	{
		auto entityId = entitySortList[lineNumber];
		auto bP_UI_Timeline_node_line_data = Cast<UBP_UI_Timeline_node_line_data>(
			CreateWidget(GetWorld(), class_BP_UI_Timeline_node_Line_C));
		blockInfo->block->scrollBox->AddChild(bP_UI_Timeline_node_line_data);

		WritePointToLine(blockInfo, entityId, bP_UI_Timeline_node_line_data, lineNumber);
	}
}

// 将点写入行
void UBP_UI_Timeline_node_data::WritePointToLine(FBlockInfo* blockInfo, int entityId,
	UBP_UI_Timeline_node_line_data* bP_UI_Timeline_node_line_data,
	int lineNumber)
{
	if (blockInfo->entityInfoMap.Find(entityId))
	{
		auto& pointInfoList = blockInfo->entityInfoMap[entityId];
		for (int i = 0; i < pointInfoList.Num(); i++)
		{
			auto& pointInfo = pointInfoList[i];
			UBP_UI_Timeline_node_point_data* bP_UI_Timeline_node_point_data = Cast<UBP_UI_Timeline_node_point_data>(
				CreateWidget(GetWorld(), class_BP_UI_Timeline_node_point_C));
			if (bP_UI_Timeline_node_point_data)
			{
				UOverlay* overlay2 = bP_UI_Timeline_node_line_data->overlay.Get();
				bP_UI_Timeline_node_point_data->SetRenderTranslation(FVector2D(pointInfo.x, 15));
				overlay2->AddChild(bP_UI_Timeline_node_point_data);
				// UE_LOG(LogTemp, Log,
				//        TEXT(
				// 	       "[%x] [UBP_UI_Timeline_node_data::WritePointToLine] [blockIndex:%d] [lineNumber:%d] [entityId:%d] [timelineIndex:%d] [x:%f]"
				//        ), this, blockInfo->blockIndex, lineNumber, entityId, pointInfo.timelineIndex, pointInfo.x);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::WritePointToLine] err "), this);
			}
		}
	}
}

// 向左扩展空区块
void UBP_UI_Timeline_node_data::LeftExtensionNull()
{
	GetPointData();
	InitBlockView();
}

// 向右扩展空区块
void UBP_UI_Timeline_node_data::RightExtensionNull(int oldBlockIndex, int newBlockIndex)
{
	// 加入区块视图
	auto bP_UI_Timeline_node_block = Cast<UBP_UI_Timeline_node_block_data>(
		CreateWidget(GetWorld(), class_BP_UI_Timeline_node_block_C));
	testScrollBox2->AddChild(bP_UI_Timeline_node_block);

	// 初始化区块UI
	auto blockInfo = &blockList[ctli.blockCount - 1];
	blockInfo->block = Cast<UBP_UI_Timeline_node_block_data>(testScrollBox2->GetChildAt(blockInfo->blockIndex));
	blockInfo->block->SetTimeString(blockInfo->start, blockInfo->end);

	// 写入区块数据
	WriteBlockToScroll_UserScrolled(oldBlockIndex, newBlockIndex);
}

// 清理左侧扩展区块

// 清理右侧扩展区块
void UBP_UI_Timeline_node_data::DeleteRightExtensionBlock()
{
	for (int i = 0; i < testScrollBox2->GetChildrenCount(); i++)
	{
		auto thisChild = Cast<UBP_UI_Timeline_node_block_data>(testScrollBox2->GetChildAt(i));
		bool findFlag = false;
		for (int h = 0; h < ctli.blockCount; h++)
		{
			auto blockInfo = &blockList[h];
			if (blockInfo->block == thisChild)
			{
				findFlag = true;
				break;
			}
		}
		if (findFlag == false)
		{
			testScrollBox2->RemoveChildAt(i);

			UE_LOG(LogTemp, Log,
				TEXT(
					"[%x] [UBP_UI_Timeline_node_data::DeleteRightExtensionBlock] 移除右区块视图 [blockCount:%d] [i:%d]   "
				), this, testScrollBox2->GetChildrenCount(), i);
		}
	}

}

// 当前页面写一个一个点的数据(当前玩家,其他玩家)

// 其他玩家传送过来的数据扩充


// 设置纵向滚动条
void UBP_UI_Timeline_node_data::SetLineOffsetView()
{
	int oldBlockIndex = GetBlockIndexWithViewTime(ctli.oldViewTime);
	if (blockList.Num() == 0 || oldBlockIndex >= blockList.Num())
	{
		return;
	}

	if (blockList[oldBlockIndex].block == nullptr || blockList[oldBlockIndex].block->scrollBox->GetChildrenCount() == 0)
	{
		return;
	}

	// 所有区块滚动到指定位置
	// UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::Callback_MouseWheel] [value:%f]"), this, value);

	// for (int i = 0; i < blockList.Num(); i++)
	// {
	// }

	blockList[oldBlockIndex].block->scrollBox->SetScrollOffset(ctli.srollNumber);
	if (oldBlockIndex - 1 > 0)
	{
		blockList[oldBlockIndex - 1].block->scrollBox->SetScrollOffset(ctli.srollNumber);
	}
	if (oldBlockIndex + 1 < blockList.Num())
	{
		blockList[oldBlockIndex + 1].block->scrollBox->SetScrollOffset(ctli.srollNumber);
	}
}

/*------------------------------------------------------------------*/

// 事件回调 横向滚动条 滚动条变化回调
void UBP_UI_Timeline_node_data::Callback_UserScrolled(float value)
{
	if (ctli.isViewUpdate)
	{
		return;
	}


	auto viewTime = GetViewTime();

	int oldBlockIndex = GetBlockIndexWithViewTime(ctli.oldViewTime);

	int newBlockIndex = GetBlockIndexWithViewTime(viewTime);

	if (oldBlockIndex != newBlockIndex)
	{
		// 动态加载其他区块数据
		UE_LOG(LogTemp, Log,
			TEXT("[%x] [UBP_UI_Timeline_node_data::Callback_UserScrolled] [value:%f] [viewTime:%f] [newBlockIndex:%d]"), this,
			value, viewTime, newBlockIndex);
		ctli.oldViewTime = viewTime;

		int farLeftBlockIndex = ctli.minTime / ctli.blockSeconds;
		int farRightBlockIndex = ctli.maxTime / ctli.blockSeconds;

		// 当前视野最大最小未超出界限
		if (0 < newBlockIndex && newBlockIndex < (ctli.blockCount - 1))
		{
			WriteBlockToScroll_UserScrolled(oldBlockIndex, newBlockIndex);

			// 清理掉右侧扩展部分
			if (oldBlockIndex > newBlockIndex)
			{
				// DeleteRightBlockData();
				// DeleteRightExtensionBlock();
			}
			// 清理掉左侧扩展部分
			else
			{

			}
		}
		// 最左,左侧扩展+刷新
		else if (newBlockIndex == 0)
		{
			GetLeftBlockData();
			// LeftExtensionNull();
		}
		// 最右,右边扩展
		else if (newBlockIndex == (ctli.blockCount - 1))
		{
			GetRightBlockData();
			RightExtensionNull(oldBlockIndex, newBlockIndex);
		}

		SetLineOffsetView();
	}
}


// 事件回调 横向滚动条 鼠标滚轮回调
void UBP_UI_Timeline_node_data::Callback_MouseWheel(float value)
{
	if (ctli.isViewUpdate)
	{
		return;
	}

	int oldBlockIndex = GetBlockIndexWithViewTime(ctli.oldViewTime);
	if (blockList.Num() == 0 && oldBlockIndex < blockList.Num())
	{
		return;
	}

	if (blockList[oldBlockIndex].block->scrollBox->GetChildrenCount() == 0)
	{
		return;
	}

	float lenght = blockList[oldBlockIndex].block->scrollBox->GetScrollOffsetOfEnd();

	ctli.srollNumber += value * 30;
	if (ctli.srollNumber < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::Callback_MouseWheel] 最顶 [time:%f]"), this,
			GetWorld()->TimeSeconds);
		ctli.srollNumber = 0;
	}
	else if (ctli.srollNumber > lenght)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::Callback_MouseWheel] 最底 [time:%f]"), this,
			GetWorld()->TimeSeconds);
		ctli.srollNumber = lenght;
	}

	SetLineOffsetView();
}


/*------------------------------------------------------------------*/
