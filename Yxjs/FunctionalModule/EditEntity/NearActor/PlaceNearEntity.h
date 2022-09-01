// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <list>
#include <map>
#include <vector>
#include "PlaceNearEntity.generated.h"


class APlaceNearActorView;

class UStaticMeshComponent;
class UStaticMesh;

//using namespace std;


UCLASS()
class  APlaceNearEntity : public AActor
{
	GENERATED_BODY()

public:


	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* meshComponent;


	//
	TWeakObjectPtr<APlaceNearActorView> placeNearActorView;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// Sets default values for this actor's properties
	APlaceNearEntity();

	//
	void Start(APlaceNearActorView* placeNearActorView_, UStaticMesh* mesh);

	//
	void End();


};
