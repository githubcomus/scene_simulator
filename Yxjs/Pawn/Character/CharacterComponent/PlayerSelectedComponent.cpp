// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSelectedComponent.h"
#include "FunctionalModule/Selected/SelectedViewActor.h"
#include "FunctionalModule/Selected/SelectedControllerComponent.h"
#include "FunctionalModule/Selected/SelectedDataComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "Core/BaseGameInstance.h"
#include "Tools/CalculationObject.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewMeshComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewCollisionComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"
#include "GameMode/BaseGameMode.h"
#include "UI/BaseHud.h"
#include "UI/BaseHUDInfo.h"
#include "Tools/CommonlyObject.h"
#include "UI/Game/EditEntity/Timeline/TimelineMainUserWidget.h"
#include "UI/Game/EditEntity/Timeline/TimelinePanelUserWidget.h"
#include "UI/Game/EditEntity/Main/EditEntityMainUserWidget.h"
#include "UI/Game/EditEntity/Main/EditEntityMainBaseSyncInfoUserWidget.h"
#include "Core/SoundEffect/MySoundEffect.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityChildComponent.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityEditComponent.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityTimelineComponent.h"
#include "Yxjs/UI/Game/EditEntity/Main/EditEntityMainUserWidget.h"

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
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include <math.h>
#include "../../../UI/HUDStaticFunc.h"
#include "../../../UI/Game/EditEntity/EntityMaterial/EntityMaterialMainUserWidget.h"
#include "../../../UI/Game/EditEntity/EntityMaterial/EntityMaterialPanelUserWidget.h"
#include "PlayerInputComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

/*
 *????????????????????????,??????
 *
 */

 /*/////////////////////////////////////////////////////////////*/
 // ??????
 /*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

// ?????? ????????????view
TWeakObjectPtr<ASelectedViewActor> UPlayerSelectedComponent::GetSelectedView()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->selectedViewActor;
}

// ?????? ????????????controller
UEditEntityManageController* UPlayerSelectedComponent::GetPlaceController()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage->controller;
}

// ?????? ?????????????????????????????????????????????
bool UPlayerSelectedComponent::GetMultipleChoiceIndex(int32 entityId, int32& arrayIndex)
{
	bool flag = false;
	for (int i = 0; i < cvMultipleInstanceInfo.selectedList.Num(); i++)
	{
		auto node = &cvMultipleInstanceInfo.selectedList[i];
		if (entityId == node->entityId)
		{
			arrayIndex = i;
			flag = true;
			break;
		}
	}
	return flag;
}

// ?????? ???????????????
int UPlayerSelectedComponent::GetSelectMasterInstance()
{
	// ?????? -1:???

	auto len = cvMultipleInstanceInfo.selectedList.Num();
	if (len == 0)
	{
		return -1;
	}
	return cvMultipleInstanceInfo.selectedList[len - 1].entityId;
}

// ??????????????????
bool UPlayerSelectedComponent::IsSelectEntity()
{
	return cvMultipleInstanceInfo.selectedList.Num() > 0 ? true : false;
}

// ?????????????????????
bool UPlayerSelectedComponent::IsSelectAxis()
{
	auto selectedView = GetSelectedView();

	bool flag = false;

	if (cvMultipleInstanceInfo.selectedList.Num() > 0)
	{
		if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldLocation
			|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyLocation)
		{
			if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationX
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationY
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationZ
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::planeMove)
			{
				flag = true;
			}
		}
		else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldRotator
			|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
		{
			if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorPitch
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorYaw
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorRoll)
			{
				flag = true;
			}
		}
		else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::Scale)
		{
			if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleX
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleY
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleZ)
			{
				flag = true;
			}
		}
	}
	return flag;
}

// ?????? ??????????????????
void UPlayerSelectedComponent::GetSelectInstanceList(std::vector<int>& selectList)
{
	for (auto node : cvMultipleInstanceInfo.selectedList)
	{
		selectList.push_back(node.entityId);
	}
}

// ?????? ???????????????????????????????????????(??????????????????)
bool UPlayerSelectedComponent::TimelineIsSelectKeyFrame(int& timeKey, int& entityId, int& uniqueMark)
{
	auto timeLine = character->myData->hud->GetChildUI(EUIChildName::ui_name_timeline);
	if (timeLine == nullptr)return false;
	auto timelinePanel = Cast<UTimelineMainUserWidget>(timeLine)->timelinePanel;
	if (timelinePanel.IsValid() == false)return false;

	return timelinePanel->TimelineIsSelectKeyFrame(timeKey, entityId, uniqueMark);
}

// ?????? ?????????UI
TWeakObjectPtr<UTimelinePanelUserWidget> UPlayerSelectedComponent::GetTimelineUI()
{

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto playerController = Cast<APlayerController>(character->GetController());
	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
	if (hud == nullptr)return nullptr;
	auto ui_name_timeline = hud->GetChildUI(EUIChildName::ui_name_timeline);
	if (ui_name_timeline == nullptr)return nullptr;
	TWeakObjectPtr<UTimelinePanelUserWidget>& timelinePanel = Cast<UTimelineMainUserWidget>(ui_name_timeline)->timelinePanel;

	return timelinePanel;
}

// ??????????????????UI
TWeakObjectPtr < UEditEntityMainUserWidget>  UPlayerSelectedComponent::GetEditEntityMainUI()
{
	auto playerController = Cast<APlayerController>(character->GetController());
	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
	if (hud == nullptr)return nullptr;
	auto ui_name_edit_entity = Cast<UEditEntityMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_edit_entity));
	if (ui_name_edit_entity == nullptr)return nullptr;
	if (hud->IsUIShow(EUIChildName::ui_name_edit_entity) == false)return nullptr;
	return ui_name_edit_entity;
}

/*/////////////////////////////////////////////////////////////*/
// ?????????
/*/////////////////////////////////////////////////////////////*/

 // Sets default values for this component's properties
UPlayerSelectedComponent::UPlayerSelectedComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	cvMoveStrategyInfo.isLeftPressed = false;

	cvCopyInfo.isCopy = false;
	cvCopyInfo.isCrop = false;
}

// Called when the game starts
void UPlayerSelectedComponent::BeginPlay()
{
	Super::BeginPlay();

	character = Cast<ABaseCharacter>(GetOwner());
}

// EndPlay
void UPlayerSelectedComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// START
void UPlayerSelectedComponent::Start()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::Start] "), this);

	// ?????????????????????????????????????????????
	auto camear = character->GetComponentByClass(UCameraComponent::StaticClass());
	auto springArmComponent = character->GetComponentByClass(USpringArmComponent::StaticClass());
	auto camear_ = Cast<USceneComponent>(camear);
	if (camear_)
	{
		camearBox = NewObject<UBoxComponent>(character.Get(), TEXT("camearBox"));
		camearBox->RegisterComponent();
		camearBox->AttachToComponent(camear_, FAttachmentTransformRules::KeepRelativeTransform);
		camearBox->CanCharacterStepUpOn = ECB_No;
		camearBox->SetGenerateOverlapEvents(false);
		camearBox->SetCollisionObjectType(SelectedInstance);
		camearBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		camearBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		camearBox->SetEnableGravity(false);
		camearBox->SetBoxExtent(FVector(10, 100, 100));
		camearBox->SetRelativeLocation(FVector(-10, 0, 0));
		camearBox->SetRelativeRotation(FRotator(0, 0, 0));
		camearBox->SetHiddenInGame(true);
	}

	//{
	//    auto components = character->GetComponents();
	//    for (auto node : components)
	//    {
	//        auto node_=Cast<USceneComponent>(node);
	//        auto compontName = node->GetName();
	//        if (node_)
	//        {
	//            auto objectType = node_->GetCollisionObjectType();
	//            UE_LOG(LogTemp, Log, TEXT("[%x] [Start] b  [compontName:%s] [%d] "),
	//                   this, *compontName, objectType);
	//        }
	//        else
	//        {
	//            UE_LOG(LogTemp, Log, TEXT("[%x] [Start] a  [compontName:%s] "), this, *compontName);
	//        }
	//    }

	//}
}

// END
void UPlayerSelectedComponent::End()
{
	// camearBox->RemoveFromRoot();
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::End]   "), this);
}

/*------------------------------------------------------------------*/

// Tick Called every frame
void UPlayerSelectedComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ??????????????????/??????????????????,????????????,??????xy????????????,????????????????????????,????????????.
	auto nowTime = GetWorld()->TimeSeconds;
	if (lastTimeTick + 0.1 > nowTime)
	{
		return;
	}
	lastTimeTick = nowTime;

	if (character.IsValid() == false)return;
	UPawnMovementComponent* movementComponent = character->GetMovementComponent();
	auto& inputKeyInfo = character->myData->inputKeyInfo;
	if (
		(character->inputComp->inputMode == ECharacterStatusType::CharacterStatusType_EditEntity || character->inputComp->inputMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
		&& inputKeyInfo.mouseLeftEnter == false
		&& inputKeyInfo.mouseRightEnter == false
		&& movementComponent->Velocity.Size() == 0
		&& IsSelectEntity()
		)
	{

		auto playerController = Cast<APlayerController>(character->GetController());
		float x, y;
		playerController->GetMousePosition(x, y);
		if (lastTimeMousePosion.Equals(FVector2D(x, y)) == false)
		{
			lastTimeMousePosion = FVector2D(x, y);
			//UE_LOG(LogTemp, Log, TEXT("[%x] [???????????????] [TickComponent] [nowTime:%f]"), this, nowTime);
			Input_Base_HighlightArrow();
		}

	}

}

