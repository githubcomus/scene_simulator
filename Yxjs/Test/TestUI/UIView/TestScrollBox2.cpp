// Fill out your copyright notice in the Description page of Project Settings.


#include "TestScrollBox2.h"

// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/ScrollBox.h"
#include "Containers/Ticker.h"
#include "Components/ScrollBoxSlot.h"
#include "UObject/EditorObjectVersion.h"
#include "Styling/SlateTypes.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UTestScrollBox2

static FScrollBoxStyle* DefaultScrollBoxStyle = nullptr;
static FScrollBarStyle* DefaultScrollBoxBarStyle = nullptr;


UTestScrollBox2::UTestScrollBox2(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , Orientation(Orient_Vertical)
	  , ScrollBarVisibility(ESlateVisibility::Visible)
	  , ConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible)
	  , ScrollbarThickness(9.0f, 9.0f)
	  , ScrollbarPadding(2.0f)
	  , AlwaysShowScrollbar(false)
	  , AlwaysShowScrollbarTrack(false)
	  , AllowOverscroll(true)
	  , NavigationDestination(EDescendantScrollDestination::IntoView)
	  , NavigationScrollPadding(0.0f)
	  , ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
{
	bIsVariable = false;

	Visibility = ESlateVisibility::Visible;
	Clipping = EWidgetClipping::ClipToBounds;

	if (DefaultScrollBoxStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
		DefaultScrollBoxStyle = new FScrollBoxStyle(FCoreStyle::Get().GetWidgetStyle<FScrollBoxStyle>("ScrollBox"));

		// Unlink UMG default colors from the editor settings colors.
		DefaultScrollBoxStyle->UnlinkColors();
	}

	if (DefaultScrollBoxBarStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
		DefaultScrollBoxBarStyle = new FScrollBarStyle(FCoreStyle::Get().GetWidgetStyle<FScrollBarStyle>("ScrollBar"));

		// Unlink UMG default colors from the editor settings colors.
		DefaultScrollBoxBarStyle->UnlinkColors();
	}

	WidgetStyle = *DefaultScrollBoxStyle;
	WidgetBarStyle = *DefaultScrollBoxBarStyle;

	bAllowRightClickDragScrolling = true;

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::UTestScrollBox2] "), this);
}

void UTestScrollBox2::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyScrollBox.Reset();
}

UClass* UTestScrollBox2::GetSlotClass() const
{
	return UScrollBoxSlot::StaticClass();
}

void UTestScrollBox2::OnSlotAdded(UPanelSlot* InSlot)
{


	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::OnSlotAdded] "), this);
	
	// Add the child to the live canvas if it already exists
	if (MyScrollBox.IsValid())
	{
		CastChecked<UScrollBoxSlot>(InSlot)->BuildSlot(MyScrollBox.ToSharedRef());
	}
}

void UTestScrollBox2::OnSlotRemoved(UPanelSlot* InSlot)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::OnSlotRemoved] "), this);
	
	// Remove the widget from the live slot if it exists.
	if (MyScrollBox.IsValid())
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			MyScrollBox->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

TSharedRef<SWidget> UTestScrollBox2::RebuildWidget()
{
	MyScrollBox = SNew(TestSScrollBox)
		.Style(&WidgetStyle)
		.ScrollBarStyle(&WidgetBarStyle)
		.Orientation(Orientation)
		.ConsumeMouseWheel(ConsumeMouseWheel)
		.NavigationDestination(NavigationDestination)
		.NavigationScrollPadding(NavigationScrollPadding)
		.ScrollWhenFocusChanges(ScrollWhenFocusChanges)
		.AnimateWheelScrolling(bAnimateWheelScrolling)
		.WheelScrollMultiplier(WheelScrollMultiplier)
		.OnUserScrolled(BIND_UOBJECT_DELEGATE(FOnUserScrolled, SlateHandleUserScrolled));

	MyScrollBox->testScrollBox2 = this;

	// TWeakPtr<UTestScrollBox2> MyClass_Weak(this);
	// SharedThis(this);
	// MyScrollBox->testScrollBox2_ptr = SharedThis(this);
	
	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UScrollBoxSlot* TypedSlot = Cast<UScrollBoxSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyScrollBox.ToSharedRef());
		}
	}
	// MyScrollBox->bIsScrollingActiveTimerRegistered = false;
	return MyScrollBox.ToSharedRef();
}

