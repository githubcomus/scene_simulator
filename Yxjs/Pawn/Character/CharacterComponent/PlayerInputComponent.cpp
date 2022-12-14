// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInputComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerPlaceComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerMaterialComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Net/NetMainComponent.h"
#include "Core/BaseGameInstance.h"
#include "UI/BaseHud.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/MyPython/MyPythonActor.h"
#include "FunctionalModule/OverallSituation/OverallSituationView.h"
#include "Pawn/Character/CharacterComponent/PlayerPythonComponent.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Yxjs/GameMode/BaseGameMode.h"
#include "PlayerPreviewComponent.h"
#include "PlayerEditEntityComponent.h"
#include "PlayerTimelineComponent.h"
#include "PlayerCommandComponent.h"
#include "Yxjs/UI/Game/Character/Main/CharacterMainUserWidget.h"
#include "../../../GameMode/Component/BaseGameModeLevelManage.h"

 /*/////////////////////////////////////////////////////////////*/
 // data
 /*/////////////////////////////////////////////////////////////*/

//
ABaseHUD* UPlayerInputComponent::GetHUD()
{
	if (view.IsValid() == false)return nullptr;
	auto playerController = Cast<APlayerController>(view->Controller);
	auto hud = Cast<ABaseHUD>(playerController->GetHUD());;
	return hud;
}

//
UCharacterControllerComponent* UPlayerInputComponent::GetControllerComponent()
{
	if (view.IsValid() == false)return nullptr;
	return view->myController;
}

//
UPlayerSelectedComponent* UPlayerInputComponent::GetSelectedComponent()
{
	if (view.IsValid() == false)return nullptr;
	return view->selectComponent;
}

//
UCharacterDataComponent* UPlayerInputComponent::GetDataComponent()
{
	if (view.IsValid() == false)return nullptr;
	return view->myData;
}

// ???????????????????????????
bool UPlayerInputComponent::CanInputToGame()
{
	// ???????????????,???python???????????????

	auto dataComp = GetDataComponent();
	auto myPythonActor = dataComp->gameMode->editEntityManage->myPythonActor;
	if (myPythonActor == nullptr)return true;
	auto selectMode = inputMode;
	if (selectMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		// ????????????python??????????????????game??????
		if (view->pythonComponent->isOpenUI)
		{
			return false;
		}
	}
	return true;
}

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

UPlayerInputComponent::UPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

//
void UPlayerInputComponent::Start()
{
	view = Cast<ABaseCharacter>(GetOwner());
}

//
void UPlayerInputComponent::End()
{
}

