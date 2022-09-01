// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Layout/Geometry.h"

class UTestScrollBox2;

/**
 * 滚动条处理类,屏蔽惯性滚动
 */
class  TestSScrollBox : public SScrollBox
{
public :

	// UTestScrollBox2* testScrollBox2 = nullptr;
	// std::weak_ptr<UTestScrollBox2> testScrollBox2;
	TWeakObjectPtr<UTestScrollBox2> testScrollBox2;
	
	void SetTest();

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	// virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	// virtual FReply OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
};
