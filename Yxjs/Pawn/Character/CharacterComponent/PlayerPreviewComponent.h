// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "PlayerPreviewComponent.generated.h"

class APreviewActor;
class ABaseCharacter;
class AEditEntityManage;
class ACharacter;
class UArrowComponent;
class UBoxComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlayerPreviewComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	ABaseCharacter* character;

	//
	UPROPERTY(VisibleAnywhere)
		APreviewActor* previewActor;

	// 
	bool isTick = false;

	// 
	FVector characterLocation;

	//
	FRotator characterRotator;

	//
	FVector2D mousePostion;

	// 面向自己(资源文件夹面向自己,收纳盒按照原来的位置)
	bool faceOneself = false;

	// 射线检测失败的距离
	//int lineLen = 0;

	/*------------------------------------------------------------------*/

	//Sets default values for this component's properties
	UPlayerPreviewComponent();
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// START
	void Start();

	// END
	void End();

	void OpenClosePerview(bool open, Gamedata::EntityInfo* syncInfoInfo, bool faceOneself_);
	void MouseMove();
	void PawnMove();
	void Frame();
	/*------------------------------------------------------------------*/
};
