// Fill out your copyright notice in the Description page of Project Settings.

#include "TimelinePanelUserWidget.h"
#include "Tools/CommonlyObject.h"
#include "TimelinePointUserWidget.h"
#include "TimelineTimeUserWidget.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "TimelineLeftLineUserWidget.h"
#include "TimelineCenterLineUserWidget.h"
#include "TimelineMainUserWidget.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "TimelineTimeAxisUserWidget.h"
#include "Tools/CalculationObject.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerMoveTimelineObject.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityTimelineComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerEditEntityComponent.h"
#include "FunctionalModule/EditEntity/Timeline/EditEntityManageTimelineComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerTimelineComponent.h"
#include "UI/Game/EditEntity/Main/EditEntityMainUserWidget.h"
#include "FunctionalModule/Selected/SelectedViewActor.h"
#include "FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityEditComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Overlay.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "UMG/Public/Components/VerticalBox.h"
#include <Components/EditableText.h>
#include <Components/TextBlock.h>
#include <Components/CheckBox.h>
#include "Yxjs/Core/GameInstanceStaticFunc.h"
#include "Yxjs/UI/HUDStaticFunc.h"
#include "Components/Image.h"
#include "Components/ComboBoxString.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"

/*
 * ????????????????????????:
 * 1.????????????????????????????????????????????????
 * 2.game->ui???????????? /  ui->game ??????
 * 3.????????? ????????? ???????????????/?????????/?????????(??????????????????)
 * 4.????????????/????????????
 * 5.?????????????????????????????????,??????????????????
 * 6.????????????????????????,????????????????????????????????????,????????????????????????????????????????????????
 * 7.
 * 8.

 ??????->????????????->??????????????????->????????????

 ???????????????????????????????????????
 void UPlayerInputComponent::MouseLeft_Released()
 void UPlayerSelectedComponent::Input_Timeline_LeftReleased()
 void UPlayerSelectedComponent::OperationResultsSwitch(Operation_Type type, bool mast)
 void UPlayerSelectedComponent::UICallback_TimelineMove()
 void UTimelinePanelUserWidget::Call_EntityChangeTransform(int entityId, FTransform wT)
 */


 /*/////////////////////////////////////////////////////////////*/
 // data
 /*/////////////////////////////////////////////////////////////*/

 // ??????????????????
float UTimelinePanelUserWidget::GetNowTimelineTime()
{
	float time = GetTimeWithX(ctli.timeAxisX);
	return time;
}

// ?????? ????????????
float UTimelinePanelUserWidget::GetViewTime()
{
	// if (testScrollBox2.IsValid() == false)return 0;
	//
	// // ?????????????????????????????????
	//
	// float lenght = abs(ctli.minTime) + abs(ctli.maxTime);
	// float offset = testScrollBox2->GetScrollOffset() / testScrollBox2->GetScrollOffsetOfEnd();
	// float var = lenght * offset;
	// return ctli.minTime + var;
	//
	return 0;
}

// ?????? ??????????????????x??????
float UTimelinePanelUserWidget::GetXWithTime(float time)
{
	// time ?????????????????????,????????????1000
	return time / ctli.nodeTimeLenght * ctli.oneSecondLenght;
}

// ?????? ??????x??????????????????
float UTimelinePanelUserWidget::GetTimeWithX(float x)
{
	return x / ctli.oneSecondLenght * ctli.nodeTimeLenght;
}

// ?????? ????????????????????????UI????????????
FVector2D UTimelinePanelUserWidget::GetMouseViewPoint(FVector2D mousePoint) const
{
	FGeometry gemotry = GetPaintSpaceGeometry();
	FVector2D size = gemotry.Size;
	float scale = gemotry.Scale;
	// {
	//
	// 	auto mousePosion2 = cuoi.mouseButtonEnterPosion * scale;
	//
	//
	// 	auto movePosition = (cuoi.absolutePosition - cuoi.mouseButtonEnterPosion) / scale;
	//
	// 	// ???????????????????????????
	// 	// ??????????????????
	// 	FIntPoint intPoint = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();
	// 	auto xPoint = (intPoint.X / scale - size.X) / 2;
	// 	auto yPoint = intPoint.Y / scale - size.Y;
	//
	// 	//
	// 	FVector2D widgetStartPoint = FVector2D(xPoint, yPoint);
	//
	// 	// ???????????????????????????
	// 	widgetStartPoint = widgetStartPoint * scale;
	// 	// ????????????????????????????????????????????????????????????
	// 	start = (cuoi.mouseButtonEnterPosion - widgetStartPoint) / scale;
	// 	end = start + movePosition;
	// }
	{
		UOverlay* overlay = Cast<UOverlay>(GetParent());
		if (overlay)
		{
			UCanvasPanelSlot* canvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(overlay);
			if (canvasPanelSlot)
			{
				FMargin margin = canvasPanelSlot->GetOffsets();

				// UE_LOG(LogTemp, Log, TEXT("[%x] [GetMouseViewPoint]  [left:%f] [top:%f]"), this, margin.Left, margin.Top);

				// ???????????????????????????
				// ??????????????????
				FIntPoint intPoint = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();
				auto xPoint = intPoint.X / scale / 2 + margin.Left;
				auto yPoint = intPoint.Y / scale + margin.Top;

				//
				FVector2D widgetStartPoint = FVector2D(xPoint, yPoint);

				// ???????????????????????????
				widgetStartPoint = widgetStartPoint * scale;
				// ????????????????????????????????????????????????????????????
				return (mousePoint - widgetStartPoint) / scale;
				// auto start = (cuoi.mouseButtonEnterPosion - widgetStartPoint) / scale;
				// auto movePosition = (cuoi.absolutePosition - cuoi.mouseButtonEnterPosion) / scale;
				// end = start + movePosition;
			}
		}
	}
	return FVector2D(0, 0);
}

// ????????????????????????(????????????)
FVector2D UTimelinePanelUserWidget::GetSelectBoxViewPoint(FVector2D start)
{
	FGeometry gemotry = GetPaintSpaceGeometry();
	FVector2D size = gemotry.Size;
	float scale = gemotry.Scale;
	auto movePosition = (cuoi.absolutePosition - cuoi.mouseButtonEnterPosion) / scale;
	return start + movePosition;
}

// ?????? ????????????????????????
FVector2D UTimelinePanelUserWidget::GetOverlayPointMargin()
{
	if (overlayPoint.IsValid() && overlayShowNode.IsValid())
	{
		// UCanvasPanelSlot* canvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(overlayPoint.Get());
		// if (canvasPanelSlot)
		// {
		// 	return canvasPanelSlot->GetOffsets();
		// }
		FVector2D result(0, 0);
		FGeometry gemotry = overlayPoint->GetPaintSpaceGeometry();

		UCanvasPanelSlot* canvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(overlayShowNode.Get());
		if (canvasPanelSlot)
		{
			FMargin margin = canvasPanelSlot->GetOffsets();
			result = FVector2D(margin.Left, margin.Top);
		}

		return FVector2D(gemotry.Position.X + result.X, gemotry.Position.Y + result.Y);
	}
	// return FMargin(0);
	return FVector2D(0, 0);
}

// ?????? ????????????????????????????????????
FVector2D UTimelinePanelUserWidget::GetOverlayTimeAxisMargin()
{
	if (overlayTimeAxis.IsValid() && overlayShowNode.IsValid())
	{
		FVector2D result(0, 0);
		FGeometry gemotry = overlayTimeAxis->GetPaintSpaceGeometry();

		UCanvasPanelSlot* canvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(overlayShowNode.Get());
		if (canvasPanelSlot)
		{
			FMargin margin = canvasPanelSlot->GetOffsets();
			result = FVector2D(margin.Left, margin.Top);
		}

		//return gemotry.Position + result;
		return FVector2D(gemotry.Position.X + result.X, gemotry.Position.Y + result.Y);
	}
	return FVector2D(0, 0);
}

// ?????? ????????? ???????????????ID
FTimelineEnterPointInfo* UTimelinePanelUserWidget::GetTimelinePointInfoWithUniqueMark(int entityId, int uniqueMark)
{
	if (entityLineInfoMap.Find(entityId) == nullptr)
	{
		return nullptr;
	}
	auto& pointInfoList = entityLineInfoMap[entityId].pointInfoList;

	FTimelineEnterPointInfo* ret = nullptr;
	for (int i = 0; i < pointInfoList.Num(); i++)
	{
		auto& node = pointInfoList[i];
		if (node.uniqueMark == uniqueMark)
		{
			ret = &node;
			break;
		}
	}
	return ret;
}

// ?????? ????????? ???????????????
FTimelineEnterPointInfo* UTimelinePanelUserWidget::GetTimelinePointInfoWithKeyTime(int entityId, int keyTime)
{
	if (entityLineInfoMap.Find(entityId) == nullptr)
	{
		return nullptr;
	}
	auto& pointInfoList = entityLineInfoMap[entityId].pointInfoList;

	FTimelineEnterPointInfo* ret = nullptr;
	for (int i = 0; i < pointInfoList.Num(); i++)
	{
		auto& node = pointInfoList[i];
		if (node.keyTime == keyTime)
		{
			ret = &node;
			break;
		}
	}
	return ret;
}

// ??????????????????
bool UTimelinePanelUserWidget::GetSelectPointList(std::vector<FTimelinePointIndex>& selectPointList,
	std::vector<int>& selectEntityId)
{
	bool selectStopInPoint = false;

	// ????????????
	for (auto it : showPointMap)
	{
		auto key = it.first;
		auto& value = it.second;

		FTimelineEnterPointInfo* pointInfo = GetTimelinePointInfoWithUniqueMark(value.entityId, value.uniqueMark);
		if (pointInfo)
		{
			if (pointInfo->isSelect)
			{
				// ????????????????????????
				if (cuoi.mouseRealTimeMode == CvMouseLeftMode::CvMouseLeftMode_PointMove && pointInfo->entityId == cuoi.stopInPoint_pointIndex.entityId && value.
					uniqueMark == cuoi
					.
					stopInPoint_pointIndex
					.uniqueMark
					)
				{
					selectStopInPoint = true;
				}
				// else
				{
					FTimelinePointIndex selectInfo(pointInfo->entityId, value.uniqueMark);
					selectPointList.push_back(selectInfo);

					auto it2 = std::find(selectEntityId.begin(), selectEntityId.end(), pointInfo->entityId);
					if (it2 == selectEntityId.end())
					{
						selectEntityId.push_back(pointInfo->entityId);
					}
				}
			}
		}
	}

	return selectStopInPoint;
}

// ????????????????????????????????????
bool UTimelinePanelUserWidget::IsSelectPoint(std::vector<FTimelinePointIndex>& selectPointList,
	std::vector<int>& selectEntityId, FTimelinePointIndex thisPoint)
{
	if (std::find(selectEntityId.begin(), selectEntityId.end(), thisPoint.entityId) == selectEntityId.end())
	{
		return false;
	}

	// if (std::find(selectPointList.begin(), selectPointList.end(), thisPoint) == selectPointList.end())
	// {
	// 	return false;
	// }
	// return true;

	bool flag = false;
	for (auto node : selectPointList)
	{
		if (node.entityId == thisPoint.entityId)
		{
			if (node.uniqueMark == thisPoint.uniqueMark)
			{
				flag = true;
				break;
			}
		}
	}
	return flag;
}

// ?????? ?????????????????????????????????id
bool UTimelinePanelUserWidget::GetMouseEntityId(int& entityId, FVector2D& offsetPoint_)
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController == nullptr)return false;

	// ???????????????
	FVector2D mousePosion;
	playerController->GetMousePosition(mousePosion.X, mousePosion.Y);
	FVector2D mousePosionInUI = GetMouseViewPoint(mousePosion);

	// ??????UI???????????????
	FVector2D offsetPoint(mousePosionInUI.X + ctli.viewLeftPoint.X, mousePosionInUI.Y + ctli.viewLeftPoint.Y);

	offsetPoint -= GetOverlayPointMargin();
	offsetPoint_ = offsetPoint;

	// ????????????entiry
	bool flag = false;

	for (int i = 0; i < realShowLineList.Num(); i++)
	{
		auto tempEntityId = realShowLineList[i];
		auto entityPointInfo = &entityLineInfoMap[tempEntityId];
		if (entityPointInfo->y <= offsetPoint.Y && offsetPoint.Y <= entityPointInfo->y + ctli.entityHight)
		{
			entityId = tempEntityId;
			flag = true;
			break;
		}
	}
	// return entityId;

	// if (flag == false)
	// {
	// 	return;
	// }
	return flag;
}

// ?????? ????????????????????????
bool UTimelinePanelUserWidget::IsSelectEntity(int entityId)
{
	bool flag = false;
	auto selectList = GetSelectLine();
	if (selectList)
	{
		for (auto node : *selectList)
		{
			if (entityId == node.entityId)
			{
				flag = true;
				break;
			}
		}
	}

	return flag;
}

