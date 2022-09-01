// Fill out your copyright notice in the Description page of Project Settings.

#include "ImageUserWidget.h"

#include "Components/Image.h"

// //
// void UImageUserWidget::PostLoad()
// {
// 	Super::PostLoad();
// 	UE_LOG(LogTemp, Log, TEXT("[%x] [UImageUserWidget::PostLoad]   "), this);
// }
//
// //
// void UImageUserWidget::BeginDestroy()
// {
// 	Super::BeginDestroy();
// 	UE_LOG(LogTemp, Log, TEXT("[%x] [UImageUserWidget::BeginDestroy]   "), this);
// }

void UImageUserWidget::Start()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UImageUserWidget::Start]   "), this);
}

void UImageUserWidget::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UImageUserWidget::End]   "), this);
}

void UImageUserWidget::LoadImage(FString url)
{
	// 找到world
	// 找到gi
	// 找到网络资源管理器
	// 加载图片到组件
}

// bool UImageUserWidget::Initialize()
// {
// 	Super::Initialize();
// 	UE_LOG(LogTemp, Log, TEXT("[%x] [UImageUserWidget::Initialize]   "), this);
// }