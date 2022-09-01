// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceNearEntity.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "../EditEntityManage.h"
//#include "Yxjs/Pawn/Racing/Player/PlayerRacingPawn.h"
#include "PlaceNearDataComponent.h"
#include "PlaceNearControllerComponent.h"

#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

 /*----------------------------------------------------*/
 /*----------------------------------------------------*/

//
APlaceNearEntity::APlaceNearEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UStaticMeshComponent"));
	RootComponent = meshComponent;

	meshComponent->SetHiddenInGame(true);
	meshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	meshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	meshComponent->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
	meshComponent->SetGenerateOverlapEvents(false);
	meshComponent->SetCastShadow(false);
}

//
void APlaceNearEntity::Start(APlaceNearActorView* placeNearActorView_, UStaticMesh* mesh)
{
	placeNearActorView = placeNearActorView_;

	if (mesh)
	{
		meshComponent->SetStaticMesh(mesh);
	}
}

//
void APlaceNearEntity::End()
{
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/
