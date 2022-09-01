// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "Protobuf/game.pb.h"
#include "UI/BaseHUDInfo.h"
#include "PlayerInputComponent.generated.h"

class ABaseHUD;
class ABaseCharacter;
class UCharacterDataComponent;
class UCharacterControllerComponent;
class UPlayerSelectedComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlayerInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 视图
	TWeakObjectPtr<ABaseCharacter> view;

	/*------------------------------------------------------------------*/

	// 间隔时间
	float intervalTime;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// 当前输入模式,由子UI控制
	ECharacterStatusType inputMode = ECharacterStatusType::CharacterStatusType_None;


	/*------------------------------------------------------------------*/

	// Sets default values for this component's properties
	UPlayerInputComponent();

	//
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	void SetInputMode(ECharacterStatusType inputMode_);
	/*------------------------------------------------------------------*/

	//
	void Start();

	//
	void End();
	ABaseHUD* GetHUD();
	UCharacterControllerComponent* GetControllerComponent();
	UPlayerSelectedComponent* GetSelectedComponent();
	UCharacterDataComponent* GetDataComponent();

	bool CanInputToGame();
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	//
	void MoveUp(float Value);

	// E下降
	void MoveDown(float Value);

	void Space_Pressed();

	void Space_Released();

	//
	void TurnAtRate(float Rate);

	//
	void LookUpAtRate(float Rate);

	void MouseLeft_Pressed();

	void MouseLeft_Released();

	void MouseRight_Pressed();

	void MouseRight_Released();

	void MousePrevious_Pressed();

	void MousePrevious_Released();

	void MouseNext_Pressed();

	void MouseNext_Released();

	void Turn(float val);

	void LookUp(float val);

	void Num1_Pressed();

	void Num1_Released();

	void Num2_Pressed();

	void Num2_Released();

	void Num3_Pressed();

	void Num3_Released();

	void Num4_Pressed();

	void Num4_Released();

	void Num5_Pressed();

	void Num5_Released();

	void Num6_Pressed();

	void Num6_Released();

	void Num7_Pressed();

	void Num7_Released();

	void Num8_Pressed();

	void Num8_Released();

	void Num9_Pressed();

	void Num9_Released();

	void Num0_Pressed();

	void Num0_Released();

	void Q_Pressed();

	void Q_Released();

	void W_Pressed();

	void W_Released();

	//
	void E_Pressed();

	//
	void E_Released();

	//
	void R_Pressed();

	//
	void R_Released();

	//
	void U_Pressed();

	//
	void U_Released();

	//
	void I_Pressed();

	//
	void I_Released();

	//
	void O_Pressed();

	//
	void O_Released();

	//
	void P_Pressed();

	//
	void P_Released();

	//
	void Z_Pressed();

	//
	void Z_Released();

	//
	void X_Pressed();

	//
	void X_Released();

	//
	void C_Pressed();

	//
	void C_Released();

	//
	void V_Pressed();

	//
	void V_Released();
	void F_Pressed();
	void F_Released();
	void B_Pressed();
	void B_Released();

	//
	void Ctrl_Pressed();

	//
	void Ctrl_Released();
	void Backspace_Pressed();
	void Backspace_Released();
	void Tab_Pressed();
	void Tab_Released();

	void LeftShift_Pressed();
	void LeftShift_Released();
	void MiddleMouseButton_Released();
	void MiddleMouseButton_Pressed();
	/*------------------------------------------------------------------*/
};