// Tick ?????? ???????????????????????? ?????????????????????
void UPlayerSelectedComponent::Input_Base_HighlightArrow()
{
	auto selectedView = GetSelectedView();
	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto playerController = Cast<APlayerController>(character->GetController());
	auto camearBoxWTransform = camearBox->GetComponentTransform();

	float x, y;
	playerController->GetMousePosition(x, y);

	FVector startLocation, endLocation;
	FVector mouseLocation;
	FRotator mouseRotator;
	{
		FVector mouseRotator_;
		playerController->DeprojectMousePositionToWorld(mouseLocation, mouseRotator_);
		mouseRotator = mouseRotator_.Rotation();
		startLocation = mouseLocation;
		FRotator r1 = mouseRotator;
		int32 lenght = (character->GetActorLocation() - selectedView->GetActorLocation()).Size() * 2;
		FVector m1 = r1.Vector() * lenght;
		endLocation = mouseLocation + m1;

	}

	//bool isComplexCollision = false;
	//if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldRotator
	//	|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
	//{
	//	isComplexCollision = true;
	//}

	FCollisionQueryParams traceParams(FName(TEXT("test")), true, nullptr);
	TArray<struct FHitResult> hitResultList;

	TArray<AActor*> hitActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(SelectedInstance));

	bool bBlocked = GetOwner()->GetWorld()->LineTraceMultiByObjectType(hitResultList, startLocation, endLocation,
		objectTypes, traceParams);

	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation, endLocation, FColor(99, 99, 99), 2.0, 0.2f);
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::Input_Base_HighlightArrow] [%d]"), this, bBlocked);

	ESelectedMoveStrategy type = ESelectedMoveStrategy::None;

	for (int i = 0; i < hitResultList.Num(); i++)
	{
		auto node = hitResultList[i];
		auto actor = hitResultList[i].GetActor();
		auto compont = hitResultList[i].GetComponent();
		auto compontName = compont->GetName();

		if (Cast<ASelectedViewActor>(actor) == nullptr)continue;

		if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldLocation
			|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyLocation)
		{
			// ????????????
			if (compontName.Equals(TEXT("NameMyPlaneMove")))
			{
				type = ESelectedMoveStrategy::planeMove;
				break;
			}
			// ??????x
			if (compontName.Equals(TEXT("NameMyLocationX")))
			{
				type = ESelectedMoveStrategy::LocationX;
				break;
			}
			// ??????y
			if (compontName.Equals(TEXT("NameMyLocationY")))
			{
				type = ESelectedMoveStrategy::LocationY;
				break;
			}
			// ??????z
			if (compontName.Equals(TEXT("NameMyLocationZ")))
			{
				type = ESelectedMoveStrategy::LocationZ;
				break;
			}
		}
		else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldRotator
			|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
		{
			// ??????x
			if (compontName.Equals(TEXT("NameMyRotatorPitch")))
			{
				type = ESelectedMoveStrategy::RotatorPitch;
				break;
			}
			// ??????y
			if (compontName.Equals(TEXT("NameMyRotatorYaw")))
			{
				type = ESelectedMoveStrategy::RotatorYaw;
				break;
			}
			// ??????z
			if (compontName.Equals(TEXT("NameMyRotatorRoll")))
			{
				type = ESelectedMoveStrategy::RotatorRoll;
				break;
			}
		}
		else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::Scale)
		{
			// ??????x
			if (compontName.Equals(TEXT("NameMyScaleX")))
			{
				type = ESelectedMoveStrategy::ScaleX;
				break;
			}
			// ??????y
			if (compontName.Equals(TEXT("NameMyScaleY")))
			{
				type = ESelectedMoveStrategy::ScaleY;
				break;
			}
			// ??????z
			if (compontName.Equals(TEXT("NameMyScaleZ")))
			{
				type = ESelectedMoveStrategy::ScaleZ;
				break;
			}
		}
	}

	selectedView->controller->RestoreAxisArrowColor(type);
	selectedView->controller->ChangeAxisArrowColor(type);

}

/*------------------------------------------------------------------*/

// ???????????? ???????????????????????? ??????ctrl??????
void UPlayerSelectedComponent::Input_Base_IsMultipleChoice(bool isMultipleChoice_)
{
	cvMultipleInstanceInfo.isMultipleChoice = isMultipleChoice_;

	auto ui_name_edit_entity = GetEditEntityMainUI();
	ui_name_edit_entity->EnterCtrl(isMultipleChoice_);
}

// ???????????? ???????????????????????? ??????????????????
void UPlayerSelectedComponent::Input_Base_LeftPressed()
{
	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto selectedView = GetSelectedView();
	auto placeController = GetPlaceController();
	auto ui_name_edit_entity = GetEditEntityMainUI();

	cvMoveStrategyInfo.isLeftPressed = true;
	cvMoveStrategyInfo.selectedMoveStrategy = ESelectedMoveStrategy::None;

	// ????????????????????????

	int32 entityId;
	int32 selectIndex;
	// ???????????????
	if (GetSelectedInstance(cvMoveStrategyInfo.selectedMoveStrategy))
	{
		SelectAxis();
	}
	// ??????????????????
	else if (GetPlaceInstance(entityId, selectIndex))
	{
		SelectInstance(entityId);
		selectedView->controller->SetIsSelected(true);

		UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI_??????);
		UHUDStaticFunc::PushMessage(this, FString::Printf(TEXT("????????????(%d)"), entityId));

		UICall_ChoseMaterial();
	}
	// ?????????????????????,???????????????????????????
	else
	{
		// ???????????????
		if (cvMultipleInstanceInfo.isMultipleChoice == false)
		{
			SetSelectHidden();
		}
	}

	std::vector<int> selectList;
	GetSelectInstanceList(selectList);
	ui_name_edit_entity->SetButtonStopEnter(selectList);
}

// ???????????? ???????????????????????? ??????????????????
void UPlayerSelectedComponent::Input_Base_LeftReleased()
{
	auto selectedView = GetSelectedView();


	// ??????????????????
	Input_Base_LeftMove();

	// ??????UI
	UICall_SelectToSetUI();

	selectedView->controller->SetShowSelectedMoveStrategyAll();

	// ?????????????????????,????????????????????????????????????
	if (selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
	{
		SetScaleAxis();
	}
	cvMoveStrategyInfo.isLeftPressed = false;

	OperationResultsSwitch(Operation_Type::Operation_Type_Update, false);
}

// ???????????? ???????????????????????? ????????????????????????
void UPlayerSelectedComponent::Input_Base_LeftMove()
{
	auto selectedView = GetSelectedView();

	// ????????????
	if (cvMoveStrategyInfo.isLeftPressed == false)
	{
	}
	// ????????????
	else
	{
		if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::None)
		{
			return;
		}

		if (cvMultipleInstanceInfo.selectedList.Num() > 0)
		{
			if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldLocation
				|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyLocation)
			{
				SetMultiplePanAxis();
			}
			else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldRotator
				|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
			{
				SetMultipleSpinAxis();
			}
			else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::Scale)
			{
				SetSingeZoom();
			}

			// ??????UI
			UICall_SelectToSetUI();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::Input_Base_LeftMove] ?????????????????? "), this);
		}
	}
}

// ???????????? ???????????????????????? ????????????,?????????????????????
void UPlayerSelectedComponent::Input_SetWorkMode(ESelectedWorkMode selectedWorkMode_)
{
	auto selectedController = GetSelectedView()->controller;
	selectedController->SetWorkMode(selectedWorkMode_);
	SetScaleAxis();
}

// ???????????? ????????????
void UPlayerSelectedComponent::Input_Base_MiddleMouseButton()
{

	auto playerController = Cast<APlayerController>(character->GetController());
	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
	if (hud == nullptr)return;
	auto ui_name_edit_entity = Cast<UEditEntityMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_edit_entity));
	if (ui_name_edit_entity == nullptr)return;
	if (hud->IsUIShow(EUIChildName::ui_name_edit_entity) == false)return;

	SetSelectHidden();
	std::vector<int> selectList;
	ui_name_edit_entity->SetButtonStopEnter(selectList);
}

/*------------------------------------------------------------------*/

// ???????????? ????????????????????? ??????ctrl??????
void UPlayerSelectedComponent::Input_Timeline_IsMultipleChoice(bool isMultipleChoice_)
{
	cvMultipleInstanceInfo.isMultipleChoice = isMultipleChoice_;
}

// ???????????? ????????????????????? ??????????????????
void UPlayerSelectedComponent::Input_Timeline_MouseLeftButtonPressed()
{
	cvMoveStrategyInfo.isLeftPressed = true;
	cvMoveStrategyInfo.selectedMoveStrategy = ESelectedMoveStrategy::None;

	// ???????????????
	if (GetSelectedInstance(cvMoveStrategyInfo.selectedMoveStrategy))
	{
		SelectAxis();
	}
}

// ???????????? ????????????????????? ????????????????????????
void UPlayerSelectedComponent::Input_Timeline_MouseLeftButtonEnterMove()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	TWeakObjectPtr<UTimelinePanelUserWidget> timelinePanel = GetTimelineUI();

	auto selectedView = GetSelectedView();

	int timeKey = 0;
	int entityId = 0;
	int uniqueMark = 0;
	if (TimelineIsSelectKeyFrame(timeKey, entityId, uniqueMark) == false)//?????????????????????
	{
		return;
	}

	// ????????????
	if (cvMoveStrategyInfo.isLeftPressed == false)
	{
	}
	// ????????????
	else
	{
		if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::None) // ?????????????????????
		{
			return;
		}

		if (cvMultipleInstanceInfo.selectedList.Num() > 0)
		{
			if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldLocation
				|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyLocation)
			{
				SetMultiplePanAxis();
			}
			else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldRotator
				|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
			{
				SetMultipleSpinAxis();
			}

			auto entityActor = editEntityManage->data->GetEntityActor(entityId);
			if (entityActor.IsValid())
			{
				auto nowTransform = entityActor->GetNowTransform();
				auto observerTransform = entityActor->GetObserverTransform();
				auto relationTransform = observerTransform.GetRelativeTransform(nowTransform);
				auto relationLocation = UCalculationObject::WorldToRelativeTransform(observerTransform, nowTransform.GetLocation());
				auto relationRotator = UCalculationObject::WorldToRelativeTransform(observerTransform, nowTransform.GetRotation().Rotator());

				timelinePanel->SetTextValue(relationLocation, relationRotator);

				//{

				//	auto tempV = observerTransform.GetLocation();
				//	auto tempR = observerTransform.GetRotation().Rotator();
				//	UE_LOG(LogTemp, Log, TEXT("[%x] [?????????] [Input_Timeline_MouseLeftButtonEnterMove] ?????? [??????:%f %f %f] [??????:%f %f %f]"), this,
				//		tempV.X, tempV.Y, tempV.Z,
				//		tempR.Pitch, tempR.Yaw, tempR.Roll
				//	);
				//}

				//{

				//	auto tempV = nowTransform.GetLocation();
				//	auto tempR = nowTransform.GetRotation().Rotator();
				//	UE_LOG(LogTemp, Log, TEXT("[%x] [?????????] [Input_Timeline_MouseLeftButtonEnterMove] ?????? [??????:%f %f %f] [??????:%f %f %f]"), this,
				//		tempV.X, tempV.Y, tempV.Z,
				//		tempR.Pitch, tempR.Yaw, tempR.Roll
				//	);
				//}

				//{
				//	auto tempV = relationTransform.GetLocation();
				//	auto tempR = relationTransform.GetRotation().Rotator();
				//	UE_LOG(LogTemp, Log, TEXT("[%x] [?????????] [Input_Timeline_MouseLeftButtonEnterMove] ?????? [??????:%f %f %f] [??????:%f %f %f]"), this,
				//		tempV.X, tempV.Y, tempV.Z,
				//		tempR.Pitch, tempR.Yaw, tempR.Roll
				//	);
				//}
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::Input_Base_LeftMove] ?????????????????? "), this);
		}
	}
}