// ?????? ????????????????????? ???????????????????????????(?????????????????????)
bool UTimelinePanelUserWidget::IsTimeAxisStopInTimePoint(FTimelinePointIndex& movePointInfo)
{
	// ????????????????????????

	bool flag = false;

	auto& selectEntityList = *Get_SelectEntityList();
	if (selectEntityList.Num() == 0)
	{
		return false;
	}

	// ????????????????????????
	if (cuoi.stopInPoint_pointIndex.entityId != 0 && cuoi.stopInPoint_pointIndex.uniqueMark != 0)
	{
		movePointInfo.entityId = cuoi.stopInPoint_pointIndex.entityId;
		movePointInfo.uniqueMark = cuoi.stopInPoint_pointIndex.uniqueMark;
		return true;
	}

	int keyTime = GetTimeWithX(ctli.timeAxisX) * 1000;

	// ?????????????????????
	int entityId = selectEntityList[selectEntityList.Num() - 1].entityId;
	for (auto it : showPointMap)
	{
		auto key = it.first;
		auto& value = it.second;

		if (entityId == value.entityId)
		{
			FTimelineEnterPointInfo* pointInfo = GetTimelinePointInfoWithUniqueMark(value.entityId, value.uniqueMark);
			if (pointInfo)
			{
				if (abs(keyTime - pointInfo->keyTime) < 50)	// ?????????50????????????
				{
					flag = true;
					movePointInfo.entityId = pointInfo->entityId;
					movePointInfo.uniqueMark = pointInfo->uniqueMark;
					break;
				}
			}
		}
	}

	return flag;
	//int timeKey = 0;
	//return TimelineIsSelectKeyFrame(timeKey, movePointInfo.entityId, movePointInfo.uniqueMark);
}

// ?????? ???????????????????????????????????????
bool UTimelinePanelUserWidget::TimelineIsSelectKeyFrame(int& timeKey, int& entityId, int& uniqueMark)
{
	bool flag = false;

	auto& selectEntityList = *Get_SelectEntityList();
	if (selectEntityList.Num() == 0)
	{
		return false;
	}

	int keyTime = GetTimeWithX(ctli.timeAxisX) * 1000;

	// ?????????????????????
	entityId = selectEntityList[selectEntityList.Num() - 1].entityId;
	for (auto it : showPointMap)
	{
		auto key = it.first;
		auto& value = it.second;

		if (entityId == value.entityId)
		{
			FTimelineEnterPointInfo* pointInfo = GetTimelinePointInfoWithUniqueMark(value.entityId, value.uniqueMark);
			if (pointInfo)
			{
				if (abs(keyTime - pointInfo->keyTime) < 50)	// ?????????50????????????
				{
					flag = true;
					entityId = pointInfo->entityId;
					uniqueMark = pointInfo->uniqueMark;
					break;
				}
			}
		}
	}
	return flag;
}

// ?????? ???????????????????????????????????????
bool UTimelinePanelUserWidget::CanSave()
{

	int timeKey = 0;
	int entityId = 0;
	int uniqueMark = 0;
	return TimelineIsSelectKeyFrame(timeKey, entityId, uniqueMark);
}

// ?????? ????????????????????????????????????
bool UTimelinePanelUserWidget::CanEntityAddPoint()
{
	int entityId;
	FVector2D offsetPoint;
	if (GetMouseEntityId(entityId, offsetPoint) == false)
	{
		return false;
	}

	if (IsSelectEntity(entityId) == false)
	{
		return false;
	}

	Gamedata::EntityInfo* syncInfo = GetSyncInfo(entityId);
	if (syncInfo == nullptr)
	{
		return false;
	}

	float time = GetTimeWithX(offsetPoint.X);

	float x = GetXWithTime(time);

	bool flag = true;
	// ????????????
	for (auto it : showPointMap)
	{
		auto key = it.first;
		auto& value = it.second;

		FTimelineEnterPointInfo* pointInfo = GetTimelinePointInfoWithUniqueMark(value.entityId, value.uniqueMark);
		if (pointInfo)
		{
			if (pointInfo->x - 20 < x && x < pointInfo->x + 20)
			{
				flag = false;
				break;
			}
		}
	}

	return flag;
}

// ???????????? ?????? ?????????
TArray<SelectInstanceInfo>* UTimelinePanelUserWidget::GetSelectLine()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(playerController->GetPawn());
	if (baseCharacter == nullptr)return nullptr;
	return &baseCharacter->selectComponent->cvMultipleInstanceInfo.selectedList;
}

// ???????????? ?????? ????????????view
TWeakObjectPtr<AEditEntityManage> UTimelinePanelUserWidget::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// ???????????? ?????? ????????????
Gamedata::EntityInfo* UTimelinePanelUserWidget::GetSyncInfo(int entityId)
{
	// auto syncMap = GetSyncEditEntityManageInfoMap();;
	// if (syncMap == nullptr)return nullptr;
	//
	// TMap<int32, Gamedata::EntityInfo>& syncEntityInfoMap = *syncMap;
	// if (syncEntityInfoMap.Find(entityId))
	// {
	// 	Gamedata::EntityInfo* syncInfo = &syncEntityInfoMap[entityId];
	// 	return syncInfo;
	// }
	// return nullptr;

	auto view = GetEditEntityManage();
	if (view == nullptr)return nullptr;

	return view->data->GetSyncInfo(entityId);
}

// ???????????? ????????????
TWeakObjectPtr<AEditEntityActor> UTimelinePanelUserWidget::GetEntityAcotr(int entityId)
{
	auto view = GetEditEntityManage();
	if (view == nullptr)return nullptr;

	return view->data->GetEntityActor(entityId);
}

// ???????????? ?????? xls??????
xlsdata::PlaceTableDefine* UTimelinePanelUserWidget::GetXlsInfo(int entityId)
{
	auto view = GetEditEntityManage();
	if (view == nullptr)return nullptr;

	return view->data->GetXlsInfo(entityId);
}

// ???????????? ????????????????????????
TArray<SelectInstanceInfo>* UTimelinePanelUserWidget::Get_SelectEntityList()
{
	auto player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto character = Cast<ABaseCharacter>(player->GetPawn());
	if (character == nullptr)return nullptr;
	if (character->selectComponent)
	{
		return &character->selectComponent->cvMultipleInstanceInfo.selectedList;
	}

	return nullptr;
}

// ?????? ????????????????????????????????????
bool UTimelinePanelUserWidget::IsNeedSaveData()
{
	bool flag = false;
	if (cuoi.saveDataCount > 0)flag = true;
	cuoi.saveDataCount = 0;

	return flag;
}

// ?????????????????????actor
TWeakObjectPtr<AEditEntityActor> UTimelinePanelUserWidget::GetSelectEntityActor()
{

	//auto player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//auto character = Cast<ABaseCharacter>(player->GetPawn());
	//if (character == nullptr)return;
	//if (character->selectComponent == nullptr)return;

	auto entityList = Get_SelectEntityList();
	if (entityList == nullptr)return nullptr;
	auto& entityList_ = *entityList;
	if (entityList_.Num() == 0)return nullptr;
	int masterEntityId = entityList_[entityList_.Num() - 1].entityId;

	//auto& lineInfo = entityLineInfoMap[masterEntityId];
	auto actor = GetEntityAcotr(masterEntityId);
	//if (actor.IsValid()) {
	//}
	return actor;
}

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

  //
void UTimelinePanelUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::NativeOnInitialized] "), this);
}

//
void UTimelinePanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::NativeConstruct] "), this);

	bIsFocusable = true;		// NativeOnKeyDown????????????
	//SetKeyboardFocus();
	Init();
}

//
void UTimelinePanelUserWidget::Init()
{
	if (isInit)
	{
		return;
	}

	{
		FString bpPath = TEXT(
			"WidgetBlueprint'/Game/Yxjs/UI/EditEntity/Timeline/BP_UI_Timeline_time.BP_UI_Timeline_time_C'");
		class_BP_UI_Timeline_time_C = LoadClass<UTimelineTimeUserWidget>(this, *bpPath);
		if (class_BP_UI_Timeline_time_C == nullptr)
		{
			return;
		}
	}
	{
		FString bpPath = TEXT(
			"WidgetBlueprint'/Game/Yxjs/UI/EditEntity/Timeline/BP_UI_Timeline_point.BP_UI_Timeline_point_C'");
		class_BP_UI_Timeline_point_C = LoadClass<UTimelinePointUserWidget>(this, *bpPath);
		if (class_BP_UI_Timeline_point_C == nullptr)
		{
			return;
		}
	}
	{
		FString bpPath = TEXT(
			"WidgetBlueprint'/Game/Yxjs/UI/EditEntity/Timeline/BP_UI_Timeline_centerLine.BP_UI_Timeline_centerLine_C'");
		class_BP_UI_Timeline_centerLine_C = LoadClass<UTimelineCenterLineUserWidget>(this, *bpPath);
		if (class_BP_UI_Timeline_centerLine_C == nullptr)
		{
			return;
		}
	}
	{
		FString bpPath = TEXT(
			"WidgetBlueprint'/Game/Yxjs/UI/EditEntity/Timeline/BP_UI_Timeline_leftLine.BP_UI_Timeline_leftLine_C'");
		class_BP_UI_Timeline_leftLine_C = LoadClass<UTimelineLeftLineUserWidget>(this, *bpPath);
		if (class_BP_UI_Timeline_leftLine_C == nullptr)
		{
			return;
		}
	}

	//
	overlayPoint = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_point")));
	overlayTime = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_time")));
	overlayCenterLine = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_centerLine")));

	overlayShowNode = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_show_node")));

	overlayTimeAxis = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_timeAxis")));
	BP_UI_Timeline_timeAxis = Cast<UUserWidget>(GetWidgetFromName(TEXT("BP_UI_Timeline_timeAxis")));

	EditableText_x = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_x")));
	EditableText_y = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_y")));
	EditableText_z = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_z")));
	EditableText_pitch = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_pitch")));
	EditableText_yaw = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_yaw")));
	EditableText_roll = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_roll")));
	EditableText_time = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_time")));
	EditableText_r = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_r")));
	EditableText_g = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_g")));
	EditableText_b = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_b")));
	CheckBox_locationTeleport = Cast<UCheckBox>(GetWidgetFromName(TEXT("CheckBox_locationTeleport")));
	CheckBox_colorTeleport = Cast<UCheckBox>(GetWidgetFromName(TEXT("CheckBox_colorTeleport")));

	ComboBoxString_highlight = Cast<UComboBoxString>(GetWidgetFromName(TEXT("ComboBoxString_highlight")));
	TextBlock_highlight_str = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_highlight_str")));

	Image_stop1 = Cast<UImage>(GetWidgetFromName(TEXT("Image_stop1")));
	Image_stop2 = Cast<UImage>(GetWidgetFromName(TEXT("Image_stop2")));

	if (CheckBox_locationTeleport.IsValid() == false
		|| ComboBoxString_highlight.IsValid() == false
		|| CheckBox_colorTeleport.IsValid() == false
		|| Image_stop1.IsValid() == false
		|| Image_stop2.IsValid() == false)
	{
		return;
	}

	FScriptDelegate func;
	func.BindUFunction(this, TEXT("MySelectionChanged"));
	ComboBoxString_highlight->OnSelectionChanged.AddUnique(func);


	isInit = true;

	SetViewData(FVector2D(0, 0));

	DrawLine(FVector2D(0, 0), FVector2D(220, 220));


	SetKeyFrameCanWrite(false, true);
}

//
void UTimelinePanelUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
	UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::NativeDestruct] "), this);
}

/*-------------------------------------------------------------*/

// ??????
int32 UTimelinePanelUserWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
	int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	auto ret = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
		bParentEnabled);
	FPaintContext Context2(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	DrawSelectBox(Context2);
	return ret;
}

// ?????? NativePaint()->??????
void UTimelinePanelUserWidget::DrawSelectBox(FPaintContext& Context) const
{
	// UWidgetBlueprintLibrary::DrawBox(Context, FVector2D(22, 22), FVector2D(222, 222), nullptr, FLinearColor::Red);

	if (cuoi.leftMouseButtonEnter && cuoi.mouseLeftMode == CvMouseLeftMode::CvMouseLeftMode_PointSelect)
	{
		// // auto topLeft = Context.MyCullingRect.GetTopLeft();
		// auto mousePosion2 = cuoi.mouseButtonEnterPosion * Context.AllottedGeometry.Scale;
		//
		//
		// auto movePosition = (cuoi.absolutePosition - cuoi.mouseButtonEnterPosion) / Context.AllottedGeometry.Scale;
		//
		//
		// // ???????????????????????????
		//
		// auto size = Context.AllottedGeometry.Size;
		//
		// // ??????????????????
		// FIntPoint intPoint = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();
		// auto acale = Context.AllottedGeometry.Scale;
		// auto xPoint = (intPoint.X / acale - size.X) / 2;
		// auto yPoint = intPoint.Y / acale - size.Y;
		//
		// //
		// FVector2D widgetStartPoint = FVector2D(xPoint, yPoint);
		//
		// // ???????????????????????????
		// widgetStartPoint = widgetStartPoint * Context.AllottedGeometry.Scale;
		// // ????????????????????????????????????????????????????????????
		// auto start = (cuoi.mouseButtonEnterPosion - widgetStartPoint) / Context.AllottedGeometry.Scale;
		// auto end = start + movePosition;

		FVector2D start = GetMouseViewPoint(cuoi.absolutePosition);
		FVector2D end = GetMouseViewPoint(cuoi.mouseButtonEnterPosion);

		FVector2D point2(end.X, start.Y);
		FVector2D point4(start.X, end.Y);

		UWidgetBlueprintLibrary::DrawLine(Context, start, point2, FLinearColor::White, true, 1.0f);
		UWidgetBlueprintLibrary::DrawLine(Context, point2, end, FLinearColor::White, true, 1.0f);
		UWidgetBlueprintLibrary::DrawLine(Context, end, point4, FLinearColor::White, true, 1.0f);
		UWidgetBlueprintLibrary::DrawLine(Context, point4, start, FLinearColor::White, true, 1.0f);
	}

	// UE_LOG(LogTemp, Log,
	// 	TEXT(
	// 		"[%x] [UTimelinePanelUserWidget::TestFunc] ###########################################"), this);
}

