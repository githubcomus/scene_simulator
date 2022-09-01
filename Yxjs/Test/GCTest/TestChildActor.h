// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestChildActor.generated.h"


class UPlayerRangedWeaponComponent;

class UBoxComponent;
class ABuildManageActor;
class UMaterial;
class UPlaceStaticMeshComponent;
class USceneComponent;
class UArrowComponent;
class USplineComponent;
class USplineMeshComponent;
class UStaticMesh;


UCLASS()
class ATestChildActor : public AActor
{
	GENERATED_BODY()

public:

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* sceneRoot;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* box;

	// 终点mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* sphereMesh;

	// int
	int number = 0;

	/*------------------------------------------------------------------*/

	// Sets default values for this actor's properties
	ATestChildActor();
	~ATestChildActor();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// EndPlay
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// PostInitializeComponents
	virtual void PostInitializeComponents() override;

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	// START
	void Start();

	// END
	void End();
	void Test();

	/*------------------------------------------------------------------*/
};
