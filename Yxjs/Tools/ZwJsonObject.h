// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZwJsonObject.generated.h"

/**
 *
 */
UCLASS()
class  UZwJsonObject : public UObject
{
	GENERATED_BODY()
public:

	UZwJsonObject();


	static bool GetJsonArray(FString& josnStr, class TArray<class TSharedPtr<class FJsonValue>>& jsonArray);
	static void GetJsonArrayString(class TArray<class TSharedPtr<class FJsonValue>> jsonArray, int32 index, FString key, FString& value);
	static void GetJsonArrayInt(class TArray<class TSharedPtr<class FJsonValue>> jsonArray, int32 index, FString key, int32& value);
	static bool EncodeJson(const TSharedPtr<class FJsonObject>& jsonObject, FString& jsonStr);
	static void DecodeJson(FString& jsonStr);
	static void TestFunc();
	static TSharedPtr<FJsonObject> NewJsonObject();
	//static void DeleteJsonObject(TSharedPtr<FJsonObject> jsonObject);
};