// ?????? NativePaint()->????????????
void UTimelinePanelUserWidget::DrawDottedLine(FPaintContext& Context) const
{
	//??????????????????????????????
	const float ImaginaryLine = 10;

	//???????????????????????????
	const float InaginaryGap = 20;
	//
	//??????????????? ??????????????????????????????
	FVector2D MarkPos(55, 55);
	FVector2D UnitPos(1600, 1600);

	//??????????????????????????????
	float allLen = FVector2D::Distance(UnitPos, MarkPos);

	//???????????????????????????????????????????????????????????????
	int num = allLen / (ImaginaryLine + InaginaryGap);

	//??????????????????????????????x y???
	float next_x = ImaginaryLine * (MarkPos.X - UnitPos.X) / allLen + UnitPos.X;
	float next_y = ImaginaryLine * (MarkPos.Y - UnitPos.Y) / allLen + UnitPos.Y;

	//???????????????
	FVector2D startPos = UnitPos;
	FVector2D nextPos = FVector2D(next_x, next_y);
	for (int i = 1; i <= num; i++)
	{
		//????????????
		UWidgetBlueprintLibrary::DrawLine(Context, startPos, nextPos, FLinearColor::White, true, 1.0f);
		//???????????????????????????
		float begin_x = (ImaginaryLine + InaginaryGap) * i * (MarkPos.X - UnitPos.X) / allLen + UnitPos.X;
		float begin_y = (ImaginaryLine + InaginaryGap) * i * (MarkPos.Y - UnitPos.Y) / allLen + UnitPos.Y;
		startPos = FVector2D(begin_x, begin_y);

		float next_x2 = begin_x + ImaginaryLine * (MarkPos.X - UnitPos.X) / allLen;
		float next_y2 = begin_y + ImaginaryLine * (MarkPos.Y - UnitPos.Y) / allLen;
		nextPos = FVector2D(next_x2, next_y2);
	}
}

/*-------------------------------------------------------------*/

// ??????????????? ??????
void UTimelinePanelUserWidget::TimeAxisMove(float x)
{
	if (BP_UI_Timeline_timeAxis.IsValid() == false)return;

	ctli.timeAxisX = x;
	FVector2D location(ctli.timeAxisX - ctli.viewLeftPoint.X, 0);
	if (location.X < -50)
	{
		location.X = -50;
		BP_UI_Timeline_timeAxis->SetRenderTranslation(location);
	}
	else if (location.X > ctli.blockXLenght)
	{
		location.X = ctli.blockXLenght;
		BP_UI_Timeline_timeAxis->SetRenderTranslation(location);
	}
	else
	{
		BP_UI_Timeline_timeAxis->SetRenderTranslation(location);
	}

	// ???????????? game entity ??????
	float time = GetTimeWithX(x);
	Callback_TimeAxisChange(time);

	auto actor = GetSelectEntityActor();
	if (actor.IsValid())
	{
		FVector color;
		FVector location;
		FRotator rotator;
		actor->myTimelineComponent->GetNowTransform(true, time, location, rotator, color);

		auto syncInfo = GetSyncInfo(actor->entityId);
		auto& transform_timeline = syncInfo->transform_timeline();

		CvTimelineUIInfo info;
		info.location = location;
		info.rotator = rotator;
		info.color = color;
		info.highlight = transform_timeline.timeline_highlight();
		info.locationTeleport = transform_timeline.location_teleport();
		info.colorTeleport = transform_timeline.color_teleport();
		info.time = GetTimeWithX(ctli.timeAxisX);
		SetTextValue(info);
	}

	// ?????????????????????UI??????????????????
	{
		int timeKey = 0;
		int entityId = 0;
		int uniqueMark = 0;
		bool flag = TimelineIsSelectKeyFrame(timeKey, entityId, uniqueMark);

		SetKeyFrameCanWrite(flag, false);
	}

}

// ??????????????? TimeAxisMove->?????????????????????
void UTimelinePanelUserWidget::Callback_TimeAxisChange(float time)
{
	auto actor = GetSelectEntityActor();
	if (actor.IsValid())
	{
		actor->myTimelineComponent->MoveTimelineUI(false, time);
	}
}

// ???????????? ??????
bool UTimelinePanelUserWidget::CenterLineCreate(FTimelineEntityLineInfo& timelineEntityLineInfo)
{
	if (overlayCenterLine.IsValid() && timelineEntityLineInfo.centerLineUserWidget.IsValid() == false)
	{
		UTimelineCenterLineUserWidget* userWidget = Cast<UTimelineCenterLineUserWidget>(
			CreateWidget(GetWorld(), class_BP_UI_Timeline_centerLine_C));

		userWidget->panel = this;
		userWidget->entityId = timelineEntityLineInfo.entityId;
		timelineEntityLineInfo.centerLineUserWidget = userWidget;
		overlayCenterLine->AddChild(userWidget);

		// UE_LOG(LogTemp, Log, TEXT("[%x] [CenterLineCreate] ?????? [index:%d] [y:%f] [entityId:%d] "), this, timelineEntityLineInfo.timelineIndex, timelineEntityLineInfo.y, timelineEntityLineInfo.entityId);

		userWidget->SetSelect(timelineEntityLineInfo.isSelect);

		return true;
	}
	else
	{
		return false;
	}
}

// ???????????? ??????
void UTimelinePanelUserWidget::CenterLineRemove(FTimelineEntityLineInfo& timelineEntityLineInfo)
{
	if (overlayCenterLine.IsValid() == false)return;

	if (timelineEntityLineInfo.centerLineUserWidget.IsValid() == false)return;

	timelineEntityLineInfo.centerLineUserWidget->RemoveFromParent();
	timelineEntityLineInfo.centerLineUserWidget = nullptr;

	// UE_LOG(LogTemp, Log, TEXT("[%x] [CenterLineRemove] ?????? [index:%d] [y:%f] [entityId:%d] "), this, timelineEntityLineInfo.timelineIndex, timelineEntityLineInfo.y, timelineEntityLineInfo.entityId);
}

// ???????????? ??????
void UTimelinePanelUserWidget::CenterLineMove(FTimelineEntityLineInfo& timelineEntityLineInfo)
{
	if (overlayCenterLine.IsValid() == false)return;

	if (timelineEntityLineInfo.centerLineUserWidget.IsValid() == false)return;

	auto y = entityLineInfoMap[timelineEntityLineInfo.entityId].y;
	auto setX = 0;
	auto setY = y - ctli.viewLeftPoint.Y;

	FVector2D location(setX, setY);
	timelineEntityLineInfo.centerLineUserWidget->SetRenderTranslation(location);
}

// ???????????? ??????
void UTimelinePanelUserWidget::CenterLineChoose(FTimelineEntityLineInfo& timelineEntityLineInfo, bool flag)
{
	if (overlayCenterLine.IsValid() == false)return;

	if (timelineEntityLineInfo.centerLineUserWidget.IsValid() == false)return;

	Cast<UTimelineCenterLineUserWidget>(timelineEntityLineInfo.centerLineUserWidget)->SetSelect(flag);
}

// ????????????????????? ??????
void UTimelinePanelUserWidget::LeftLineRemove(FTimelineEntityLineInfo& timelineEntityLineInfo)
{
	// if (verticalBox_entityName.IsValid() == false)return;

	if (timelineEntityLineInfo.leftLineUserWidget.IsValid() == false)return;

	timelineEntityLineInfo.leftLineUserWidget->RemoveFromParent();
	timelineEntityLineInfo.leftLineUserWidget = nullptr;

	// UE_LOG(LogTemp, Log, TEXT("[%x] [LeftLineRemove] ?????? [index:%d] [y:%f] [entityId:%d] "), this, timelineEntityLineInfo.timelineIndex, timelineEntityLineInfo.y, timelineEntityLineInfo.entityId);
}

// ????????????????????? ?????????????????????????????????
void UTimelinePanelUserWidget::LeftLineMove(FTimelineEntityLineInfo& timelineEntityLineInfo)
{
	// if (scrollBox_entityName.IsValid() == false)return;
	// scrollBox_entityName->SetScrollOffset(ctli.viewLeftPoint.Y);
	//

	//if (overlayLeftLine.IsValid() == false)return;

	if (timelineEntityLineInfo.leftLineUserWidget.IsValid() == false)return;

	auto y = entityLineInfoMap[timelineEntityLineInfo.entityId].y;
	auto setX = 0;
	auto setY = y - ctli.viewLeftPoint.Y;

	FVector2D location(setX, setY);
	timelineEntityLineInfo.leftLineUserWidget->SetRenderTranslation(location);
}

// ????????????????????? ??????
void UTimelinePanelUserWidget::LeftLineChoose(FTimelineEntityLineInfo& timelineEntityLineInfo, bool flag)
{
}

// ?????????????????????????????????
void UTimelinePanelUserWidget::SetKeyFrameCanWrite(bool canWrite, bool isMast)
{

	if (uiState.transformCanWrite == canWrite && isMast == false)
	{
		return;
	}
	uiState.transformCanWrite = canWrite;
	if (UFunction* func = FindFunction("Event_SetKeyFrameCanWrite"))
	{
		struct TempParam
		{
			bool canWrite = false;
		};
		TempParam tempParam;
		tempParam.canWrite = canWrite;;
		ProcessEvent(func, &tempParam);
	}

	UHUDStaticFunc::ChangeInputHelp(this, EUIChildName::ui_name_timeline, canWrite == false ? PlayerOperateType::PlayerOperateType_timeline_cant_edit : PlayerOperateType::PlayerOperateType_timeline_edit);

	if (canWrite)
	{
		Image_stop1->SetVisibility(ESlateVisibility::Hidden);
		Image_stop2->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_stop2->SetVisibility(ESlateVisibility::Hidden);
		Image_stop1->SetVisibility(ESlateVisibility::Visible);
	}

}

/*-------------------------------------------------------------*/

// ????????? ??????
void UTimelinePanelUserWidget::ViewMove(FVector2D newPosition)
{
	// isTimelineView true:??????????????????????????? false:??????????????????????????????

	// UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::ViewMove] ?????????????????? [time:%f] [x:%f y:%f]"), this,
	//       GetWorld()->TimeSeconds, move.X, move.Y);

	// auto move = newPosition - ctli.viewLeftPoint;

	SetViewData(newPosition);

	ViewShowTime();

	ViewShowPoint();

	ViewShowLine();

	// auto newX = ctli.timeAxisX + move.X;
	TimeAxisMove(ctli.timeAxisX);

	// if(isTimelineView)
	// {
	// 	LeftLineMove();
	//
	// }

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::SetViewData] ?????? [?????????:%s] [?????????:%s]"), this,
	//	*ctli.viewLeftPoint.ToString(), *ctli.viewRightPoint.ToString());
}

// ????????? ?????? ViewMove()->????????????????????????
void UTimelinePanelUserWidget::SetViewData(FVector2D move_)
{
	// ?????????
	ctli.srollNumber = 0;

	// ??????????????????
	ctli.viewLeftPoint = move_;
	if (ctli.viewLeftPoint.Y < 0)
	{
		ctli.viewLeftPoint.Y = 0;
	}
	auto maxY = entitySortList.Num() * ctli.entityHight - ctli.blockYLenght;
	//if (ctli.viewLeftPoint.Y > maxY)
	//{
	//	ctli.viewLeftPoint.Y = maxY;
	//}
	ctli.viewLeftPoint.Y = 0;

	// ??????????????????
	ctli.viewRightPoint.X = ctli.viewLeftPoint.X + ctli.blockXLenght;
	ctli.viewRightPoint.Y = ctli.viewLeftPoint.Y + ctli.blockYLenght;

	// ??????
	ctli.viewLeftTime = float(ctli.viewLeftPoint.X) / float(ctli.oneSecondLenght);

	// ?????????????????????????????????
	{
		readyShowLineList.Reset();
		int i = ctli.viewLeftPoint.Y / ctli.entityHight - 1;
		if (i < 0)i = 0;
		int maxNum = i + ctli.blockYLenght / ctli.entityHight + 1;
		for (; i < entitySortList.Num() && i <= maxNum; i++)
		{
			readyShowLineList.Add(entitySortList[i]);
		}
	}
}

