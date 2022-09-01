// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineResourcesComponent.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "GameMode/Component/BaseGameModeController.h"
#include "Tools/RingBuffer.h"
#include "DownloadSaveFiles.h"
#include "LoadImageObject.h"
#include "GameMode/BaseGameMode.h"
#include "GameMode/Component/BaseGameModeController.h"

#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Runtime/Engine/Classes/GameFramework/GameModeBase.h"
#include "Runtime/UMG/Public/Components/PanelSlot.h"
#include "Json/Public/Serialization/JsonSerializer.h"


#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <memory>
#include <list>
#include <process.h>

//--------冲突的头文件--------//
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"

#include <Ws2tcpip.h>  //InetNtop 和InetPton 声明的文件
#include <windows.h>
#include <winsock2.h>//管道

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
//--------冲突的头文件--------//


/*
 * 增删改查依旧由game服务器控制,本控制器只控制下发列表去网络中下载图片列队任务.
 * 本地已经下载数据照片管理和数据库库管理
 *
 */


 /*------------------------------------------------------------------*/

 // Sets default values for this component's properties
UOnlineResourcesComponent::UOnlineResourcesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOnlineResourcesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

/*------------------------------------------------------------------*/

//
void UOnlineResourcesComponent::Start(UBaseGameInstance* gameInstance_)
{
	gameInstance = gameInstance_;
	UE_LOG(LogTemp, Log, TEXT("[%x] [UOnlineResourcesComponent::Start] [gameInstance_:%x]"), this, gameInstance_);

	loadImageObject = NewObject<ULoadImageObject>(this, TEXT("ULoadImageObject"));

	childThreadInfo.downloadSaveFiles = std::make_shared<DownloadSaveFiles>(this);
	mainQueueInfo.ringBuffer = new RingBuffer(1024);
	childQueueInfo.ringBuffer = new RingBuffer(1024 * 10);
	ChildController(true);
}

//
void UOnlineResourcesComponent::End()
{
	ChildController(false);

	delete mainQueueInfo.ringBuffer;
	delete childQueueInfo.ringBuffer;
	mainQueueInfo.ringBuffer = nullptr;
	childQueueInfo.ringBuffer = nullptr;

	UE_LOG(LogTemp, Log, TEXT("[%x] [UOnlineResourcesComponent::End] [gameInstance_:%x]"), this, gameInstance);
}

/*------------------------------------------------------------------*/

// 主线程队列 读数据
void UOnlineResourcesComponent::TickFrame(float DeltaTime)
{

	auto nowTime = gameInstance->GetWorld()->TimeSeconds;

	if (nowTime - mainQueueInfo.lastTime > 0.2)
	{
		mainQueueInfo.lastTime = nowTime;

		std::string recvData;
		if (mainQueueInfo.ringBuffer && mainQueueInfo.ringBuffer->Read(recvData))
		{
			MastrThreadReadData(recvData);
		}
	}

}

// 主线程队列 写数据
void UOnlineResourcesComponent::MasterDownloadImageAndPreview(FString url)
{
	// 给队列一个事件
	FString str = MakeEventJsonStr(Event_MasterDownloadImageAndPreview, 0, url);
	std::string str2(TCHAR_TO_UTF8(*str));
	mainQueueInfo.ringBuffer->Write(str2);
}

// 主线程队列 处理数据
void UOnlineResourcesComponent::MastrThreadReadData(std::string& readData)
{
	FString str(readData.c_str());
	int type = -1;
	int id;
	FString value;
	ParsingEventJson(str, type, id, value);

	// 回调预览图片
	if (type == Event_MasterDownloadImageAndPreview)
	{
		LoadImageOk(value);
	}
}

/*------------------------------------------------------------------*/

// 显示日志
void UOnlineResourcesComponent::ShowLog(std::string str)
{
	// 解决中文乱码
	FString str2 = UTF8_TO_TCHAR(str.c_str());
	UE_LOG(LogTemp, Log, TEXT("%s"), *str2);

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UOnlineResourcesComponent::ShowLog] [world:%x] "), this, gameInstance->GetWorld());
}

/*------------------------------------------------------------------*/

// 获取 时间
float UOnlineResourcesComponent::GetTime()
{
	auto w = gameInstance->GetWorld();
	if (w)
	{
		return gameInstance->GetWorld()->TimeSeconds;
	}
	return 0;
}

// 获取 时间字符串
std::string UOnlineResourcesComponent::GetTimeStr()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UOnlineResourcesComponent::GetTimeStr] [world:%x] "), this, gameInstance->GetWorld());
	auto w = gameInstance->GetWorld();
	if (w)
	{
		float time = gameInstance->GetWorld()->TimeSeconds;
		char temp[20];
		sprintf_s(temp, 20, "%f", time);
		std::string str = temp;
		return str;
	}
	return "err";
}