// ???????????? ????????????????????? ??????????????????
void UPlayerSelectedComponent::Input_Timeline_MouseLeftButtonReleased()
{
	TWeakObjectPtr<UTimelinePanelUserWidget> timelinePanel = GetTimelineUI();
	auto selectedView = GetSelectedView();

	UICall_SelectToSetUI();// ??????UI

	selectedView->controller->SetShowSelectedMoveStrategyAll();

	// ?????????????????????,????????????????????????????????????
	if (selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
	{
		SetScaleAxis();
	}
	cvMoveStrategyInfo.isLeftPressed = false;

	OperationResultsSwitch(Operation_Type::Operation_Type_Update, false);
	timelinePanel->UICall_SaveData(false);
}

// ???????????? ????????????????????? ???????????????????????????
void UPlayerSelectedComponent::Input_Timeline_NextFunction(int var)
{
	auto selectedController = GetSelectedView()->controller;
	auto workMode = selectedController->GetWorkMode();
	if (workMode == ESelectedWorkMode::WorldLocation)
	{
		selectedController->SetWorkMode(ESelectedWorkMode::WorldRotator);
	}
	else if (workMode == ESelectedWorkMode::WorldRotator)
	{
		selectedController->SetWorkMode(ESelectedWorkMode::RelativelyLocation);
	}
	else if (workMode == ESelectedWorkMode::RelativelyLocation)
	{
		selectedController->SetWorkMode(ESelectedWorkMode::RelativelyRotator);
	}
	else if (workMode == ESelectedWorkMode::RelativelyRotator)
	{
		selectedController->SetWorkMode(ESelectedWorkMode::WorldLocation);
	}
	SetScaleAxis();
}

/*------------------------------------------------------------------*/

// ??????UI??????????????????
void UPlayerSelectedComponent::UICall_SelectToSetUI()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto selectedView = GetSelectedView();
	auto placeController = GetPlaceController();
	auto placeData = editEntityManage->data;

	auto playerController = Cast<APlayerController>(character->GetController());
	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
	if (hud == nullptr)return;
	auto ui_name_edit_entity = Cast<UEditEntityMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_edit_entity));
	if (ui_name_edit_entity == nullptr)return;
	if (hud->IsUIShow(EUIChildName::ui_name_edit_entity) == false)return;

	// HUD???????????????????????????
	int32 entityId = GetSelectMasterInstance();
	if (entityId != -1)
	{
		auto syncInfo = placeData->GetSyncInfo(entityId);
		auto t1 = syncInfo->transform();
		auto spawnWTransform = UCommonlyObject::PBToUe4Transform(&t1);
		auto t = spawnWTransform;

		ui_name_edit_entity->BP_UI_EditEntity_BaseSyncInfo->SetData(t);
	}
	else
	{
		FTransform t(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
		ui_name_edit_entity->BP_UI_EditEntity_BaseSyncInfo->SetData(t);
	}
}

// ?????????UI?????????????????????
void UPlayerSelectedComponent::UICall_ChoseMaterial()
{

	APlayerController* playerController = Cast<APlayerController>(character->GetController());
	ABaseHUD* hud = Cast<ABaseHUD>(playerController->GetHUD());
	if (hud == nullptr)return;
	UEntityMaterialMainUserWidget* ui_name_entity_material = Cast<UEntityMaterialMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_entity_material));
	if (ui_name_entity_material == nullptr)return;
	if (hud->IsUIShow(EUIChildName::ui_name_entity_material) == false)return;

	ui_name_entity_material->panelUserWidget->OpenUISetSelect();
}

/*------------------------------------------------------------------*/

// ???????????? ????????????
void UPlayerSelectedComponent::OperationResultsSwitch(Operation_Type type, bool mast)
{
	// type 1:?????? 2:?????? 3:?????? 4:?????? 5:??????

	// ????????????
	if (character->myData->GetInputMode() == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		switch (type)
		{
		case Operation_Type::Operation_Type_Update:
			NetCall_EntityUpdate(mast);
			break;
		case Operation_Type::Operation_Type_Copy:
			NetCall_CopyInstance();
			break;
		case Operation_Type::Operation_Type_Crop:
			NetCall_CropInstance();
			break;
		case Operation_Type::Operation_Type_paste:
			NetCall_PasteInstance();

			UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_ui_???);
			UHUDStaticFunc::PushMessage(this, FString::Printf(TEXT("????????????(%d)"), GetSelectMasterInstance()));

			break;
		case Operation_Type::Operation_Type_Delete:
			NetCall_DeletedInstance();

			UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI_??????);
			UHUDStaticFunc::PushMessage(this, FString::Printf(TEXT("????????????(%d)"), GetSelectMasterInstance()));

			break;
		case Operation_Type::Operation_Type_None:
			break;
		}
	}
	// ???????????????
	else if (character->myData->GetInputMode() == ECharacterStatusType::CharacterStatusType_EditTimeline)
	{
		switch (type)
		{
		case Operation_Type::Operation_Type_Update:

			if (IsSelectAxis() || mast)
			{
			}
			break;

		case Operation_Type::Operation_Type_None:
			break;
		}
	}
}

// ???????????? ??????????????????
void UPlayerSelectedComponent::SetSelectMesh(bool isShow, int32 entityId)
{
	if (entityId == -1)
	{
		return;
	}

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto placeController = GetPlaceController();
	auto placeData = editEntityManage->data;
	auto syncInfo = placeData->GetSyncInfo(entityId);
	auto entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(entityId));
	if (entityActor == nullptr)return;

	if (isShow)
	{
		SetSelectHidden(true);

		auto t2 = syncInfo->transform();
		auto tempLocation = UCommonlyObject::PBToUe4Transform(&t2).GetLocation();
		GetSelectedView()->SetActorLocation(tempLocation);
		GetSelectedView()->controller->FrameSetScale();

		entityActor->CreateSelectMesh();
	}
	else
	{
		SetSelectHidden();
		entityActor->DestroySelectMesh();
	}
}

// ???????????? ??????????????????
void UPlayerSelectedComponent::JustLoseMaster()
{
	auto oldMasterId = GetSelectMasterInstance();
	RemoveAllSelected();
	SelectInstance(oldMasterId);
}

// ???????????? ??????????????????
void UPlayerSelectedComponent::Call_SelectEntity(int entityId)
{
	RemoveAllSelected();
	SelectInstance(entityId);
}

/*------------------------------------------------------------------*/

// ???????????? ????????????
void UPlayerSelectedComponent::NetCall_Lock()
{
}

// ???????????? ????????????
void UPlayerSelectedComponent::NetCall_EntityUpdate(bool mast)
{
	// mast true:??????????????????->???????????? false:??????????????????

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(character->GetWorld()->GetAuthGameMode());
	auto selectedView = GetSelectedView();
	auto placeController = GetPlaceController();
	auto editEntityManage = baseGameMode->editEntityManage;

	if (cvMultipleInstanceInfo.selectedList.Num() > 0)
	{
		bool flag = false;
		if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldLocation
			|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyLocation)
		{
			if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationX
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationY
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationZ)
			{
				flag = true;
			}
		}
		else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldRotator
			|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
		{
			if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorPitch
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorYaw
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorRoll)
			{
				flag = true;
			}
		}
		else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::Scale)
		{
			if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleX
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleY
				|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleZ)
			{
				flag = true;
			}
		}
		if (flag || mast)
		{
			TArray<int32> entityIdList;
			for (auto node : cvMultipleInstanceInfo.selectedList)
			{
				auto syncInfo = editEntityManage->data->GetSyncInfo(node.entityId);
				if (syncInfo)
				{
					entityIdList.Add(syncInfo->entityid());
				}
			}
			if (entityIdList.Num() > 0)
			{
				placeController->NetCall_UpdateData(entityIdList);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::NetCall_EntityUpdate] ?????????????????? "), this);
	}
}

// ???????????? ????????????
void UPlayerSelectedComponent::NetCall_CopyInstance()
{
	//// ????????????????????????
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::CopyInstance] ?????? "), this);

	//cvCopyInfo.selectedList.Reset();

	//if (cvMultipleInstanceInfo.selectedList.Num() > 0)
	//{
	//    for (int i = 0; i < cvMultipleInstanceInfo.selectedList.Num(); i++)
	//    {
	//        cvCopyInfo.selectedList.Add(cvMultipleInstanceInfo.selectedList[i].entityId);
	//    }
	//    cvCopyInfo.isCopy = true;
	//}
	//else
	//{
	//    cvCopyInfo.isCopy = false;
	//}
}

// ???????????? ????????????
void UPlayerSelectedComponent::NetCall_CropInstance()
{
}

// ???????????? ????????????
void UPlayerSelectedComponent::NetCall_PasteInstance()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::PasteInstance] ?????? "), this);
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto selectedView = GetSelectedView();
	auto placeController = GetPlaceController();

	cvCopyInfo.selectedList.Reset();

	if (cvMultipleInstanceInfo.selectedList.Num() > 0)
	{
		for (int i = 0; i < cvMultipleInstanceInfo.selectedList.Num(); i++)
		{
			cvCopyInfo.selectedList.Add(cvMultipleInstanceInfo.selectedList[i].entityId);
		}
	}

	// ????????????(?????????????????????????????????????????????)
	TArray<int32> entityIdList;
	for (auto entityId : cvCopyInfo.selectedList)
	{
		auto syncInfo = editEntityManage->data->GetSyncInfo(entityId);
		if (syncInfo)
		{
			entityIdList.Add(syncInfo->entityid());
		}
	}
	if (entityIdList.Num() > 0)
	{
		placeController->NetCall_NewData(entityIdList);
	}
}

// ???????????? ????????????
void UPlayerSelectedComponent::NetCall_DeletedInstance()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::DeletedInstance] ???????????? "), this);
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto selectedView = GetSelectedView();
	auto placeController = GetPlaceController();

	cvCopyInfo.selectedList.Reset();

	if (cvMultipleInstanceInfo.selectedList.Num() > 0)
	{
		for (int i = 0; i < cvMultipleInstanceInfo.selectedList.Num(); i++)
		{
			cvCopyInfo.selectedList.Add(cvMultipleInstanceInfo.selectedList[i].entityId);
		}
	}

	// ????????????(?????????????????????????????????????????????)
	TArray<int32> entityIdList;
	for (auto entityId : cvCopyInfo.selectedList)
	{
		auto syncInfo = editEntityManage->data->GetSyncInfo(entityId);
		if (syncInfo)
		{
			entityIdList.Add(syncInfo->entityid());
		}
	}
	if (entityIdList.Num() > 0)
	{
		placeController->NetCall_RemoveData(entityIdList);
	}
}

/*------------------------------------------------------------------*/