// ????????? ?????? ?????????????????????
void UTimelinePanelUserWidget::ViewShowPoint()
{
	float minX = ctli.viewLeftPoint.X;
	float maxX = ctli.viewRightPoint.X;
	float minY = ctli.viewLeftPoint.Y;
	float maxY = ctli.viewRightPoint.Y;

	// ?????????????????????????????????
	for (auto it : showPointMap)
	{
		auto key = it.first;
		auto& value = it.second;
		showPointMap[key].isDeleted = true;
	}

	// ????????????????????????????????????
	for (auto& it : readyShowLineList)
	{
		auto entityId = it;
		auto& timelineEntityLineInfo = entityLineInfoMap[it];

		// y????????????
		if (minY <= timelineEntityLineInfo.y && timelineEntityLineInfo.y <= maxY)
		{
			// ?????????
			auto& pointInfoList = timelineEntityLineInfo.pointInfoList;
			for (int i = 0; i < pointInfoList.Num(); i++)
			{
				FTimelineEnterPointInfo& pointInfo = pointInfoList[i];
				// x????????????
				if (minX <= pointInfo.x && pointInfo.x <= maxX)
				{
					FTimelinePointIndex pointIndex(entityId, pointInfo.uniqueMark);

					if (PointCreate(pointInfo, pointIndex))
					{
						SetShowPointMap(true, pointIndex);
					}

					{
						std::string mapKey = UCommonlyObject::IntToString(pointIndex.entityId) + "_" + UCommonlyObject::IntToString(pointIndex.uniqueMark);
						showPointMap[mapKey].isDeleted = false;
					}

					FVector2D move(0, 0);
					PointMove(pointInfo, move);
				}
			}
		}
	}

	// ???????????????????????????
	std::list<std::string> delList;
	for (auto it : showPointMap)
	{
		auto key = it.first;
		auto& value = it.second;
		if (value.isDeleted)
		{
			auto pointInfo = GetTimelinePointInfoWithUniqueMark(value.entityId, value.uniqueMark);
			PointRemove(*pointInfo);
			delList.push_back(key);
		}
	}

	// ????????????
	for (auto it = delList.begin(); it != delList.end(); it++)
	{
		showPointMap.erase(*it);
	}
}

// ????????? ?????? ??????????????????????????????
void UTimelinePanelUserWidget::ViewShowTime()
{
	if (class_BP_UI_Timeline_time_C == nullptr)return;

	float minX = ctli.viewLeftPoint.X - ctli.oneSecondLenght;
	float maxX = ctli.viewRightPoint.X + ctli.oneSecondLenght;
	float minY = ctli.viewLeftPoint.Y;
	float maxY = ctli.viewRightPoint.Y;

	if (ctli.viewLeftPoint.X > 0)minX = ctli.viewLeftPoint.X;
	if (ctli.viewRightPoint.X < 0)maxX = ctli.viewRightPoint.X;


	int forCount = UCalculationObject::RoundFloat((maxX - minX)) / ctli.oneSecondLenght;

	for (auto node : timeInfoMap)
	{
		timeInfoMap[node.Key].isDelete = true;
	}
	FString str1 = TEXT("");
	FString str2 = TEXT("");
	for (int i = 0; i < forCount; i++)
	{
		float itemXTemp2 = (minX + i * ctli.oneSecondLenght) / ctli.oneSecondLenght;


		int itemXTemp = itemXTemp2;


		// ??????
		if (timeInfoMap.Find(itemXTemp) == nullptr)
		{
			FTimelineTimeInfo timelineTimeInfo;

			timelineTimeInfo.x = itemXTemp * ctli.oneSecondLenght;
			timelineTimeInfo.itemIndex = itemXTemp;

			auto bp_UTimelineTimeUserWidget = Cast<UTimelineTimeUserWidget>(
				CreateWidget(GetWorld(), class_BP_UI_Timeline_time_C));
			if (bp_UTimelineTimeUserWidget == nullptr)return;

			overlayTime->AddChildToOverlay(bp_UTimelineTimeUserWidget);
			timelineTimeInfo.timeUserWidget = bp_UTimelineTimeUserWidget;

			timeInfoMap.Add(itemXTemp, timelineTimeInfo);

			bp_UTimelineTimeUserWidget->InitData(&timeInfoMap[itemXTemp]);

			str1 = str1 + FString::Printf(TEXT(",%d"), itemXTemp);
		}

		// ????????????
		FTimelineTimeInfo* timelineTimeInfo = &timeInfoMap[itemXTemp];

		FVector2D location(timelineTimeInfo->x - ctli.viewLeftPoint.X, 0);
		if (timelineTimeInfo->timeUserWidget.IsValid())
		{
			timelineTimeInfo->timeUserWidget->SetRenderTranslation(location);
		}
		timelineTimeInfo->isDelete = false;
	}

	// ??????????????????????????????
	{
		TArray<int> delList;
		for (auto node : timeInfoMap)
		{
			if (node.Value.isDelete)
			{
				delList.Push(node.Key);
			}
		}
		for (auto node : delList)
		{
			FTimelineTimeInfo* timeInfo = &timeInfoMap[node];
			if (timeInfo->timeUserWidget.IsValid())
			{
				timeInfo->timeUserWidget->RemoveFromParent();
				timeInfo->timeUserWidget = nullptr;
			}
			timeInfoMap.Remove(node);
			str2 = str2 + FString::Printf(TEXT(",%d"), node);
		}
	}
	//UE_LOG(LogTemp, Log, TEXT("[%x] [?????????] [ViewShowTime] [x:%d-%d] [??????:%s] [??????:%s]"), this, (int)minX, (int)maxX, *str1, *str2);


}

// ????????? ?????? ?????????????????????
void UTimelinePanelUserWidget::ViewShowLine()
{
	float minX = ctli.viewLeftPoint.X;
	float maxX = ctli.viewRightPoint.X;
	float minY = ctli.viewLeftPoint.Y - ctli.entityHight;
	float maxY = ctli.viewRightPoint.Y + ctli.entityHight;

	// ??????????????????
	TArray<int> tempRealShowLineList;

	// ????????????
	for (auto& it : readyShowLineList)
	{
		auto entityId = it;
		auto& timelineEntityLineInfo = entityLineInfoMap[it];

		// y????????????
		if (minY <= timelineEntityLineInfo.y && timelineEntityLineInfo.y <= maxY)
		{
			// ??????
			//if (LeftLineCreate(timelineEntityLineInfo) && CenterLineCreate(timelineEntityLineInfo))
			//{
			//	// UE_LOG(LogTemp, Log, TEXT("[%x] [ViewShowLine] ?????? [y:%f] [entityId:%d] "), this, timelineEntityLineInfo.y, entityId);
			//}

			// ??????
			LeftLineMove(timelineEntityLineInfo);
			CenterLineMove(timelineEntityLineInfo);

			tempRealShowLineList.Add(entityId);
		}
	}

	// ??????
	for (auto node : realShowLineList)
	{
		if (tempRealShowLineList.Find(node) == false)
		{
			auto& timelineEntityLineInfo = entityLineInfoMap[node];
			LeftLineRemove(timelineEntityLineInfo);
			CenterLineRemove(timelineEntityLineInfo);

			// UE_LOG(LogTemp, Log, TEXT("[%x] [ViewShowLine] ?????? [y:%f] [entityId:%d] "), this, timelineEntityLineInfo.y, node);
		}
	}

	// ??????
	realShowLineList = tempRealShowLineList;
}

// ????????????
void UTimelinePanelUserWidget::SaveChangeDate()
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UEditEntityManageController* placeController = baseGameMode->editEntityManage->controller;
	if (placeController == nullptr)return;

	TArray<int32> entityIdList;
	int entityId = character->selectComponent->GetSelectMasterInstance();
	entityIdList.Add(entityId);
	if (IsNeedSaveData())
	{
		placeController->NetCall_UpdateData(entityIdList);
	}
}

/*------------------------------------------------------------------*/

// UI?????? ??????????????????
void UTimelinePanelUserWidget::OnExpandCollapse(bool isShow)
{

	FString str = isShow ? TEXT("??????") : TEXT("??????");
	UE_LOG(LogTemp, Log, TEXT("[%x] [?????????] [OnExpandCollapse] [%s]"), this, *str);

	if (isShow)
	{
		OpenUI();
	}
	else
	{
		CloseUI();
	}
}

// UI?????? ??????
void UTimelinePanelUserWidget::OpenUI()
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;

	if (character->selectComponent->IsSelectEntity())
	{
		int entityId = character->selectComponent->GetSelectMasterInstance();
		auto entitySyncInfo = character->myData->gameMode->editEntityManage->data->GetSyncInfo(entityId);
		Call_InitData(false, entitySyncInfo);
	}

	character->timelineComponent->Call_EnterEditTimelineStaus(true);
}

// UI?????? ?????? ???????????????
void UTimelinePanelUserWidget::Call_InitData(bool mast, Gamedata::EntityInfo* entityInfo)
{
	auto view = GetEditEntityManage();

	entitySortList.Reset();
	entitySortList.Add(entityInfo->entityid());

	SetBlockData();

	// ????????????????????????????????????

	// ??????????????????time 0

	ViewMove(FVector2D(ctli.timeAxisX - 480, 0));

}

// UI?????? ?????? Call_InitData()->??????????????????
void UTimelinePanelUserWidget::SetBlockData()
{
	// ???????????????????????????
	{
		// ????????????????????????
		ctli.minTime = ctli.maxTime = 0;
		for (auto& entityId : entitySortList)
		{
			Gamedata::EntityInfo* syncInfo = GetSyncInfo(entityId);
			if (syncInfo == nullptr)
			{
				continue;
			}
			{
				auto& transform_timeline = syncInfo->transform_timeline();
				auto& point_map = transform_timeline.point_map();
				std::vector<int> timelimeKeyList;
				for (auto node : point_map)
				{
					auto key = node.first;
					timelimeKeyList.push_back(key);
				}
				if (timelimeKeyList.size() > 1)
				{
					std::sort(timelimeKeyList.begin(), timelimeKeyList.end());
					float tempMin = timelimeKeyList[0] / 1000.0f;
					float tempMax = timelimeKeyList[timelimeKeyList.size() - 1] / 1000.0f;
					if (tempMin < ctli.minTime)
					{
						ctli.minTime = tempMin;
					}
					if (tempMax > ctli.maxTime)
					{
						ctli.maxTime = tempMax;
					}
				}
			}
		}
		UE_LOG(LogTemp, Log,
			TEXT(
				"[%x] [UBP_UI_Timeline_node_data::SetBlockData] ???????????? [ctli.minTime:%f] [ctli.maxTime:%f]  "
			), this, ctli.minTime, ctli.maxTime);
	}

	// ?????????????????????????????????
	int lenght = abs(ctli.minTime) + abs(ctli.maxTime);

	int timelineIndex = 0;
	for (auto node : entityLineInfoMap)
	{
		PointRemoveShowInfo(node.Value);
		LeftLineRemove(node.Value);
		CenterLineRemove(node.Value);
	}

	entityLineInfoMap.Reset();

	// ????????????
	for (auto& entityId : entitySortList)
	{
		// ??????
		if (entityLineInfoMap.Find(entityId) == nullptr)
		{
			FTimelineEntityLineInfo entityLineInfo;
			entityLineInfo.timelineIndex = timelineIndex;
			entityLineInfo.entityId = entityId;
			entityLineInfo.y = timelineIndex * ctli.entityHight;
			entityLineInfoMap.Add(entityId, entityLineInfo);
			timelineIndex++;
		}

		// ??????????????????
		UpdateEntityDataToUIData(entityId, ETimelineOperationType::TimelineOperation_UpdateEntityDataToUIData_create);
	}
}

// UI?????? ?????? SetBlockData()->??????????????????
void UTimelinePanelUserWidget::PointRemoveShowInfo(FTimelineEntityLineInfo& timelineEntityLineInfo)
{
	std::list<std::string> delList;
	for (auto it : showPointMap)
	{
		auto key = it.first;
		auto& value = it.second;
		{
			auto pointInfo = GetTimelinePointInfoWithUniqueMark(value.entityId, value.uniqueMark);
			PointRemove(*pointInfo);
			delList.push_back(key);
		}
	}

	// ????????????
	for (auto it = delList.begin(); it != delList.end(); it++)
	{
		showPointMap.erase(*it);
	}
}

// UI?????? ??????
void UTimelinePanelUserWidget::CloseUI()
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	auto hud = Cast<ABaseHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (hud == nullptr)return;
	if (character->timelineComponent == nullptr)return;
	auto placeData = character->myData->gameMode->editEntityManage->data;

	if (UFunction* func = FindFunction("Event_CloseUI"))
	{
		struct TempParam
		{
		};
		TempParam tempParam;

		ProcessEvent(func, &tempParam);
	}

	if (character->selectComponent->IsSelectEntity())
	{	// time=0??????????????? ?????? syncinfo?????????
		int entityId = character->selectComponent->GetSelectMasterInstance();

		auto entityActor = placeData->GetEntityActor(entityId);
		if (entityActor.IsValid())
		{
			entityActor->myTimelineComponent->MoveTimelineUI(true, 0);
		}
	}

	character->timelineComponent->Call_EnterEditTimelineStaus(false);
	//character->timelineComponent->SaveChangeDate();
	SaveChangeDate();

}

