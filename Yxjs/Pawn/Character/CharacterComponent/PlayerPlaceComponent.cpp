// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPlaceComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Core/BaseGameInstance.h"
#include "Tools/CalculationObject.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/Selected/SelectedViewActor.h"
#include "Net/NetMainComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerMoveTimelineObject.h"
#include "Tools/CommonlyObject.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerScenesTimelineObject.h"
#include "UI/BaseHud.h"
#include "GameMode/Component/BaseGameModeController.h"
#include "CharacterDataComponent.h"
#include "UI/Game/EditEntity/Timeline/TimelinePanelUserWidget.h"
#include "Pawn/Character/Actor/PreviewActor.h"

#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Runtime/Engine/Classes/Engine/GameViewportClient.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"

#include <math.h>
#include "UI/Game/EditEntity/Timeline/TimelineMainUserWidget.h"
#include <Components/HierarchicalInstancedStaticMeshComponent.h>
#include "PlayerPreviewComponent.h"

/*------------------------------------------------------------------*/

// 获取 动态获取view
TWeakObjectPtr<AEditEntityManage> UPlayerPlaceComponent::GetEditEntityManage()
{
	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetOwner()->GetGameInstance());
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// 获取 射线检测的位置
FVector UPlayerPlaceComponent::GetHitLocation()
{
	ABaseCharacter* character = Cast<ABaseCharacter>(GetOwner());

	TArray<FHitResult> outHits;
	FVector start;
	FVector end;

	//auto character = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	auto playerController = GetWorld()->GetFirstPlayerController();
	UActorComponent* camera = character->GetComponentByClass(UCameraComponent::StaticClass());
	UCameraComponent* cameraComponent = Cast<UCameraComponent>(camera);
	start = cameraComponent->GetComponentLocation();
	FRotator cameraR = cameraComponent->GetComponentRotation();
	end = start + cameraR.Vector() * 800;

	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), start, end, FColor(0, 255, 255), 5, 1.0f);

	FCollisionObjectQueryParams objectQueryParams;
	FCollisionQueryParams params;
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	FVector hitLocation;
	GetWorld()->LineTraceMultiByObjectType(outHits, start, end, objectQueryParams, params);
	bool checkFlag = false;
	for (auto node : outHits)
	{
		auto component = node.GetComponent();
		auto component2 = Cast<UActorComponent>(component);
		UBoxComponent* buildBox = Cast<UBoxComponent>(component);
		ECollisionChannel collisionChannel = component->GetCollisionObjectType();

		if (collisionChannel == ECollisionChannel::ECC_WorldStatic)
		{
			hitLocation = node.ImpactPoint;
			checkFlag = true;
			break;
		}
	}
	if (checkFlag == false)
	{
		hitLocation = end;
	}

	return hitLocation;
}

// 获取 玩家摄像头旋转
FRotator UPlayerPlaceComponent::GetPlayerCameraRotator()
{
	//auto character = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ABaseCharacter* character = Cast<ABaseCharacter>(GetOwner());
	UActorComponent* camera = character->GetComponentByClass(UCameraComponent::StaticClass());
	UCameraComponent* cameraComponent = Cast<UCameraComponent>(camera);
	FRotator cameraR = cameraComponent->GetComponentRotation();
	return cameraR;
}