// ??????????????? ????????????????????????
void UPlayerSelectedComponent::GetLineResponset(Operation_Type type, TArray<struct FHitResult>& hitResultList,
	FVector& startLocation, FRotator& mouseRotator)
{
	// type=1 ????????????????????????
	// type=2 ??????????????????
	// type=3 ??????????????????(????????????box,????????????mesh)
	// type=4 ??????????????????

	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetOwner()->GetGameInstance());
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto playerController = Cast<APlayerController>(character->GetController());
	auto camearBoxWTransform = camearBox->GetComponentTransform();
	auto editEntityManage = baseGameMode->editEntityManage;
	auto selectedView = GetSelectedView();

	bool flag = false, flag2 = false;

	float x, y;
	playerController->GetMousePosition(x, y);

	auto lineLen = baseGameMode->syncSize;

	FVector endLocation;
	FVector mouseLocation;
	{
		FVector mouseRotator_;
		playerController->DeprojectMousePositionToWorld(mouseLocation, mouseRotator_);
		mouseRotator = mouseRotator_.Rotation();
		startLocation = mouseLocation;
		FRotator r1 = mouseRotator;
		FVector m1 = r1.Vector() * lineLen * 2;
		endLocation = mouseLocation + m1;
	}

	//bool isComplexCollision = false;
	//if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldRotator
	//	|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
	//{
	//	isComplexCollision = true;
	//}
	FCollisionQueryParams traceParams(FName(TEXT("test")), true, nullptr);

	TArray<AActor*> hitActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	switch (type)
	{
	case Operation_Type_GetLineResponset_find_first:
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
		objectTypes.Add(UEngineTypes::ConvertToObjectType(BuildObject));
		break;
	case Operation_Type_GetLineResponset_find_move_type:
		objectTypes.Add(UEngineTypes::ConvertToObjectType(SelectedInstance));
		break;
	case Operation_Type_GetLineResponset_find_entity:
		objectTypes.Add(UEngineTypes::ConvertToObjectType(BuildObject));
		break;
	case Operation_Type_GetLineResponset_finde_move_location:
		objectTypes.Add(UEngineTypes::ConvertToObjectType(SelectedInstance));
		break;
	}

	bool bBlocked = GetOwner()->GetWorld()->LineTraceMultiByObjectType(hitResultList, startLocation, endLocation,
		objectTypes, traceParams);
}

// ??????????????? ??????????????????box???????????????mesh
bool UPlayerSelectedComponent::GetHitBoxWithMesh(int32 entityId, int32 selectIndex, FVector hitBoxLocation, FVector& hitLocation)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto placeData = editEntityManage->data;

	auto syncInfo = placeData->GetSyncInfo(entityId);
	auto entityActor = Cast<AEditEntityActor>(placeData->GetEntityActor(entityId));

	bool flag = false;

	// ?????????mesh??????
	auto xlsInfo = entityActor->GetChildMeshXlsInfo(selectIndex);
	entityActor->ShowCollisionMeshComponent(true, selectIndex);

	if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_Image)
	{
		hitLocation = hitBoxLocation;
		flag = true;
	}
	// ?????????mesh
	else if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_Mesh)
	{
		TArray< FHitResult> hitResultList;
		FVector startLocation;
		FRotator mouseRotator;
		GetLineResponset(Operation_Type_GetLineResponset_find_entity, hitResultList, startLocation, mouseRotator);

		for (int i = 0; i < hitResultList.Num(); i++)
		{
			auto node = hitResultList[i];
			auto compont = node.GetComponent();
			auto compontName = compont->GetName();
			auto meshComponent = Cast<UStaticMeshComponent>(compont);
			auto meshComponent2 = Cast<UHierarchicalInstancedStaticMeshComponent>(compont);
			// ??????????????????mesh??????????????????instance
			if (meshComponent && meshComponent2 == nullptr)
			{
				if (meshComponent == entityActor->childComponentList[selectIndex]->collisionMeshComponent)
				{
					hitLocation = node.ImpactPoint;
					flag = true;
				}

				break;
			}
		}
	}
	else if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_laser)
	{
		hitLocation = hitBoxLocation;
		flag = true;
	}

	// ??????mesh
	entityActor->ShowCollisionMeshComponent(false, selectIndex);

	// ????????????
	return flag;
}

// ??????????????? ???????????????
bool UPlayerSelectedComponent::GetPlaceInstance(int32& entityId, int32& selectIndex)
{
	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetOwner()->GetGameInstance());
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto playerController = Cast<APlayerController>(character->GetController());
	auto camearBoxWTransform = camearBox->GetComponentTransform();
	auto editEntityManage = baseGameMode->editEntityManage;
	auto placeData = editEntityManage->data;

	bool flag = false;
	TArray< FHitResult> hitResultList;
	FVector startLocation;
	FRotator mouseRotator;
	GetLineResponset(Operation_Type_GetLineResponset_find_entity, hitResultList, startLocation, mouseRotator);

	// ????????????????????????
	entityId = 0;
	selectIndex = 0;
	//if (hitResultList.Num() == 1)
	//{
	//	auto node = hitResultList[0];
	//	auto compont = node.GetComponent();
	//	auto boxComponent = Cast<UBoxComponent>(compont);
	//	if (boxComponent)
	//	{
	//		if (editEntityManage->data->GetEntityIdWithBox(boxComponent, entityId, selectIndex))
	//		{
	//			flag = true;
	//		}
	//	}
	//}
	//else
	{
		// ???????????????????????????????????????????????????

		// ???????????????????????????
		int findCount = 0;
		int32 nearEntityDistance = 0;
		for (int i = 0; i < hitResultList.Num(); i++)
		{
			auto node = hitResultList[i];
			auto compont = node.GetComponent();
			auto compontName = compont->GetName();
			auto boxComponent = Cast<UBoxComponent>(compont);

			if (boxComponent)
			{
				int tempEntityId = 0;
				int tempSelectIndex = 0;
				if (editEntityManage->data->GetEntityIdWithBox(boxComponent, tempEntityId, tempSelectIndex))
				{
					FVector hitLocation;
					if (GetHitBoxWithMesh(tempEntityId, tempSelectIndex, node.ImpactPoint, hitLocation))
					{
						// ???????????????
						auto tempDistance = (hitLocation - startLocation).Size();

						if (i == 0 || entityId == 0 || tempDistance < nearEntityDistance)
						{
							nearEntityDistance = tempDistance;
							entityId = tempEntityId;
							selectIndex = tempSelectIndex;

							flag = true;
							findCount++;
							// ??????????????????
							if (findCount >= 2)
							{
								break;
							}
						}
					}
				}
			}
		}
	}

	if (flag)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::GetPlaceInstance] ???????????? [flag:%d] [%d] "),
			this, flag, entityId);
	}

	return flag;
}

// ??????????????? ???????????????
bool UPlayerSelectedComponent::GetSelectedInstance(ESelectedMoveStrategy& selectedMoveStrategy_)
{
	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetOwner()->GetGameInstance());
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto playerController = Cast<APlayerController>(character->GetController());
	auto camearBoxWTransform = camearBox->GetComponentTransform();
	auto editEntityManage = baseGameMode->editEntityManage;
	auto selectedView = GetSelectedView();

	bool flag = false, flag2 = false;
	TArray< FHitResult> hitResultList;
	GetLineResponset(Operation_Type_GetLineResponset_find_move_type, hitResultList, cvMoveStrategyInfo.shootStartLocation, cvMoveStrategyInfo.shootRotator);

	for (int i = 0; i < hitResultList.Num(); i++)
	{
		auto node = hitResultList[i];
		auto actor = hitResultList[i].GetActor();
		auto compont = hitResultList[i].GetComponent();
		auto compontName = compont->GetName();

		cvMoveStrategyInfo.shootEndLocation = node.ImpactPoint;
		cvMoveStrategyInfo.enterScale = selectedView->data->scale;

		//UE_LOG(LogTemp, Log, TEXT("[%x] [GetHitTransform]  [compontName:%s] "), this, *compontName);
		if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldLocation
			|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyLocation)
		{
			// ????????????
			if (compontName.Equals(TEXT("NameMyPlaneMove")))
			{
				selectedMoveStrategy_ = ESelectedMoveStrategy::planeMove;
				flag2 = GetPanAxisTransform();
				break;
			}
			// ??????x
			if (compontName.Equals(TEXT("NameMyLocationX")))
			{
				selectedMoveStrategy_ = ESelectedMoveStrategy::LocationX;
				flag2 = GetPanAxisTransform();
				break;
			}
			// ??????y
			if (compontName.Equals(TEXT("NameMyLocationY")))
			{
				selectedMoveStrategy_ = ESelectedMoveStrategy::LocationY;
				flag2 = GetPanAxisTransform();
				break;
			}
			// ??????z
			if (compontName.Equals(TEXT("NameMyLocationZ")))
			{
				selectedMoveStrategy_ = ESelectedMoveStrategy::LocationZ;
				flag2 = GetPanAxisTransform();
				break;
			}
		}
		else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldRotator
			|| selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
		{
			// ??????????????????

			// ??????x
			if (compontName.Equals(TEXT("NameMyRotatorPitch")))
			{
				selectedMoveStrategy_ = ESelectedMoveStrategy::RotatorPitch;
				flag2 = true;
				break;
			}
			// ??????y
			if (compontName.Equals(TEXT("NameMyRotatorYaw")))
			{
				selectedMoveStrategy_ = ESelectedMoveStrategy::RotatorYaw;
				flag2 = true;
				break;
			}
			// ??????z
			if (compontName.Equals(TEXT("NameMyRotatorRoll")))
			{
				selectedMoveStrategy_ = ESelectedMoveStrategy::RotatorRoll;
				flag2 = true;
				break;
			}
		}
		else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::Scale)
		{
			// ??????x
			if (compontName.Equals(TEXT("NameMyScaleX")))
			{
				selectedMoveStrategy_ = ESelectedMoveStrategy::ScaleX;
				flag2 = true;
				break;
			}
			// ??????y
			if (compontName.Equals(TEXT("NameMyScaleY")))
			{
				selectedMoveStrategy_ = ESelectedMoveStrategy::ScaleY;
				flag2 = true;
				break;
			}
			// ??????z
			if (compontName.Equals(TEXT("NameMyScaleZ")))
			{
				selectedMoveStrategy_ = ESelectedMoveStrategy::ScaleZ;
				flag2 = true;
				break;
			}
		}
	}

	if (flag2 == false)
	{
		selectedMoveStrategy_ = ESelectedMoveStrategy::None;
		UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::GetSelectedInstance] flag2 err"), this);
	}
	else
	{
		// ????????????????????????
	}

	if (flag2 && selectedMoveStrategy_ != ESelectedMoveStrategy::None)
	{
		flag = true;
	}
	return flag;
}

/*------------------------------------------------------------------*/

