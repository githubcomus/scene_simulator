// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "PlayerEntityBackpackComponent.generated.h"

class ABaseCharacter;
class AEditEntityManage;
class ACharacter;
class UArrowComponent;
class UBoxComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlayerEntityBackpackComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	//// 背包格子信息
	//struct CvBackpackItemInfo
	//{
	//	// xlsId
	//	int xlsId = 0;

	//	// 在list的索引
	//	int listIndex = 0;

	//	// 左侧list索引
	//	int leftIndex = 0;

	//	// 行
	//	int row = 0;

	//	// 列
	//	int column = 0;

	//	// name
	//	FString name;

	//	// 描述
	//	FString description;

	//	// Icon
	//	FString iconPath;

	//	// 摆放指针
	//	xlsdata::PlaceTableDefine* place = nullptr;

	//	// 材质指针
	//	xlsdata::MaterialTableDefine* material = nullptr;

	//	// 图片url
	//	FString imageUrl;
	//};

	//// 背包左侧按钮
	//struct CvBackpackLeftItemInfo
	//{
	//	// 一级类型
	//	int firstMenuType;

	//	// 二级类型
	//	int secondMenuType;

	//	// 在格子的索引
	//	int listIndex;

	//	// name
	//	FString name;

	//	// Icon
	//	FString iconPath;

	//	// 可搜索标签?

	//	// 右侧格子数据
	//	TArray<CvBackpackItemInfo>list;
	//};

	//

	/*------------------------------------------------------------------*/

	//
	ABaseCharacter* character;


	//// 本地 背包数据第一层key是第一层菜单类型(放置物/材质/图片/视频)
	//// 第二层key是第二层菜单类型(网格,粒子,声音,激光,灯光,雾)
	//TMap<int, TMap<int, CvBackpackLeftItemInfo>> biiMap;

	///*------------------------------------------------------------------*/

	//// 本地 格子列数(每行6个格子)
	//int gridColumnCount = 6;

	/*------------------------------------------------------------------*/

	//Sets default values for this component's properties
	UPlayerEntityBackpackComponent();

	//Called when the game starts
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*------------------------------------------------------------------*/

	// START
	void Start();

	// END
	void End();
	//void InitBackpackData();
	//void SetItemInfo(int firstType, int secondType);
	//xlsdata::PlaceTableDefine* GetPlaceXlsInfoWithXlsId(int32 xlsId);
	//xlsdata::MaterialTableDefine* GetMaterialXlsInfoWithXlsId(int32 xlsId);
	//auto UI_InitUI(int firstType);
	//void UI_SetSecondItem(int firstType);
	//void UI_SetThirdItem(int firstType, int secondType);
	//void UI_InitUI();
	//xlsdata::BackpackTableDefine* GetBackpackXlsInfoWithXlsId(int32 xlsId);

	//UFUNCTION(BlueprintCallable)
	//	void UICallback_SelectFirstMenu(int firstType);
	//UFUNCTION(BlueprintCallable)
	//	void UICallback_SelectSecondMenu(int firstType, int secondType);
	//UFUNCTION(BlueprintCallable)
	//	void UICallback_SelectItem(int firstType, int secondType, int listIndex);
	//UFUNCTION(BlueprintCallable)
	//	void UICallback_RefreshPic(UObject* component, FString url);
	//UFUNCTION(BlueprintCallable)
	//	void UICallback_CloseImage(UObject* component);
	//UFUNCTION(BlueprintCallable)
	//	void UICallback_SavePic(FString url, FString name);

	/*------------------------------------------------------------------*/

};
