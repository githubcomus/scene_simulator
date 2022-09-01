// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "string"
#include "list"
#include "vector"
#include "PlayerCharacterMovementComponent.generated.h"

//using namespace std;

/**
 *
 */
UCLASS()
class YXJS_API UPlayerCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	// 移动状态
	enum MoveType
	{
		MoveType_none,
		MoveType_start,
		MoveType_run,
		MoveType_end
	};

	// 到达目标点
	struct CvGoToLocation
	{
		//
		FVector location;

		//
		float time;

		//
		MoveType moveType = MoveType_none;

		// 到达此节点的速度
		float speed = 0;

		// 开始移动的时间
		float moveStartTime = 0;
	};

	// 移动操作
	struct CvMovePoint
	{
		//// 强制移动开始时间
		//float moveStartTime;

		//// 开始位置
		//FVector startMoveLocation;

		//// 目标位置
		//FVector endMoveLocation;

		//// 目标yaw值
		//float yaw = 0;

		//// 速度
		//FVector velocity;

		//// 速度
		//float speed = 0;

		//// 移动
		//bool isMove = false;

		////
		//MoveType moveType= MoveType_none;

		//--------------------------------------------------

		//
		std::vector<CvGoToLocation> goToLocationList;

		//
		int runIndex = 0;
	};

	// 移动操作
	CvMovePoint movePoint;

	FVector oldV;

	UPlayerCharacterMovementComponent();

	///**
	// * React to new transform from network update. Sets bNetworkSmoothingComplete to false to ensure future smoothing updates.
	// * IMPORTANT: It is expected that this function triggers any movement/transform updates to match the network update if desired.
	// */
	//virtual void SmoothCorrection(const FVector& OldLocation, const FQuat& OldRotation, const FVector& NewLocation, const FQuat& NewRotation) override;

	///**
	// * Smooth mesh location for network interpolation, based on values set up by SmoothCorrection.
	// * Internally this simply calls SmoothClientPosition_Interpolate() then SmoothClientPosition_UpdateVisuals().
	// * This function is not called when bNetworkSmoothingComplete is true.
	// * @param DeltaSeconds Time since last update.
	// */
	//virtual void SmoothClientPosition(float DeltaSeconds);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetTargetLocation(FVector velocity, FVector endMoveLocation);
};