// 获取 反查获取图片数据
UOnlineResourcesComponent::CvFLocalImageInfo* UOnlineResourcesComponent::GetImageInfo(FString url)
{
	if (urlDataMap.Find(url))
	{
		return &urlDataMap[url];
	}
	return nullptr;
}

// 获取 获取图片数据
UOnlineResourcesComponent::CvFLocalImageInfo* UOnlineResourcesComponent::GetImageInfo(UObject* component)
{
	if (componentUrlMap.Find(component))
	{
		auto info = &componentUrlMap[component];
		return GetImageInfo(info->url);
	}
	return nullptr;
}

// 获取 反查数据
UOnlineResourcesComponent::CvEntityInfo* UOnlineResourcesComponent::GetEntityInfo(UObject* component)
{
	if (componentUrlMap.Find(component))
	{
		return &componentUrlMap[component];
	}
	return nullptr;
}

// 获取 本地图片是否已经存在
bool UOnlineResourcesComponent::IsLocalImageExist(FString path)
{
	std::string filePath(TCHAR_TO_UTF8(*path));
	if (childThreadInfo.downloadSaveFiles->FileExistsStatus(filePath.c_str()))
	{
		// 验证图片是否正常解析

		return true;
	}
	return false;
}

/*------------------------------------------------------------------*/

// 返回jsn字符串
FString UOnlineResourcesComponent::GetJsonStr(TSharedPtr<FJsonObject> jsonObject)
{
	FString jsonStr = TEXT("");
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<>::Create(&jsonStr);
	bool result = FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
	writer->Close();
	return jsonStr;
}

// 制作事件json数据
FString UOnlineResourcesComponent::MakeEventJsonStr(int type, int id, FString value)
{
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);
	jsonObject->SetNumberField(TEXT("type"), type);
	jsonObject->SetNumberField(TEXT("id"), id);
	jsonObject->SetStringField(TEXT("value"), value);
	return GetJsonStr(jsonObject);
}

// 解析事件json数据
void UOnlineResourcesComponent::ParsingEventJson(FString& str, int& type, int& id, FString& value)
{
	TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(str);
	TSharedPtr<FJsonObject> jsonParsed;
	bool jsonFlag = FJsonSerializer::Deserialize(jsonReader, jsonParsed);
	if (jsonFlag == false)
	{
		return;
	}

	type = jsonParsed->GetNumberField(TEXT("type"));
	id = jsonParsed->GetNumberField(TEXT("id"));
	value = jsonParsed->GetStringField(TEXT("value"));
}

/*------------------------------------------------------------------*/

// 下载线程 子线程队列
void UOnlineResourcesComponent::ChildController(bool flag)
{
	if (flag && childThreadInfo.state == ThreadWait)
	{
		childThreadInfo.state = ThreadRun;

		_beginthreadex(nullptr, 0, ChildThread, this, 0, nullptr);
	}
	else if (flag == false && childThreadInfo.state == ThreadRun)
	{
		childThreadInfo.state = ThreadStoping;

		bool flag2 = false;
		// 等待20s没有退出,错误.
		for (int i = 0; i < 100; i++)
		{
			if (childThreadInfo.state == ThreadStop)
			{
				flag2 = true;
				break;
			}
			FPlatformProcess::Sleep(0.05);
		}
		// ERR
		if (flag2 == false)
		{
			std::string str = "[ChildController] ERR1 ";
		}
	}
	// ERR
	else
	{
		std::string str = "[ChildController] ERR2 ";
	}
}

// 下载线程 线程
unsigned __stdcall UOnlineResourcesComponent::ChildThread(void* arg)
{
	UOnlineResourcesComponent* gwi = (UOnlineResourcesComponent*)arg;
	gwi->ChildThreadFunc();
	return 0;
}

// 下载线程 读数据
void UOnlineResourcesComponent::ChildThreadFunc()
{
	UE_LOG(LogTemp, Log, TEXT("[UOnlineResourcesComponent::ChildThreadFunc] start"));

	int run = 0;
	while (childThreadInfo.state == ThreadRun)
	{
		// 读取队列数据处理
		std::string readData;
		if (childQueueInfo.ringBuffer->Read(readData))
		{
			ChildThreadReadData(readData);
			Sleep(100);
		}
		else
		{
			for (run = 0; run < 10 && childThreadInfo.state == ThreadRun; run++)
			{
				Sleep(100);
			}
		}
	}

	childThreadInfo.state = ThreadStop;

	UE_LOG(LogTemp, Log, TEXT("[UOnlineResourcesComponent::ChildThreadFunc] end"));
}