// 获取 灯光束目标点 射线碰撞到的点
bool UPlayerPlaceComponent::GetHitPoint(bool hitPlace, FVector& result)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(GetOwner());
	auto playerController = Cast<APlayerController>(character->GetController());
	character->GetActorLocation();
	auto controller = GetEditEntityManage()->controller;
	auto data = GetEditEntityManage()->data;

	FVector startLocation, endLocation;

	float x, y;
	playerController->GetMousePosition(x, y);

	FVector mouseLocation;
	FRotator mouseRotator;
	{
		FVector mouseRotator_;
		playerController->DeprojectMousePositionToWorld(mouseLocation, mouseRotator_);
		mouseRotator = mouseRotator_.Rotation();
		startLocation = mouseLocation;
		FRotator r1 = mouseRotator;
		int32 lenght = 50 * 400;
		FVector m1 = r1.Vector() * lenght;
		endLocation = mouseLocation + m1;
	}
	result = endLocation;

	FCollisionQueryParams traceParams(FName(TEXT("test")), false, nullptr);
	TArray<struct FHitResult> hitResultList;

	TArray<AActor*> hitActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	if (hitPlace)
	{
		objectTypes.Add(UEngineTypes::ConvertToObjectType(BuildObject));
	}
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	bool bBlocked = GetOwner()->GetWorld()->LineTraceMultiByObjectType(hitResultList, startLocation, endLocation,
		objectTypes,
		traceParams);
	bool flag = false;
	if (hitResultList.Num() > 0)
	{
	}
	bool hitSelf = false;
	int i = 0;
	int32 hitEntityId = 0;
	int32 selectIndex = 0;
	for (auto node : hitResultList)
	{
		hitSelf = false;

		// 忽略自身
		{
			if (Cast<AEditEntityManage>(node.GetActor()))
			{
				auto box = Cast<UBoxComponent>(node.GetComponent());
				if (box && box->GetName().Contains(TEXT("Place_Box")))
				{
					if (data->GetEntityIdWithBox(box, hitEntityId, selectIndex))
					{
						if (GetSelect() == hitEntityId)
						{
							hitSelf = true;
						}
					}
				}
			}
		}

		// 如果是移动,忽略mesh?  零时代码
		if (Cast<AEditEntityManage>(node.GetActor()) && Cast<UHierarchicalInstancedStaticMeshComponent>(
			node.GetComponent()))
		{
			hitSelf = true;
		}

		if (hitSelf == false)
		{
			result = node.ImpactPoint;
			// UE_LOG(LogTemp, Log,
			//        TEXT(
			// 	       "[%x] [UEditEntityManageController::GetHitPoint] [i:%d]  [actor:%s] [component:%s] [hitEntityId:%d] [GetSelect():%d]"
			//        ), this, i,
			//        *node.GetActor()->GetName(), *node.GetComponent()->GetName(), hitEntityId, GetSelect());
			break;
		}

		i++;
	}

	flag = true;

	// UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), result, 5, 5, FRotator::ZeroRotator, FLinearColor::Blue, 60.2,
	//                                        1);

	// if(result == endLocation)
	// {
	// 	UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation, result, FLinearColor::Red, 1, 1);
	// 	UE_LOG(LogTemp, Log, TEXT("[%x] [UEditEntityManageController::GetHitPoint] [count:%d]"),this,i,hitResultList.Num());
	// }

	return flag;
}

// 获取 能否进入位移时间轴状态
bool UPlayerPlaceComponent::CanEnterEditMoveTimeline()
{
	// 当前选中的是否是灯光
	ABaseCharacter* character = Cast<ABaseCharacter>(GetOwner());
	auto placeView = GetEditEntityManage();

	auto entityId = character->selectComponent->GetSelectMasterInstance();

	auto syncInfo = placeView->data->GetSyncInfo(entityId);
	if (syncInfo == nullptr)return false;
	auto xlsInfo = placeView->data->GetXlsInfo(entityId);
	// GetSelect() = character->selectComponent->GetSelectMasterInstance();
	// if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_laser)
	// {
	//
	// 	return true;
	// }

	return true;
}

// 获取 位移时间轴控制器
TWeakObjectPtr<UPlaceControllerMoveTimelineObject> UPlayerPlaceComponent::GetMoveTimelineController()
{
	auto placeView = GetEditEntityManage();
	if (placeView == nullptr)return nullptr;
	return placeView->controller->moveTimelineController;
}

// 获取 hud
ABaseHUD* UPlayerPlaceComponent::GetHUD()
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController)
	{
		auto hud = Cast<ABaseHUD>(playerController->GetHUD());
		return hud;
	}
	return nullptr;
}

// 获取 当前选中
int UPlayerPlaceComponent::GetSelect()
{
	return  view->selectComponent->GetSelectMasterInstance();
}

/*------------------------------------------------------------------*/

// Sets default values for this component's properties
UPlayerPlaceComponent::UPlayerPlaceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPlayerPlaceComponent::BeginPlay()
{
	Super::BeginPlay();

	// this->AddToRoot();

	// character = Cast<ACharacter>(GetOwner());

	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::BeginPlay]   "), this);
}

// EndPlay
void UPlayerPlaceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// this->RemoveFromRoot();

	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::EndPlay]   "), this);
}