// ????????????
void UPlayerInputComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("TurnRate", this, &UPlayerInputComponent::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &UPlayerInputComponent::LookUpAtRate);

	PlayerInputComponent->BindAxis("Turn", this, &UPlayerInputComponent::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &UPlayerInputComponent::LookUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &UPlayerInputComponent::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &UPlayerInputComponent::MoveRight);

	PlayerInputComponent->BindAxis("MoveUp", this, &UPlayerInputComponent::MoveUp);
	PlayerInputComponent->BindAxis("MoveDown", this, &UPlayerInputComponent::MoveDown);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &UPlayerInputComponent::Space_Pressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &UPlayerInputComponent::Space_Released);

	PlayerInputComponent->BindAction("MouseLeft", IE_Pressed, this, &UPlayerInputComponent::MouseLeft_Pressed);
	PlayerInputComponent->BindAction("MouseLeft", IE_Released, this, &UPlayerInputComponent::MouseLeft_Released);

	PlayerInputComponent->BindAction("MouseRight", IE_Pressed, this, &UPlayerInputComponent::MouseRight_Pressed);
	PlayerInputComponent->BindAction("MouseRight", IE_Released, this, &UPlayerInputComponent::MouseRight_Released);

	PlayerInputComponent->BindAction("MousePrevious", IE_Pressed, this, &UPlayerInputComponent::MousePrevious_Pressed);
	PlayerInputComponent->BindAction("MousePrevious", IE_Released, this,
		&UPlayerInputComponent::MousePrevious_Released);

	PlayerInputComponent->BindAction("MouseNext", IE_Pressed, this, &UPlayerInputComponent::MouseNext_Pressed);
	PlayerInputComponent->BindAction("MouseNext", IE_Released, this, &UPlayerInputComponent::MouseNext_Released);

	PlayerInputComponent->BindAction("MiddleMouseButton", IE_Pressed, this, &UPlayerInputComponent::MiddleMouseButton_Pressed);
	PlayerInputComponent->BindAction("MiddleMouseButton", IE_Released, this, &UPlayerInputComponent::MiddleMouseButton_Released);


	PlayerInputComponent->BindAction("1", IE_Pressed, this, &UPlayerInputComponent::Num1_Pressed);
	PlayerInputComponent->BindAction("1", IE_Released, this, &UPlayerInputComponent::Num1_Released);

	PlayerInputComponent->BindAction("2", IE_Pressed, this, &UPlayerInputComponent::Num2_Pressed);
	PlayerInputComponent->BindAction("2", IE_Released, this, &UPlayerInputComponent::Num2_Released);

	PlayerInputComponent->BindAction("3", IE_Pressed, this, &UPlayerInputComponent::Num3_Pressed);
	PlayerInputComponent->BindAction("3", IE_Released, this, &UPlayerInputComponent::Num3_Released);

	PlayerInputComponent->BindAction("4", IE_Pressed, this, &UPlayerInputComponent::Num4_Pressed);
	PlayerInputComponent->BindAction("4", IE_Released, this, &UPlayerInputComponent::Num4_Released);

	PlayerInputComponent->BindAction("5", IE_Pressed, this, &UPlayerInputComponent::Num5_Pressed);
	PlayerInputComponent->BindAction("5", IE_Released, this, &UPlayerInputComponent::Num5_Released);

	PlayerInputComponent->BindAction("6", IE_Pressed, this, &UPlayerInputComponent::Num6_Pressed);
	PlayerInputComponent->BindAction("6", IE_Released, this, &UPlayerInputComponent::Num6_Released);

	PlayerInputComponent->BindAction("7", IE_Pressed, this, &UPlayerInputComponent::Num7_Pressed);
	PlayerInputComponent->BindAction("7", IE_Released, this, &UPlayerInputComponent::Num7_Released);

	PlayerInputComponent->BindAction("8", IE_Pressed, this, &UPlayerInputComponent::Num8_Pressed);
	PlayerInputComponent->BindAction("8", IE_Released, this, &UPlayerInputComponent::Num8_Released);

	PlayerInputComponent->BindAction("9", IE_Pressed, this, &UPlayerInputComponent::Num9_Pressed);
	PlayerInputComponent->BindAction("9", IE_Released, this, &UPlayerInputComponent::Num9_Released);

	PlayerInputComponent->BindAction("0", IE_Pressed, this, &UPlayerInputComponent::Num0_Pressed);
	PlayerInputComponent->BindAction("0", IE_Released, this, &UPlayerInputComponent::Num0_Released);

	PlayerInputComponent->BindAction("Q", IE_Pressed, this, &UPlayerInputComponent::Q_Pressed);
	PlayerInputComponent->BindAction("Q", IE_Released, this, &UPlayerInputComponent::Q_Released);

	PlayerInputComponent->BindAction("W", IE_Pressed, this, &UPlayerInputComponent::W_Pressed);
	PlayerInputComponent->BindAction("W", IE_Released, this, &UPlayerInputComponent::W_Released);

	PlayerInputComponent->BindAction("E", IE_Pressed, this, &UPlayerInputComponent::E_Pressed);
	PlayerInputComponent->BindAction("E", IE_Released, this, &UPlayerInputComponent::E_Released);

	PlayerInputComponent->BindAction("R", IE_Pressed, this, &UPlayerInputComponent::R_Pressed);
	PlayerInputComponent->BindAction("R", IE_Released, this, &UPlayerInputComponent::R_Released);

	PlayerInputComponent->BindAction("U", IE_Pressed, this, &UPlayerInputComponent::U_Pressed);
	PlayerInputComponent->BindAction("U", IE_Released, this, &UPlayerInputComponent::U_Released);

	PlayerInputComponent->BindAction("I", IE_Pressed, this, &UPlayerInputComponent::I_Pressed);
	PlayerInputComponent->BindAction("I", IE_Released, this, &UPlayerInputComponent::I_Released);

	PlayerInputComponent->BindAction("O", IE_Pressed, this, &UPlayerInputComponent::O_Pressed);
	PlayerInputComponent->BindAction("O", IE_Released, this, &UPlayerInputComponent::O_Released);

	PlayerInputComponent->BindAction("P", IE_Pressed, this, &UPlayerInputComponent::P_Pressed);
	PlayerInputComponent->BindAction("P", IE_Released, this, &UPlayerInputComponent::P_Released);

	PlayerInputComponent->BindAction("Z", IE_Pressed, this, &UPlayerInputComponent::Z_Pressed);
	PlayerInputComponent->BindAction("Z", IE_Released, this, &UPlayerInputComponent::Z_Released);

	PlayerInputComponent->BindAction("X", IE_Pressed, this, &UPlayerInputComponent::X_Pressed);
	PlayerInputComponent->BindAction("X", IE_Released, this, &UPlayerInputComponent::X_Released);

	PlayerInputComponent->BindAction("C", IE_Pressed, this, &UPlayerInputComponent::C_Pressed);
	PlayerInputComponent->BindAction("C", IE_Released, this, &UPlayerInputComponent::C_Released);

	PlayerInputComponent->BindAction("V", IE_Pressed, this, &UPlayerInputComponent::V_Pressed);
	PlayerInputComponent->BindAction("V", IE_Released, this, &UPlayerInputComponent::V_Released);

	PlayerInputComponent->BindAction("F", IE_Pressed, this, &UPlayerInputComponent::F_Pressed);
	PlayerInputComponent->BindAction("F", IE_Released, this, &UPlayerInputComponent::F_Released);

	PlayerInputComponent->BindAction("B", IE_Pressed, this, &UPlayerInputComponent::B_Pressed);
	PlayerInputComponent->BindAction("B", IE_Released, this, &UPlayerInputComponent::B_Released);

	PlayerInputComponent->BindAction("Ctrl", IE_Pressed, this, &UPlayerInputComponent::Ctrl_Pressed);
	PlayerInputComponent->BindAction("Ctrl", IE_Released, this, &UPlayerInputComponent::Ctrl_Released);

	PlayerInputComponent->BindAction("Backspace", IE_Pressed, this, &UPlayerInputComponent::Backspace_Pressed);
	PlayerInputComponent->BindAction("Backspace", IE_Released, this, &UPlayerInputComponent::Backspace_Released);

	PlayerInputComponent->BindAction("Tab", IE_Pressed, this, &UPlayerInputComponent::Tab_Pressed);
	PlayerInputComponent->BindAction("Tab", IE_Released, this, &UPlayerInputComponent::Tab_Released);

	PlayerInputComponent->BindAction("Shift", IE_Pressed, this, &UPlayerInputComponent::LeftShift_Pressed);
	PlayerInputComponent->BindAction("Shift", IE_Released, this, &UPlayerInputComponent::LeftShift_Released);
}

