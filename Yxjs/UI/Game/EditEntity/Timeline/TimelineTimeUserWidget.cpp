// Fill out your copyright notice in the Description page of Project Settings.


#include "TimelineTimeUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "TimelineInfo.h"

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// view
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UTimelineTimeUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UTimelineTimeUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UTimelineTimeUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UTimelineTimeUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

//
FReply UTimelineTimeUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelineTimeUserWidget::NativeOnMouseButtonDown] 按下 A [time:%f]"), this,
	//        GetWorld()->TimeSeconds);

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UTimelineTimeUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	// UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelineTimeUserWidget::NativeOnMouseButtonUp] 松开 B [time:%f]"), this,
	//        GetWorld()->TimeSeconds);

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UTimelineTimeUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelineTimeUserWidget::NativeOnMouseEnter] 进入 A [time:%f]"), this,
	//        GetWorld()->TimeSeconds);

	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UTimelineTimeUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	// UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelineTimeUserWidget::NativeOnMouseEnter] 离开 B [time:%f]"), this,
	//        GetWorld()->TimeSeconds);


	Super::NativeOnMouseLeave(InMouseEvent);
}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// data
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

// // 获取 视野时间
// float UTimelineTimeUserWidget::GetViewTime()
// {
// 	// if (testScrollBox2.IsValid() == false)return 0;
// 	//
// 	// // 滚动条位置确认当前时间
// 	//
// 	// float lenght = abs(ctli.minTime) + abs(ctli.maxTime);
// 	// float offset = testScrollBox2->GetScrollOffset() / testScrollBox2->GetScrollOffsetOfEnd();
// 	// float var = lenght * offset;
// 	// return ctli.minTime + var;
// 	//
// 	return 0;
// }
//
// // 获取 同步数据
// Gamedata::EntityInfo* UTimelineTimeUserWidget::GetSyncInfo(int entityId)
// {
// 	if (syncEntityInfoMap.Find(entityId))
// 	{
// 		Gamedata::EntityInfo* syncInfo = &syncEntityInfoMap[entityId];
// 		return syncInfo;
// 	}
// 	return nullptr;
// }
//
// // 新增 区块数据
// void UTimelineTimeUserWidget::GetBlockData()
// {
// 	// 实例的最大最小时间
// 	{
// 		// 实例最大最小范围
// 		ctli.minTime = ctli.maxTime = 0;
// 		for (auto& entityId : entitySortList)
// 		{
// 			Gamedata::EntityInfo* syncInfo = GetSyncInfo(entityId);
// 			if (syncInfo == nullptr)
// 			{
// 				continue;
// 			}
// 			if (syncInfo->movetimeline_size() > selectTimelineIndex)
// 			{
// 				auto& movetimeline = syncInfo->movetimeline(selectTimelineIndex);
// 				auto& nodemap = movetimeline.locationmap();
// 				std::vector<int> timelimeKeyList;
// 				for (auto node : nodemap)
// 				{
// 					auto key = node.first;
// 					timelimeKeyList.push_back(key);
// 				}
// 				if (timelimeKeyList.size() > 1)
// 				{
// 					std::sort(timelimeKeyList.begin(), timelimeKeyList.end());
// 					float tempMin = timelimeKeyList[0] / 1000.0f;
// 					float tempMax = timelimeKeyList[timelimeKeyList.size() - 1] / 1000.0f;
// 					if (tempMin < ctli.minTime)
// 					{
// 						ctli.minTime = tempMin;
// 					}
// 					if (tempMax > ctli.maxTime)
// 					{
// 						ctli.maxTime = tempMax;
// 					}
// 				}
// 			}
// 		}
// 		UE_LOG(LogTemp, Log,
// 			TEXT(
// 				"[%x] [UBP_UI_Timeline_node_data::GetBlockData] 最大最小 [ctli.minTime:%f] [ctli.maxTime:%f]  "
// 			), this, ctli.minTime, ctli.maxTime);
// 	}
//
//
// 	// 综合的最大最小区间长度
// 	int lenght = abs(ctli.minTime) + abs(ctli.maxTime);
//
//
// 	int timelineIndex = 0;
// 	entityLineInfoMap.Reset();
// 	// 所有实例
// 	for (auto& entityId : entitySortList)
// 	{
//
// 		Gamedata::EntityInfo* syncInfo = GetSyncInfo(entityId);
// 		if (syncInfo == nullptr)
// 		{
// 			continue;
// 		}
// 		
// 		if (syncInfo->movetimeline_size() <= selectTimelineIndex)
// 		{
// 			continue;
// 		}
// 		
// 		// 存入实例数据
// 		{
// 			FTimelineEntityLineInfo entityInfo2;
// 			entityLineInfoMap.Add(entityId, entityInfo2);
// 		}
// 		FTimelineEntityLineInfo* entityInfo2 = &entityLineInfoMap[entityId];
// 		entityInfo2->timelineIndex = timelineIndex;
// 		timelineIndex++;
// 		entityInfo2->y= entityInfo2->timelineIndex * ctli.entityHight;
//
// 		// 存入时间轴点数据
// 		const Gamedata::EntityMoveTimeline& movetimeline = syncInfo->movetimeline(selectTimelineIndex);
// 		if (movetimeline.locationmap_size() > 0)
// 		{
// 			// 时间轴排序
// 			auto& nodemap = movetimeline.locationmap();
// 			std::vector<int> timelimeKeyList;
// 			for (auto node : nodemap)
// 			{
// 				auto key = node.first;
// 				timelimeKeyList.push_back(key);
// 			}
// 			if (timelimeKeyList.size() > 1)
// 			{
// 				std::sort(timelimeKeyList.begin(), timelimeKeyList.end());
// 			}
// 			entityInfo2->start = timelimeKeyList[0] / 1000.0f;
// 			entityInfo2->end = timelimeKeyList[timelimeKeyList.size() - 1] / 1000.0f;
//
//
// 			for (int i = 0; i < timelimeKeyList.size(); i++)
// 			{
// 				int keyTime = timelimeKeyList[i];
// 				float itemIndex = keyTime / 1000.0f;
//
// 				const Gamedata::FVector* entityMoveFrameKey = &nodemap.find(keyTime)->second;
// 				FTimelineEnterPointInfo pointInfo;
// 				pointInfo.entityId = entityId;
// 				pointInfo.itemIndex = itemIndex;
// 				pointInfo.keyTime = keyTime;
// 				// pointInfo.move = UCommonlyObject::PBToUe4Vector(entityMoveFrameKey);
// 				pointInfo.x = itemIndex * ctli.oneSecondLenght;
//
// 				entityInfo2->pointInfoList.Add(pointInfo);
//
// 				// UE_LOG(LogTemp, Log,
// 				// 	TEXT(
// 				// 		"[%x] [UBP_UI_Timeline_node_data::GetPointData_InBlock] [entityId:%d] [count:%d/%d] [time:%f] [x:%f y:%f]"
// 				// 	), this, entityId,i, timelimeKeyList.size(),  itemIndex, pointInfo.point.X, pointInfo.point.Y);
// 			
// 			}
// 		}
// 	}
// 	
// }

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// controller
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UTimelineTimeUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	textBlockUp = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_up")));
	//textBlockDown = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_down")));

}

//
void UTimelineTimeUserWidget::SetStr(FString str)
{
	if (textBlockUp.IsValid() == false)return;

	textBlockUp->SetText(FText::FromString(str));
	//textBlockDown->SetText(FText::FromString(str));
}

// 初始化数据
void UTimelineTimeUserWidget::InitData(FTimelineTimeInfo* info)
{
	// auto time= info->itemIndex*
	std::string str = UCommonlyObject::FloatToString(info->itemIndex);
	FString str2 = UTF8_TO_TCHAR(str.c_str());

	// FString str = FString::Printf(TEXT("%f"), info->itemIndex);
	SetStr(str2);
}