// START
void UPlayerPlaceComponent::Start()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::Start] "), this);
	view = Cast<ABaseCharacter>(GetOwner());
}

// END
void UPlayerPlaceComponent::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::End]   "), this);
}

/*------------------------------------------------------------------*/

// tick Called every frame
void UPlayerPlaceComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickTimeline();
}

// tick 实例时间轴 tick
void UPlayerPlaceComponent::TickTimeline()
{
	// 预览时间轴

	// 由UI发起,程序控制UItick.

	if (clui.isPlay == false)return;

	// ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	// auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	auto editEntityManage = GetEditEntityManage();
	auto placeController = editEntityManage->controller;
	auto placeData = editEntityManage->data;
	// auto scenesTimelineController = placeController->scenesTimelineController;
	auto moveTimelineController = GetMoveTimelineController();
	if (moveTimelineController == nullptr)return;

	auto nowTime = GetWorld()->TimeSeconds;
	// 当前选中时间轴

	if (placeData->IsExistData(GetSelect()) == false)return;
	auto syncInfo = placeData->GetSyncInfo(GetSelect());
	//auto localInfo = placeData->GetLocalInfo(GetSelect());

	auto playTime = nowTime - clui.startPlaySystemTime;
	// auto playTime2 = UCalculationObject::TwoDecimalPlaces(playTime);
	// auto timeKey = ((float)((int)((enterTime + 0.005) * 100))) / 100;

	clui.nowPlayTime = (clui.startPlayUITime + playTime) * 1000;

	//bool lastFrame = moveTimelineController->SetCurrentPlayLocationAndRotator(
	//	clui.timelineIndex, clui.nowPlayTime, syncInfo, localInfo);
	//if (lastFrame)
	//{
	//	UICallback_Play(false);
	//}
}

/*------------------------------------------------------------------*/

// 键盘输入 F键 (进入状态)
void UPlayerPlaceComponent::Input_EnterEdit()
{
	auto placeView = GetEditEntityManage();
	if (placeView->data->IsExistData(GetSelect()) == false)return;
	auto syncInfo = placeView->data->GetSyncInfo(GetSelect());
	//auto localInfo = placeView->data->GetLocalInfo(GetSelect());
	auto moveTimelineController = GetMoveTimelineController();
	if (moveTimelineController == nullptr)return;

	// 初始化时间轴数据
	//moveTimelineController->InitTimeline(syncInfo, localInfo);

	// 展开灯光操作UI
	//GetHUD()->Call_ChangeMainUI(EGameMainOperationMode::open_laser);

	// 初始化控制器数据
	clui.timelineIndex = 0;
	cmfi.totalTime = 0;
	cmfi.lastTimeMove = FVector::ZeroVector;

	cmi.lastTime = 0;

	view->selectComponent->JustLoseMaster();
}

// 键盘输入 F键 (退出状态)
void UPlayerPlaceComponent::Input_LeaveEdit()
{
	// 关闭灯光操作UI
	//GetHUD()->Call_ChangeMainUI(EGameMainOperationMode::game_main_operation_mode_edit_etity);
}

// 键盘输入 鼠标左键按下松开 (写入开始结束)
void UPlayerPlaceComponent::Input_WriteStartEnd(bool isEnter)
{
	auto placeView = GetEditEntityManage();
	auto moveTimelineController = placeView->controller->moveTimelineController;
	if (placeView->data->IsExistData(GetSelect()) == false)return;

	//// 按下
	//if (isEnter)
	//{
	//	if (view->myData->GetInputMode() == ECharacterStatusType::CharacterStatusType_EditLightRotator)
	//	{
	//		cmfi.enterTime = GetWorld()->TimeSeconds;
	//		UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::Input_WriteStartEnd]  [enterTime:%f] "), this,
	//			cmfi.enterTime);
	//		Input_WriteMove(true);
	//	}
	//}
	//// 松开
	//else
	//{
	//	if (view->myData->GetInputMode() == ECharacterStatusType::CharacterStatusType_EditLightRotator)
	//	{
	//		Input_WriteMove(true);
	//		auto syncInfo = placeView->data->GetSyncInfo(GetSelect());
	//		//auto localInfo = placeView->data->GetLocalInfo(GetSelect());
	//		moveTimelineController->WriteTimelineKeyList(clui.timelineIndex, syncInfo, localInfo);
	//		cmfi.totalTime += (GetWorld()->TimeSeconds - cmfi.enterTime);
	//		cmfi.enterTime = 0;
	//		UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::Input_WriteStartEnd]  [Time:%f] "), this,
	//			GetWorld()->TimeSeconds);
	//	}
	//}
}