// ???????????? ??????????????????????????????
bool UPlayerSelectedComponent::GetHitBoxLocation(FVector start, FVector end, FVector& result)
{
	FCollisionQueryParams traceParams(FName(TEXT("test")), false, nullptr);
	TArray<struct FHitResult> hitResultList;

	TArray<AActor*> hitActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(SelectedInstance));

	bool bBlocked = GetOwner()->GetWorld()->LineTraceMultiByObjectType(hitResultList, start, end, objectTypes,
		traceParams);

	bool flag = false;
	for (auto node : hitResultList)
	{
		auto actor = node.GetActor();
		auto compont = node.GetComponent();
		auto compontName = compont->GetName();

		//UE_LOG(LogTemp, Log, TEXT("[%x] [GetHitBoxLocation]  [compontName:%s] "), this, *compontName);

		// ????????????box
		if (compontName.Equals(TEXT("camearBox")))
		{
			result = node.ImpactPoint;
			flag = true;
			break;
		}
	}

	//if (flag == false)
	//{
	//    UKismetSystemLibrary::DrawDebugLine(GetWorld(), start, end, FColor(0, 255, 255), 10.0f, 0.2f);
	//}

	return flag;
}

// ???????????? ????????????a?????????b ????????????
FTransform UPlayerSelectedComponent::GetTransformWithLine(bool isX, FVector location, FVector line1, FVector line2)
{
	// ????????????????????????????????????????????????????????????????????????
	// line1 ??? line2 ?????????90??????????????????

	FTransform t;

	auto r1 = line1.Rotation();
	t.SetLocation(location);
	t.SetRotation(r1.Quaternion());
	t.SetScale3D(FVector(1, 1, 1));

	// yaw?????????
	FRotator r3(0, 90, 0);
	auto worldRotator = UCalculationObject::RelativeToWorldTransform(t, r3);
	auto v3 = location + worldRotator.Vector() * 100;

	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), v3, location, FColor(222, 22, 22), 10.0f, 2.2f);

	// rool????????????
	auto line3 = v3 - location;
	float angle1 = UCalculationObject::GetVectorAngle(line3, line2);

	int32 angle2;
	int32 angle3;
	FTransform t1, t2;
	{
		t1 = t;
		FRotator r2(0, 0, angle1);
		auto r4 = UCalculationObject::RelativeToWorldTransform(t, r2);
		t1.SetRotation(r4.Quaternion());

		FRotator r5(0, 90, 0);
		auto worldRotator3 = UCalculationObject::RelativeToWorldTransform(t1, r5);
		auto angle2_ = UCalculationObject::GetVectorAngle(worldRotator3.Vector(), line2);
		angle2 = UCalculationObject::RoundFloat(angle2_);
	}
	{
		t2 = t;
		FRotator r2(0, 0, -angle1);
		auto r4 = UCalculationObject::RelativeToWorldTransform(t, r2);
		t2.SetRotation(r4.Quaternion());

		FRotator r5(0, 90, 0);
		auto worldRotator3 = UCalculationObject::RelativeToWorldTransform(t2, r5);
		auto angle3_ = UCalculationObject::GetVectorAngle(worldRotator3.Vector(), line2);
		angle3 = UCalculationObject::RoundFloat(angle3_);
	}
	//UE_LOG(LogTemp, Log, TEXT("[GetTransformWithLine] [??????] [%f][%d][%d] "), angle1, angle2, angle3);
	if (angle2 == 0 || angle2 == 180)t = t1;
	else if (angle3 == 0 || angle3 == 180)t = t2;
	return t;
}

// ???????????? ????????????????????????????????????????????????,????????????n????????????????????????
bool UPlayerSelectedComponent::GetPanAxisTransform()
{
	// type 1:x,2:y,3:z

	auto selectedView = GetSelectedView();
	auto playerController = Cast<APlayerController>(character->GetController());
	auto camearBoxWTransform = camearBox->GetComponentTransform();

	bool flag = false;

	{
		if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationX)
		{
			cvSelectMoveInfo.componentTransform = selectedView->myLocationX->GetComponentTransform();
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationY)
		{
			cvSelectMoveInfo.componentTransform = selectedView->myLocationY->GetComponentTransform();
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationZ)
		{
			cvSelectMoveInfo.componentTransform = selectedView->myLocationZ->GetComponentTransform();
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::planeMove)
		{
			FTransform componentTransform = selectedView->GetActorTransform();
			auto controllerRotator = playerController->GetControlRotation();
			auto v1 = componentTransform.GetLocation();
			FTransform t1(controllerRotator, v1);
			FRotator r1(0, 90, 0);
			auto r2 = UCalculationObject::RelativeToWorldTransform(t1, r1);
			t1.SetRotation(r2.Quaternion());
			cvSelectMoveInfo.componentTransform = t1;
		}
	}
	FVector componentLocation = cvSelectMoveInfo.componentTransform.GetLocation();
	FRotator componentRotator = cvSelectMoveInfo.componentTransform.GetRotation().Rotator();

	//// ?????????????????????, ????????????????????????????????????????????????
	//{
	//    cvMoveStrategyInfo.shootEndLocation = componentTransform.GetLocation() + componentTransform.GetRotation().Vector() * 200;
	//    cvMoveStrategyInfo.shootRotator = (cvMoveStrategyInfo.shootEndLocation - cvMoveStrategyInfo.shootStartLocation).Rotation();
	//}

	int32 lenght = (character->GetActorLocation() - componentLocation).Size() * 2;
	// ??????????????????????????????
	FRotator reverseShootRotator = cvMoveStrategyInfo.shootRotator;
	{
		reverseShootRotator.Pitch += 180;
		reverseShootRotator.Roll = reverseShootRotator.Roll * -1;
	}

	FVector hitLocation1, hitLocation2;
	bool flag1, flag2;
	FVector startLocation1, endLocation1, startLocation2, endLocation2, startLocation4, startLocation5;

	// ??????????????????
	//{
	//    endLocation3 = cvMoveStrategyInfo.shootEndLocation + reverseShootRotator.Vector() * lenght;
	//    flag3 = GetHitBoxLocation(cvMoveStrategyInfo.shootEndLocation, endLocation3, hitLocation3);
	//}

	// ??????
	{
		startLocation1 = cvMoveStrategyInfo.shootEndLocation + componentRotator.Vector() * -50;
		startLocation2 = cvMoveStrategyInfo.shootEndLocation + componentRotator.Vector() * 50;
		startLocation4 = cvMoveStrategyInfo.shootEndLocation + componentRotator.Vector() * 500;
		startLocation5 = cvMoveStrategyInfo.shootEndLocation + componentRotator.Vector() * -500;
		endLocation1 = startLocation1 + reverseShootRotator.Vector() * lenght;
		endLocation2 = startLocation2 + reverseShootRotator.Vector() * lenght;
		flag1 = GetHitBoxLocation(startLocation1, endLocation1, hitLocation1);
		flag2 = GetHitBoxLocation(startLocation2, endLocation2, hitLocation2);

		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation1, endLocation1, FColor(0, 255, 255), 10.0f, 5.2f);
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation2, endLocation2, FColor(0, 255, 255), 10.0f, 5.2f);
	}

	if (flag1 && flag2)
	{
		// ?????????????????????
		{
			FRotator newR = (hitLocation2 - hitLocation1).Rotation();
			cvSelectMoveInfo.mouseArrowTransform.SetLocation(cvMoveStrategyInfo.shootStartLocation);
			cvSelectMoveInfo.mouseArrowTransform.SetRotation(newR.Quaternion());
			cvSelectMoveInfo.mouseArrowTransform.SetScale3D(FVector(1, 1, 1));
			// UpdateTestArrow(TEXT("cvSelectMoveInfo.mouseArrowTransform_"), cvSelectMoveInfo.mouseArrowTransform);
		}

		// ????????????????????????
		{
			// ??????
			auto line1 = startLocation4 - cvMoveStrategyInfo.shootEndLocation;
			auto line2 = cvMoveStrategyInfo.shootStartLocation - cvMoveStrategyInfo.shootEndLocation;
			float angle1 = UCalculationObject::GetVectorAngle(line1, line2);
			angle1 = 90 - angle1;

			// ??????90???
			// cos
			double pi = 3.1415927;
			auto x = (angle1 / 180 * pi);
			auto v1 = sin(x);
			auto lineLenght = v1 * line2.Size();

			// 90????????????
			auto startLocation6 = cvMoveStrategyInfo.shootEndLocation + line1.Rotation().Vector() * lineLenght;

			// ?????????
			auto line3 = startLocation6 - cvMoveStrategyInfo.shootStartLocation;
			float angle2 = UCalculationObject::GetVectorAngle(line1, line3);
			// ?????????
			auto line4 = cvMoveStrategyInfo.shootEndLocation - startLocation6;
			// ?????????2
			auto line5 = componentRotator.Vector() * 100;
			cvSelectMoveInfo.lineFaceTransform = GetTransformWithLine(true, startLocation6, line3, line5);
			//UE_LOG(LogTemp, Log, TEXT("[%x] GetPanAxisTransform [??????] [%f] [%f] [%f] [%f]"), this, angle1, angle2,v1, lineLenght / line2.Size() );
			// UpdateTestArrow(TEXT("arrow2"), cvSelectMoveInfo.lineFaceTransform);

			//// ??????lineFaceTransform?????????3????????????
			//{
			//    //FRotator r2(90, 0, 0);
			//    //auto worldRotator2 = UCalculationObject::RelativeToWorldTransform(cvSelectMoveInfo.lineFaceTransform, r2);
			//    //auto v2 = startLocation6 + worldRotator2.Vector() * 100;
			//    //UKismetSystemLibrary::DrawDebugLine(GetWorld(), v2, startLocation6, FColor(222, 33, 22), 10.0f, 2.2f);

			//    FRotator r3(0,90,0);
			//    auto worldRotator3 = UCalculationObject::RelativeToWorldTransform(cvSelectMoveInfo.lineFaceTransform, r3);
			//    auto v3 = startLocation6 + worldRotator3.Vector() * 100;
			//    UKismetSystemLibrary::DrawDebugLine(GetWorld(), v3, startLocation6, FColor(22, 222, 22), 10.0f, 2.2f);

			//    //auto r4 = line3.Rotation();
			//    //auto v4 = startLocation6 + r4.Vector() * 100;
			//    //UKismetSystemLibrary::DrawDebugLine(GetWorld(), v4, startLocation6, FColor(22, 33, 222), 10.0f, 2.2f);
			//}

			//{
			//    UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation1, hitLocation1, FColor(0, 255, 255), 10.0f, 0.2f);
			//    UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation2, hitLocation2, FColor(0, 255, 255), 10.0f, 0.2f);
			//    UKismetSystemLibrary::DrawDebugLine(GetWorld(), cvMoveStrategyInfo.shootEndLocation, cvMoveStrategyInfo.shootStartLocation, FColor(255, 255, 255), 10.0f, 0.2f);
			//    UKismetSystemLibrary::DrawDebugLine(GetWorld(), hitLocation1, hitLocation2, FColor(0, 255, 255), 10.0f, 0.2f);
			//    UKismetSystemLibrary::DrawDebugLine(GetWorld(), cvMoveStrategyInfo.shootStartLocation, cvSelectMoveInfo.lineFaceTransform.GetLocation(), FColor(0, 255, 255), 10.0f, 0.2f);

			//    UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation1, startLocation4, FColor(0, 255, 255), 10.0f, 0.2f);
			//    UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation1, startLocation5, FColor(0, 255, 255), 10.0f, 0.2f);
			//}
		}
		return true;
	}
	return false;
}