/*------------------------------------------------------------------*/

// UI?????? ????????????1 
void UTimelinePanelUserWidget::UICall_SaveData(bool justSync)
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto selectedView = baseGameMode->selectedViewActor;
	auto& placeData = character->myData->gameMode->editEntityManage->data;

	// ??????1
	int timeKey = 0;
	int entityId = 0;
	int uniqueMark = 0;
	bool flag = TimelineIsSelectKeyFrame(timeKey, entityId, uniqueMark);
	if (flag == false)return;
	if (uiState.transformCanWrite == false)return;

	// ??????2
	auto entityActor = placeData->GetEntityActor(entityId);
	if (entityActor.IsValid() == false)return;

	// ????????????
	CvTimelineUIInfo info;
	GetEditTransformData(info);
	memcpy(&oldUIinfo, &info, sizeof(CvTimelineUIInfo));

	// ??????3
	auto nowWorldLocation = entityActor->GetNowTransform().GetLocation();
	auto newWorldLocation = UCalculationObject::RelativeToWorldTransform(entityActor->GetObserverTransform(), info.location);
	auto nowWorldRotation = entityActor->GetNowTransform().GetRotation();
	auto newWorldRotation = UCalculationObject::RelativeToWorldTransform(entityActor->GetObserverTransform(), info.rotator);
	bool isSave = ((newWorldLocation - nowWorldLocation).Size() > 0.1
		|| nowWorldRotation.Equals(newWorldRotation.Quaternion(), 0.00001) == false);

	UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [UICall_ChangeTransformData] ???????????? [location:%s] [rotator:%s] [color:%s]"),
		this, *info.location.ToString(), *info.rotator.ToString(), *info.color.ToString());

	// ????????????
	selectedView->SetActorLocation(newWorldLocation);
	entityActor->editComponent->EntityLocalMoveMain(ESelectedWorkMode::WorldTransform, newWorldLocation,
		newWorldRotation, FVector::ZeroVector);
	entityActor->ColorChange(info.color);

	// ???????????????
	Call_EntityChangeTransform(entityId, uniqueMark, info);

	// ????????????
	if (justSync)
	{
		SaveChangeDate();
	}
}

// UI?????? ????????????1 
void UTimelinePanelUserWidget::Call_EntityChangeTransform(int entityId, int uniqueMark, CvTimelineUIInfo& info)
{
	auto syncInfo = GetSyncInfo(entityId);
	if (syncInfo == nullptr)return;

	FTimelinePointIndex pointIndex;
	pointIndex.entityId = entityId;
	pointIndex.uniqueMark = uniqueMark;

	FTimelineEnterPointInfo* timelinePointInfo = GetTimelinePointInfoWithUniqueMark(pointIndex.entityId,
		pointIndex.uniqueMark);
	if (timelinePointInfo == nullptr)return;

	auto entityAcotr = GetEntityAcotr(entityId);

	entityAcotr->myTimelineComponent->UpdateEntityDataTransform(timelinePointInfo, entityId, pointIndex, info);


	cuoi.saveDataCount++;
}

// UI?????? ????????????2 ????????????
void UTimelinePanelUserWidget::UICall_ChangeTimeData(float time)
{

	FTimelinePointIndex movePointInfo;
	if (IsTimeAxisStopInTimePoint(movePointInfo) == false)
	{
		return;
	}
	auto timelinePointInfo = GetTimelinePointInfoWithUniqueMark(movePointInfo.entityId, movePointInfo.uniqueMark);
	if (timelinePointInfo == nullptr)return;

	// ???????????????????????????????????????
	auto newX = GetXWithTime(time);
	auto moveX = newX - timelinePointInfo->x;
	auto move = FVector2D(moveX, 0);

	// ????????????
	auto entityActor = GetEntityAcotr(movePointInfo.entityId);
	if (entityActor.IsValid())
	{
		int newTime = time * 1000;
		entityActor->myTimelineComponent->MoveEntityData(timelinePointInfo, newTime, move);
	}

	UpdateEntityDataToUIData(movePointInfo.entityId, ETimelineOperationType::TimelineOperation_UpdateEntityDataToUIData_update);

	ViewShowPoint();// ??????point???

	cuoi.saveDataCount++;

	TimeAxisMove(timelinePointInfo->x);// ????????????????????????????????????

}

// UI?????? ????????????3 ????????????
void UTimelinePanelUserWidget::OnEnterColor(FVector color)
{
	SetTextValue(color);
	UICall_SaveData(false);
}

// UI?????? ???????????? ????????? (??????)
void UTimelinePanelUserWidget::UICall_EnterButton_PreviousFrame()
{

	if (entitySortList.Num() == 0)return;

	int entityId = entitySortList[0];
	int keyTime = GetTimeWithX(ctli.timeAxisX) * 1000;

	auto entityActor = GetEntityAcotr(entityId);
	if (entityActor == nullptr)
	{
		return;
	}

	// ?????????
	FTimelineEnterPointInfo* ret = nullptr;
	if (entityLineInfoMap.Find(entityId) == nullptr)
	{
		return;
	}
	auto& pointInfoList = entityLineInfoMap[entityId].pointInfoList;
	for (int i = pointInfoList.Num() - 1; i >= 0; i--)
	{
		auto& node = pointInfoList[i];
		if (abs(node.keyTime - keyTime) > 100 && node.keyTime < keyTime)
		{
			ret = &node;
			break;
		}
	}

	if (ret)
	{
		TimeAxisMove(ret->x);
	}
	else
	{
		auto syncInfo = &entityActor->syncInfo;
		FTransform newWorldTransform = UCommonlyObject::PBToUe4Transform(syncInfo->mutable_transform());
		FTransform relativeTransform = newWorldTransform.GetRelativeTransform(entityActor->GetObserverTransform());

		if (pointInfoList.Num() == 0)
		{
			EntityAddPoint(entityId, 0, relativeTransform, FVector(0, 0, 0));
		}
		else
		{
			auto& node = pointInfoList[0];

			float newTime = node.keyTime / 1000.0f;
			newTime = newTime - 0.5;
			EntityAddPoint(entityId, newTime, relativeTransform, FVector(0, 0, 0));
		}
	}
}

// UI?????? ???????????? ?????????
void UTimelinePanelUserWidget::UICall_EnterButton_NextFrame()
{

	if (entitySortList.Num() == 0)return;

	int entityId = entitySortList[0];
	int keyTime = GetTimeWithX(ctli.timeAxisX) * 1000;

	auto entityActor = GetEntityAcotr(entityId);
	if (entityActor == nullptr)
	{
		return;
	}

	//1. ???????????????????????????
	{
		FTimelineEnterPointInfo* ret = nullptr;
		if (entityLineInfoMap.Find(entityId) == nullptr)
		{
			return;
		}
		TArray < FTimelineEnterPointInfo>& pointInfoList = entityLineInfoMap[entityId].pointInfoList;
		for (int i = pointInfoList.Num() - 1; i >= 0; i--)
		{
			FTimelineEnterPointInfo& node = pointInfoList[i];
			if (abs(node.keyTime - keyTime) < 100)
			{
				ret = &node;
				break;
			}
		}
		if (ret)
		{
			UHUDStaticFunc::PushMessage(this, TEXT("??????????????????"));
			UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_??????8);
			return;
		}
	}

	//2. ???????????????
	{
		auto syncInfo = &entityActor->syncInfo;
		FTransform newWorldTransform = UCommonlyObject::PBToUe4Transform(syncInfo->mutable_transform());
		FTransform relativeTransform = newWorldTransform.GetRelativeTransform(entityActor->GetObserverTransform());

		EntityAddPoint(entityId, keyTime / 1000.0f, relativeTransform, FVector(0, 0, 0));
	}

	UHUDStaticFunc::PushMessage(this, TEXT("???????????????"));
	UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_ui_???);
}

// UI?????? ???????????? ?????????
void UTimelinePanelUserWidget::UICall_EnterButton_Delete()
{

	FTimelinePointIndex movePointInfo;
	if (IsTimeAxisStopInTimePoint(movePointInfo) == false)
	{
		return;
	}

	// ?????????????????????????????????
	std::vector<FTimelinePointIndex> selectPointList;
	FTimelinePointIndex timelinePointIndex;
	timelinePointIndex.entityId = movePointInfo.entityId;
	timelinePointIndex.uniqueMark = movePointInfo.uniqueMark;
	selectPointList.push_back(timelinePointIndex);

	PointDelete(selectPointList);	// ????????????

	UHUDStaticFunc::PushMessage(this, TEXT("???????????????"));
	UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_ui_???);
}

// UI?????? ????????????
void UTimelinePanelUserWidget::MySelectionChanged(FString  SelectedItem, ESelectInfo::Type  SelectionType)
{


	bool isEdit = false;
#if WITH_EDITORONLY_DATA
	isEdit = true;
#endif

	UE_LOG(LogTemp, Log, TEXT("[%x] [?????????] [MySelectionChanged:%s] ???????????? [isEdit:%d] --"), this, *SelectedItem, isEdit);

	TextBlock_highlight_str->SetText(FText::FromString(SelectedItem));

	UICall_SaveData(true);


}
/*------------------------------------------------------------------*/

// ?????? ??????
FReply UTimelinePanelUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::NativeOnMouseButtonDown] ?????? A [time:%f]"), this,
	//        GetWorld()->TimeSeconds);

	if (UKismetInputLibrary::PointerEvent_IsMouseButtonDown(InMouseEvent, EKeys::RightMouseButton))
	{
		Event_RightMouseButton(true);
	}
	else if (UKismetInputLibrary::PointerEvent_IsMouseButtonDown(InMouseEvent, EKeys::LeftMouseButton))
	{
		Event_LeftMouseButton(true);
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

// ?????? ??????
FReply UTimelinePanelUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (cuoi.rightMouseButtonEnter && UKismetInputLibrary::PointerEvent_IsMouseButtonDown(
		InMouseEvent, EKeys::RightMouseButton) == false)
	{
		Event_RightMouseButton(false);
	}
	else if (cuoi.leftMouseButtonEnter && UKismetInputLibrary::PointerEvent_IsMouseButtonDown(
		InMouseEvent, EKeys::LeftMouseButton) == false)
	{
		Event_LeftMouseButton(false);
	}
	// UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::NativeOnMouseButtonUp] ?????? B [time:%f]"), this,
	//        GetWorld()->TimeSeconds);

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

// ?????? ????????????
FReply UTimelinePanelUserWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	if (CanEntityAddPoint())
	{
		int entityId;
		FVector2D offsetPoint;
		if (GetMouseEntityId(entityId, offsetPoint))
		{
			if (IsSelectEntity(entityId))
			{
				auto entityActor = GetEntityAcotr(entityId);
				if (entityActor.IsValid())
				{
					auto syncInfo = &entityActor->syncInfo;
					FTransform newWorldTransform = UCommonlyObject::PBToUe4Transform(syncInfo->mutable_transform());
					FTransform relativeTransform = newWorldTransform.GetRelativeTransform(entityActor->GetObserverTransform());

					float time = GetTimeWithX(offsetPoint.X);
					EntityAddPoint(entityId, time, relativeTransform, FVector(0, 0, 0));
				}
			}
		}
	}

	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

// ?????? ??????
FReply UTimelinePanelUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnMouseMove(true, InMouseEvent);

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

// ?????? NativeOnMouseMove()->??????
void UTimelinePanelUserWidget::OnMouseMove(bool isPanel, const FPointerEvent& InMouseEvent)
{
	FGeometry gemotry = GetPaintSpaceGeometry();

	OnMousePointEnterLeave();

	// ??????????????????
	if (cuoi.rightMouseButtonEnter && UKismetInputLibrary::PointerEvent_IsMouseButtonDown(
		InMouseEvent, EKeys::RightMouseButton))
	{
		FVector2D mousePosion;
		auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		playerController->GetMousePosition(mousePosion.X, mousePosion.Y);
		if ((cuoi.absolutePosition - mousePosion).Size() > 1)
		{
			auto move2 = cuoi.absolutePosition - mousePosion;
			cuoi.absolutePosition = mousePosion;

			auto movePosition = (cuoi.mouseButtonEnterPosion - mousePosion) / gemotry.Scale;

			auto move3 = ctli.oldViewLeftPoint + movePosition;
			ViewMove(move3);
		}
	}
	// ??????????????????
	else if (cuoi.leftMouseButtonEnter && UKismetInputLibrary::PointerEvent_IsMouseButtonDown(
		InMouseEvent, EKeys::LeftMouseButton))
	{
		// UE_LOG(LogTemp, Log, TEXT("[%x] [OnMouseMove] ???????????? [isPanel:%d] [time:%f]"), this,
		// 	isPanel,GetWorld()->TimeSeconds);

		FVector2D mousePosion;
		auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		playerController->GetMousePosition(mousePosion.X, mousePosion.Y);
		if ((cuoi.absolutePosition - mousePosion).Size() > 3)
		{
			auto move2 = cuoi.absolutePosition - mousePosion;
			cuoi.absolutePosition = mousePosion;

			if (cuoi.mouseLeftMode == CvMouseLeftMode::CvMouseLeftMode_PointSelect)
			{
				BoxSelection();
			}
			else if (cuoi.mouseLeftMode == CvMouseLeftMode::CvMouseLeftMode_PointMove)
			{
				auto movePosition = (cuoi.mouseButtonEnterPosion - mousePosion) / gemotry.Scale * -1;

				EntityMovePointProcess(movePosition);
			}
			else if (cuoi.mouseLeftMode == CvMouseLeftMode::CvMouseLeftMode_TimeAxis)
			{
				auto movePosition = (cuoi.mouseButtonEnterPosion - mousePosion) / gemotry.Scale * -1;
				auto move = ctli.oldTimeAxisX + movePosition.X;
				TimeAxisMove(move);
			}
		}
	}
}

// ?????? OnMouseMove()->??????????????????
void UTimelinePanelUserWidget::OnMousePointEnterLeave()
{
	if (cuoi.leftMouseButtonEnter)
	{
		return;
	}

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController == nullptr)return;

	FVector2D mousePosion;
	playerController->GetMousePosition(mousePosion.X, mousePosion.Y);

	// ????????????????????????
	{
		FVector2D mousePosion2 = GetMouseViewPoint(mousePosion);

		FVector2D move = GetOverlayPointMargin();

		mousePosion2 -= move;
		OnMouseEnterLeaveEntityPoint(mousePosion2);
	}

	// ??????????????????????????????
	{
		FVector2D mousePosion2 = GetMouseViewPoint(mousePosion);
		FVector2D move = GetOverlayTimeAxisMargin();
		mousePosion2 -= move;
		OnMouseEnterLeaveTimeAxis(mousePosion2);
	}
}