// ??????????????????
void UPlayerInputComponent::SetInputMode(ECharacterStatusType inputMode_)
{
	if (inputMode_ != inputMode)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [???????????????] [SetInputMode] [??????????????????] [%d->%d] "), this, inputMode, inputMode_);
		inputMode = inputMode_;
	}
}

//
void UPlayerInputComponent::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	Cast<ACharacter>(GetOwner())->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//
void UPlayerInputComponent::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	Cast<ACharacter>(GetOwner())->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Turn ???????????????,???tick????????????
void UPlayerInputComponent::Turn(float val)
{
	if ((val != 0.0f))
	{
		//UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [Turn] [????????????] [time:%f] [val:%f]  "), this, GetWorld()->TimeSeconds, val);

		auto dataComp = GetDataComponent();
		// ????????????????????????
		if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
		{
			if (dataComp->inputKeyInfo.mouseRightEnter)
			{
				Cast<ACharacter>(GetOwner())->AddControllerYawInput(val);
			}
			else
			{
				view->selectComponent->Input_Base_LeftMove();
			}
		}
		// ???????????????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
		{
			if (dataComp->inputKeyInfo.mouseRightEnter)
			{
				Cast<ACharacter>(GetOwner())->AddControllerYawInput(val);
			}
			else
			{
				view->selectComponent->Input_Timeline_MouseLeftButtonEnterMove();
			}
		}
		// ??????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_Preview)
		{
			if (dataComp->inputKeyInfo.mouseRightEnter)
			{
				Cast<ACharacter>(GetOwner())->AddControllerYawInput(val);
			}
		}
		// ?????????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_LockEntity)
		{
			if (dataComp->inputKeyInfo.mouseRightEnter)
			{
				Cast<ACharacter>(GetOwner())->AddControllerYawInput(val);
			}
		}
		// ??????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_None)
		{
			Cast<ACharacter>(GetOwner())->AddControllerYawInput(val);
		}
		// ????????????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_Player_Mouse)
		{
			if (dataComp->inputKeyInfo.mouseRightEnter)
			{
				Cast<ACharacter>(GetOwner())->AddControllerYawInput(val);
			}
		}
	}
}

// LookUp ???????????????,???tick????????????
void UPlayerInputComponent::LookUp(float val)
{
	if ((val != 0.0f))
	{
		auto dataComp = GetDataComponent();
		// ????????????????????????
		if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
		{
			if (dataComp->inputKeyInfo.mouseRightEnter)
			{
				Cast<ACharacter>(GetOwner())->AddControllerPitchInput(val);
			}
			else
			{
				view->selectComponent->Input_Base_LeftMove();
			}
		}
		// ???????????????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
		{
			if (dataComp->inputKeyInfo.mouseRightEnter)
			{
				Cast<ACharacter>(GetOwner())->AddControllerPitchInput(val);
			}
			else
			{
				view->selectComponent->Input_Timeline_MouseLeftButtonEnterMove();
			}
		}
		// ??????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_Preview)
		{
			if (dataComp->inputKeyInfo.mouseRightEnter)
			{
				Cast<ACharacter>(GetOwner())->AddControllerPitchInput(val);
			}
		}
		// ?????????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_LockEntity)
		{

			if (dataComp->inputKeyInfo.mouseRightEnter)
			{
				Cast<ACharacter>(GetOwner())->AddControllerPitchInput(val);
			}
		}
		// ??????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_None)
		{
			Cast<ACharacter>(GetOwner())->AddControllerPitchInput(val);
		}
		// ????????????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_Player_Mouse)
		{
			if (dataComp->inputKeyInfo.mouseRightEnter)
			{
				Cast<ACharacter>(GetOwner())->AddControllerPitchInput(val);
			}
		}
	}
}

