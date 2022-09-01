// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollisionTestActor.generated.h"


class UBulletDataComponent;
class UBulletControllerComponent;
class UPlayerRangedWeaponComponent;

class UBoxComponent;
class ABuildManageActor;
class UMaterial;
class UPlaceStaticMeshComponent;
class USceneComponent;
class UArrowComponent;


UCLASS()
class   ACollisionTestActor : public AActor
{
    GENERATED_BODY()

public:

    // 根场景
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent* sceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UBoxComponent* boxComponent;
	
    /*------------------------------------------------------------------*/

	//
    float lastTimeTickTest = 0;
	
    /*------------------------------------------------------------------*/
    /*------------------------------------------------------------------*/


    // Sets default values for this actor's properties
    ACollisionTestActor();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // EndPlay
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;


    // PostInitializeComponents
    virtual void PostInitializeComponents() override;

    /*------------------------------------------------------------------*/

    // START
    void Start();

    // END
    void End();
    void TickTest();
    void UseActorAConter(FTransform actorA, FTransform& actorB);
    bool CylinderAndCuboidOverlap(FVector cylinderConter, float cylinderHalfHeight, float circleRadius,
                                  FVector cuboidConter, FVector cuboidExtion);
    bool LineSegmentOverlaps(float start1, float end1, float start2, float end2);
    bool CircleAndRectangleOverlap(FVector2D RectangleCenter, FVector2D RectangleRadius, FVector2D circleCenter,
                                   float circleRadius);
    bool CylinderAndCylinderOverlap(FVector cylinderConter1, float cylinderHalfHeight1, float circleRadius1,
                                    FVector cylinderConter2, float cylinderHalfHeight2, float circleRadius2);
    bool CircleAndCircleOverlap(FVector2D circleCenter1, float circleRadius1, FVector2D circleCenter2,
                                float circleRadius2);

    /*------------------------------------------------------------------*/


};
