// Fill out your copyright notice in the Description page of Project Settings.

#include "SimulationRacingPawn.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Racing/SimulationComponent/SimulationRacingDataComponent.h"
#include "Pawn/Racing/SimulationComponent/SimulationRacingControllerComponent.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include <AIModule/Classes/AIController.h>
#include <Components/SplineComponent.h>
#include <UMG/Public/Components/Widget.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include "../SimulationComponent/SimulationRacingMovementComponent.h"
#include <Misc/OutputDeviceDebug.h>

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values
ASimulationRacingPawn::ASimulationRacingPawn(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<USimulationRacingMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//GetCapsuleComponent()->SetCollisionResponseToChannel(BuildObject, ECR_Block);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->GravityScale = 0;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->SetHiddenInGame(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	dataComp = CreateDefaultSubobject<USimulationRacingDataComponent>(TEXT("USimulationRacingDataComponent"));

	controllerComp = CreateDefaultSubobject<USimulationRacingControllerComponent>(TEXT("USimulationRacingControllerComponent"));

	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("UWidgetComponent"));
	widgetComponent->SetupAttachment(GetMesh());
	widgetComponent->SetRelativeLocation(FVector(0, 0, 60));
}

// Called when the game starts or when spawned
void ASimulationRacingPawn::BeginPlay()
{
	Super::BeginPlay();
}

//
void ASimulationRacingPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

//
void ASimulationRacingPawn::Start(int playerId_)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [赛车] [模拟端] [Start] 生成 [playerId_:%d]"), this, playerId_);

	dataComp->Start(controllerComp, this, playerId_);

	controllerComp->Start(dataComp, this);

	GetCapsuleComponent()->SetCollisionResponseToChannel(BuildObject, ECollisionResponse::ECR_Block);

	{
		// 方便FaceRotation函数使用
		bUseControllerRotationPitch = true;
		bUseControllerRotationYaw = true;
		bUseControllerRotationRoll = true;

		SetRole(ENetRole::ROLE_SimulatedProxy);

		{
			FString path = TEXT("WidgetBlueprint'/Game/Yxjs/BP/Pawn/BP_PawnMainUI.BP_PawnMainUI_C'");
			TSubclassOf<UUserWidget> theClass = LoadClass<UUserWidget>(NULL, *path);
			if (widgetComponent)
			{
				widgetComponent->SetWidgetClass(theClass);
				widgetComponent->SetDrawSize(FVector2D(100, 50));
			}
			InitUi();
		}
	}
}

//
void ASimulationRacingPawn::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [赛车] [模拟端] [End] 销毁 [playerId_:%d]"), this, dataComp->playerId);

	dataComp->End();
	controllerComp->End();
}

//
void ASimulationRacingPawn::InitUi()
{
	if (widgetComponent == nullptr) { return; }

	auto ui = widgetComponent->GetUserWidgetObject();

	if (ui == nullptr) { return; }

	FString cmd = FString::Printf(TEXT("Event_Init %d"), dataComp->playerId);
	FOutputDeviceDebug device;
	ui->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Called every frame
void ASimulationRacingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto speed = (int)GetCharacterMovement()->Velocity.Size();
	//auto speed = GetCharacterMovement()->GetLastUpdateVelocity().Size();
	if (speed != oldSpeed)
	{
		oldSpeed = speed;
		UE_LOG(LogTemp, Log, TEXT("[%x] [赛车] [模拟端] [Tick] 速度 [playerId_:%d] [speed:%d]"), this, dataComp->playerId, speed);
	}

	if (dataComp && dataComp->gameMode.IsValid())
	{
		if (dataComp->gameMode->localPlayerCameraComponet.IsValid())
		{
			auto tempR = (dataComp->gameMode->localPlayerCameraComponet->GetComponentLocation() - GetActorLocation()).Rotation();
			tempR.Pitch = 0;
			tempR.Roll = 0;
			widgetComponent->SetWorldRotation(tempR);
		}
	}
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/
