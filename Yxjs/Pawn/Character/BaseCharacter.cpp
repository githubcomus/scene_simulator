// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/PlayerInputComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerPlaceComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerMaterialComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerEntityBackpackComponent.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/CharacterComponent/PlayerCharacterMovementComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerSkyComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerEntityMergeComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerEntityStorageComponent.h"

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
#include "CharacterComponent/PlayerPreviewComponent.h"
#include "CharacterComponent/PlayerPythonComponent.h"
#include "CharacterComponent/PlayerTimelineComponent.h"
#include "CharacterComponent/PlayerEditEntityComponent.h"
#include "Yxjs/Tools/CalculationObject.h"
#include "CharacterComponent/PlayerCommandComponent.h"

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GetCapsuleComponent()->SetCollisionResponseToChannel(BuildObject, ECR_Block);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	cameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// 摄像机
	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	followCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//sync2 = CreateDefaultSubobject<UZwSyncActorComponent>(TEXT("SyncActorComponent"));

	myData = CreateDefaultSubobject<UCharacterDataComponent>(TEXT("CharacterDataComponent"));

	myController = CreateDefaultSubobject<UCharacterControllerComponent>(TEXT("CharacterControllerComponent"));

	//widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("UWidgetComponent"));
	//widgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 测试
	//Start();
}

//
void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//测试
	//End();
}

//
void ABaseCharacter::Start(int playerId_, bool isLocal_, Gamedata::PlayerInfo& syncPlayerInfo)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [ABaseCharacter::Start] 人物生成 "),
		this);

	myData->Start(myController, this, playerId_, isLocal_, syncPlayerInfo);

	myController->Start(myData, this);

	GetCapsuleComponent()->SetCollisionResponseToChannel(BuildObject, ECollisionResponse::ECR_Block);

	// 本地客户端
	if (isLocal_)
	{
		//SetRole(ENetRole::ROLE_None);

		// 输入组件
		inputComp = NewObject<UPlayerInputComponent>(this, TEXT("UPlayerInputComponent"));
		inputComp->RegisterComponent();

		// 选中操作组件
		selectComponent = NewObject<UPlayerSelectedComponent>(this, TEXT("UPlayerSelectedComponent"));
		selectComponent->RegisterComponent();

		// 放置组件
		placeComponent = NewObject<UPlayerPlaceComponent>(this, TEXT("UPlayerPlaceComponent"));
		placeComponent->RegisterComponent();

		// 材质组件
		materialComponent = NewObject<UPlayerMaterialComponent>(this, TEXT("UPlayerMaterialComponent"));
		materialComponent->RegisterComponent();

		// 背包组件
		entitybBackpackComponent = NewObject<UPlayerEntityBackpackComponent>(this, TEXT("UPlayerEntityBackpackComponent"));
		entitybBackpackComponent->RegisterComponent();

		// 天气组件
		skyComponent = NewObject<UPlayerSkyComponent>(this, TEXT("UPlayerSkyComponent"));
		skyComponent->RegisterComponent();

		// 实例合成组件
		entityMergeComponent = NewObject<UPlayerEntityMergeComponent>(this, TEXT("UPlayerEntityMergeComponent"));
		entityMergeComponent->RegisterComponent();

		// 预览组件
		previewComponent = NewObject<UPlayerPreviewComponent>(this, TEXT("UPlayerPreviewComponent"));
		previewComponent->RegisterComponent();

		pythonComponent = NewObject<UPlayerPythonComponent>(this, TEXT("UPlayerPythonComponent"));
		pythonComponent->RegisterComponent();

		timelineComponent = NewObject<UPlayerTimelineComponent>(this, TEXT("UPlayerTimelineComponent"));
		timelineComponent->RegisterComponent();

		editEntityComponent = NewObject<UPlayerEditEntityComponent>(this, TEXT("UPlayerEditEntityComponent"));
		editEntityComponent->RegisterComponent();

		commandComponent = NewObject<UPlayerCommandComponent>(this, TEXT("UPlayerCommandComponent"));
		commandComponent->RegisterComponent();

		entityStorageComponent = NewObject<UPlayerEntityStorageComponent>(this, TEXT("UPlayerEntityStorageComponent"));
		entityStorageComponent->RegisterComponent();

		// 控制器绑定当前玩家
		auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		playerController->Possess(this);

		//UE_LOG(LogTemp, Log, TEXT("[%x] [ABaseCharacter::Start]  b "), this);

		inputComp->Start();
		selectComponent->Start();
		placeComponent->Start();
		materialComponent->Start();
		entitybBackpackComponent->Start();
		skyComponent->Start();
		entityMergeComponent->Start();
		previewComponent->Start();
		pythonComponent->Start();
		timelineComponent->Start();
		editEntityComponent->Start();
		commandComponent->Start();
		entityStorageComponent->Start();

		//    // 当自己这个最重要的通道加载进来以后,开始请求gamemode相关的数据
		//    ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		//    baseGameMode->controller->RequestGetAllAtt();
	}
	// 模拟客户端
	else
	{
		// 方便FaceRotation函数使用
		bUseControllerRotationPitch = true;
		bUseControllerRotationYaw = true;
		bUseControllerRotationRoll = true;

		SetRole(ENetRole::ROLE_SimulatedProxy);

		{
			//FString path = TEXT("WidgetBlueprint'/Game/Yxjs/BP/Pawn/BP_PawnMainUI.BP_PawnMainUI_C'");
			//TSubclassOf<UUserWidget> theClass = LoadClass<UUserWidget>(NULL, *path);
			//if (widgetComponent) {
			//	widgetComponent->SetWidgetClass(theClass);
			//	widgetComponent->SetDrawSize(FVector2D(100, 50));
			//}
			//InitUi();
		}
	}
}