void UTestScrollBox2::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyScrollBox->SetScrollOffset(DesiredScrollOffset);
	MyScrollBox->SetOrientation(Orientation);
	MyScrollBox->SetScrollBarVisibility(UWidget::ConvertSerializedVisibilityToRuntime(ScrollBarVisibility));
	MyScrollBox->SetScrollBarThickness(ScrollbarThickness);
	MyScrollBox->SetScrollBarPadding(ScrollbarPadding);
	MyScrollBox->SetScrollBarAlwaysVisible(AlwaysShowScrollbar);
	MyScrollBox->SetScrollBarTrackAlwaysVisible(AlwaysShowScrollbarTrack);
	MyScrollBox->SetAllowOverscroll(AllowOverscroll ? EAllowOverscroll::Yes : EAllowOverscroll::No);
	MyScrollBox->SetScrollBarRightClickDragAllowed(bAllowRightClickDragScrolling);
	MyScrollBox->SetConsumeMouseWheel(ConsumeMouseWheel);
	MyScrollBox->SetAnimateWheelScrolling(bAnimateWheelScrolling);
	MyScrollBox->SetWheelScrollMultiplier(WheelScrollMultiplier);
}

float UTestScrollBox2::GetScrollOffset() const
{
	if (MyScrollBox.IsValid())
	{
		return MyScrollBox->GetScrollOffset();
	}

	return 0;
}

float UTestScrollBox2::GetScrollOffsetOfEnd() const
{
	if (MyScrollBox.IsValid())
	{
		return MyScrollBox->GetScrollOffsetOfEnd();
	}

	return 0;
}

float UTestScrollBox2::GetViewOffsetFraction() const
{
	if (MyScrollBox.IsValid())
	{
		return MyScrollBox->GetViewOffsetFraction();
	}

	return 0;
}

void UTestScrollBox2::SetScrollOffset(float NewScrollOffset)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::SetScrollOffset] "), this);
	DesiredScrollOffset = NewScrollOffset;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetScrollOffset(NewScrollOffset);
	}
}

void UTestScrollBox2::ScrollToStart()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::ScrollToStart] "), this);
	if (MyScrollBox.IsValid())
	{
		MyScrollBox->ScrollToStart();
	}
}

void UTestScrollBox2::ScrollToEnd()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::ScrollToEnd] "), this);
	if (MyScrollBox.IsValid())
	{
		MyScrollBox->ScrollToEnd();
	}
}

void UTestScrollBox2::ScrollWidgetIntoView(UWidget* WidgetToFind, bool AnimateScroll,
                                           EDescendantScrollDestination InScrollDestination, float Padding)
{
	TSharedPtr<SWidget> SlateWidgetToFind;
	if (WidgetToFind)
	{
		SlateWidgetToFind = WidgetToFind->GetCachedWidget();
	}

	if (MyScrollBox.IsValid())
	{
		// NOTE: Pass even if null! This, in effect, cancels a request to scroll which is necessary to avoid warnings/ensures 
		//       when we request to scroll to a widget and later remove that widget!
		MyScrollBox->ScrollDescendantIntoView(SlateWidgetToFind, AnimateScroll, InScrollDestination, Padding);
	}
}

#if WITH_EDITORONLY_DATA

void UTestScrollBox2::Serialize(FArchive& Ar)
{
	Ar.UsingCustomVersion(FEditorObjectVersion::GUID);

	const bool bDeprecateThickness = Ar.IsLoading() && Ar.CustomVer(FEditorObjectVersion::GUID) < FEditorObjectVersion::
		ScrollBarThicknessChange;
	if (bDeprecateThickness)
	{
		// Set ScrollbarThickness property to previous default value.
		ScrollbarThickness.Set(5.0f, 5.0f);
	}

	Super::Serialize(Ar);

	if (bDeprecateThickness)
	{
		// Implicit padding of 2 was removed, so ScrollbarThickness value must be incremented by 4.
		ScrollbarThickness += FVector2D(4.0f, 4.0f);
	}
}

void UTestScrollBox2::PostLoad()
{
	Super::PostLoad();

	if (GetLinkerUE4Version() < VER_UE4_DEPRECATE_UMG_STYLE_ASSETS)
	{
		if (Style_DEPRECATED != nullptr)
		{
			const FScrollBoxStyle* StylePtr = Style_DEPRECATED->GetStyle<FScrollBoxStyle>();
			if (StylePtr != nullptr)
			{
				WidgetStyle = *StylePtr;
			}

			Style_DEPRECATED = nullptr;
		}

		if (BarStyle_DEPRECATED != nullptr)
		{
			const FScrollBarStyle* StylePtr = BarStyle_DEPRECATED->GetStyle<FScrollBarStyle>();
			if (StylePtr != nullptr)
			{
				WidgetBarStyle = *StylePtr;
			}

			BarStyle_DEPRECATED = nullptr;
		}
	}
}

#endif // if WITH_EDITORONLY_DATA

void UTestScrollBox2::SetConsumeMouseWheel(EConsumeMouseWheel NewConsumeMouseWheel)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::SetConsumeMouseWheel] "), this);
	ConsumeMouseWheel = NewConsumeMouseWheel;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetConsumeMouseWheel(NewConsumeMouseWheel);
	}
}

void UTestScrollBox2::SetOrientation(EOrientation NewOrientation)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::SetOrientation] "), this);
	Orientation = NewOrientation;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetOrientation(Orientation);
	}
}

