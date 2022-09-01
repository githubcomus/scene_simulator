// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Yxjs/FunctionalModule/Selected/SelectedInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "PlayerEditEntityComponent.generated.h"

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class AMyPythonActor;
class UPlayerSelectedComponent;
class ABaseHUD;
class ABaseCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YXJS_API UPlayerEditEntityComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 常用指针
	TWeakObjectPtr<UBaseGameInstance> gameInstance;

	// 常用指针
	TWeakObjectPtr<APlayerController> playerController;

	// 常用指针
	TWeakObjectPtr<ABaseGameMode> gameMode;

	// 常用指针
	TWeakObjectPtr<ABaseHUD> hud;

	//
	TWeakObjectPtr<UPlayerSelectedComponent> selectComponent = nullptr;

	//
	TWeakObjectPtr<ABaseCharacter> view = nullptr;

	/*------------------------------------------------------------------*/

	bool CheckTargetData();
	bool CheckDefaultData();
	UFUNCTION(BlueprintCallable)
		bool CanOpenUI();
	UPlayerEditEntityComponent();
	virtual void BeginPlay() override;

	void Start();
	void End();
	void SwitchPreviewStaus(bool inEnter);
	UFUNCTION(BlueprintCallable)
		void Function_EditUiOrder(EEditUIBaseOrder order);
	void Function_NextFunction(int var);
	void Function_EditMode(bool flag);
};