// 下载线程 写数据
void UOnlineResourcesComponent::ChildDownloadImageAndPreview(FString url)
{
	// 给队列一个事件
	FString str = MakeEventJsonStr(Event_ChildDownloadImageAndPreview, 0, url);
	std::string str2(TCHAR_TO_UTF8(*str));
	childQueueInfo.ringBuffer->Write(str2);
}

// 下载线程 处理数据
void UOnlineResourcesComponent::ChildThreadReadData(std::string& readData)
{
	FString str(readData.c_str());
	int type = -1;
	int id;
	FString value;
	ParsingEventJson(str, type, id, value);

	// 下载并预览图片
	if (type == Event_ChildDownloadImageAndPreview)
	{
		DownloadImage(value);
	}
}

/*------------------------------------------------------------------*/

// 业务 从网络下载一张照片
void UOnlineResourcesComponent::DownloadImage(FString url)
{
	auto imageInfo = GetImageInfo(url);
	if (imageInfo)
	{
		std::string url2(TCHAR_TO_UTF8(*url));
		std::string filePath(TCHAR_TO_UTF8(*imageInfo->path));
		if (childThreadInfo.downloadSaveFiles->DownloadFiles(url2.c_str(), filePath.c_str()))
		{
			// UE_LOG(LogTemp, Log, TEXT("[DownloadImage] 下载照片:%s"), *url);

			bool isValid;
			int w, h;
			imageInfo->texture2D = loadImageObject->LoadTexture2D(imageInfo->path, isValid, w, h);

			MasterDownloadImageAndPreview(url);
		}
	}
}

// 业务 加载本地照片到UI
void UOnlineResourcesComponent::LoadImageOk(FString url)
{
	auto imageInfo = GetImageInfo(url);
	if (imageInfo)
	{
		for (auto imageComponent : imageInfo->componentList)
		{
			LoadSingleImage(imageComponent, imageInfo);
		}
		UE_LOG(LogTemp, Log, TEXT("[UOnlineResourcesComponent::LoadImageOk] 加载照片:%s [count:%d]"), *url,
			imageInfo->componentList.Num());
	}
}

// 业务 加载本地
void UOnlineResourcesComponent::LoadSingleImage(UObject* imageComponent, CvFLocalImageInfo* imageInfo)
{
	if (imageComponent && IsValid(imageComponent))
	{
		// 反查数据
		auto entityInfo = GetEntityInfo(imageComponent);
		if (entityInfo->type == CallBackEventType::Event_UILoad)
		{
			LoadImageToUI(imageComponent, imageInfo, entityInfo);
		}
		else if (entityInfo->type == CallBackEventType::Event_GameLoad)
		{
			LoadImageToEntity(imageComponent, imageInfo, entityInfo);
		}
	}
}

// 业务 加载照片到UI
void UOnlineResourcesComponent::LoadImageToUI(UObject* component, CvFLocalImageInfo* imageInfo, CvEntityInfo* entityInfo)
{
	UImage* image = Cast<UImage>(component);
	if (imageInfo->texture2D.IsValid() && image)
	{
		image->SetBrushFromTexture(imageInfo->texture2D.Get());
		auto imageX = imageInfo->texture2D->GetSizeX();
		auto imageY = imageInfo->texture2D->GetSizeY();

		auto brushX = image->Brush.ImageSize.X;
		auto brushY = image->Brush.ImageSize.Y;

		// image->SetBrushSize(FVector2D(imageX, imageY));

		// 在框内
		if (imageX <= brushX && imageY <= brushY)
		{
			image->SetBrushSize(FVector2D(imageX, imageY));
		}
		// 超出框
		else
		{
			// 同时超出
			if (imageX > brushX && imageY > brushY)
			{
				image->SetBrushSize(FVector2D(brushX, brushX));
			}
			// 单边超出
			else if (imageX > brushX)
			{
				// imageX* n = brushX;
				// imageY* n = Y;
				auto y = (brushX / imageX) * imageY;
				image->SetBrushSize(FVector2D(brushX, y));
			}
			// 单边超出
			else if (imageY > brushY)
			{
				// imageY* n = brushY;
				// imageX* n = X;
				auto x = (brushY / imageY) * imageX;
				image->SetBrushSize(FVector2D(x, brushY));
			}
		}
	}
}