void UTestScrollBox2::SetScrollBarVisibility(ESlateVisibility NewScrollBarVisibility)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::SetScrollBarVisibility] "), this);
	ScrollBarVisibility = NewScrollBarVisibility;

	if (MyScrollBox.IsValid())
	{
		switch (ScrollBarVisibility)
		{
		case ESlateVisibility::Collapsed: MyScrollBox->SetScrollBarVisibility(EVisibility::Collapsed);
			break;
		case ESlateVisibility::Hidden: MyScrollBox->SetScrollBarVisibility(EVisibility::Hidden);
			break;
		case ESlateVisibility::HitTestInvisible: MyScrollBox->SetScrollBarVisibility(EVisibility::HitTestInvisible);
			break;
		case ESlateVisibility::SelfHitTestInvisible: MyScrollBox->SetScrollBarVisibility(
				EVisibility::SelfHitTestInvisible);
			break;
		case ESlateVisibility::Visible: MyScrollBox->SetScrollBarVisibility(EVisibility::Visible);
			break;
		}
	}
}

void UTestScrollBox2::SetScrollbarThickness(const FVector2D& NewScrollbarThickness)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::SetScrollbarThickness] "), this);
	ScrollbarThickness = NewScrollbarThickness;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetScrollBarThickness(ScrollbarThickness);
	}
}

void UTestScrollBox2::SetScrollbarPadding(const FMargin& NewScrollbarPadding)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::SetScrollbarPadding] "), this);
	ScrollbarPadding = NewScrollbarPadding;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetScrollBarPadding(ScrollbarPadding);
	}
}

void UTestScrollBox2::SetAlwaysShowScrollbar(bool NewAlwaysShowScrollbar)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::SetScrollbarPadding] "), this);
	AlwaysShowScrollbar = NewAlwaysShowScrollbar;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetScrollBarAlwaysVisible(AlwaysShowScrollbar);
	}
}

void UTestScrollBox2::SetAllowOverscroll(bool NewAllowOverscroll)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::SetAllowOverscroll] "), this);
	AllowOverscroll = NewAllowOverscroll;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetAllowOverscroll(AllowOverscroll ? EAllowOverscroll::Yes : EAllowOverscroll::No);
	}
}

void UTestScrollBox2::SetAnimateWheelScrolling(bool bShouldAnimateWheelScrolling)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::SetAnimateWheelScrolling] "), this);
	bAnimateWheelScrolling = bShouldAnimateWheelScrolling;
	if (MyScrollBox)
	{
		MyScrollBox->SetAnimateWheelScrolling(bShouldAnimateWheelScrolling);
	}
}

void UTestScrollBox2::SetWheelScrollMultiplier(float NewWheelScrollMultiplier)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::SetWheelScrollMultiplier] "), this);
	WheelScrollMultiplier = NewWheelScrollMultiplier;
	if (MyScrollBox)
	{
		MyScrollBox->SetWheelScrollMultiplier(NewWheelScrollMultiplier);
	}
}

void UTestScrollBox2::EndInertialScrolling()
{
	if (MyScrollBox.IsValid())
	{
		MyScrollBox->EndInertialScrolling();
	}
}

void UTestScrollBox2::SlateHandleUserScrolled(float CurrentOffset)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::SlateHandleUserScrolled] [CurrentOffset:%f] "), this, CurrentOffset);
	OnUserScrolled.Broadcast(CurrentOffset);
}

#if WITH_EDITOR

const FText UTestScrollBox2::GetPaletteCategory()
{
	return LOCTEXT("Panel", "Panel");
}

void UTestScrollBox2::OnDescendantSelectedByDesigner(UWidget* DescendantWidget)
{
	UWidget* SelectedChild = UWidget::FindChildContainingDescendant(this, DescendantWidget);
	if (SelectedChild)
	{
		ScrollWidgetIntoView(SelectedChild, true);

		if (TickHandle.IsValid())
		{
			FTicker::GetCoreTicker().RemoveTicker(TickHandle);
			TickHandle.Reset();
		}
	}
}

void UTestScrollBox2::OnDescendantDeselectedByDesigner(UWidget* DescendantWidget)
{
	if (TickHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(TickHandle);
		TickHandle.Reset();
	}

	// because we get a deselect before we get a select, we need to delay this call until we're sure we didn't scroll to another widget.
	TickHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float) -> bool
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_UTestScrollBox2_ScrollToStart_LambdaTick);
		this->ScrollToStart();
		return false;
	}));
}

#endif
//
// /////////////////////////////////////////////////////
//
#undef LOCTEXT_NAMESPACE


// 鼠标滚轮事件
void UTestScrollBox2::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// UE_LOG(LogTemp, Log, TEXT("[%x] [UTestScrollBox2::OnMouseWheel] [%f]"), this, MouseEvent.GetWheelDelta());
	onMouseWheelEvent.Broadcast(MouseEvent.GetWheelDelta());
}