// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <string>
#include "Protobuf/the_public.pb.h"
#include "CommonlyObject.generated.h"

//using namespace std;


class UActorComponent;
class UZwInputComponent;


/**
 *
 */
UCLASS()
class  UCommonlyObject : public UObject
{
	GENERATED_BODY()
public:

	// 转换位置
	UFUNCTION(BlueprintCallable, Category = "UCommonlyObject")
		static FTransform SetTransform(FVector location, FVector rotator, FVector scale);
	static FString GetJiugongge(float x, float y, int syncSize);

	UFUNCTION(BlueprintCallable, Category = "UCommonlyObject")
		static float StringToFloat(FString var);
	static FVector PBToUe4Vector(Gamedata::FVector* sourceLocation);
	static FVector PBToUe4Vector(const Gamedata::FVector* source);
	static FRotator PBToUe4Rotator(Gamedata::FRotator* sourceRotator);
	static FRotator PBToUe4Rotator(const Gamedata::FRotator* sourceRotator);
	static FTransform PBToUe4Transform(Gamedata::FTransform* sourceTransform);
	static void Ue4ToPbVector(FVector sourceLocation, Gamedata::FVector* targetLocation);
	static Gamedata::FVector Ue4ToPbVector(FVector source);
	static void Ue4ToPbRotator(FRotator source, Gamedata::FRotator* target);
	static  Gamedata::FRotator Ue4ToPbRotator(FRotator source);
	static void Ue4ToPbTransform(FTransform source, Gamedata::FTransform* target);
	static std::string FStringToString(FString& str);
	static FString StringToFString(std::string& str);
	static std::string FloatToString(float val);
	static std::string IntToString(int val);
	static void DataComparison2(std::list<int>& beforeList, std::list<int>& nowList, std::list<int>& addList, std::list<int>& removeList, std::list<int>& constantList);
};
