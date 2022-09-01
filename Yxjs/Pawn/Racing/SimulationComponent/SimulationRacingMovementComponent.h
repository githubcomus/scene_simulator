// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "string"
#include "list"
#include "vector"
#include "SimulationRacingMovementComponent.generated.h"

//using namespace std;

/**
 *
 */
UCLASS()
class YXJS_API USimulationRacingMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:



	USimulationRacingMovementComponent();

	//virtual void SmoothCorrection(const FVector& OldLocation, const FQuat& OldRotation, const FVector& NewLocation, const FQuat& NewRotation);

	//virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration);

	//virtual void MoveSmooth(const FVector& InVelocity, const float DeltaSeconds, FStepDownResult* OutStepDownResult = NULL);

	//virtual void SimulateMovement(float DeltaTime);
};
