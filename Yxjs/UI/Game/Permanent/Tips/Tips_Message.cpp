// Fill out your copyright notice in the Description page of Project Settings.

#include "Tips_Message.h"

#include "Tools/CommonlyObject.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Tools/CalculationObject.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerMoveTimelineObject.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityTimelineComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerCommandComponent.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Overlay.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "UMG/Public/Components/VerticalBox.h"
#include <Components/EditableText.h>
#include <Components/TextBlock.h>
#include "Tips_Main.h"

/*


*/


/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UTips_Message::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UTips_Message::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UTips_Message::NativeDestruct()
{
	Super::NativeDestruct();


}

//
void UTips_Message::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	//
	TextBlock_str = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_str")));
	CanvasPanel_move = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_move")));

	UCanvasPanelSlot* canvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CanvasPanel_move.Get());
	if (canvasPanelSlot)
	{
		canvasPanelSlot->SetZOrder(20);
	}
}

//
void UTips_Message::Start(TWeakObjectPtr<UTips_Main> owner, FString str)
{
	owner_ = owner;

	TextBlock_str->SetText(FText::FromString(*str));
}

//
void UTips_Message::End()
{

}