// 业务 加载照片到游戏
void UOnlineResourcesComponent::LoadImageToEntity(UObject* component, CvFLocalImageInfo* imageInfo,
	CvEntityInfo* entityInfo)
{
	UImage* image = Cast<UImage>(component);
	if (imageInfo->texture2D.IsValid() && image)
	{
		// image->SetBrushFromTexture(imageInfo->texture2D);
		auto imageX = imageInfo->texture2D->GetSizeX();
		auto imageY = imageInfo->texture2D->GetSizeY();
		//
		// UCanvasPanelSlot* canvasPanelSlot = Cast<UCanvasPanelSlot>(image->Slot);
		// if(canvasPanelSlot)
		// {
		// 	FMargin margin;
		// 	margin.Left = -(imageX/2);
		// 	margin.Top = -(imageY/2);
		// 	margin.Right = imageX;
		// 	margin.Bottom = imageY;
		// 	canvasPanelSlot->LayoutData.Offsets = margin;
		// 	auto temp = canvasPanelSlot->LayoutData;
		// 	temp.Offsets = margin;
		// 	canvasPanelSlot->SetLayout(temp);
		//
		// 	// UE_LOG(LogTemp, Log, TEXT("[UOnlineResourcesComponent::LoadImageToEntity] [w:%d] [h:%d]"), 
		// 	// 	imageX, imageY);
		// }
		//
		// auto brushX = image->Brush.ImageSize.X;
		// auto brushY = image->Brush.ImageSize.Y;
		//
		// image->RenderTransform.Scale.X = brushX;
		// image->RenderTransform.Scale.Y = brushY;
		//
		// image->SetBrushSize(FVector2D(imageX, imageY));

		// 回调entity
		auto gameMode = gameInstance->GetWorld()->GetAuthGameMode();
		auto gameMode_ = Cast<ABaseGameMode>(gameMode);
		if (gameMode && gameMode_)
		{
			gameMode_->controller->OnlineResources_Callback_To_PlaceView(entityInfo->entityId, FVector2D(imageX, imageY), image, imageInfo->texture2D);
		}
	}
}

/*------------------------------------------------------------------*/

// 外部调用 UI显示照片
void UOnlineResourcesComponent::GameShowImage(UObject* component, FString url, int32 entityId, CallBackEventType type)
{
	CvEntityInfo entityInfo;
	entityInfo.url = url;
	entityInfo.entityId = entityId;
	entityInfo.type = type;
	componentUrlMap.Add(component, entityInfo);

	FString loadStr = TEXT("");

	// 已经加载/加载中
	if (urlDataMap.Find(url))
	{
		auto info = &urlDataMap[url];
		info->componentList.Add(component);

		// 直接加载
		if (info->texture2D.IsValid())
		{
			loadStr = TEXT("直接加载");
			// bool isValid;
			// int w, h;
			// info->texture2D = loadImageObject->LoadTexture2D(info->path, isValid, w, h);
			LoadSingleImage(component, info);
		}
		// 加入等待加载队列
		else
		{
			loadStr = TEXT("加入等待加载队列");
		}
	}
	// 尚未加载
	else
	{
		FString name = url;
		name = name.Replace(TEXT("https:"), TEXT("_"), ESearchCase::CaseSensitive);
		name = name.Replace(TEXT("http:"), TEXT("_"), ESearchCase::CaseSensitive);
		name = name.Replace(TEXT("/"), TEXT("_"), ESearchCase::CaseSensitive);

		// url是否已经存在,存在则remove

		FString filePath = TEXT("E://Yxjs//Client//Yxjs//Binaries//Win64//SaveData");
		CvFLocalImageInfo localImageInfo;
		localImageInfo.url = url;
		localImageInfo.componentList.Add(component);
		// 固定路径+url
		localImageInfo.path = FString::Printf(TEXT("%s//%s"), *filePath, *name);

		urlDataMap.Add(url, localImageInfo);

		// 已经下载并且下载数据解析正常
		if (IsLocalImageExist(localImageInfo.path))
		{
			loadStr = TEXT("直接本地加载");
			bool isValid;
			int w, h;
			auto imageInfo = GetImageInfo(url);
			imageInfo->texture2D = loadImageObject->LoadTexture2D(imageInfo->path, isValid, w, h);
			LoadSingleImage(component, imageInfo);
		}
		// 尚未下载
		else
		{
			// 数据推送到子线程
			loadStr = TEXT("网络下载");
			ChildDownloadImageAndPreview(url);
		}
	}

	auto imageInfo = GetImageInfo(url);
	UE_LOG(LogTemp, Log, TEXT("[UOnlineResourcesComponent::UIShowImage] [%s] [count:%d] 照片:%s "), *loadStr,
		imageInfo->componentList.Num(), *url);
}

// 外部调用 UI停止使用照片
void UOnlineResourcesComponent::GameCloseImage(UObject* component)
{
	// auto imageInfo = GetImageInfo(component);
	// if (imageInfo == nullptr)
	// {
	// 	return;
	// }
	//
	// if (imageInfo->componentList.Num() == 1)
	// {
	// 	urlDataMap.Remove(imageInfo->url);
	// }
	// else
	// {
	// 	imageInfo->componentList.Remove(component);
	// 	componentUrlMap.Remove(component);
	// }
}

/*------------------------------------------------------------------*/
