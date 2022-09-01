// Fill out your copyright notice in the Description page of Project Settings.


#include "TestChildActor.h"


#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


/*
view
*/


/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
ATestChildActor::ATestChildActor()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	sceneRoot->SetMobility(EComponentMobility::Movable);
	RootComponent = sceneRoot;

	sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UStaticMeshComponent"));
	// sphereMesh->SetMobility(EComponentMobility::Static);
	sphereMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	sphereMesh->bReceivesDecals = false;
	sphereMesh->CastShadow = false;
	sphereMesh->SetGenerateOverlapEvents(false);
	sphereMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	sphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	sphereMesh->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator::ZeroRotator);

	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestChildActor::ATestChildActor] start"), this);
}


ATestChildActor::~ATestChildActor()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestChildActor::~ATestChildActor] end"), this);
}

//
void ATestChildActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//
void ATestChildActor::BeginPlay()
{
	Super::BeginPlay();
	//Start();
	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestChildActor::BeginPlay] "), this);

	FString meshPath = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'");
	auto staticMeshObject = LoadObject<UStaticMesh>(NULL, *meshPath);
	sphereMesh->SetStaticMesh(staticMeshObject);
}

//
void ATestChildActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//End();
	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestChildActor::EndPlay] "), this);
}

// tick
void ATestChildActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

// START
void ATestChildActor::Start()
{
	// UE_LOG(LogTemp, Log, TEXT("[%x] [ATestChildActor::Start] "), this);
}

// END
void ATestChildActor::End()
{
	// UE_LOG(LogTemp, Log, TEXT("[%x] [ATestChildActor::End]   "), this);
}

/*------------------------------------------------------------------*/

// Test
void ATestChildActor::Test()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [ATestChildActor::Test] [nowTime:%f] [number:%d]  "), this, GetWorld()->TimeSeconds,
	       number);
}
