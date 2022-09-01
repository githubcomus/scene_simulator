// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HallHUD.generated.h"

class UUserWidget;

/**
 *
 */
UCLASS()
class YXJS_API AHallHUD : public AHUD
{
	GENERATED_BODY()

public:

	// 主页
	UPROPERTY(VisibleAnywhere)
		UUserWidget* hallPtr = nullptr;

	// 房间
	UPROPERTY(VisibleAnywhere)
		UUserWidget* roomPtr = nullptr;

	/*------------------------------------------------------------------*/

	AHallHUD();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void LoadHallUI();
	void LoadRoomUI();
};