// 键盘输入 空格
void UPlayerPlaceComponent::Input_EnterKeyWriteMove(float writeTime)
{
	// writeTime -1:没有设置写入时间

	auto world = GetOwner()->GetWorld();
	auto nowTime = world->TimeSeconds;
	// UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::Input_WriteMove] [nowTime:%f] "), this, nowTime);

	ABaseCharacter* character = Cast<ABaseCharacter>(GetOwner());
	auto playerController = Cast<APlayerController>(character->GetController());
	auto placeView = GetEditEntityManage();
	auto moveTimelineController = placeView->controller->moveTimelineController;

	if (placeView->data->IsExistData(GetSelect()) == false)return;
	auto syncInfo = placeView->data->GetSyncInfo(GetSelect());
	if (syncInfo == nullptr)return;
	//auto localInfo = placeView->data->GetLocalInfo(GetSelect());
	auto t1 = syncInfo->transform();
	auto spawnWTransform = UCommonlyObject::PBToUe4Transform(&t1);

	FVector newVector = spawnWTransform.GetLocation();

	// 采用一次摁键间隔1s的方式
	if (writeTime == -1)
	{
		writeTime = cmi.lastTime + 0.2;
		cmi.lastTime = writeTime;
	}

	//{
	//	// 写入数据
	//	moveTimelineController->WriteTimelineLightData(syncInfo, localInfo, newVector, writeTime, clui.timelineIndex);

	//	// 重置play数据
	//	moveTimelineController->WriteTimelineKeyList(clui.timelineIndex, syncInfo, localInfo);

	//	UI_UpdataLaserUI(syncInfo, localInfo, false);

	//	UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), newVector, 5, 5, FRotator::ZeroRotator, FLinearColor::Blue,
	//		60.2,
	//		1);
	//}
}

// 键盘输入 鼠标移动 (写入变化)
void UPlayerPlaceComponent::Input_WriteMove(bool isMast)
{
	auto world = GetOwner()->GetWorld();
	auto nowTime = world->TimeSeconds;
	// UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::Input_WriteMove] [nowTime:%f] "), this, nowTime);

	ABaseCharacter* character = Cast<ABaseCharacter>(GetOwner());
	auto playerController = Cast<APlayerController>(character->GetController());
	auto placeView = GetEditEntityManage();
	auto moveTimelineController = placeView->controller->moveTimelineController;

	if (placeView->data->IsExistData(GetSelect()) == false)return;
	auto syncInfo = placeView->data->GetSyncInfo(GetSelect());
	if (syncInfo == nullptr)return;
	//auto localInfo = placeView->data->GetLocalInfo(GetSelect());
	auto t1 = syncInfo->transform();
	auto spawnWTransform = UCommonlyObject::PBToUe4Transform(&t1);

	//auto& moveTimeline = syncInfo->movetimeline();

	bool isSave = false;

	// 此处不再主动位移,直接去select组件位移好的数据即可

	FVector newVector = spawnWTransform.GetLocation();
	// FVector hitLocation;
	// GetHitPoint(true, hitLocation);
	// FRotator rotator = (hitLocation - cmfi.lastTimeMove).Rotation();
	//
	// // 位移
	// if (moveTimeline.ismove())
	// {
	// 	newVector = hitLocation;
	//
	// 	moveTimelineController->SetEntityLocalTempLocationOrRotator(GetSelect(), true, newVector);
	//
	// 	// // 前进方向夹角
	// 	// auto angle = UCalculationObject::GetVectorAngle(rotator.Vector(), cmfi.lastTimeMove);
	// 	// if (angle > 6)
	// 	// {
	// 	// 	isSave = true;
	// 	// 	cmfi.lastTimeMove = rotator.Vector();
	// 	// 	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::Input_WriteMove] [nowTime:%f] [angle:%f]"), this, nowTime, angle);
	// 	// }
	// }
	// 	// 旋转
	// else
	// {
	// 	// FRotator rotator = (hitLocation - spawnWTransform.GetLocation()).Rotation();
	// 	newVector = rotator.Vector();
	//
	// 	moveTimelineController->SetEntityLocalTempLocationOrRotator(GetSelect(), false, newVector);
	//
	// 	// 旋转夹角
	// 	auto angle = UCalculationObject::GetVectorAngle(newVector, cmfi.lastTimeMove);
	// 	if (angle > 3)
	// 	{
	// 		isSave = true;
	// 		cmfi.lastTimeMove = newVector;
	// 	}
	// }

	//if (isMast || isSave)
	//{
	//	cmfi.lastTimeSetEditLightRotator = nowTime;
	//	moveTimelineController->WriteTimelineLightData(syncInfo, localInfo, newVector,
	//		cmfi.totalTime + (nowTime - cmfi.enterTime), clui.timelineIndex);

	//	UI_UpdataLaserUI(syncInfo, localInfo, false);

	//	UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), newVector, 5, 5, FRotator::ZeroRotator, FLinearColor::Blue,
	//		60.2,
	//		1);
	//}
}

