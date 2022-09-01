// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ImageUserWidget.generated.h"

class UImage;

/**
 *
 */
UCLASS()
class YXJS_API UImageUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	int i;

	// // 图片正面
	UImage* imagePositive;

	// // 照片背面
	UImage* imageBack;

	// virtual bool Initialize();
	// virtual void BeginDestroy() override;
	// virtual void PostLoad() override;

	void Start();
	void End();
	void LoadImage(FString url);
};
