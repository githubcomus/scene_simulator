// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "PlayerMaterialComponent.generated.h"

class ABaseCharacter;
class AEditEntityManage;
class ACharacter;
class UArrowComponent;
class UBoxComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlayerMaterialComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	//
	ABaseCharacter* character;


	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	//Sets default values for this component's properties
	UPlayerMaterialComponent();

	//Called when the game starts
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	// START
	void Start();

	// END
	void End();
	xlsdata::MaterialTableDefine* GetXlsInfoWithXlsId(int32 xlsId);
	void CallInput_SetMaterial(int xlsId);

	////
	//AEditEntityManage* GetView();

	////
	//UArrowComponent* CreateTestArrow(FString name, FTransform t);

	////
	//void DestroyTestArrow(UArrowComponent* arrow);

	////
	//void UpdateTestArrow(FString name, FTransform t);

	//void SpawnBuildInstance();
	//FVector GetHitLocation();
	//FRotator GetPlayerCameraRotator();
	//void SetMasterNode();

	/*------------------------------------------------------------------*/

};
