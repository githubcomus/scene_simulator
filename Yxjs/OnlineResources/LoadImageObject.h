// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LoadImageObject.generated.h"

class UTexture2D;

/**
 *
 */
UCLASS()
class ULoadImageObject : public UObject
{
	GENERATED_BODY()
public:

	ULoadImageObject();


	void TestFunc();
	UTexture2D* LoadTexture2D(FString path, bool& IsValid, int32& OutWidth, int32& OutHeight);
};