// ?????? OnMousePointEnterLeave()->????????????????????? ??????????????????????????????
void UTimelinePanelUserWidget::OnMouseEnterLeaveEntityPoint(FVector2D mousePosion)
{
	if (cuoi.mouseLeftMode == CvMouseLeftMode::CvMouseLeftMode_TimeAxis)
	{
		return;
	}

	bool flag = false;
	FTimelinePointIndex nowPointIndexInfo;
	if (showPointMap.size() > 0)
	{
		for (auto it : showPointMap)
		{
			auto key = it.first;
			auto& value = it.second;

			FTimelineEnterPointInfo* pointInfo = GetTimelinePointInfoWithUniqueMark(value.entityId, value.uniqueMark);
			if (pointInfo)
			{
				UTimelinePointUserWidget* timelinePointUserWidget = Cast<UTimelinePointUserWidget>(
					pointInfo->pointUserWidget);
				if (timelinePointUserWidget)
				{
					auto setX = pointInfo->x - ctli.viewLeftPoint.X;
					auto setY = pointInfo->y - ctli.viewLeftPoint.Y;

					FVector2D pointLocation(setX, setY);

					// 5??????????????????point???ui????????????
					if (pointLocation.X - 5 <= mousePosion.X && mousePosion.X <= pointLocation.X + ctli.pointSize.X - 5)
					{
						if (pointLocation.Y <= mousePosion.Y && mousePosion.Y <= pointLocation.Y + ctli.pointSize.Y)
						{
							nowPointIndexInfo.entityId = timelinePointUserWidget->pointIndexInfo.entityId;
							nowPointIndexInfo.uniqueMark = timelinePointUserWidget
								->pointIndexInfo.uniqueMark;

							flag = true;
							break;
						}
					}
				}
			}
		}
	}

	if (flag)
	{
		// ????????????
		if (cuoi.mouseRealTimeMode == CvMouseLeftMode::CvMouseLeftMode_PointMove
			&& cuoi.stopInPoint_pointIndex.entityId == nowPointIndexInfo.entityId
			&& cuoi.stopInPoint_pointIndex.uniqueMark == nowPointIndexInfo.uniqueMark
			)
		{
		}
		// ??????
		else
		{
			// ???????????????
			if (cuoi.mouseRealTimeMode == CvMouseLeftMode::CvMouseLeftMode_PointMove)
			{
				MouseEnterLeaveFalse(cuoi.stopInPoint_pointIndex, false);
			}

			// ????????????
			{
				MouseEnterLeaveFalse(nowPointIndexInfo, true);
			}

			cuoi.mouseRealTimeMode = CvMouseLeftMode::CvMouseLeftMode_PointMove;
			cuoi.stopInPoint_pointIndex.entityId = nowPointIndexInfo.entityId;
			cuoi.stopInPoint_pointIndex.uniqueMark = nowPointIndexInfo.uniqueMark;

			// UE_LOG(LogTemp, Log, TEXT("[%x] [SetSelectEntityPoint] ??????"), this);
		}
	}
	else
	{
		if (cuoi.mouseRealTimeMode == CvMouseLeftMode::CvMouseLeftMode_PointMove)
		{
			MouseEnterLeaveFalse(cuoi.stopInPoint_pointIndex, false);

			cuoi.mouseRealTimeMode = CvMouseLeftMode::CvMouseLeftMode_None;
			cuoi.stopInPoint_pointIndex.entityId = 0;
			cuoi.stopInPoint_pointIndex.uniqueMark = 0;

			// UE_LOG(LogTemp, Log, TEXT("[%x] [SetSelectEntityPoint] ??????"), this);
		}
	}
}

// ?????? OnMouseEnterLeaveEntityPoint()->?????????????????????
void UTimelinePanelUserWidget::MouseEnterLeaveFalse(FTimelinePointIndex& nowPointIndexInfo, bool flag)
{
	FTimelineEnterPointInfo* pointInfo = GetTimelinePointInfoWithUniqueMark(nowPointIndexInfo.entityId,
		nowPointIndexInfo.uniqueMark);
	if (pointInfo)
	{
		UTimelinePointUserWidget* timelinePointUserWidget = Cast<UTimelinePointUserWidget>(
			pointInfo->pointUserWidget);
		if (timelinePointUserWidget)
		{
			timelinePointUserWidget->OnMouseEnterLeave(flag);
		}
	}
}

// ?????? OnMousePointEnterLeave()->???????????????????????????
void UTimelinePanelUserWidget::OnMouseEnterLeaveTimeAxis(FVector2D mousePosion)
{
	// return;

	if (cuoi.mouseLeftMode == CvMouseLeftMode::CvMouseLeftMode_PointMove)
	{
		return;
	}
	auto realX = BP_UI_Timeline_timeAxis->RenderTransform.Translation.X;

	bool flag = false;

	if (realX - 20 <= mousePosion.X && mousePosion.X < realX + 20)
	{
		// ??????
		if (0 < mousePosion.Y && mousePosion.Y < ctli.timeAxis_top)
		{
			cuoi.mouseRealTimeMode = CvMouseLeftMode::CvMouseLeftMode_TimeAxis;
			flag = true;
		}
		// ??????
		else if (ctli.timeAxis_down - ctli.timeAxis_top < mousePosion.Y && mousePosion.Y < ctli.timeAxis_down)
		{
			cuoi.mouseRealTimeMode = CvMouseLeftMode::CvMouseLeftMode_TimeAxis;
			flag = true;
		}
	}

	Cast<UTimelineTimeAxisUserWidget>(BP_UI_Timeline_timeAxis)->OnMouseEnterLeave(flag);

	if (flag == false && cuoi.mouseRealTimeMode == CvMouseLeftMode::CvMouseLeftMode_TimeAxis)
	{
		cuoi.mouseRealTimeMode = CvMouseLeftMode::CvMouseLeftMode_None;
	}

	// UE_LOG(LogTemp, Log, TEXT("[%x] [OnMouseEnterLeaveTimeAxis] [%f  %f]  "), this, mousePosion.X, mousePosion.Y);
}

// ?????? ?????? ??????????????????????????????
void UTimelinePanelUserWidget::BoxSelection()
{
	FVector2D start = GetMouseViewPoint(cuoi.absolutePosition);
	FVector2D end = GetMouseViewPoint(cuoi.mouseButtonEnterPosion);

	if (cuoi.mouseLeftMode != CvMouseLeftMode::CvMouseLeftMode_PointSelect)
	{
		return;
	}

	FVector2D move = GetOverlayPointMargin();

	start -= move;
	end -= move;

	// UE_LOG(LogTemp, Log, TEXT("[%x] [SetSelectEntity] [start:%s] [end:%s]"), this, *start.ToString(), *end.ToString());

	int count = 0;
	for (auto it : showPointMap)
	{
		auto key = it.first;
		auto& value = it.second;

		FTimelineEnterPointInfo* pointInfo = GetTimelinePointInfoWithUniqueMark(value.entityId, value.uniqueMark);
		if (pointInfo)
		{
			UTimelinePointUserWidget* timelinePointUserWidget = Cast<UTimelinePointUserWidget>(
				pointInfo->pointUserWidget);
			if (timelinePointUserWidget)
			{
				bool flag = false;

				auto setX = pointInfo->x - ctli.viewLeftPoint.X;
				auto setY = pointInfo->y - ctli.viewLeftPoint.Y;

				FVector2D pointLocation(setX, setY);

				if ((start.X <= pointLocation.X && pointLocation.X <= end.X)
					|| (start.X >= pointLocation.X && pointLocation.X >= end.X))
				{
					if ((start.Y <= pointLocation.Y && pointLocation.Y <= end.Y)
						|| (start.Y >= pointLocation.Y && pointLocation.Y >= end.Y))
					{
						timelinePointUserWidget->SetSelect(true);
						pointInfo->isSelect = true;
						flag = true;
						count++;
					}
				}

				if (flag == false)
				{
					timelinePointUserWidget->SetSelect(false);
					pointInfo->isSelect = false;
				}
			}
		}
	}

	//??????
	count = count + 1 - 1;
}

// ?????? ????????????UI??????
void UTimelinePanelUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	cuoi.mouseInSpace = true;
	UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::NativeOnMouseEnter] ?????? A [time:%f]"), this,
		GetWorld()->TimeSeconds);

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(playerController);

	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	SetKeyboardFocus();	// ????????????,????????????????????????????????????
}

// ?????? ????????????UI??????
void UTimelinePanelUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	cuoi.mouseInSpace = false;
	UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::NativeOnMouseEnter] ?????? B [time:%f]"), this,
		GetWorld()->TimeSeconds);

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	Super::NativeOnMouseLeave(InMouseEvent);
	// ??????SetInputMode_GameAndUI????????????????????????move??????

	playerController->bShowMouseCursor = true;// UI??????????????????
	EMouseLockMode InMouseLockMode = EMouseLockMode::DoNotLock;
	bool bHideCursorDuringCapture = false;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(InMouseLockMode);
	InputMode.SetHideCursorDuringCapture(bHideCursorDuringCapture);
	playerController->SetInputMode(InputMode);
	//UWidgetBlueprintLibrary::SetInputMode_GameAndUI(playerController);
	UWidgetBlueprintLibrary::SetFocusToGameViewport();
}

// ?????? ??????????????????????????????(??????game???ui????????????,????????????????????????)
void UTimelinePanelUserWidget::Event_RightMouseButton(bool isEnter)
{
	if (isEnter)
	{
		if (cuoi.mouseInSpace)
		{
			// ??????

			// UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::Event_RightMouseButton] ?????? A [time:%f]"), this,
			// 	GetWorld()->TimeSeconds);
			cuoi.rightMouseButtonEnter = true;

			auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			playerController->GetMousePosition(cuoi.mouseButtonEnterPosion.X, cuoi.mouseButtonEnterPosion.Y);
			cuoi.absolutePosition = cuoi.mouseButtonEnterPosion;

			ctli.oldViewLeftPoint = ctli.viewLeftPoint;
		}
	}
	else
	{
		// ??????
		// UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::Event_RightMouseButton] ?????? B [time:%f]"), this,
		// 	GetWorld()->TimeSeconds);
		cuoi.rightMouseButtonEnter = false;
	}
}

