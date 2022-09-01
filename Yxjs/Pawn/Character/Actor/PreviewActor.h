// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreviewActor.generated.h"

class UPlayerBuildComponent;
class ABuildManageActor;
class UBuildControllerComponent;
class UTipsInteractiveCommon;
class UTipsDetailCommon;
class ABurnActor;

class ABaseCharacter;
class AEditEntityManage;

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class ABaseHUD;

class UBoxComponent;
class UMaterial;
class UBoxComponent;
class USceneComponent;
class UArrowComponent;

UCLASS()
class  APreviewActor : public AActor
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 常用指针
	TWeakObjectPtr<UBaseGameInstance> gameInstance;

	// 常用指针
	TWeakObjectPtr<APlayerController> playerController;

	// 常用指针
	TWeakObjectPtr<ABaseGameMode> gameMode;

	// 常用指针
	TWeakObjectPtr<ABaseHUD> hud;

	/*------------------------------------------------------------------*/

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* sceneRoot;

	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* boxComponent;

	////
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	UArrowComponent* arrowComponent;

	// 加载mesh信息
	struct ChildMeshInfo
	{
		// id
		//std::string idStr;

		// mesh实例Id
		int meshIstanceId = 0;

		// 相对位置
		FTransform childRTransform;

		// 相对位置
		FTransform meshRTransform;
	};

	// mesh id 列表
	std::vector<ChildMeshInfo> meshIDList;

	/*------------------------------------------------------------------*/

	// Sets default values for this actor's properties
	APreviewActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Start();
	//
	void End();
	bool CheckDefaultData();
	TWeakObjectPtr< AEditEntityManage> GetEditEntityManage();
	void StartPerview(int xlsId, bool isMerge);
	void UpdatePerview();
	void EndPerview();
};
