// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSScrollBox.h"
#include "TestScrollBox2.h"

#include "Rendering/DrawElements.h"
#include "Types/SlateConstants.h"
#include "Layout/LayoutUtils.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Images/SImage.h"


/*
 * allow overscroll 允许到达终点时的惯性应该关闭
 * 
 */

void TestSScrollBox::SetTest()
{
	bIsScrollingActiveTimerRegistered = false;
}

FReply TestSScrollBox::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// UE_LOG(LogTemp, Log, TEXT("[%x] [TestSScrollBox::OnMouseButtonUp] "), this);
	
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton && bAllowsRightClickDragScrolling)
	{
		if (!bIsScrollingActiveTimerRegistered && IsRightClickScrolling())
		{
			// Register the active timer to handle the inertial scrolling
			CachedGeometry = MyGeometry;
			// BeginInertialScrolling();
		}

		AmountScrolledWhileRightMouseDown = 0;

		Invalidate(EInvalidateWidget::Layout);

		FReply Reply = FReply::Handled().ReleaseMouseCapture();
		bShowSoftwareCursor = false;

		// If we have mouse capture, snap the mouse back to the closest location that is within the panel's bounds
		if (HasMouseCapture())
		{
			FSlateRect PanelScreenSpaceRect = MyGeometry.GetLayoutBoundingRect();
			FVector2D CursorPosition = MyGeometry.LocalToAbsolute(SoftwareCursorPosition);
		
			FIntPoint BestPositionInPanel(
				FMath::RoundToInt(FMath::Clamp(CursorPosition.X, PanelScreenSpaceRect.Left, PanelScreenSpaceRect.Right)),
				FMath::RoundToInt(FMath::Clamp(CursorPosition.Y, PanelScreenSpaceRect.Top, PanelScreenSpaceRect.Bottom))
			);
		
			Reply.SetMousePos(BestPositionInPanel);
		}

		return Reply;
	}

	return FReply::Unhandled();
}

// 鼠标滚轮事件
FReply TestSScrollBox::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// if (auto tmp = testScrollBox2.lock())
	if(testScrollBox2.IsValid())
	// if(testScrollBox2!=nullptr)
	{
		testScrollBox2->OnMouseWheel(MyGeometry, MouseEvent);
	}
	
	return SScrollBox::OnMouseWheel(MyGeometry, MouseEvent);
	// return FReply::Unhandled();
}


// FReply TestSScrollBox::OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
// {
// 	
// 	UE_LOG(LogTemp, Log, TEXT("[%x] [TestSScrollBox::OnPreviewMouseButtonDown] "), this);
// 	
// 	if (MouseEvent.IsTouchEvent() && !bFingerOwningTouchInteraction.IsSet())
// 	{
// 		// Clear any inertia 
// 		InertialScrollManager.ClearScrollVelocity();
// 		// We have started a new interaction; track how far the user has moved since they put their finger down.
// 		AmountScrolledWhileRightMouseDown = 0;
// 		PendingScrollTriggerAmount = 0;
// 		// Someone put their finger down in this list, so they probably want to drag the list.
// 		bFingerOwningTouchInteraction = MouseEvent.GetPointerIndex();
//
// 		Invalidate(EInvalidateWidget::Layout);
// 	}
// 	return FReply::Unhandled();
// }

// FReply TestSScrollBox::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
// {
// 	const float ScrollByAmountScreen = GetScrollComponentFromVector(MouseEvent.GetCursorDelta());
// 	const float ScrollByAmountLocal = ScrollByAmountScreen / MyGeometry.Scale;
//
// 	if (MouseEvent.IsTouchEvent())
// 	{
// 		FReply Reply = FReply::Unhandled();
// 	
// 		if (!bTouchPanningCapture)
// 		{
// 			if (bFingerOwningTouchInteraction.IsSet() && MouseEvent.IsTouchEvent() && !HasMouseCapture())
// 			{
// 				PendingScrollTriggerAmount += ScrollByAmountScreen;
// 	
// 				if (FMath::Abs(PendingScrollTriggerAmount) > FSlateApplication::Get().GetDragTriggerDistance())
// 				{
// 					bTouchPanningCapture = true;
// 					ScrollBar->BeginScrolling();
// 	
// 					// The user has moved the list some amount; they are probably
// 					// trying to scroll. From now on, the list assumes the user is scrolling
// 					// until they lift their finger.
// 					Reply = FReply::Handled().CaptureMouse(AsShared());
// 				}
// 				else
// 				{
// 					Reply = FReply::Handled();
// 				}
// 			}
// 		}
// 		else
// 		{
// 			if (bFingerOwningTouchInteraction.IsSet() && HasMouseCaptureByUser(MouseEvent.GetUserIndex(), MouseEvent.GetPointerIndex()))
// 			{
// 				LastScrollTime = FSlateApplication::Get().GetCurrentTime();
// 				InertialScrollManager.AddScrollSample(-ScrollByAmountScreen, FSlateApplication::Get().GetCurrentTime());
// 				ScrollBy(MyGeometry, -ScrollByAmountLocal, EAllowOverscroll::Yes, false);
// 	
// 				Reply = FReply::Handled();
// 			}
// 		}
// 	
// 		return Reply;
// 	}
// 	else
// 	{
// 		if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && bAllowsRightClickDragScrolling)
// 		{
// 			// If scrolling with the right mouse button, we need to remember how much we scrolled.
// 			// If we did not scroll at all, we will bring up the context menu when the mouse is released.
// 			AmountScrolledWhileRightMouseDown += FMath::Abs(ScrollByAmountScreen);
//
// 			// Has the mouse moved far enough with the right mouse button held down to start capturing
// 			// the mouse and dragging the view?
// 			if (IsRightClickScrolling())
// 			{
// 				InertialScrollManager.AddScrollSample(-ScrollByAmountScreen, FPlatformTime::Seconds());
// 				const bool bDidScroll = ScrollBy(MyGeometry, -ScrollByAmountLocal, AllowOverscroll, false);
//
// 				FReply Reply = FReply::Handled();
//
// 				// Capture the mouse if we need to
// 				if (HasMouseCapture() == false)
// 				{
// 					Reply.CaptureMouse(AsShared()).UseHighPrecisionMouseMovement(AsShared());
// 					SoftwareCursorPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
// 					bShowSoftwareCursor = true;
// 				}
//
// 				// Check if the mouse has moved.
// 				if (bDidScroll)
// 				{
// 					SetScrollComponentOnVector(SoftwareCursorPosition, GetScrollComponentFromVector(SoftwareCursorPosition) + GetScrollComponentFromVector(MouseEvent.GetCursorDelta()));
// 				}
//
// 				return Reply;
// 			}
// 		}
// 	}
//
// 	return FReply::Unhandled();
// }
