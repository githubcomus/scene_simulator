// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerPythonComponent.generated.h"

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class AMyPythonActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YXJS_API UPlayerPythonComponent : public UActorComponent
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
	TWeakObjectPtr<AMyPythonActor> myPythonActor;

	// python编辑ui是否打开
	bool isOpenUI = false;

	/*------------------------------------------------------------------*/

	bool CheckTargetData();
	bool CheckDefaultData();
	// Sets default values for this component's properties
	UPlayerPythonComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void UICall_SetOpenCloseUI(bool flag);
	UFUNCTION(BlueprintCallable)
		void UICall_SetRunPython(bool flag);
	UFUNCTION(BlueprintCallable)
		void NetCall_ChangeSkySet(int time, int weather, int cloud_coverageoverall, int cloud_speed, int moon_color);
	// START
	void Start();

	// END
	void End();
};