//
void ABaseCharacter::End()
{
	// 本地客户端
	if (myData->isLocal)
	{
		inputComp->End();
		selectComponent->End();
		placeComponent->End();
		materialComponent->End();
		entitybBackpackComponent->End();
		skyComponent->End();
		entityMergeComponent->End();
		previewComponent->End();
		pythonComponent->End();

		if (inputComp->IsRegistered())
		{
			inputComp->UnregisterComponent();
		}
		inputComp->DestroyComponent();
		inputComp = nullptr;

		if (selectComponent->IsRegistered())
		{
			selectComponent->UnregisterComponent();
		}
		selectComponent->DestroyComponent();
		selectComponent = nullptr;

		if (placeComponent->IsRegistered())
		{
			placeComponent->UnregisterComponent();
		}
		placeComponent->DestroyComponent();
		placeComponent = nullptr;

		if (materialComponent->IsRegistered())
		{
			materialComponent->UnregisterComponent();
		}
		materialComponent->DestroyComponent();
		materialComponent = nullptr;

		if (entitybBackpackComponent->IsRegistered())
		{
			entitybBackpackComponent->UnregisterComponent();
		}
		entitybBackpackComponent->DestroyComponent();
		entitybBackpackComponent = nullptr;

		if (skyComponent->IsRegistered())
		{
			skyComponent->UnregisterComponent();
		}
		skyComponent->DestroyComponent();
		skyComponent = nullptr;

		if (entityMergeComponent->IsRegistered())
		{
			entityMergeComponent->UnregisterComponent();
		}
		entityMergeComponent->DestroyComponent();
		entityMergeComponent = nullptr;

		if (previewComponent->IsRegistered())
		{
			previewComponent->UnregisterComponent();
		}
		previewComponent->DestroyComponent();
		previewComponent = nullptr;

		if (pythonComponent->IsRegistered())
		{
			pythonComponent->UnregisterComponent();
		}
		pythonComponent->DestroyComponent();
		pythonComponent = nullptr;

		timelineComponent->End();
		if (timelineComponent->IsRegistered())
		{
			timelineComponent->UnregisterComponent();
		}
		timelineComponent->DestroyComponent();
		timelineComponent = nullptr;

		editEntityComponent->End();
		if (editEntityComponent->IsRegistered())
		{
			editEntityComponent->UnregisterComponent();
		}
		editEntityComponent->DestroyComponent();
		editEntityComponent = nullptr;

		commandComponent->End();
		if (commandComponent->IsRegistered())
		{
			commandComponent->UnregisterComponent();
		}
		commandComponent->DestroyComponent();
		commandComponent = nullptr;

		entityStorageComponent->End();
		if (entityStorageComponent->IsRegistered())
		{
			entityStorageComponent->UnregisterComponent();
		}
		entityStorageComponent->DestroyComponent();
		entityStorageComponent = nullptr;

	}

	myData->End();
	myController->End();

	UE_LOG(LogTemp, Log, TEXT("[%x] [ABaseCharacter::End] 人物销毁 "), this);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	inputComp->SetupPlayerInputComponent(PlayerInputComponent);
}