// MoveForward ????????????,???tick????????????
void UPlayerInputComponent::MoveForward(float Value)
{
	if ((Value != 0.0f))
	{
		auto dataComp = GetDataComponent();
		auto character = Cast<ACharacter>(GetOwner());
		auto controller = character->Controller;
		auto newLocation = character->GetActorLocation() + controller->GetControlRotation().Vector() * Value *
			dataComp->editMoveSpeed;
		// find out which way is forward
		const FRotator Rotation = Cast<ACharacter>(GetOwner())->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// ??????????????????
		auto selectMode = inputMode;
		if (selectMode == ECharacterStatusType::CharacterStatusType_EditEntity
			|| selectMode == ECharacterStatusType::CharacterStatusType_EditTimeline
			|| selectMode == ECharacterStatusType::CharacterStatusType_LockEntity)
		{

			if (dataComp->inputKeyInfo.mouseLeftEnter == false)// ??????????????????
			{
				character->SetActorLocation(newLocation);
			}
		}
		// ??????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_Preview)
		{
			character->SetActorLocation(newLocation);
		}
		// ??????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_None)
		{
			Cast<ACharacter>(GetOwner())->AddMovementInput(Direction, Value);
		}
		// ????????????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_Player_Mouse)
		{
			Cast<ACharacter>(GetOwner())->AddMovementInput(Direction, Value);
		}
	}
}

// MoveRight ????????????,???tick????????????
void UPlayerInputComponent::MoveRight(float Value)
{
	if ((Value != 0.0f))
	{
		auto dataComp = GetDataComponent();
		const FRotator Rotation = Cast<ACharacter>(GetOwner())->GetControlRotation();// find out which way is right
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);// get right vector
		auto character = Cast<ACharacter>(GetOwner());
		auto controller = character->Controller;
		auto newLocation = character->GetActorLocation() + Direction * Value * dataComp->editMoveSpeed;

		// ??????????????????
		auto selectMode = inputMode;
		if (selectMode == ECharacterStatusType::CharacterStatusType_EditEntity
			|| selectMode == ECharacterStatusType::CharacterStatusType_EditTimeline
			|| selectMode == ECharacterStatusType::CharacterStatusType_LockEntity)
		{

			if (dataComp->inputKeyInfo.mouseLeftEnter == false) // ??????????????????
			{
				character->SetActorLocation(newLocation);
			}
		}
		// ??????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_Preview)
		{
			character->SetActorLocation(newLocation);
		}
		// ??????/????????????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_None)
		{
			Cast<ACharacter>(GetOwner())->AddMovementInput(Direction, Value);
		}
		// ????????????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_Player_Mouse)
		{
			Cast<ACharacter>(GetOwner())->AddMovementInput(Direction, Value);
		}
	}
}

// Q??????
void UPlayerInputComponent::MoveUp(float Value)
{
	if ((Value != 0.0f))
	{
		auto dataComp = GetDataComponent();
		auto character = Cast<ACharacter>(GetOwner());
		auto controller = character->Controller;
		auto Direction = FVector::DownVector * Value * dataComp->editMoveSpeed;
		auto newLocation = character->GetActorLocation() + Direction;
		// ??????????????????
		auto selectMode = inputMode;
		if (selectMode == ECharacterStatusType::CharacterStatusType_EditEntity
			|| selectMode == ECharacterStatusType::CharacterStatusType_EditTimeline
			|| selectMode == ECharacterStatusType::CharacterStatusType_LockEntity)
		{
			// ??????????????????
			if (dataComp->inputKeyInfo.mouseLeftEnter == false)
			{
				character->SetActorLocation(newLocation);
			}
		}
		// ??????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_Preview)
		{
			character->SetActorLocation(newLocation);
		}
		// ??????
		else
		{
		}
	}
}

// E??????
void UPlayerInputComponent::MoveDown(float Value)
{
	if ((Value != 0.0f))
	{
		auto dataComp = GetDataComponent();
		auto character = Cast<ACharacter>(GetOwner());
		auto controller = character->Controller;
		auto Direction = FVector::DownVector * Value * dataComp->editMoveSpeed;
		auto newLocation = character->GetActorLocation() + Direction;

		// ??????????????????
		auto selectMode = inputMode;
		if (selectMode == ECharacterStatusType::CharacterStatusType_EditEntity
			|| selectMode == ECharacterStatusType::CharacterStatusType_EditTimeline
			|| selectMode == ECharacterStatusType::CharacterStatusType_LockEntity
			)
		{
			// ??????????????????
			if (dataComp->inputKeyInfo.mouseLeftEnter == false)
			{
				character->SetActorLocation(newLocation);
			}
		}
		// ??????
		else if (inputMode == ECharacterStatusType::CharacterStatusType_Preview)
		{
			character->SetActorLocation(newLocation);
		}
		// ??????
		else
		{
		}
	}
}