// ???????????? ?????????????????????????????????
bool UPlayerSelectedComponent::GetPanAxisRealData(FVector& move)
{
	// type 1:x;2:y;3:z

	// ??????????????????????????????????????????,?????????????????????.????????????????????????????????????????????????
	// 3??????????????????,??????????????????????????????????????????????????????????????????.

	auto view = GetSelectedView();

	FVector line1, line2, line3;
	float angle1, angle2;
	FVector mouseLocation2;
	FVector tempshootEndLocation;

	// ?????????????????????n?????????
	{
		// ????????????
		FVector mouseLocation;
		FVector mouseRotator_;
		auto playerController = Cast<APlayerController>(character->GetController());
		playerController->DeprojectMousePositionToWorld(mouseLocation, mouseRotator_);
		auto camearBoxWTransform = camearBox->GetComponentTransform();

		// ???????????????????????????????????????
		auto m1 = UCalculationObject::WorldToRelativeTransform(cvSelectMoveInfo.mouseArrowTransform, mouseLocation);
		m1.Z = 0;
		m1.Y = 0;
		auto start = UCalculationObject::RelativeToWorldTransform(cvSelectMoveInfo.mouseArrowTransform, m1);
		mouseLocation2 = start;

		// ??????????????????????????????(??????????????????????????????,??????????????????)
		FRotator r1 = UCalculationObject::WorldToRelativeTransform(cvSelectMoveInfo.lineFaceTransform,
			mouseRotator_.Rotation());
		r1.Pitch = 0;
		r1 = UCalculationObject::RelativeToWorldTransform(cvSelectMoveInfo.lineFaceTransform, r1);

		FVector m2 = r1.Vector() * 100;
		auto end = start + m2;
		line2 = end - start;

		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), start, end, FColor(33, 33, 33), 5.0, 2.2f);
	}

	// ?????????????????????0?????????
	{
		auto l1 = cvSelectMoveInfo.lineFaceTransform.GetLocation();
		auto l2 = l1 + cvSelectMoveInfo.lineFaceTransform.GetRotation().Rotator().Vector() * 100;
		line1 = l2 - l1;
	}

	// ?????????????????????yaw90?????????
	{
		FRotator r1(0, 90, 0);
		auto l1 = r1.Vector() * 100;
		auto l2 = UCalculationObject::RelativeToWorldTransform(cvSelectMoveInfo.lineFaceTransform, l1);
		line3 = l2 - cvSelectMoveInfo.lineFaceTransform.GetLocation();
	}

	// ??????
	{
		angle1 = UCalculationObject::GetVectorAngle(line1, line2);
		angle2 = UCalculationObject::GetVectorAngle(line3, line2);
		//    UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::GetPanAxisRealData]  [angle1:%f] [angle2:%f] "),
		//       this, angle1, angle2);
	}

	// ???????????????
	if (angle1 >= 90)
	{
		return false;
	}

	// ????????????????????????,??????????????????????????????
	{
		auto nowScale = view->data->scale;
		auto enterScale = cvMoveStrategyInfo.enterScale;
		if (nowScale == 1)
		{
			tempshootEndLocation = cvMoveStrategyInfo.shootEndLocation;
		}
		else
		{
			auto rShootEndLocation = UCalculationObject::WorldToRelativeTransform(
				cvSelectMoveInfo.componentTransform, cvMoveStrategyInfo.shootEndLocation);
			auto nexX = nowScale / (enterScale / rShootEndLocation.X);
			rShootEndLocation.X = nexX;
			tempshootEndLocation = UCalculationObject::RelativeToWorldTransform(
				cvSelectMoveInfo.componentTransform, rShootEndLocation);
		}
	}

	// ??????
	{
		auto location1 = UCalculationObject::WorldToRelativeTransform(cvSelectMoveInfo.lineFaceTransform,
			mouseLocation2);

		FRotator r1 = FRotator(0, 0, 0);
		r1.Yaw = 90;
		FVector location2 = r1.Vector() * location1.Y;
		location2 = UCalculationObject::RelativeToWorldTransform(cvSelectMoveInfo.lineFaceTransform, location2);
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), mouseLocation2, location2, FColor(111, 111, 111), 5.0, 2.2f);

		auto newLineFaceTransform = cvSelectMoveInfo.lineFaceTransform;
		newLineFaceTransform.SetLocation(location2);

		float line1Lenght = location1.X;

		// ????????????????????????
		double pi = 3.1415927;
		auto x = (angle1 / 180 * pi);
		auto v2 = tan(x);
		auto line2Lenght = v2 * line1Lenght;

		if (angle2 >= 90)
		{
			FRotator r2 = FRotator(0, 0, 0);
			r2.Yaw = 90;
			FVector pointW = r2.Vector() * line2Lenght;
			pointW = UCalculationObject::RelativeToWorldTransform(newLineFaceTransform, pointW);

			//UKismetSystemLibrary::DrawDebugLine(GetWorld(), mouseLocation2, pointW, FColor(99, 99, 99), 5.0, 2.2f);
			move = pointW - tempshootEndLocation;
		}
		else
		{
			FRotator r2 = FRotator(0, 0, 0);
			r2.Yaw = -90;
			FVector pointW = r2.Vector() * line2Lenght;
			pointW = UCalculationObject::RelativeToWorldTransform(newLineFaceTransform, pointW);
			//UKismetSystemLibrary::DrawDebugLine(GetWorld(), mouseLocation2, pointW, FColor(99, 99, 99), 5.0, 2.2f);
			move = pointW - tempshootEndLocation;
		}
		//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::GetPanAxisRealData]  [s:%s] "), this, *move.ToString());
	}

	return true;
}

// ???????????? ????????????????????????
void UPlayerSelectedComponent::SetMultiplePanAxis()
{
	auto placeController = GetPlaceController();
	auto selectedView = GetSelectedView();
	auto characterLocation = character->GetActorLocation();
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto maxMoveSize = baseGameMode->syncSize;

	FVector move;
	bool flag = false;
	// ??????xyz
	if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationX
		|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationY
		|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::LocationZ)
	{
		flag = GetPanAxisRealData(move);
	}
	// ????????????
	else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::planeMove)
	{
		flag = GetPlaneMoveRealData(move);
	}
	else
	{
		return;
	}

	if (flag == false)
	{
		//UE_LOG(LogTemp, Log, TEXT("[%x] [SetMultiplePanAxis]  ???????????????"), this);
		return;
	}

	int count = cvMultipleInstanceInfo.selectedList.Num();

	{
		auto node = &cvMultipleInstanceInfo.selectedList[count - 1];
		auto newLocation = node->oldSpawnWTransform.GetLocation() + move;
		// ??????????????????????????????
		auto size = (characterLocation - newLocation).Size();
		if (size <= maxMoveSize)
		{
			selectedView->SetActorLocation(newLocation);
			GetSelectedView()->controller->FrameSetScale();
			//UE_LOG(LogTemp, Log, TEXT("[%x] [SetMultiplePanAxis]  move [size:%f] "), this, size);
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("[%x] [SetMultiplePanAxis]  ?????????????????? [size:%f] "), this, size);
			return;
		}
	}

	if (move.Size() > 0.1)
	{
		for (int i = 0; i < count; i++)
		{
			auto node = &cvMultipleInstanceInfo.selectedList[i];
			auto newLocation = node->oldSpawnWTransform.GetLocation() + move;

			if (auto entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(node->entityId)))
			{
				entityActor->editComponent->EntityLocalMoveMain(selectedView->data->selectedWorkMode, newLocation,
					FRotator::ZeroRotator, FVector::ZeroVector);
			}
		}
	}
}

/*------------------------------------------------------------------*/

// ???????????? ?????????????????????????????????
bool UPlayerSelectedComponent::GetPlaneMoveRealData(FVector& move)
{
	auto view = GetSelectedView();
	auto playerController = Cast<APlayerController>(character->GetController());

	FVector line1, line2;
	float angle2, line2Lenght;

	// ???????????????
	FVector mouseLocation;
	// ?????????90??????
	FVector point90Angle;
	// ???????????????
	FVector pointEnd;
	FVector mouseRotator_;

	// ????????????
	playerController->DeprojectMousePositionToWorld(mouseLocation, mouseRotator_);

	// ????????????
	{
		auto start = mouseLocation;
		auto end = start + mouseRotator_ * 400;
		line2 = start - end;

		// UKismetSystemLibrary::DrawDebugLine(GetWorld(), start, end, FColor(33, 33, 33), 5.0, 0.2f);
	}

	// ???????????????
	{
		auto point1 = UCalculationObject::WorldToRelativeTransform(cvSelectMoveInfo.lineFaceTransform, mouseLocation);
		point1.X = 0;
		point90Angle = UCalculationObject::RelativeToWorldTransform(cvSelectMoveInfo.lineFaceTransform, point1);
		line1 = mouseLocation - point90Angle;

		// UKismetSystemLibrary::DrawDebugLine(GetWorld(), mouseLocation, point90Angle,
		//                                    FColor(222, 222, 33), 5.0, 0.2f);
	}

	// ????????????
	{
		auto line1Lenght = line1.Size();
		angle2 = UCalculationObject::GetVectorAngle(line1, line2);
		double pi = 3.1415927;
		auto x = (angle2 / 180 * pi);
		auto v2 = cos(x);
		line2Lenght = line1Lenght / v2;
	}

	// ???????????????.????????????
	{
		pointEnd = mouseLocation + mouseRotator_ * line2Lenght;

		// UKismetSystemLibrary::DrawDebugLine(GetWorld(), mouseLocation, pointEnd, FColor(99, 99, 99), 5.0, 0.2f);
		// UKismetSystemLibrary::DrawDebugLine(GetWorld(), point90Angle, pointEnd, FColor(33, 33, 33), 5.0, 0.2f);
	}

	{
		move = pointEnd - cvMoveStrategyInfo.shootEndLocation;
	}

	return true;
}

/*------------------------------------------------------------------*/