// ?????? ??????????????????????????????(??????game???ui????????????,????????????????????????)
void UTimelinePanelUserWidget::Event_LeftMouseButton(bool isEnter)
{
	if (isEnter)
	{
		if (cuoi.mouseInSpace)
		{
			// ??????
			// UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::Event_LeftMouseButton] ?????? A [time:%f] "), this,
			// 	GetWorld()->TimeSeconds);

			cuoi.leftMouseButtonEnter = true;
			auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			playerController->GetMousePosition(cuoi.mouseButtonEnterPosion.X, cuoi.mouseButtonEnterPosion.Y);
			cuoi.absolutePosition = cuoi.mouseButtonEnterPosion;

			if (cuoi.mouseRealTimeMode == CvMouseLeftMode::CvMouseLeftMode_PointMove)
			{
				cuoi.mouseLeftMode = CvMouseLeftMode::CvMouseLeftMode_PointMove;

				FTimelinePointIndex movePointInfo;
				if (IsTimeAxisStopInTimePoint(movePointInfo))
				{
					auto timelinePointInfo = GetTimelinePointInfoWithUniqueMark(movePointInfo.entityId, movePointInfo.uniqueMark);
					TimeAxisMove(timelinePointInfo->x);
				}
			}
			else if (cuoi.mouseRealTimeMode == CvMouseLeftMode::CvMouseLeftMode_TimeAxis)
			{
				cuoi.mouseLeftMode = CvMouseLeftMode::CvMouseLeftMode_TimeAxis;
				ctli.oldTimeAxisX = ctli.timeAxisX;
			}
			else
			{
				cuoi.mouseLeftMode = CvMouseLeftMode::CvMouseLeftMode_PointSelect;

				ctli.oldViewLeftPoint = ctli.viewLeftPoint;

				BoxSelection();

				//
			}
		}
	}
	// ??????
	else
	{
		// UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::Event_LeftMouseButton] ?????? B [time:%f]"), this,
		// 	GetWorld()->TimeSeconds);
		if (cuoi.mouseLeftMode == CvMouseLeftMode::CvMouseLeftMode_PointMove)
		{
			FGeometry gemotry = GetPaintSpaceGeometry();
			FVector2D mousePosion;
			auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			playerController->GetMousePosition(mousePosion.X, mousePosion.Y);
			auto movePosition = (cuoi.mouseButtonEnterPosion - mousePosion) / gemotry.Scale * -1;

			// ??????????????????
			if (movePosition.Size() > 1)
			{
				EntityMovePointEnd(movePosition);
			}
			// ????????????????????????????????????
			else
			{
			}
		}
		else if (cuoi.mouseLeftMode == CvMouseLeftMode::CvMouseLeftMode_PointSelect)
		{
		}
		else if (cuoi.mouseLeftMode == CvMouseLeftMode::CvMouseLeftMode_TimeAxis)
		{
		}
		cuoi.leftMouseButtonEnter = false;
		cuoi.mouseLeftMode = CvMouseLeftMode::CvMouseLeftMode_None;
	}
}

/*------------------------------------------------------------------*/

// ?????? ??????
FReply UTimelinePanelUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::NativeOnKeyDown] ?????? ?????? [key:%s]"), this,
	//	*InKeyEvent.GetKey().ToString());
	//if (InKeyEvent.GetKey() == EKeys::BackSpace) {
	//}
	return FReply::Handled();
}

// ?????? ??????
FReply UTimelinePanelUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTimelinePanelUserWidget::NativeOnKeyUp] ?????? ?????? [key:%s]"), this,
	//	*InKeyEvent.GetKey().ToString());
	if (InKeyEvent.GetKey() == EKeys::BackSpace)
	{
		Event_DeletePoint();
	}
	return FReply::Handled();
}

// ?????? ?????????
void UTimelinePanelUserWidget::Event_DeletePoint()
{
	// ????????????
	std::vector<FTimelinePointIndex> selectPointList;
	std::vector<int> selectEntityId;
	GetSelectPointList(selectPointList, selectEntityId);
	if (selectPointList.size() == 0)return;

	PointDelete(selectPointList);	// ????????????

}

// ?????? ??? ????????? ????????????
void UTimelinePanelUserWidget::PointDelete(std::vector<FTimelinePointIndex>& selectPointList)
{
	if (selectPointList.size() == 0)return;

	int entityId = selectPointList[0].entityId;

	// ????????????
	auto entityActor = GetEntityAcotr(entityId);
	if (entityActor.IsValid())
	{
		for (auto node : selectPointList)
		{
			FTimelineEnterPointInfo* timelinePointInfo = GetTimelinePointInfoWithUniqueMark(entityId, node.uniqueMark);
			if (timelinePointInfo)
			{
				entityActor->myTimelineComponent->PointDeleteData(timelinePointInfo, node);
			}
		}
	}


	UpdateEntityDataToUIData(entityId, ETimelineOperationType::TimelineOperation_UpdateEntityDataToUIData_remove); // ????????????

	ViewShowPoint();// ??????point???

	// ??????????????????
	auto& transform_timeline = entityActor->syncInfo.transform_timeline();
	if (transform_timeline.point_map_size() == 0)
	{
		entityActor->myTimelineComponent->MoveTimelineUI(true, 0);
	}

	cuoi.saveDataCount++;
}

/*-------------------------------------------------------------*/

// ??? ?????????
void UTimelinePanelUserWidget::EntityAddPoint(int entityId, float time, FTransform relativeTransform, FVector color)
{
	auto entityActor = GetEntityAcotr(entityId);
	if (entityActor == nullptr)
	{
		return;
	}
	entityActor->myTimelineComponent->EntityAddPointData(entityId, time, ETimelinePointType::TimelinePointType_location, relativeTransform.GetLocation(), relativeTransform.GetRotation().Rotator(), color);// ????????????

	UpdateEntityDataToUIData(entityId, ETimelineOperationType::TimelineOperation_UpdateEntityDataToUIData_create);// ????????????

	ViewShowPoint();// ??????point???

	// ???????????????????????????
	{
		int keyTime2 = time * 1000;
		auto timelinePointInfo = GetTimelinePointInfoWithKeyTime(entityId, keyTime2);
		if (timelinePointInfo)
		{
			auto setX = timelinePointInfo->x;
			TimeAxisMove(setX);
		}
	}

	cuoi.saveDataCount++;
}

// ??? ?????????(????????????)
void UTimelinePanelUserWidget::EntityMovePointProcess(FVector2D move, bool mast)
{
	if (cuoi.mouseRealTimeMode != CvMouseLeftMode::CvMouseLeftMode_PointMove && mast == false)return;

	move.Y = 0;

	// ????????????
	std::vector<FTimelinePointIndex> selectPointList;
	std::vector<int> selectEntityId;
	if (GetSelectPointList(selectPointList, selectEntityId) == false)
	{
		selectPointList.clear();
		selectEntityId.clear();
		selectPointList.push_back(cuoi.stopInPoint_pointIndex);
		selectEntityId.push_back(cuoi.stopInPoint_pointIndex.entityId);
	}

	for (auto node : selectPointList)
	{
		FTimelineEnterPointInfo* timelinePointInfo = GetTimelinePointInfoWithUniqueMark(node.entityId, node.uniqueMark);
		if (timelinePointInfo)
		{
			PointMove(*timelinePointInfo, move);
		}
	}

	// ???????????????????????????(??????)
	FTimelinePointIndex movePointInfo;
	if (IsTimeAxisStopInTimePoint(movePointInfo))
	{
		auto timelinePointInfo = GetTimelinePointInfoWithUniqueMark(movePointInfo.entityId, movePointInfo.uniqueMark);
		if (timelinePointInfo)
		{
			auto setX = timelinePointInfo->x + move.X;
			TimeAxisMove(setX);
		}
	}
}

// ??? ?????????(????????????)
void UTimelinePanelUserWidget::EntityMovePointEnd(FVector2D move)
{
	if (cuoi.mouseRealTimeMode != CvMouseLeftMode::CvMouseLeftMode_PointMove)return;

	move.Y = 0;

	// ????????????
	std::vector<FTimelinePointIndex> selectPointList;
	std::vector<int> selectEntityId;
	if (GetSelectPointList(selectPointList, selectEntityId) == false)
	{
		selectPointList.clear();
		selectEntityId.clear();
		selectPointList.push_back(cuoi.stopInPoint_pointIndex);
		selectEntityId.push_back(cuoi.stopInPoint_pointIndex.entityId);
	}

	// ????????????
	for (auto node : selectPointList)
	{
		FTimelineEnterPointInfo* timelinePointInfo = GetTimelinePointInfoWithUniqueMark(node.entityId, node.uniqueMark);
		if (timelinePointInfo)
		{
			auto entityActor = GetEntityAcotr(node.entityId);
			if (entityActor.IsValid())
			{
				int newTime = GetTimeWithX(timelinePointInfo->x + move.X) * 1000;
				entityActor->myTimelineComponent->MoveEntityData(timelinePointInfo, newTime, move);
			}
		}
	}

	// ????????????
	for (auto entityId : selectEntityId)
	{
		UpdateEntityDataToUIData(entityId, ETimelineOperationType::TimelineOperation_UpdateEntityDataToUIData_update);
	}

	ViewShowPoint();// ??????point???

	cuoi.saveDataCount++;
}

// ??? ???????????????syncinfo?????????UI????????? FTimelineEnterPointInfo
void UTimelinePanelUserWidget::UpdateEntityDataToUIData(int entityId, ETimelineOperationType type)
{
	Gamedata::EntityInfo* syncInfo = GetSyncInfo(entityId);
	if (syncInfo == nullptr)
	{
		return;
	}

	if (entityLineInfoMap.Find(entityId) == nullptr)
	{
		return;
	}

	FTimelineEntityLineInfo* entityLineInfo = &entityLineInfoMap[entityId];

	// ??????????????????????????????
	auto& transform_timeline = syncInfo->transform_timeline();
	if (transform_timeline.point_map_size() > 0)
	{
		// ?????????????????????
		auto& point_map = transform_timeline.point_map();
		std::vector<int> timelimeKeyList;
		for (auto node : point_map)
		{
			auto key = node.first;
			timelimeKeyList.push_back(key);
		}
		if (timelimeKeyList.size() > 1)
		{
			std::sort(timelimeKeyList.begin(), timelimeKeyList.end());
		}
		entityLineInfo->start = timelimeKeyList[0] / 1000.0f;
		entityLineInfo->end = timelimeKeyList[timelimeKeyList.size() - 1] / 1000.0f;

		// ???????????????
		std::vector<FTimelinePointIndex> selectPointList;
		std::vector<int> selectEntityId;
		bool selectStopInPoint = false;
		if (ETimelineOperationType::TimelineOperation_UpdateEntityDataToUIData_update == type)
		{
			if (GetSelectPointList(selectPointList, selectEntityId) == false)
			{
				selectPointList.clear();
				selectEntityId.clear();
				selectPointList.push_back(cuoi.stopInPoint_pointIndex);
				selectEntityId.push_back(cuoi.stopInPoint_pointIndex.entityId);
			}
			else
			{
				selectStopInPoint = true;
			}
		}

		// ????????????
		{
			ClearAllPoint(entityLineInfo);

			// ?????????????????????
			for (int i = 0; i < timelimeKeyList.size(); i++)
			{
				int keyTime = timelimeKeyList[i];
				float showTime = keyTime / 1000.0f;

				auto pointIndo = &point_map.find(keyTime)->second;
				const Gamedata::FVector* location = &pointIndo->location();
				auto location2 = UCommonlyObject::PBToUe4Vector(location);
				auto uniqueMark = pointIndo->uniquemark();

				//UE_LOG(LogTemp, Log, TEXT("[????????????] [UpdateEntityDataToUIData] [entityId:%d] [keyTime:%d] [uniqueMark:%d]"), entityId, keyTime, uniqueMark);
				FTimelineEnterPointInfo pointInfo;
				pointInfo.entityId = entityId;
				pointInfo.showTime = showTime;
				pointInfo.keyTime = keyTime;
				pointInfo.x = showTime * ctli.oneSecondLenght;
				pointInfo.y = entityLineInfo->y + 45;
				pointInfo.uniqueMark = uniqueMark;
				entityLineInfo->pointInfoList.Add(pointInfo);
			}
			//UE_LOG(LogTemp, Log, TEXT("[????????????] [UpdateEntityDataToUIData] [entityId:%d] [size:%d] [size:%d]"), entityId, point_map.size(), timelimeKeyList.size());
		}

		// ???????????????
		if (ETimelineOperationType::TimelineOperation_UpdateEntityDataToUIData_update == type
			&& selectStopInPoint)
		{
			for (int i = 0; i < timelimeKeyList.size(); i++)
			{
				int keyTime = timelimeKeyList[i];
				auto pointIndo = &point_map.find(keyTime)->second;

				FTimelinePointIndex pointIndex(entityId, pointIndo->uniquemark());
				if (IsSelectPoint(selectPointList, selectEntityId, pointIndex))
				{
					entityLineInfo->pointInfoList[i].isSelect = true;
					SetShowPointMap(true, pointIndex);
				}
			}
		}
	}
	// ??????????????????
	else
	{
		ClearAllPoint(entityLineInfo);
		// ?????????edit???????????????

	}
}

// ??? ??????????????????
void UTimelinePanelUserWidget::ClearAllPoint(FTimelineEntityLineInfo* entityLineInfo)
{

	// ?????????????????????
	for (int i = 0; i < entityLineInfo->pointInfoList.Num(); i++)
	{
		auto pointInfo = &entityLineInfo->pointInfoList[i];
		FTimelinePointIndex timelinePointInfoIndex(pointInfo->entityId, pointInfo->uniqueMark);
		SetShowPointMap(false, timelinePointInfoIndex);
		PointRemove(*pointInfo);
	}

	// ?????????????????????
	entityLineInfo->pointInfoList.Reset();
}