// ????????????
void UPlayerInputComponent::Space_Pressed()
{
	if (CanInputToGame() == false)return;
	auto dataComp = GetDataComponent();
	auto selectMode = inputMode;
	if (selectMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
	}
	else if (selectMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
	{
	}
	else
	{
		Cast<ACharacter>(GetOwner())->Jump();
	}
}

// ????????????
void UPlayerInputComponent::Space_Released()
{
	if (CanInputToGame() == false)return;
	auto dataComp = GetDataComponent();

	auto selectMode = inputMode;
	if (selectMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
	}
	else if (selectMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
	{
	}
	else
	{
		Cast<ACharacter>(GetOwner())->StopJumping();
	}
}

// ??????????????????
void UPlayerInputComponent::MouseLeft_Pressed()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [MouseLeft_Pressed] [??????????????????] [time:%f]   "), this, GetWorld()->TimeSeconds);
	if (CanInputToGame() == false)return;
	auto dataComp = GetDataComponent();
	dataComp->inputKeyInfo.mouseLeftEnter = true;

	auto character = Cast<ACharacter>(GetOwner());
	auto playerController = Cast<APlayerController>(character->GetController());

	auto selectMode = inputMode;

	// ????????????????????????
	if (dataComp->inputKeyInfo.mouseRightEnter || dataComp->inputKeyInfo.shiftEnter)
	{
		return;
	}

	// ??????????????????
	if (selectMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		view->selectComponent->Input_Base_LeftPressed();
	}
	else if (selectMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
	{
		view->selectComponent->Input_Timeline_MouseLeftButtonPressed();
	}
	// ?????????
	else if (inputMode == ECharacterStatusType::CharacterStatusType_LockEntity)
	{
	}
}

// ??????????????????
void UPlayerInputComponent::MouseLeft_Released()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [MouseLeft_Released] [??????????????????] [time:%f]   "), this, GetWorld()->TimeSeconds);
	if (CanInputToGame() == false)return;
	auto character = Cast<ACharacter>(GetOwner());
	auto playerController = Cast<APlayerController>(character->GetController());
	auto dataComp = GetDataComponent();

	auto selectMode = inputMode;
	// ????????????????????????
	if (dataComp->inputKeyInfo.mouseRightEnter || dataComp->inputKeyInfo.shiftEnter)
	{
		return;
	}

	dataComp->inputKeyInfo.mouseLeftEnter = false;

	// ??????????????????
	if (selectMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		view->selectComponent->Input_Base_LeftReleased();
	}
	else if (selectMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
	{
		view->selectComponent->Input_Timeline_MouseLeftButtonReleased();
	}
	else if (selectMode == ECharacterStatusType::CharacterStatusType_Preview)
	{
		view->previewComponent->OpenClosePerview(false, nullptr, false);
		inputMode = ECharacterStatusType::CharacterStatusType_EditEntity; // ??????????????????????????????
	}
	// ?????????
	else if (inputMode == ECharacterStatusType::CharacterStatusType_LockEntity)
	{
	}
}

// ??????????????????
void UPlayerInputComponent::MouseRight_Pressed()
{
	auto character = Cast<ACharacter>(GetOwner());
	auto playerController = Cast<APlayerController>(character->GetController());
	auto dataComp = GetDataComponent();

	// ??????????????????
	if (dataComp->inputKeyInfo.mouseLeftEnter)
	{
		MouseLeft_Released();
	}

	dataComp->inputKeyInfo.mouseRightEnter = true;

	// ?????????????????? ??????????????????
	auto selectMode = inputMode;
	if (selectMode == ECharacterStatusType::CharacterStatusType_EditEntity
		|| selectMode == ECharacterStatusType::CharacterStatusType_EditTimeline
		|| selectMode == ECharacterStatusType::CharacterStatusType_LockEntity
		|| selectMode == ECharacterStatusType::CharacterStatusType_Preview
		|| selectMode == ECharacterStatusType::CharacterStatusType_Player_Mouse)
	{
		// ??????????????????
		playerController->GetMousePosition(dataComp->inputKeyInfo.mousePoint.X,
			dataComp->inputKeyInfo.mousePoint.Y);

		// ?????????????????????
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(playerController);
		playerController->bShowMouseCursor = false;
	}
	// ????????????
	else
	{
	}
}

// ??????????????????
void UPlayerInputComponent::MouseRight_Released()
{
	//if (CanInputToGame() == false)return;
	auto character = Cast<ACharacter>(GetOwner());
	auto playerController = Cast<APlayerController>(character->GetController());
	auto dataComp = GetDataComponent();
	dataComp->inputKeyInfo.mouseRightEnter = false;

	// ?????????????????? ??????????????????
	auto selectMode = inputMode;
	if (selectMode == ECharacterStatusType::CharacterStatusType_EditEntity
		|| selectMode == ECharacterStatusType::CharacterStatusType_EditTimeline
		|| selectMode == ECharacterStatusType::CharacterStatusType_LockEntity
		|| selectMode == ECharacterStatusType::CharacterStatusType_Preview
		|| selectMode == ECharacterStatusType::CharacterStatusType_Player_Mouse
		)
	{
		playerController->SetMouseLocation(dataComp->inputKeyInfo.mousePoint.X,
			dataComp->inputKeyInfo.mousePoint.Y);

		// UI??????????????????
		playerController->bShowMouseCursor = true;

		EMouseLockMode InMouseLockMode = EMouseLockMode::DoNotLock;
		bool bHideCursorDuringCapture = false;
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(InMouseLockMode);
		InputMode.SetHideCursorDuringCapture(bHideCursorDuringCapture);
		playerController->SetInputMode(InputMode);
	}
	else
	{
	}
}

// ??????????????????
void UPlayerInputComponent::MousePrevious_Pressed()
{
	if (CanInputToGame() == false)return;
	auto dataComp = GetDataComponent();
	if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		view->editEntityComponent->Function_NextFunction(-1);
	}
	else if (inputMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
	{
		view->selectComponent->Input_Timeline_NextFunction(-1);
	}
}

// ??????????????????
void UPlayerInputComponent::MousePrevious_Released()
{
	if (CanInputToGame() == false)return;
}

// ??????????????????
void UPlayerInputComponent::MouseNext_Pressed()
{
	if (CanInputToGame() == false)return;
	auto dataComp = GetDataComponent();
	if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		view->editEntityComponent->Function_NextFunction(1);
	}
	else if (inputMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
	{
		view->selectComponent->Input_Timeline_NextFunction(1);
	}
}

// ??????????????????
void UPlayerInputComponent::MouseNext_Released()
{
	if (CanInputToGame() == false)return;
}

// 
void UPlayerInputComponent::Num1_Pressed()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(true, 1);

		//baseGameMode->levelManage->ChangeSkyLevel();

	}
	else if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		//view->materialComponent->CallInput_SetMaterial(1000);
	}
}

