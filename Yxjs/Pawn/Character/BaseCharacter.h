
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Yxjs/Protobuf/game.pb.h"
#include "BaseCharacter.generated.h"

class UCharacterDataComponent;
class UZwSyncActorComponent;
class UCharacterControllerComponent;
class UBaseGameInstance;
class ABaseGameMode;
class UPlayerPlaceComponent;
class UPlayerMaterialComponent;
class UPlayerEntityBackpackComponent;
class UPlayerEntityMergeComponent;
class UPlayerPreviewComponent;
class UPlayerEntityStorageComponent;

class UCharacterControllerComponent;
class UPlayerInputComponent;
class UPlayerSelectedComponent;
class UPlayerSkyComponent;
class UPlayerPythonComponent;
class UPlayerTimelineComponent;
class UPlayerEditEntityComponent;
class UPlayerCommandComponent;

class USpringArmComponent;
class UCameraComponent;
class USplineComponent;
class UWidgetComponent;

UCLASS()
class  ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 数据组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCharacterDataComponent* myData;

	// 控制器组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCharacterControllerComponent* myController;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* cameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* followCamera;

	// 输入组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerInputComponent* inputComp;

	// 选中操作组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerSelectedComponent* selectComponent;

	// 放置组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerPlaceComponent* placeComponent;

	// 材质使用组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerMaterialComponent* materialComponent;

	// 背包组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerEntityBackpackComponent* entitybBackpackComponent;

	// 天气组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerSkyComponent* skyComponent;

	// 实例合成组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerEntityMergeComponent* entityMergeComponent;

	// 血条
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UWidgetComponent* widgetComponent;

	// 预览组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerPreviewComponent* previewComponent;

	// python组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerPythonComponent* pythonComponent;

	// 时间轴组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerTimelineComponent* timelineComponent;

	// 编辑实例组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerEditEntityComponent* editEntityComponent;

	// 命令控制组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerCommandComponent* commandComponent;

	// 实例收纳组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerEntityStorageComponent* entityStorageComponent;

	// 摄像头相对位置
	FTransform cameraRTransform;

	// 进入了编辑状态
	bool isEdit = false;

	//
	float lastTimeTick = 0;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// Sets default values for this character's properties
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//
	virtual void PossessedBy(AController* NewController) override;

	void InitUi();
	//
	void Start(int playerId_, bool isLocal_, Gamedata::PlayerInfo& syncPlayerInfo);

	//
	void End();

	// 进入编辑状态
	void EnterEditStaus();

	// 进入空状态
	void EnterNoneStaus();

	//void CreateSplinePoint(FVector location);

	//void SplineCreateLine(std::vector<FVector> locationList);

/*----------------------------------------------------*/
/*----------------------------------------------------*/
};