// 键盘输入 空格 (插入关键帧)
void UPlayerPlaceComponent::Input_WriteKey(bool isOpen)
{
}

// 键盘输入 I键 打开关闭总时间轴
void UPlayerPlaceComponent::Input_OpenTimeline(bool flag)
{
	auto hud = GetHUD();
	auto placeView = GetEditEntityManage();

	// auto uiOrder = hud->GetUIOrder();

	if (flag)
	{
		//hud->Call_ChangeMainUI(EGameMainOperationMode::open_timeline);

		//// 捕获范围内的数据初始化
		//auto timelineMainUserWidget = Cast<UTimelineMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_timeline));
		//if (timelineMainUserWidget && timelineMainUserWidget->timelinePanel.IsValid())
		//{
		//	timelineMainUserWidget->timelinePanel->Call_InitData(false);
		//}
	}
	else
	{
		//hud->Call_ChangeMainUI(EGameMainOperationMode::game_main_operation_mode_base_character);
	}

	// if (view->dataComp->characterStatus == ECharacterStatusType::CharacterStatusType_EditEntity)
	// {
	// 	if(uiOrder == UIOrder::open_editmode)
	// 	{
	// 		hud->ChangeUI(UIOrder::open_timeline);
	//
	// 		// 捕获范围内的数据初始化
	// 		auto timelineMainUserWidget = Cast<UTimelineMainUserWidget>(hud->GetChildUI(UIName::ui_name_timeline));
	// 		if(timelineMainUserWidget && timelineMainUserWidget->timelinePanel.IsValid())
	// 		{
	// 			timelineMainUserWidget->timelinePanel->InitData(false);
	// 		}
	// 	}
	// 	else
	// 	{
	// 		hud->ChangeUI(UIOrder::open_editmode);
	// 	}
	// }
}

/*------------------------------------------------------------------*/

// UI输入 实例时间轴 点击播放
void UPlayerPlaceComponent::UICallback_TimelinePlay(bool flag)
{
	// TimelinePlay(flag);
}

// UI输入 实例时间轴 点击播放
void UPlayerPlaceComponent::UICallback_Play(bool isStart)
{
	clui.isPlay = isStart;
	clui.timelineIndex = 0;
	clui.startPlayUITime = 0;
	clui.startPlaySystemTime = GetWorld()->TimeSeconds;
	clui.nowPlayTime = clui.startPlayUITime;

	// 开始播放
	if (isStart)
	{
		// 关闭高亮
		view->selectComponent->SetSelectMesh(false, GetSelect());

	}
	// 停止播放
	else
	{
		// 开启高亮
		view->selectComponent->SetSelectMesh(true, GetSelect());
	}
}

