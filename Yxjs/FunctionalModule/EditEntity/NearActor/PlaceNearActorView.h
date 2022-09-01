// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <list>
#include <map>
#include <vector>
#include "PlaceNearActorView.generated.h"


class UPlaceNearControllerComponent;
class UPlaceNearDataComponent;

class UBoxComponent;
class UMaterial;
class UBoxComponent;
class USceneComponent;
class USceneStaticMeshComponent;
class APawnLineActor;

//using namespace std;

class AEditEntityManage;

UCLASS()
class  APlaceNearActorView : public AActor
{
	GENERATED_BODY()

public:


	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* sceneRoot;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlaceNearDataComponent* data;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlaceNearControllerComponent* controller;

	//
	TWeakObjectPtr<AEditEntityManage> editEntityManage;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// Sets default values for this actor's properties
	APlaceNearActorView();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//
	void Start(AEditEntityManage* editEntityManage_);

	//
	void End();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