// ?????? ?????????????????????????????????
FRotator UPlayerSelectedComponent::GetSpinRealData()
{
	// ??????????????????????????????????????????

	auto playerController = Cast<APlayerController>(character->GetController());
	auto selectedView = GetSelectedView();

	FTransform spinTransform;
	FVector componentLocation;
	{
		if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorPitch)
		{
			spinTransform = selectedView->myRotatorPitch->GetComponentTransform();
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorYaw)
		{
			spinTransform = selectedView->myRotatorYaw->GetComponentTransform();
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorRoll)
		{
			spinTransform = selectedView->myRotatorRoll->GetComponentTransform();
		}
		componentLocation = spinTransform.GetLocation();
	}

	// ????????????0????????????
	FVector zeroLine;
	{
		zeroLine = cvMoveStrategyInfo.shootEndLocation - componentLocation;
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), componentLocation, cvMoveStrategyInfo.shootEndLocation,
			FColor(33, 33, 33), 1.0, 2.2f);
	}

	// ????????????90????????????
	FVector angle90Line;
	{
		auto rLocation = UCalculationObject::WorldToRelativeTransform(spinTransform,
			cvMoveStrategyInfo.shootEndLocation);
		auto rRotator = rLocation.Rotation();
		rRotator.Yaw += 90;
		auto rLocation2 = rRotator.Vector() * zeroLine.Size();
		auto wLocation = UCalculationObject::RelativeToWorldTransform(spinTransform, rLocation2);
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), componentLocation, wLocation, FColor(33, 33, 33), 1.0, 2.2f);

		angle90Line = wLocation - componentLocation;
	}

	// ??????????????????
	FVector wLocation4;
	{
		// ????????????
		FVector mouseLocation;
		FVector mouseRotator_;
		playerController->DeprojectMousePositionToWorld(mouseLocation, mouseRotator_);
		// ???????????????????????????????????????,??????x?????????
		auto rLocation = UCalculationObject::WorldToRelativeTransform(spinTransform, mouseLocation);
		rLocation.Z = 0;
		// ???????????????
		auto wLocation1 = UCalculationObject::RelativeToWorldTransform(spinTransform, rLocation);
		// ???????????????
		auto wLocation2 = mouseLocation + mouseRotator_.Rotation().Vector() * 100;

		auto line1 = wLocation1 - mouseLocation;
		auto line2 = wLocation2 - mouseLocation;
		// ??????
		auto angle1 = UCalculationObject::GetVectorAngle(line1, line2);
		auto line1Lenght = line1.Size();

		// ????????????????????????2?????????
		double pi = 3.1415927;
		auto x = (angle1 / 180 * pi);
		auto v2 = cos(x);
		auto line2Lenght = line1Lenght / v2;

		// ??????????????????
		FVector wLocation3 = mouseLocation + mouseRotator_.Rotation().Vector() * line2Lenght;

		// ????????????????????????
		auto wRotator = (wLocation3 - componentLocation).Rotation();
		wLocation4 = componentLocation + wRotator.Vector() * zeroLine.Size();

		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), componentLocation, wLocation4, FColor(255, 255, 255), 0.5, 1.0f);
	}

	// ?????????
	FRotator wRotator2(0, 0, 0);
	{
		float angle2;
		auto line4 = wLocation4 - componentLocation;
		angle2 = UCalculationObject::GetVectorAngle(zeroLine, line4);
		auto angle3 = UCalculationObject::GetVectorAngle(angle90Line, line4);
		if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorPitch)
		{
			if (angle3 > 90)
			{
				angle2 = angle2 * -1;
			}
			else
			{
			}
			wRotator2.Pitch = angle2;
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorYaw)
		{
			if (angle3 > 90)
			{
				angle2 = angle2 * -1;
			}
			else
			{
			}
			wRotator2.Yaw = angle2;
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorRoll)
		{
			if (angle3 > 90)
			{
			}
			else
			{
				angle2 = angle2 * -1;
			}
			wRotator2.Roll = angle2;
		}
		//UE_LOG(LogTemp, Log, TEXT("[%x] [SetSingeSpin]  [%f] "), this, angle2);
	}
	return wRotator2;
}

// ?????? ????????????????????????
void UPlayerSelectedComponent::SetMultipleSpinAxis()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto placeController = GetPlaceController();
	auto selectedView = GetSelectedView();
	auto editEntityManage = baseGameMode->editEntityManage;

	if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorPitch
		|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorYaw
		|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorRoll)
	{
	}
	else
	{
		return;
	}

	// ???view????????????????????????
	auto spin = GetSpinRealData();

	//UE_LOG(LogTemp, Log, TEXT("[%x] [SetSingeSpin]  [%s]  "), this, *spin.ToString());
	FRotator newRatator;

	auto count = cvMultipleInstanceInfo.selectedList.Num();
	SelectInstanceInfo* select = &cvMultipleInstanceInfo.selectedList[count - 1];

	// ????????????
	if (selectedView->data->selectedWorkMode == ESelectedWorkMode::WorldRotator)
	{
		auto enterMeshRotator = select->oldSpawnWTransform.GetRotation().Rotator();
		if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorPitch)
		{
			newRatator = UCalculationObject::RotatorInWorldSpin(2, enterMeshRotator, spin.Pitch);
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorYaw)
		{
			newRatator = UCalculationObject::RotatorInWorldSpin(1, enterMeshRotator, spin.Yaw);
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorRoll)
		{
			newRatator = UCalculationObject::RotatorInWorldSpin(3, enterMeshRotator, spin.Roll);
		}
	}
	// ????????????
	else if (selectedView->data->selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
	{
		if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorPitch)
		{
			spin.Pitch = -spin.Pitch;
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorYaw)
		{
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::RotatorRoll)
		{
			spin.Roll = -spin.Roll;
		}
		newRatator = UKismetMathLibrary::TransformRotation(select->oldSpawnWTransform, spin);
	}
	//UE_LOG(LogTemp, Log, TEXT("[%x] [SetSingeSpin]  [%s] [%s] "), this, *spin.ToString(),*newRatator.ToString());

	// entity????????????

	if (auto entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(select->entityId)))
	{
		entityActor->editComponent->EntityLocalMoveMain(selectedView->data->selectedWorkMode, FVector::ZeroVector,
			newRatator, FVector::ZeroVector);
	}

	// ???????????????????????????????????????,?????????????????????
	if (count >= 2)
	{
		auto corePlcaeIndex = cvMultipleInstanceInfo.selectedList[count - 1].entityId;
		auto syncInfo2 = editEntityManage->data->GetSyncInfo(corePlcaeIndex);
		//auto localInfo2 = editEntityManage->data->GetLocalInfo(corePlcaeIndex);

		auto t2 = syncInfo2->transform();
		auto spawnWTransform = UCommonlyObject::PBToUe4Transform(&t2);

		auto coreWTransfor = spawnWTransform;
		for (auto i = 0; i < count - 1; i++)
		{
			auto node = &cvMultipleInstanceInfo.selectedList[i];
			auto syncInfo3 = editEntityManage->data->GetSyncInfo(node->entityId);
			//auto localInfo3 = editEntityManage->data->GetLocalInfo(node->entityId);
			auto newWTransform = UCalculationObject::RelativeToWorldTransform(coreWTransfor, node->rWithCoreTransform);

			if (auto entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(node->entityId)))
			{
				entityActor->editComponent->EntityLocalMoveMain(ESelectedWorkMode::CoreRotator,
					newWTransform.GetLocation(),
					newWTransform.GetRotation().Rotator(), FVector::ZeroVector);
			}
		}
	}
}

/*------------------------------------------------------------------*/

// ?????? ???????????????
float UPlayerSelectedComponent::GetZoomRealData()
{
	auto playerController = Cast<APlayerController>(character->GetController());
	auto selectedView = GetSelectedView();

	FTransform scaleTransform;
	{
		if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleX)
		{
			scaleTransform = selectedView->myScaleX->GetComponentTransform();
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleY)
		{
			scaleTransform = selectedView->myScaleY->GetComponentTransform();
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleZ)
		{
			scaleTransform = selectedView->myScaleZ->GetComponentTransform();
		}
	}

	// ??????????????????????????????
	FRotator reverseShootRotator = cvMoveStrategyInfo.shootRotator;
	{
		reverseShootRotator.Pitch += 180;
		reverseShootRotator.Roll = reverseShootRotator.Roll * -1;
	}

	// ??????
	{
		bool flag1, flag2;
		FVector hitLocation1, hitLocation2;

		FRotator componentRotator = scaleTransform.GetRotation().Rotator();
		FVector startLocation1, startLocation2;
		startLocation1 = cvMoveStrategyInfo.shootEndLocation + componentRotator.Vector() * -50;
		startLocation2 = cvMoveStrategyInfo.shootEndLocation + componentRotator.Vector() * 50;

		int32 lenght = (character->GetActorLocation() - scaleTransform.GetLocation()).Size() * 2;
		auto endLocation1 = startLocation1 + reverseShootRotator.Vector() * lenght;
		auto endLocation2 = startLocation2 + reverseShootRotator.Vector() * lenght;
		flag1 = GetHitBoxLocation(startLocation1, endLocation1, hitLocation1);
		flag2 = GetHitBoxLocation(startLocation2, endLocation2, hitLocation2);

		FRotator newR = (hitLocation2 - hitLocation1).Rotation();
		scaleTransform.SetLocation(cvMoveStrategyInfo.shootStartLocation);
		scaleTransform.SetRotation(newR.Quaternion());
		scaleTransform.SetScale3D(FVector(1, 1, 1));

		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation1, startLocation2, FColor(33, 33, 33), 5.0, 3.2f);
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation1, endLocation1, FColor(33, 33, 33), 5.0, 0.2f);
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation2, endLocation2, FColor(33, 33, 33), 5.0, 0.2f);
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), hitLocation1, hitLocation2, FColor(33, 33, 33), 5.0, 0.2f);
		if (flag1 == false || flag2 == false)
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [GetZoomRealData] err   "), this);
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation1, endLocation1, FColor(33, 33, 33), 5.0,
				0.2f);
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation2, endLocation2, FColor(33, 33, 33), 5.0,
				0.2f);
			return 0.0f;
		}
	}

	float move = 0;
	{
		// ????????????
		FVector mouseLocation;
		FVector mouseRotator_;
		playerController->DeprojectMousePositionToWorld(mouseLocation, mouseRotator_);
		auto rLocation1 = UCalculationObject::WorldToRelativeTransform(scaleTransform, mouseLocation);
		move = rLocation1.X * selectedView->data->scale;
	}

	return move;
}