//
void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (myData->isLocal)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [玩家] [PossessedBy] 挂载 本地玩家 [playerId:%d]"), this, myData->playerId);
		myController->PossessedBy();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [玩家] [PossessedBy] 挂载 模拟玩家 [playerId:%d]"), this, myData->playerId);
	}
}

//
void ABaseCharacter::InitUi()
{
	if (widgetComponent == nullptr) { return; }

	//auto ui = widgetComponent->GetUserWidgetObject();

	//if (ui == nullptr) { return; }

	//FString cmd = FString::Printf(TEXT("Event_Init %d"), myData->playerId);
	//FOutputDeviceDebug device;
	//ui->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (myData->gameMode.IsValid() && myData->isLocal == false)
	{
		if (myData->gameMode->localPlayerCameraComponet.IsValid())
		{
			auto tempR = (myData->gameMode->localPlayerCameraComponet->GetComponentLocation() - GetActorLocation()).Rotation();
			tempR.Pitch = 0;
			tempR.Roll = 0;
			//widgetComponent->SetWorldRotation(tempR);
		}

	}

	//if (myData->isLocal) {
	//	auto nowTime = GetWorld()->TimeSeconds;
	//	if (lastTimeTick + 1 < nowTime) {
	//		lastTimeTick = nowTime;
	//		float farme = 1.0 / DeltaTime;
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("[帧数] %d"), (int)farme));
	//	}
	//}
}

// 进入编辑状态
void ABaseCharacter::EnterEditStaus()
{
	if (myData->isLocal)
	{
		//UKismetSystemLibrary::DrawDebugBox(GetWorld(), GetActorLocation(), FVector(5, 5, 5), FLinearColor::Yellow, GetActorRotation(), 10, 2);

		{
			auto wTransform1 = followCamera->GetComponentTransform();
			wTransform1.SetRotation(FRotator::ZeroRotator.Quaternion());
			FTransform wTransform2 = GetActorTransform();
			auto rLocation = UCalculationObject::WorldToRelativeTransform(wTransform1, wTransform2.GetLocation());
			cameraRTransform.SetLocation(rLocation);
			cameraRTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
		}

		USpringArmComponent* arm = Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()));
		SetActorHiddenInGame(true);
		arm->TargetArmLength = 0.0;

		auto transform = followCamera->GetComponentTransform();
		SetActorTransform(transform);
		//UKismetSystemLibrary::DrawDebugBox(GetWorld(), transform.GetLocation(), FVector(10, 10, 10), FLinearColor::Yellow, transform.GetRotation().Rotator(), 10, 2);
	}
	else
	{
	}
	// 取消碰撞
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 取消重力
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->bApplyGravityWhileJumping = false;
	// 增加QE的上下运动

	GetMovementComponent()->Velocity = FVector::ZeroVector;

	isEdit = true;
}

// 进入空状态
void ABaseCharacter::EnterNoneStaus()
{
	if (isEdit)
	{

		if (myData->isLocal)
		{
			auto wTransform1 = followCamera->GetComponentTransform();
			USpringArmComponent* arm = Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()));
			SetActorHiddenInGame(false);
			arm->TargetArmLength = 300.0;

			// 设置actor的变换
			{
				//UKismetSystemLibrary::DrawDebugBox(GetWorld(), wTransform1.GetLocation(), FVector(20, 20, 20), FLinearColor::Blue, wTransform1.GetRotation().Rotator(), 10, 2);

				FTransform rTransform = cameraRTransform;
				rTransform.SetLocation(rTransform.GetLocation());
				rTransform.SetRotation(FRotator::ZeroRotator.Quaternion());

				wTransform1.SetRotation(FRotator::ZeroRotator.Quaternion());
				auto wTransform2 = UCalculationObject::RelativeToWorldTransform(wTransform1, rTransform);
				auto tempRotator = wTransform2.GetRotation().Rotator();
				tempRotator.Pitch = 0;
				tempRotator.Roll = 0;
				wTransform2.SetRotation(tempRotator.Quaternion());
				SetActorTransform(wTransform2);

				//UKismetSystemLibrary::DrawDebugBox(GetWorld(), wTransform2.GetLocation(), FVector(20, 20, 20), FLinearColor::Red, wTransform2.GetRotation().Rotator(), 10, 2);
			}
		}
		else
		{
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		GetCharacterMovement()->GravityScale = 1;
		GetCharacterMovement()->bApplyGravityWhileJumping = true;

		isEdit = false;
	}

	//GetCharacterMovement()->IsMovingOnGround();
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/