//
void UPlayerInputComponent::Num1_Released()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(false, 1);
	}
}

//
void UPlayerInputComponent::Num2_Pressed()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(true, 2);
	}
}

//
void UPlayerInputComponent::Num2_Released()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(false, 2);
	}
}

//
void UPlayerInputComponent::Num3_Pressed()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(true, 3);
	}
}

//
void UPlayerInputComponent::Num3_Released()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(false, 3);
	}
}

// 
void UPlayerInputComponent::Num4_Pressed()
{
	if (CanInputToGame() == false)return;
	//7000000
	//int var = UKismetMathLibrary::RandomFloatInRange(10, 100);

	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(true, 4);
	}
}

//
void UPlayerInputComponent::Num4_Released()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(false, 4);
	}
}

//
void UPlayerInputComponent::Num5_Pressed()
{
	if (CanInputToGame() == false)return;
	//view->materialComponent->CallInput_SetMaterial(0);
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(true, 5);
	}

	//auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//FKey key(TEXT("LeftMouseButton"));
	//playerController->InputKey(key, EInputEvent::IE_Pressed, 1, true);
	//playerController->InputKey(key, EInputEvent::IE_Released, 1, true);

}

//
void UPlayerInputComponent::Num5_Released()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(false, 5);
	}


	//auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//FKey key(TEXT("LeftMouseButton"));
	//playerController->InputKey(key, EInputEvent::IE_Pressed, 1, true);
	//playerController->InputKey(key, EInputEvent::IE_Released, 1, true);
}

//
void UPlayerInputComponent::Num6_Pressed()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(true, 6);
	}
}

//
void UPlayerInputComponent::Num6_Released()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(false, 6);
	}
}

//
void UPlayerInputComponent::Num7_Pressed()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(true, 7);
	}
}

void UPlayerInputComponent::Num7_Released()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(false, 7);
	}
}

void UPlayerInputComponent::Num8_Pressed()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(true, 8);
	}
}

void UPlayerInputComponent::Num8_Released()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(false, 8);
	}
}

void UPlayerInputComponent::Num9_Pressed()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(true, 9);
	}
}

void UPlayerInputComponent::Num9_Released()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(false, 9);
	}
}

void UPlayerInputComponent::Num0_Pressed()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(true, 0);
	}
}

// ??????????????????
void UPlayerInputComponent::Num0_Released()
{
	if (CanInputToGame() == false)return;
	view->myController->Function_LeaveRoom();
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		view->commandComponent->Input_EnterNumber(false, 0);
	}
}

// ????????????
void UPlayerInputComponent::Q_Pressed()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::Q_Released()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::W_Pressed()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::W_Released()
{
	if (CanInputToGame() == false)return;
}

// ????????????
void UPlayerInputComponent::E_Pressed()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::E_Released()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::R_Pressed()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::R_Released()
{
	if (CanInputToGame() == false)return;
	//view->editEntityComponent->SwitchEditTimelineStaus();
}