// ?????? ????????????????????????
void UPlayerSelectedComponent::SetSingeZoom()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto selectedView = GetSelectedView();
	auto placeController = GetPlaceController();
	auto placeData = placeController->data;

	if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleX
		|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleY
		|| cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleZ)
	{
	}
	else
	{
		return;
	}

	auto scale = GetZoomRealData();

	for (auto node : cvMultipleInstanceInfo.selectedList)
	{
		SelectInstanceInfo* select = &node;

		FVector newScale = select->oldSpawnWTransform.GetScale3D();

		auto syncInfo = placeData->GetSyncInfo(select->entityId);
		float tempScale = scale;
		// ???????????????
		auto entityActor = Cast<AEditEntityActor>(placeData->GetEntityActor(select->entityId));
		auto xlsInfo = entityActor->GetChildMeshXlsInfo(0);
		if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_Image)
		{
			tempScale = tempScale / 8.0;
		}
		// ????????????
		else if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_Mesh)
		{
		}

		if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleX)
		{
			newScale.X += tempScale;
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleY)
		{
			newScale.Y += tempScale;
		}
		else if (cvMoveStrategyInfo.selectedMoveStrategy == ESelectedMoveStrategy::ScaleZ)
		{
			newScale.Z += tempScale;
		}
		if (newScale.X < 0.01)
			newScale.X = 0.01;
		if (newScale.Y < 0.01)
			newScale.Y = 0.01;
		if (newScale.Z < 0.01)
			newScale.Z = 0.01;

		if (auto entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(select->entityId)))
		{
			entityActor->editComponent->EntityLocalMoveMain(ESelectedWorkMode::Scale, FVector::ZeroVector,
				FRotator::ZeroRotator, newScale);
		}
	}
}

/*------------------------------------------------------------------*/

// ????????? ?????????????????????????????????
void UPlayerSelectedComponent::SetScaleAxis()
{
	auto selectedView = GetSelectedView();
	auto placeController = GetPlaceController();

	// ????????????????????????????????????
	if (cvMultipleInstanceInfo.selectedList.Num() > 0 && selectedView->data->selectedWorkMode == ESelectedWorkMode::
		Scale)
	{
		auto lastIndex = cvMultipleInstanceInfo.selectedList.Num() - 1;
		auto select = cvMultipleInstanceInfo.selectedList[lastIndex];
		auto entitySyncInfo = placeController->data->GetSyncInfo(select.entityId);

		auto t2 = entitySyncInfo->transform();
		auto spawnWTransform = UCommonlyObject::PBToUe4Transform(&t2);

		selectedView->controller->SetScaleAxis(spawnWTransform);
	}
	else if (cvMultipleInstanceInfo.selectedList.Num() > 0 && selectedView->data->selectedWorkMode == ESelectedWorkMode
		::WorldLocation
		|| cvMultipleInstanceInfo.selectedList.Num() > 0 && selectedView->data->selectedWorkMode == ESelectedWorkMode::
		WorldRotator)
	{
		selectedView->controller->SetWorldLocation();
	}
	else if (cvMultipleInstanceInfo.selectedList.Num() > 0 && selectedView->data->selectedWorkMode == ESelectedWorkMode
		::RelativelyLocation
		|| cvMultipleInstanceInfo.selectedList.Num() > 0 && selectedView->data->selectedWorkMode == ESelectedWorkMode::
		RelativelyRotator)
	{
		auto lastIndex = cvMultipleInstanceInfo.selectedList.Num() - 1;
		auto select = cvMultipleInstanceInfo.selectedList[lastIndex];
		auto entitySyncInfo = placeController->data->GetSyncInfo(select.entityId);

		auto t2 = entitySyncInfo->transform();
		auto spawnWTransform = UCommonlyObject::PBToUe4Transform(&t2);

		selectedView->controller->SetRelativelyLocation(spawnWTransform);
	}
}

// ????????? ??????????????????->???????????????
void UPlayerSelectedComponent::SelectAxis()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto selectedView = GetSelectedView();
	auto placeController = GetPlaceController();

	for (auto i = 0; i < cvMultipleInstanceInfo.selectedList.Num(); i++)
	{
		auto node = &cvMultipleInstanceInfo.selectedList[i];
		auto syncInfo2 = editEntityManage->data->GetSyncInfo(node->entityId);
		auto t2 = syncInfo2->transform();
		auto spawnWTransform = UCommonlyObject::PBToUe4Transform(&t2);
		node->oldSpawnWTransform = spawnWTransform;
	}

	// ??????????????????
	selectedView->controller->SetShowSelectedMoveStrategySingle(cvMoveStrategyInfo.selectedMoveStrategy);

	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::SelectAxis] ???????????? [%d] "), this,
		cvMoveStrategyInfo.selectedMoveStrategy);
}

// ????????? ????????????
void UPlayerSelectedComponent::AddSelected(int32 entityId)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto placeController = GetPlaceController();

	auto syncInfo = editEntityManage->data->GetSyncInfo(entityId);
	auto entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(entityId));
	if (entityActor == nullptr)return;

	SelectInstanceInfo info;
	info.entityId = entityId;
	cvMultipleInstanceInfo.selectedList.Add(info);

	auto t2 = syncInfo->transform();
	auto tempLocation = UCommonlyObject::PBToUe4Transform(&t2).GetLocation();
	GetSelectedView()->SetActorLocation(tempLocation);
	GetSelectedView()->controller->FrameSetScale();

	SetScaleAxis();
	// ????????????
	entityActor->CreateSelectMesh();
}

// ????????? ????????????
void UPlayerSelectedComponent::RemoveSelected(int32 entityId)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto placeController = GetPlaceController();

	auto syncInfo = editEntityManage->data->GetSyncInfo(entityId);
	auto entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(entityId));
	if (entityActor == nullptr)return;

	int32 arrayIndex;
	if (GetMultipleChoiceIndex(entityId, arrayIndex) == false)return;

	cvMultipleInstanceInfo.selectedList.RemoveAt(arrayIndex);

	// ????????????
	entityActor->DestroySelectMesh();

	// ???????????????????????????,???????????????
	if (cvMultipleInstanceInfo.selectedList.Num() == 0)
	{
		SetSelectHidden();
	}
	// ?????????????????????,????????????????????????
	else
	{
		auto lastIndex = cvMultipleInstanceInfo.selectedList.Num() - 1;
		auto entityId2 = cvMultipleInstanceInfo.selectedList[lastIndex].entityId;
		auto syncInfo2 = editEntityManage->data->GetSyncInfo(entityId2);

		auto t2 = syncInfo2->transform();
		auto tempLocation = UCommonlyObject::PBToUe4Transform(&t2).GetLocation();
		GetSelectedView()->SetActorLocation(tempLocation);
		GetSelectedView()->controller->FrameSetScale();

		SetScaleAxis();
	}

	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSelectedComponent::RemoveSelected] ???????????? [%d] "), this, entityId);
}

// ????????? ??????????????????
void UPlayerSelectedComponent::RemoveAllSelected()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto placeController = GetPlaceController();

	GetSelectedView()->SetActorLocation(FVector(0, 0, 0));
	if (cvMultipleInstanceInfo.selectedList.Num() > 0)
	{
		// ????????????
		for (auto node : cvMultipleInstanceInfo.selectedList)
		{
			auto syncInfo = editEntityManage->data->GetSyncInfo(node.entityId);
			auto entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(node.entityId));
			if (entityActor)
			{
				// ????????????
				entityActor->DestroySelectMesh();
			}
		}
		cvMultipleInstanceInfo.selectedList.Reset();
	}
	auto selectedController = baseGameMode->selectedViewActor->controller;
	//selectedController->SetWorkMode(ESelectedWorkMode::None);
}

// ????????? ????????????
void UPlayerSelectedComponent::SelectInstance(int32 entityId)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto editEntityManage = baseGameMode->editEntityManage;
	auto selectedView = GetSelectedView();
	auto placeController = GetPlaceController();
	auto placeData = editEntityManage->data;

	// HUD???????????????????????????
	UICall_SelectToSetUI();

	// ??????
	if (cvMultipleInstanceInfo.isMultipleChoice)
	{
		// ????????????,??????
		int32 arrayIndex;
		if (cvMultipleInstanceInfo.selectedList.Num() > 0 && GetMultipleChoiceIndex(entityId, arrayIndex))
		{
			RemoveSelected(entityId);
		}
		// ??????
		else
		{
			AddSelected(entityId);
		}

		// ???????????????????????????????????????,?????????????????????
		auto count = cvMultipleInstanceInfo.selectedList.Num();
		if (count >= 2)
		{
			auto corePlcaeIndex = cvMultipleInstanceInfo.selectedList[count - 1].entityId;
			auto syncInfo2 = editEntityManage->data->GetSyncInfo(corePlcaeIndex);
			auto t2 = syncInfo2->transform();
			auto spawnWTransform = UCommonlyObject::PBToUe4Transform(&t2);
			auto coreWTransfor = spawnWTransform;
			for (auto i = 0; i < count - 1; i++)
			{
				auto node = &cvMultipleInstanceInfo.selectedList[i];
				auto syncInfo3 = editEntityManage->data->GetSyncInfo(node->entityId);
				auto t3 = syncInfo3->transform();
				auto spawnWTransform2 = UCommonlyObject::PBToUe4Transform(&t3);
				auto thisWTransfor = spawnWTransform2;
				node->rWithCoreTransform = thisWTransfor.GetRelativeTransform(coreWTransfor);
			}
		}
	}
	// ??????
	else
	{
		RemoveAllSelected();
		AddSelected(entityId);
	}

	selectedView->controller->SetIsSelected(true);

	// ???????????????????????????????????????????????????,????????????location
	//if (selectedView->data->selectedWorkMode == ESelectedWorkMode::None)
	//{
	//	Input_SetWorkMode(ESelectedWorkMode::WorldLocation);
	//}
}

// ????????? ????????????
void UPlayerSelectedComponent::SetSelectHidden(bool isShow)
{
	auto selectedView = GetSelectedView();
	if (isShow)
	{
		selectedView->controller->SetIsSelected(true);
	}
	else
	{
		selectedView->controller->SetIsSelected(false);
		RemoveAllSelected();
		UICall_SelectToSetUI();// HUD???????????????????????????
	}
}

/*------------------------------------------------------------------*/

// ?????? ??????????????????
UArrowComponent* UPlayerSelectedComponent::CreateTestArrow(FString name, FTransform t)
{
	auto arrow = NewObject<UArrowComponent>(GetSelectedView().Get(), *name);

	t.SetScale3D(FVector(1, 4, 0.2f));

	arrow->RegisterComponent();
	arrow->AttachToComponent(GetSelectedView()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	arrow->SetCollisionObjectType(ECC_WorldStatic);
	arrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	arrow->SetWorldTransform(t);
	arrow->SetHiddenInGame(false);
	return arrow;
}

// ?????? ??????????????????
void UPlayerSelectedComponent::DestroyTestArrow(UArrowComponent* arrow)
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

// ?????? ????????????
void UPlayerSelectedComponent::UpdateTestArrow(FString name, FTransform t)
{
	if (arrowMap.Find(name))
	{
		DestroyTestArrow(arrowMap[name]);
		arrowMap.Remove(name);
	}
	auto arrow = CreateTestArrow(name, t);
	arrowMap.Add(name, arrow);
}

/*------------------------------------------------------------------*/
