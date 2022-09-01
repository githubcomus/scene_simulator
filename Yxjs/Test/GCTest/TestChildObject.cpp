// Fill out your copyright notice in the Description page of Project Settings.


#include "TestChildObject.h"

#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"


//
UTestChildObject::UTestChildObject()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UTestChildObject::UTestChildObject] start"), this);
}

//
UTestChildObject::~UTestChildObject()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UTestChildObject::~UTestChildObject] end"), this);
}

// 获取 九宫格
FString UTestChildObject::GetJiugongge(float x, float y, int syncSize)
{
	int x2 = (int)(x / syncSize);
	if (x >= 0)
	{
	}
	else
	{
		x2 -= 1;
	}
	int y2 = (int)(y / syncSize);
	if (y >= 0)
	{
	}
	else
	{
		y2 -= 1;
	}

	FString key = FString::Printf(TEXT("%d_%d"), x2, y2);
	return key;
}
