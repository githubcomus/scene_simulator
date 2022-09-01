// Fill out your copyright notice in the Description page of Project Settings.

#include "MySoundEffect.h"
#include "../BaseGameInstance.h"

#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceConstant.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include <Curves/CurveVector.h>
#include <GameFramework/GameUserSettings.h>
#include <Sound/SoundCue.h>

/*

// 别处引用提供


#include "Core/BaseGameInstance.h"
#include "Core/SoundEffect/MySoundEffect.h"

if (auto gi = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this)))gi->mySoundEffect->PlaySound(MySoundEffectType::MySoundEffectType_ui_买);

*/



/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

//
UMySoundEffect::UMySoundEffect()
{

}

// BeginPlay
void UMySoundEffect::Start(UBaseGameInstance* owner)
{
	owner_ = owner;

	bool isEdit = false;
#if WITH_EDITORONLY_DATA
	isEdit = true;
#endif


	auto& xlsMap = owner_->zwXlsDataActorComponent->soundEffectMap;
	for (auto node : xlsMap)
	{
		soundMap.emplace(node.second.Name_, &xlsMap[node.first]);
	}
}

// EndPlay
void UMySoundEffect::End()
{
}

// 播放声音.
void UMySoundEffect::PlaySound(EMySoundEffectType type)
{
	//

	std::string name = soundEffectList[(int)type];
	auto it = soundMap.find(name);
	if (it == soundMap.end())return;

	std::string path = it->second->Path_;
	FString name2 = UTF8_TO_TCHAR(name.c_str());
	FString path2 = UTF8_TO_TCHAR(path.c_str());

	auto tempJobId = (int)type;
	// 正在加载中的任务进行销毁
	if (handleMap.Find(tempJobId))
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[%x] [音效管理] [PlaySound] [%s]"), this, *name2);


	PlaySound(path2, tempJobId);
}

// 唯一播放.(停止所有任务)

// 独立任务播放.(直接播)

// 单例播放.(停止同款任务,启动新任务)

//
void UMySoundEffect::PlaySound(FString path, bool start, int jobId, bool useLocation, FVector location)
{
	if (start)
	{
		TSharedPtr<FStreamableHandle> handle;
		handleMap.Add(jobId, handle);

		FSoftObjectPath softObjectPath;
		softObjectPath.SetPath(path);
		handleMap[jobId] = UAssetManager::GetStreamableManager().RequestAsyncLoad(softObjectPath,
			FStreamableDelegate::CreateUObject(
				this, &UMySoundEffect::PlaySound,
				path, false, jobId, useLocation, location),
			FStreamableManager::AsyncLoadHighPriority,
			false);
	}
	else
	{
		if (handleMap.Find(jobId) == nullptr)
		{
			return;
		}
		auto& handle_ = handleMap[jobId];
		if (handle_ && handle_.IsValid() && handle_->IsActive() && handle_->HasLoadCompleted())
		{
			auto tempAss = handle_->GetLoadedAsset();
			USoundWave* ass = Cast<USoundWave>(tempAss);
			//USoundCue* ass = Cast<USoundCue>(tempAss);
			if (ass)
			{
				if (useLocation)
				{
					UGameplayStatics::PlaySoundAtLocation(this, ass, location);
				}
				else
				{
					UGameplayStatics::PlaySound2D(this, ass);
				}
			}
		}
		handleMap.Remove(jobId);
	}
}

