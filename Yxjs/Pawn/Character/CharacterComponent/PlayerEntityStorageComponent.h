// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "PlayerEntityStorageComponent.generated.h"

class ABaseCharacter;
class AEditEntityManage;
class ACharacter;
class UArrowComponent;
class UBoxComponent;
class AEditEntityManage;
class AOverallSituationView;
class UCharacterDataComponent;
class UPlayerSelectedComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlayerEntityStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 背包格子信息
	struct CvLineInfo
	{
		// xlsId
		int xlsId = 0;

		// 在list的索引
		int listIndex = 0;

		// 左侧list索引
		int leftIndex = 0;
	};

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 视图
	TWeakObjectPtr<ABaseCharacter> view;

	//
	TArray<CvLineInfo> lineList;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	UCharacterDataComponent* GetDataComponent();
	UPlayerSelectedComponent* GetSelectedComponent();
	AEditEntityManage* GetEditEntityManage();
	AOverallSituationView* GetOverallSituationView();
	xlsdata::MaterialTableDefine* GetXlsInfoWithXlsId(int32 xlsId);
	void GetSelectInstanceList(std::vector<int>& selectList);
	//Sets default values for this component's properties
	UPlayerEntityStorageComponent();

	//Called when the game starts
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// START
	void Start();

	// END
	void End();
	void UI_InitUI();

	UFUNCTION(BlueprintCallable)
		void UICallback_EntityMerge();

	void UI_MergeActor_AddLine(Gamedata::EntityMeshMasterInfo* info);
	/*----------------------------------------------------*/
	/*----------------------------------------------------*/
};
