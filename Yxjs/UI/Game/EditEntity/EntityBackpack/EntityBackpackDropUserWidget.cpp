// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityBackpackDropUserWidget.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/PlayerPreviewComponent.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Engine/Texture2D.h>
#include <Engine/AssetManager.h>
#include <Components/EditableText.h>
#include <Components/Overlay.h>
#include "EntityBackpack_Item.h"
#include "EntityBackpackPanelUserWidget.h"
#include "Yxjs/Pawn/Character/CharacterComponent/CharacterDataComponent.h"

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

//
void UEntityBackpackDropUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityBackpackDropUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityBackpackDropUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

//
bool UEntityBackpackDropUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [NativeOnDrop] 拖拽释放  "), this);
	return true;
}

//
void UEntityBackpackDropUserWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [NativeOnDragEnter] 进入拖拽  "), this);

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//1. 进入预览区域,结束拖拽
	UWidgetBlueprintLibrary::CancelDragDrop();

	//2. 输入模式改为预览
	owner->ChangeEditEntityOrPreview(false);

	//3. 启动预览
	auto item = Cast<UEntityBackpack_Item>(InOperation->Payload);
	if (item == nullptr)return;
	CreatePreview(item->xlsId);

}

//
void UEntityBackpackDropUserWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [NativeOnDragEnter] 离开拖拽  "), this);

}

//
void UEntityBackpackDropUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

}

// 创建预览
void UEntityBackpackDropUserWidget::CreatePreview(int xlsId)
{

	ABaseCharacter* character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;

	Gamedata::EntityInfo syncInfo;

	{
		syncInfo.set_entityid(0);
		syncInfo.set_playerid(character->myData->playerId);
		syncInfo.set_collision_type(Gamedata::EntityCollisionType_QueryAndPhysics);
		auto spawnWTransform = syncInfo.mutable_transform();
		FTransform transform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
		UCommonlyObject::Ue4ToPbTransform(transform, spawnWTransform);
	}

	{
		auto child = syncInfo.add_child_list();
		child->mutable_mesh()->set_xls_id(xlsId);
		child->mutable_mesh()->set_material_id(0);
		auto rTransform = child->mutable_transform();
		FTransform transform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
		UCommonlyObject::Ue4ToPbTransform(transform, rTransform);
	}

	character->previewComponent->OpenClosePerview(false, nullptr, true);
	character->previewComponent->OpenClosePerview(true, &syncInfo, true);
}
