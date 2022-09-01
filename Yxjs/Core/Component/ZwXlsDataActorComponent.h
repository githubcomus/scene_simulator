// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <string>
#include "XlsData/Logger.h"
#include "XlsData/DataReader.h"
#include "XlsData/XlsData.h"
#include "ZwXlsDataActorComponent.generated.h"

////using namespace std;

class UBaseGameInstance;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UZwXlsDataActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	UBaseGameInstance* gameInstance;

	// 数据
	xlsdata::XlsData xlsData;

	// 菜单栏目
	std::map<int32, xlsdata::MenuBarTableDefine>menuBarMap;

	// 物资文件夹
	std::map<int32, xlsdata::BackpackTableDefine>backpacktMap;

	// 摆放
	std::map<int32, xlsdata::PlaceTableDefine>placeMap;

	// 材质
	std::map<int32, xlsdata::MaterialTableDefine>materialMap;

	// 音效
	std::map<int32, xlsdata::SoundEffectTableDefine> soundEffectMap;

	/*------------------------------------------------------------------*/

	// Sets default values for this component's properties
	UZwXlsDataActorComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//
	void Start(UBaseGameInstance* gameInstance_);

	//
	void End();

	//
	std::string GBKToUTF8(const std::string& str);

	//
	std::string UTF8TOGBK(std::string strValue);

	//
	void Test();

	//
	void SetData();

	/*------------------------------------------------------------------*/
};