//
void UPlayerInputComponent::U_Pressed()
{
	if (CanInputToGame() == false)return;
	//APlayerController* playerController = Cast<APlayerController>(zwBaseCharacter->GetController());
	//playerController->bShowMouseCursor = true;
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerInputComponent::U_Pressed] [owner:%x] [playerController:%x]"),
	//       this, GetOwner(), playerController);
}

//
void UPlayerInputComponent::U_Released()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::I_Pressed()
{
	if (CanInputToGame() == false)return;
	//APlayerController* playerController = Cast<APlayerController>(zwBaseCharacter->GetController());
	//playerController->bShowMouseCursor = false;
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerInputComponent::I_Pressed] [owner:%x] [playerController:%x]"),
	//       this, GetOwner(), playerController);

	//ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//baseGameMode->overallSituationView->NetCall_ChangeSkySet();

	auto hud = GetHUD();
	if (EGameMainOperationMode::game_main_operation_mode_base_character == hud->mainRequest)
	{
		if (hud->childUI == EUIChildName::ui_name_mainui)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_setting, true);
		}
		else if (hud->childUI == EUIChildName::ui_name_setting)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_mainui, true);
		}
		else
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_setting, true);
		}
	}
	else if (EGameMainOperationMode::game_main_operation_mode_edit_etity == hud->mainRequest)
	{
		if (hud->childUI == EUIChildName::ui_name_edit_entity)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_entity_backpack, true);
		}
		else if (hud->childUI == EUIChildName::ui_name_entity_backpack)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_edit_entity, true);
		}
		else
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_entity_backpack, true);
		}
	}
}

//
void UPlayerInputComponent::I_Released()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::O_Pressed()
{
	if (CanInputToGame() == false)return;
	// ??????1
	// auto hud = GetHUD();
	// if(hud && hud->IsUIShow(UIName::ui_name_laser))
	// {
	// 	auto ui_name_laser = hud->GetChildUI(UIName::ui_name_laser);
	// 	if(ui_name_laser)
	// 	{
	// 		FString cmd = FString::Printf(TEXT("Event_Test"));
	// 		FOutputDeviceDebug device;
	// 		ui_name_laser->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
	// 	}
	// }
	//

	// // ??????2
	// auto placeView = view->placeComponent->GetEditEntityManage();
	// if (placeView->data->IsExistData(view->placeComponent->selectMasterInstance) == false)return;
	// auto syncInfo = placeView->data->GetSyncInfo(view->placeComponent->selectMasterInstance);
	// if (syncInfo == nullptr)return;
	// auto localInfo = placeView->data->GetLocalInfo(view->placeComponent->selectMasterInstance);
	// view->placeComponent->UI_UpdataLaserUI(syncInfo, localInfo, false);

	//// ??????3
	//auto placeView = view->placeComponent->GetEditEntityManage();
	//if (placeView->data->IsExistData(view->placeComponent->GetSelect()) == false)return;
	//auto syncInfo = placeView->data->GetSyncInfo(view->placeComponent->GetSelect());
	//if (syncInfo == nullptr)return;
	//auto localInfo = placeView->data->GetLocalInfo(view->placeComponent->GetSelect());
	//placeView->controller->moveTimelineController->WriteTimelineKeyList(0, syncInfo, localInfo);

	// ??????

	//view->myController->Input_ChangePawnType();


	auto hud = GetHUD();
	if (EGameMainOperationMode::game_main_operation_mode_base_character == hud->mainRequest)
	{
		if (hud->childUI == EUIChildName::ui_name_mainui)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_sky_level, true);
		}
		else if (hud->childUI == EUIChildName::ui_name_sky_level)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_mainui, true);
		}
		else
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_sky_level, true);
		}
	}
	else if (EGameMainOperationMode::game_main_operation_mode_edit_etity == hud->mainRequest)
	{
		if (hud->childUI == EUIChildName::ui_name_edit_entity)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_entity_material, true);
		}
		else if (hud->childUI == EUIChildName::ui_name_entity_material)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_edit_entity, true);
		}
		else
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_entity_material, true);
		}
	}
}

//
void UPlayerInputComponent::O_Released()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::P_Pressed()
{
	if (CanInputToGame() == false)return;


	auto hud = GetHUD();
	if (EGameMainOperationMode::game_main_operation_mode_base_character == hud->mainRequest)
	{
		if (hud->childUI == EUIChildName::ui_name_mainui)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_character, true);
		}
		else if (hud->childUI == EUIChildName::ui_name_character)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_mainui, true);
		}
		else
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_character, true);
		}
	}
	else if (EGameMainOperationMode::game_main_operation_mode_edit_etity == hud->mainRequest)
	{
		if (hud->childUI == EUIChildName::ui_name_edit_entity)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_entity_storage, true);
		}
		else if (hud->childUI == EUIChildName::ui_name_entity_storage)
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_edit_entity, true);
		}
		else
		{
			hud->UICallback_OpenChildUI(EUIChildName::ui_name_entity_storage, true);
		}
	}
}