// UI输入 场景时间轴 开关
void UPlayerPlaceComponent::TimelinePlay(bool flag)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	auto editEntityManage = GetEditEntityManage();
	auto placeController = editEntityManage->controller;
	auto placeData = editEntityManage->data;
	auto timelineController = placeController->scenesTimelineController;

	if (flag)
	{
	}
	else
	{
	}

	// 当前所在九宫格
	auto characterLocation = character->GetActorLocation();
	FString jiugongeKey = UCommonlyObject::GetJiugongge(characterLocation.X, characterLocation.Y,
		baseGameMode->syncSize);

	//// 当前九宫格时间轴实例
	//TArray<int32> entityIdList;
	//for (auto node : placeData->syncEntityInfoMap)
	//{
	//	auto entityId = node.Key;
	//	auto& syncInfo = node.Value;
	//	auto xlsInfo = placeData->GetXlsInfo(entityId);
	//	auto localInfo = placeData->GetLocalInfo(entityId);
	//	if (localInfo && xlsInfo)
	//	{
	//		if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_Timeline)
	//		{
	//			if (localInfo->jiugonggeKey.Equals(jiugongeKey))
	//			{
	//				entityIdList.Add(entityId);
	//				break;
	//			}
	//		}
	//	}
	//}

	//if (entityIdList.Num() > 0)
	//{
	//	auto syncInfo = placeData->GetSyncInfo(entityIdList[0]);
	//	if (syncInfo)
	//	{
	//		Gamedata::EntityOperationTotal total;
	//		auto node = total.add_node();
	//		node->set_thetype(Gamedata::EntityOperationType_IsPlay);
	//		auto value1 = node->add_value1();
	//		value1->set_isplay(flag);
	//		node->set_entityid(syncInfo->entityid());
	//		node->set_playerid(baseGameMode->GetLocalPlayerId());
	//		timelineController->NetCall_EntityOperation(total);
	//	}
	//}
}

// UI调用 实例时间轴 更新时间轴节点
void UPlayerPlaceComponent::UI_UpdataLaserUI(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, bool isMast)
{
	// 进入F更新,离开F更新,延时更新

	// 标准模式 0-60s
	auto hud = GetHUD();
	if (hud == nullptr)return;

	if (hud->IsUIShow(EUIChildName::ui_name_laser) == false)return;

	auto ui_name_laser = hud->GetChildUI(EUIChildName::ui_name_laser);
	if (ui_name_laser == nullptr)return;

	//if (syncInfo->laser_size() == 0)
	//{
	//	return;
	//}
	// int timelineIndex = 0;
	// // auto laser = syncInfo->laser(0);
	// auto movetimeline = syncInfo->movetimeline(timelineIndex);
	// auto& nodeMap = movetimeline.nodemap();
	//
	// int maxCount = 120;
	// std::vector<int> tempList;
	// for (int i = 0; i < maxCount; i++)
	// {
	// 	tempList.push_back(0);
	// }
	//
	// // map -> list
	// for (auto node : nodeMap)
	// {
	// 	auto key = node.first;
	// 	auto value = node.second;
	// 	float temp1 = ((float)key / 1000.0f);
	// 	float temp2 = temp1 * 2.0f;
	// 	auto newKey = (int)(temp2 * 10 + 5) / 10;
	// 	// UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::UI_UpdataLaserUI]  [%f--%f--%d]  [key:%d]"),
	// 	// 	this, temp1, temp2, newKey, key);
	// 	if (newKey < maxCount)
	// 	{
	// 		// 播放亮光
	// 		if (value.ishide())
	// 		{
	// 			tempList[newKey] = 1;
	// 		}
	// 			// 播放熄灭
	// 		else
	// 		{
	// 			tempList[newKey] = 2;
	// 		}
	// 	}
	// }
	//
	// // UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::UI_UpdataLaserUI]   [entityId:%d]  [count:%d]"),
	// // 	this, syncInfo->entityid(), nodeMap.size());
	//
	// FString showItem = TEXT("");
	// for (auto node : tempList)
	// {
	// 	if (showItem.Len() == 0)
	// 	{
	// 		showItem = showItem + FString::Printf(TEXT("%d"), node);
	// 	}
	// 	else
	// 	{
	// 		showItem = showItem + FString::Printf(TEXT("|%d"), node);
	// 	}
	// }
	//
	// // UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPlaceComponent::UI_UpdataLaserUI]   [entityId:%d] [count:%d] [showItem:%s]"),
	// //        this, syncInfo->entityid(), nodeMap.size() ,*showItem);
	// {
	// 	FString cmd = FString::Printf(TEXT("Event_SetTimeBar %d %s"), timelineIndex, *showItem);
	// 	FOutputDeviceDebug device;
	// 	ui_name_laser->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
	// }
	//
	// // std::string str2;
}

/*------------------------------------------------------------------*/

