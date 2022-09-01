// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSkyComponent.generated.h"

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YXJS_API UPlayerSkyComponent : public UActorComponent
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

	/*------------------------------------------------------------------*/

	bool CheckTargetData();
	bool CheckDefaultData();
	// Sets default values for this component's properties
	UPlayerSkyComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void NetCall_ChangeSkySet(int time, int weather, int cloud_coverageoverall, int cloud_speed, int moon_color);
	// START
	void Start();

	// END
	void End();
};