//
void UPlayerInputComponent::P_Released()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::Z_Pressed()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::Z_Released()
{
	if (CanInputToGame() == false)return;
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerInputComponent::Z_Released] [%x]"),
		this, GetOwner());
}

//
void UPlayerInputComponent::X_Pressed()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::X_Released()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::C_Pressed()
{
	if (CanInputToGame() == false)return;
	//if (dataComp->inputKeyInfo.ctrlEnter && inputMode== ECharacterStatusType::CharacterStatusType_EditEntity)
	//{
	//    view->selectComponent->CopyInstance();
	//}
	//else
	//{
	//}
}

//
void UPlayerInputComponent::C_Released()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::V_Pressed()
{
	if (CanInputToGame() == false)return;
	auto dataComp = GetDataComponent();
	if (dataComp->inputKeyInfo.ctrlEnter && inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		view->selectComponent->OperationResultsSwitch(UPlayerSelectedComponent::Operation_Type::Operation_Type_paste);
	}
	else
	{
	}
}

//
void UPlayerInputComponent::V_Released()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::F_Pressed()
{
	if (CanInputToGame() == false)return;
	auto dataComp = GetDataComponent();
	if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
	}
	else
	{
	}
}

//
void UPlayerInputComponent::F_Released()
{
	if (CanInputToGame() == false)return;
	//view->editEntityComponent->SwitchEditTimelineStaus();
}

//
void UPlayerInputComponent::B_Pressed()
{
	if (CanInputToGame() == false)return;
	//view->editEntityComponent->Function_OpenBackpack();
}

//
void UPlayerInputComponent::B_Released()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::Ctrl_Pressed()
{
	if (CanInputToGame() == false)return;
	auto dataComp = GetDataComponent();
	dataComp->inputKeyInfo.ctrlEnter = true;
	if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		view->selectComponent->Input_Base_IsMultipleChoice(true);
	}
	else if (inputMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
	{
		view->selectComponent->Input_Timeline_IsMultipleChoice(true);
	}
}

//
void UPlayerInputComponent::Ctrl_Released()
{
	if (CanInputToGame() == false)return;
	auto dataComp = GetDataComponent();
	dataComp->inputKeyInfo.ctrlEnter = false;
	if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		view->selectComponent->Input_Base_IsMultipleChoice(false);
	}
	else if (inputMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
	{
		view->selectComponent->Input_Timeline_IsMultipleChoice(false);
	}
}

//
void UPlayerInputComponent::Backspace_Pressed()
{
	if (CanInputToGame() == false)return;
	auto dataComp = GetDataComponent();
	// ?????????????????????????????????
	if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		view->selectComponent->OperationResultsSwitch(UPlayerSelectedComponent::Operation_Type::Operation_Type_Delete);
	}
}

//
void UPlayerInputComponent::Backspace_Released()
{
	if (CanInputToGame() == false)return;
	auto dataComp = GetDataComponent();
	if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
	}
	else
	{
	}
}

//
void UPlayerInputComponent::Tab_Pressed()
{
	if (CanInputToGame() == false)return;
	view->timelineComponent->SwitchEditEntityStaus();
}

//
void UPlayerInputComponent::Tab_Released()
{
	if (CanInputToGame() == false)return;
}

//
void UPlayerInputComponent::LeftShift_Pressed()
{
	auto character = Cast<ACharacter>(GetOwner());
	auto playerController = Cast<APlayerController>(character->GetController());
	auto dataComp = GetDataComponent();
	auto hud = GetHUD();
	UCharacterMainUserWidget* mainui = Cast<UCharacterMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_mainui));
	if (mainui == nullptr || mainui->shortcutBar.IsValid() == false)return;
	dataComp->inputKeyInfo.shiftEnter = true;

	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
		//hud->SetUIState(true);
		//mainui->SetFocus();
	}
}

//
void UPlayerInputComponent::LeftShift_Released()
{
	auto character = Cast<ACharacter>(GetOwner());
	auto playerController = Cast<APlayerController>(character->GetController());
	auto dataComp = GetDataComponent();
	dataComp->inputKeyInfo.shiftEnter = false;

	auto hud = GetHUD();
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_None)
	{
	}
}

//
void UPlayerInputComponent::MiddleMouseButton_Pressed()
{
	if (CanInputToGame() == false)return;

	auto hud = GetHUD();
	if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		view->selectComponent->Input_Base_MiddleMouseButton();
	}
	else if (inputMode == ECharacterStatusType::CharacterStatusType_EditTimeline)
	{

		hud->UICallback_OpenChildUI(EUIChildName::ui_name_edit_entity, true);
	}
}

//
void UPlayerInputComponent::MiddleMouseButton_Released()
{
	if (CanInputToGame() == false)return;
	if (inputMode == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
	}
}