// ??? ??????????????????
void UTimelinePanelUserWidget::SetShowPointMap(bool isAdd, FTimelinePointIndex& timelineShowPointInfo)
{
	if (isAdd)
	{
		std::string mapKey = UCommonlyObject::IntToString(timelineShowPointInfo.entityId) + "_" + UCommonlyObject::IntToString(timelineShowPointInfo.uniqueMark);
		showPointMap.emplace(mapKey, timelineShowPointInfo);
	}
	else
	{
		std::string mapKey = UCommonlyObject::IntToString(timelineShowPointInfo.entityId) + "_" + UCommonlyObject::IntToString(timelineShowPointInfo.uniqueMark);
		auto itFind = showPointMap.find(mapKey);
		if (itFind != showPointMap.end())
		{
			showPointMap.erase(mapKey);
		}
	}
}

// ??? ??????
bool UTimelinePanelUserWidget::PointCreate(FTimelineEnterPointInfo& pointInfo, FTimelinePointIndex& pointIndex)
{
	if (pointInfo.pointUserWidget.IsValid() == false)
	{
		UTimelinePointUserWidget* bp_UI_Timeline_point = Cast<UTimelinePointUserWidget>(
			CreateWidget(GetWorld(), class_BP_UI_Timeline_point_C));
		bp_UI_Timeline_point->panel = this;
		bp_UI_Timeline_point->pointIndexInfo.entityId = pointIndex.entityId;
		bp_UI_Timeline_point->pointIndexInfo.uniqueMark = pointIndex.uniqueMark;

		overlayPoint->AddChildToOverlay(bp_UI_Timeline_point);

		// ????????????
		bp_UI_Timeline_point->SetSelect(pointInfo.isSelect);

		pointInfo.pointUserWidget = bp_UI_Timeline_point;

		return true;
	}
	return false;
}

// ??? ??????
void UTimelinePanelUserWidget::PointRemove(FTimelineEnterPointInfo& pointInfo)
{
	if (pointInfo.pointUserWidget.IsValid())
	{
		pointInfo.pointUserWidget->RemoveFromParent();
		pointInfo.pointUserWidget = nullptr;
	}
}

// ??? ??????
void UTimelinePanelUserWidget::PointMove(FTimelineEnterPointInfo& pointInfo,
	FVector2D move)
{
	if (pointInfo.pointUserWidget.IsValid())
	{
		auto y = pointInfo.y;
		auto x = pointInfo.x;
		auto setX = x - ctli.viewLeftPoint.X + move.X;
		auto setY = y - ctli.viewLeftPoint.Y + move.Y;

		FVector2D location(setX, setY);
		pointInfo.pointUserWidget->SetRenderTranslation(location);
	}
}

/*-------------------------------------------------------------*/

// ????????? ????????????????????????
void UTimelinePanelUserWidget::SetTextValue(CvTimelineUIInfo& info)
{

	EditableText_x->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(info.location.X))));
	EditableText_y->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(info.location.Y))));
	EditableText_z->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(info.location.Z))));

	EditableText_pitch->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(info.rotator.Pitch))));
	EditableText_yaw->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(info.rotator.Yaw))));
	EditableText_roll->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(info.rotator.Roll))));

	EditableText_r->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), info.color.X)));
	EditableText_g->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), info.color.Y)));
	EditableText_b->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), info.color.Z)));

	ComboBoxString_highlight->SetSelectedIndex(info.highlight);
	FString SelectedItem = ComboBoxString_highlight->GetOptionAtIndex(info.highlight);
	TextBlock_highlight_str->SetText(FText::FromString(SelectedItem));

	CheckBox_locationTeleport->SetCheckedState(info.locationTeleport ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	CheckBox_colorTeleport->SetCheckedState(info.colorTeleport ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

	EditableText_time->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), info.time))); // GetTimeWithX(ctli.timeAxisX) 
}

// ????????? ????????????????????????
void UTimelinePanelUserWidget::SetTextValue(FVector location, FRotator rotation)
{

	EditableText_x->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(location.X))));
	EditableText_y->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(location.Y))));
	EditableText_z->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(location.Z))));
	EditableText_pitch->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(rotation.Pitch))));
	EditableText_yaw->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(rotation.Yaw))));
	EditableText_roll->SetText(FText::FromString(FString::Printf(TEXT("%d"), UCalculationObject::RoundFloat(rotation.Roll))));

	EditableText_time->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), GetTimeWithX(ctli.timeAxisX))));
}

// ????????? ????????????
void UTimelinePanelUserWidget::SetTextValue(FVector color)
{
	EditableText_r->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), color.X)));
	EditableText_g->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), color.Y)));
	EditableText_b->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), color.Z)));
}

// ????????? ????????????????????????
void UTimelinePanelUserWidget::GetEditTransformData(CvTimelineUIInfo& info)
{

	std::string x(TCHAR_TO_UTF8(*EditableText_x->GetText().ToString()));
	std::string y(TCHAR_TO_UTF8(*EditableText_y->GetText().ToString()));
	std::string z(TCHAR_TO_UTF8(*EditableText_z->GetText().ToString()));
	std::string pitch(TCHAR_TO_UTF8(*EditableText_pitch->GetText().ToString()));
	std::string yaw(TCHAR_TO_UTF8(*EditableText_yaw->GetText().ToString()));
	std::string roll(TCHAR_TO_UTF8(*EditableText_roll->GetText().ToString()));
	std::string r(TCHAR_TO_UTF8(*EditableText_r->GetText().ToString()));
	std::string g(TCHAR_TO_UTF8(*EditableText_g->GetText().ToString()));
	std::string b(TCHAR_TO_UTF8(*EditableText_b->GetText().ToString()));
	std::string time(TCHAR_TO_UTF8(*EditableText_time->GetText().ToString()));

	info.time = atof(time.c_str());

	info.highlight = ComboBoxString_highlight->GetSelectedIndex();
	info.locationTeleport = CheckBox_locationTeleport->IsChecked();
	info.colorTeleport = CheckBox_colorTeleport->IsChecked();

	info.location = FVector(atoi(x.c_str()), atoi(y.c_str()), atoi(z.c_str()));
	info.rotator = FRotator(atoi(pitch.c_str()), atoi(yaw.c_str()), atoi(roll.c_str()));
	info.color = FVector(atof(r.c_str()), atof(g.c_str()), atof(b.c_str()));

}

// ????????? ????????????
FVector  UTimelinePanelUserWidget::GetEditColor()
{
	FVector color;
	std::string r(TCHAR_TO_UTF8(*EditableText_r->GetText().ToString()));
	std::string g(TCHAR_TO_UTF8(*EditableText_g->GetText().ToString()));
	std::string b(TCHAR_TO_UTF8(*EditableText_b->GetText().ToString()));
	color = FVector(atof(r.c_str()), atof(g.c_str()), atof(b.c_str()));
	return color;
}

// ????????? ?????????????????????
bool UTimelinePanelUserWidget::IsUIInfoSame(CvTimelineUIInfo& oldInfo, CvTimelineUIInfo& newInfo)
{



	return true;
}

/*-------------------------------------------------------------*/

// ?????? ??????
void UTimelinePanelUserWidget::TestData()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::TestData] a [time:%f]"), this, GetWorld()->TimeSeconds);

	//for (int h = 0; h < 21; h++)
	//{
	//	int offsetValue = h * 5;
	//	int entityId = h + 1;
	//	Gamedata::EntityInfo syncInfo;

	//	auto transform_timeline = syncInfo.mutable_transform_timeline();
	//	FString timelineName = "the0";
	//	std::string str(TCHAR_TO_UTF8(*timelineName));
	//	transform_timeline->set_name(str);
	//	transform_timeline->set_entitytransform_timelineendofplay(Gamedata::EntityMoveTimelineEndOfPlay_ShutDown);
	//	transform_timeline->set_isreflection(false);
	//	transform_timeline->set_size(1);
	//	transform_timeline->set_lenght(40000);
	//	transform_timeline->set_playlenght(10);
	//	transform_timeline->set_ismove(true);
	//	transform_timeline->set_moveyaw(true);
	//	transform_timeline->set_scenestimelineindex(0);
	//	transform_timeline->set_entitylistindex(0);

	//	for (int i = 0; i < 130; i++)
	//	{
	//		int keyTime2 = (offsetValue + i) * 1000;
	//		// Gamedata::EntityMoveFrameKey entityTimeInfo;
	//		// entityTimeInfo.set_ishide(false);
	//		// entityTimeInfo.set_colortype(Gamedata::EntityMoveTimelineColorType::EntityMoveTimelineColorType_Blue);
	//		Gamedata::FVector location;
	//		location.set_x(10 * i);
	//		location.set_y(10 * i);
	//		location.set_z(10 * i);

	//		// auto nodemap = transform_timeline->mutable_locationmap();
	//		// nodemap->insert({keyTime2, location});
	//	}

	//	// syncEntityInfoMap.Add(entityId, syncInfo);
	//	// entitySortList.Add(entityId);
	//}

	UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::TestData] b [time:%f]"), this, GetWorld()->TimeSeconds);
	SetBlockData();

	UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_data::TestData] c [time:%f]"), this, GetWorld()->TimeSeconds);
	// ViewShowPoint();
	ViewMove(FVector2D(0, 0));
}

// ??????????????????
void UTimelinePanelUserWidget::DrawLine(FVector2D start, FVector2D end)
{
	// ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;//GetDebugLineBatcher(GetWorld(), bPersistentLines, LifeTime, (DepthPriority == SDPG_Foreground));
	// const float LifeTime = 10.f;
	// if (LineBatcher != NULL)
	// {
	// 	float LineLifeTime = (LifeTime > 0.f) ? LifeTime : LineBatcher->DefaultLifeTime;
	//
	// 	LineBatcher->DrawLine(FVector(50.f, 50.f, 50.f), FVector(1000.f, 1000.f, 1000.f), FLinearColor::Blue, 10, 0.f, LineLifeTime);
	// }
	//
	// FPaintContext Context;
	// UWidgetBlueprintLibrary::DrawLine(Context, start, end, FLinearColor(255,255,255,1), true, 1);
	//
	// TestFunc();
}

// ?????? ????????????overlay?????????
void UTimelinePanelUserWidget::Test_GetOverlay()
{
	// FGeometry
	FGeometry gemotry = GetPaintSpaceGeometry();
	auto oldMousePosion = 1 / gemotry.Scale * cuoi.mouseButtonEnterPosion;
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// playerController->GetMousePosition(cuoi.mouseButtonEnterPosion.X, cuoi.mouseButtonEnterPosion.Y);

	UOverlay* parent = Cast<UOverlay>(GetParent());

	// parent->

	// if(parent)
	// {
	// 	parent->over
	//
	// 	// auto uOverlay = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_point")));
	// SOverlay* overlay = Cast<SOverlay>();
	// if(overlay)
	// {
	// 	FMargin margin;
	// 	overlay->Slot().SlotPadding.Get(margin);
	// 	UE_LOG(LogTemp, Log, TEXT("[%x] [Event_LeftMouseButton]  [time:%f] [point:%s] [oldMousePosion:%s] [Left:%f] [Top:%f]"), this,
	// 	GetWorld()->TimeSeconds, *cuoi.mouseButtonEnterPosion.ToString(),*oldMousePosion.ToString(), margin.Left, margin.Top );
	// }
	// }
}

/*-------------------------------------------------------------*/

// ??????

// ?????? ?????? ?????????
void UTimelinePanelUserWidget::ViewChoose(bool chooseLeft, std::vector<int> selectEntityIdLise)
{
	// ?????????????????????

	if (chooseLeft)
	{
		// LeftLineChoose(*timelineEntityLineInfo, true);
	}

	for (auto node : entityLineInfoMap)
	{
		FTimelineEntityLineInfo* timelineEntityLineInfo = &entityLineInfoMap[node.Key];
		if (timelineEntityLineInfo->isSelect)
		{
			timelineEntityLineInfo->isSelect = false;
			CenterLineChoose(*timelineEntityLineInfo, timelineEntityLineInfo->isSelect);
		}
	}

	for (auto node : selectEntityIdLise)
	{
		if (entityLineInfoMap.Find(node))
		{
			FTimelineEntityLineInfo* timelineEntityLineInfo = &entityLineInfoMap[node];
			timelineEntityLineInfo->isSelect = true;
			CenterLineChoose(*timelineEntityLineInfo, true);
		}
	}
}

// ?????? ???????????????
void UTimelinePanelUserWidget::ViewGotoLine(int entityId)
{
	if (entityLineInfoMap.Find(entityId))
	{
		FTimelineEntityLineInfo* entityLineInfo = &entityLineInfoMap[entityId];
		auto y = entityLineInfo->y - ctli.blockYLenght / 2 + ctli.entityHight;
		FVector2D move(ctli.viewLeftPoint.X, y);
		ViewMove(move);
	}
}

// ???????????? ????????????
void UTimelinePanelUserWidget::Event_SelectEntity(std::vector<int>& selectEntityList)
{
	//ViewChoose(true, selectEntityList);

	//// ??????master??????
	//if (selectEntityList.size() > 0)
	//{
	//	ViewGotoLine(selectEntityList[selectEntityList.size() - 1]);
	//}
}