// 生成一个网格实例
void UPlayerPlaceComponent::SpawnMeshInstance(int32 xlsId, bool isMerge, FTransform transform)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(GetOwner());
	//if (character == nullptr)return;
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(character->GetWorld()->GetAuthGameMode());
	if (baseGameMode == nullptr)return;
	auto data = baseGameMode->editEntityManage->data;
	auto gameInstance = baseGameMode->gameInstance;

	Gamedata::EntityInfo syncInfo;

	{
		syncInfo.set_entityid(0);
		syncInfo.set_playerid(baseGameMode->GetLocalPlayerId());
		syncInfo.set_collision_type(Gamedata::EntityCollisionType_QueryAndPhysics);
		auto spawnWTransform = syncInfo.mutable_transform();
		UCommonlyObject::Ue4ToPbTransform(transform, spawnWTransform);
	}

	{
		auto child = syncInfo.add_child_list();
		child->mutable_mesh()->set_xls_id(xlsId);
		child->mutable_mesh()->set_material_id(0);
		child->mutable_mesh()->set_highlight(false);
		Gamedata::FVector* color = child->mutable_mesh()->mutable_color();
		color->set_x(0);
		color->set_y(0);
		color->set_z(0);
		auto rTransform = child->mutable_transform();
		FTransform transform2(FRotator::ZeroRotator, FVector::ZeroVector);
		transform2.SetScale3D(FVector::OneVector);
		UCommonlyObject::Ue4ToPbTransform(transform2, rTransform);
	}

	GetEditEntityManage()->controller->NetCall_NewData(syncInfo);
}

// 生成一个图片实例
void UPlayerPlaceComponent::SpawnImageInstance(FString url)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(GetOwner());
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(character->GetWorld()->GetAuthGameMode());
	if (baseGameMode == nullptr)return;
	auto data = baseGameMode->editEntityManage->data;
	auto gameInstance = baseGameMode->gameInstance;

	auto location = GetHitLocation();
	FTransform t;
	t.SetLocation(location);
	t.SetRotation(FRotator::ZeroRotator.Quaternion());
	t.SetScale3D(FVector(1, 1, 1));

	Gamedata::EntityInfo syncInfo;
	syncInfo.set_entityid(0);
	syncInfo.set_playerid(baseGameMode->GetLocalPlayerId());
	// syncInfo.set_type(Gamedata::EntityType::EntityType_Image);
	//syncInfo.set_xlsid(0);
	//syncInfo.set_materialid(0);
	//syncInfo.set_merge_mesh(false);
	//syncInfo.set_masterid(0);
	auto spawnWTransform = syncInfo.mutable_transform();
	UCommonlyObject::Ue4ToPbTransform(t, spawnWTransform);

	// syncInfo.spawnWTransform = t;

	GetEditEntityManage()->controller->NetCall_NewData(syncInfo);
}

/*------------------------------------------------------------------*/

// 主从设置
void UPlayerPlaceComponent::SetMasterNode()
{
	// 多选主从设置
}

// 测试 生成测试箭头
UArrowComponent* UPlayerPlaceComponent::CreateTestArrow(FString name, FTransform t)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(GetOwner());
	auto arrow = NewObject<UArrowComponent>(character, *name);

	t.SetScale3D(FVector(1, 4, 0.2f));

	arrow->RegisterComponent();
	arrow->AttachToComponent(character->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	arrow->SetCollisionObjectType(ECC_WorldStatic);
	arrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	arrow->SetWorldTransform(t);
	arrow->SetHiddenInGame(false);
	return arrow;
}

// 测试 销毁测试箭头
void UPlayerPlaceComponent::DestroyTestArrow(UArrowComponent* arrow)
{
	if (arrow)
	{
		if (arrow->IsRegistered())
		{
			arrow->UnregisterComponent();
		}
		arrow->DestroyComponent();
	}
}

// 测试 更新箭头
void UPlayerPlaceComponent::UpdateTestArrow(FString name, FTransform t)
{
	// if (arrowMap.Find(name))
	// {
	// 	DestroyTestArrow(arrowMap[name]);
	// 	arrowMap.Remove(name);
	// }
	// auto arrow = CreateTestArrow(name, t);
	// arrowMap.Add(name, arrow);
}

/*------------------------------------------------------------------*/