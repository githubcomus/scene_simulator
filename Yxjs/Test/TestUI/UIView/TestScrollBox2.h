
/*
 * 滚动条,指向拷贝的滚动代码.
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SWidget.h"
// #include "Widgets/Layout/SScrollBox.h"
#include "TestSScrollBox.h"
#include "Components/PanelWidget.h"
#include "TestScrollBox2.generated.h"

// 滚动条滚动委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserScrolledEvent2, float, CurrentOffset);

// 鼠标滚轮滚动委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseWheelEvent, float, CurrentOffset);

/**
 * An arbitrary scrollable collection of widgets.  Great for presenting 10-100 widgets in a list.  Doesn't support virtualization.
 */
UCLASS()
class YXJS_API UTestScrollBox2 : public UPanelWidget
{
	GENERATED_UCLASS_BODY()

public:

	/** The style */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "Style"))
	FScrollBoxStyle WidgetStyle;

	/** The bar style */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "Bar Style"))
	FScrollBarStyle WidgetBarStyle;

	UPROPERTY()
	USlateWidgetStyleAsset* Style_DEPRECATED;

	UPROPERTY()
	USlateWidgetStyleAsset* BarStyle_DEPRECATED;

	/** The orientation of the scrolling and stacking in the box. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	TEnumAsByte<EOrientation> Orientation;

	/** Visibility */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	ESlateVisibility ScrollBarVisibility;

	/** When mouse wheel events should be consumed. 滚轮输入捕获 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	EConsumeMouseWheel ConsumeMouseWheel;

	/** The thickness of the scrollbar thumb */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	FVector2D ScrollbarThickness;

	/** The margin around the scrollbar */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	FMargin ScrollbarPadding;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	bool AlwaysShowScrollbar;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	bool AlwaysShowScrollbarTrack;

	/**  Disable to stop scrollbars from activating inertial overscrolling */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	bool AllowOverscroll;

	/** True to lerp smoothly when wheel scrolling along the scroll box */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	bool bAnimateWheelScrolling = false;

	/** Sets where to scroll a widget to when using explicit navigation or if ScrollWhenFocusChanges is enabled */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	EDescendantScrollDestination NavigationDestination;

	/**
	 * The amount of padding to ensure exists between the item being navigated to, at the edge of the
	 * scrollbox.  Use this if you want to ensure there's a preview of the next item the user could scroll to.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	float NavigationScrollPadding;

	/** Scroll behavior when user focus is given to a child widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll", meta = (DisplayName = "Scroll When Focus Changes"))
	EScrollWhenFocusChanges ScrollWhenFocusChanges;

	/** Option to disable right-click-drag scrolling */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	bool bAllowRightClickDragScrolling;

	/** The multiplier to apply when wheel scrolling */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	float WheelScrollMultiplier = 1.f;


	UFUNCTION(BlueprintCallable, Category = "Scroll")
	void SetConsumeMouseWheel(EConsumeMouseWheel NewConsumeMouseWheel);

	UFUNCTION(BlueprintCallable, Category = "Scroll")
	void SetOrientation(EOrientation NewOrientation);

	UFUNCTION(BlueprintCallable, Category = "Scroll")
	void SetScrollBarVisibility(ESlateVisibility NewScrollBarVisibility);

	UFUNCTION(BlueprintCallable, Category = "Scroll")
	void SetScrollbarThickness(const FVector2D& NewScrollbarThickness);

	UFUNCTION(BlueprintCallable, Category = "Scroll")
	void SetScrollbarPadding(const FMargin& NewScrollbarPadding);

	UFUNCTION(BlueprintCallable, Category = "Scroll")
	void SetAlwaysShowScrollbar(bool NewAlwaysShowScrollbar);

	UFUNCTION(BlueprintCallable, Category = "Scroll")
	void SetAllowOverscroll(bool NewAllowOverscroll);

	UFUNCTION(BlueprintCallable, Category = "Scroll")
	void SetAnimateWheelScrolling(bool bShouldAnimateWheelScrolling);

	UFUNCTION(BlueprintCallable, Category = "Scroll")
	void SetWheelScrollMultiplier(float NewWheelScrollMultiplier);

	/** Instantly stops any inertial scrolling that is currently in progress */
	UFUNCTION(BlueprintCallable, Category = "Scroll")
	void EndInertialScrolling();

public:

	/** Called when the scroll has changed */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnUserScrolledEvent2 OnUserScrolled;

	/*鼠标滚轮滚动*/
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnMouseWheelEvent onMouseWheelEvent;

	/**
	 * Updates the scroll offset of the scrollbox.
	 * @param NewScrollOffset is in Slate Units.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetScrollOffset(float NewScrollOffset);

	/**
	 * Gets the scroll offset of the scrollbox in Slate Units.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	float GetScrollOffset() const;

	/** Gets the scroll offset of the bottom of the ScrollBox in Slate Units. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	float GetScrollOffsetOfEnd() const;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	float GetViewOffsetFraction() const;

	/** Scrolls the ScrollBox to the top instantly */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void ScrollToStart();

	/** Scrolls the ScrollBox to the bottom instantly during the next layout pass. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void ScrollToEnd();

	/** Scrolls the ScrollBox to the widget during the next layout pass. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void ScrollWidgetIntoView(UWidget* WidgetToFind, bool AnimateScroll = true,
	                          EDescendantScrollDestination ScrollDestination = EDescendantScrollDestination::IntoView,
	                          float Padding = 0);

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	//~ Begin UObject Interface


	void OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	// 蓝图事件 鼠标滚轮滚动
	// UFUNCTION(BlueprintImplementableEvent, Category = "UTestScrollBox2")
	// void Event_OnMouseWheel(float move);
	
#if WITH_EDITORONLY_DATA
	virtual void Serialize(FArchive& Ar) override;
	virtual void PostLoad() override;
#endif // if WITH_EDITORONLY_DATA
	//~ End UObject Interface

#if WITH_EDITOR
	//~ Begin UWidget Interface
	virtual const FText GetPaletteCategory() override;
	virtual void OnDescendantSelectedByDesigner(UWidget* DescendantWidget) override;
	virtual void OnDescendantDeselectedByDesigner(UWidget* DescendantWidget) override;
	//~ End UWidget Interface
#endif

protected:

	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget

	void SlateHandleUserScrolled(float CurrentOffset);

protected:
	/** The desired scroll offset for the underlying scrollbox.  This is a cache so that it can be set before the widget is constructed. */
	float DesiredScrollOffset;

	TSharedPtr<class TestSScrollBox> MyScrollBox;

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

#if WITH_EDITOR
	FDelegateHandle TickHandle;
#endif //WITH_EDITOR
};
