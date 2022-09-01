// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <string>
#include "TestChildObject.generated.h"

//using namespace std;


/**
 *
 */
UCLASS()
class  UTestChildObject : public UObject
{
	GENERATED_BODY()

public:

	UTestChildObject();
	~UTestChildObject();

	FString GetJiugongge(float x, float y, int syncSize);

};
