// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OnlineResourcesInfo.h"
#include "Json.h"
//#include "Json/Public/Serialization/JsonSerializer.h"
#include <atomic>
#include <string>
#include <list>

#include "OnlineResourcesComponent.generated.h"

//using namespace std;

class DownloadSaveFiles;
class ULoadImageObject;

class UBaseGameInstance;
class NetConnection;
class UNetDataObject;
class RingBuffer;

// 回调事件类型
UENUM()
enum class CallBackEventType : uint8
{
	Event_UILoad,
	Event_GameLoad
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UOnlineResourcesComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// 线程状态
	enum ThreadType
	{
		ThreadWait,
		ThreadRun,
		ThreadStoping,
		ThreadStop,
	};

	// 事件类型
	enum ResEventType
	{
		Event_ChildDownloadImageAndPreview,
		Event_MasterDownloadImageAndPreview
	};

	// 所有本地图片数据管理
	struct CvFLocalImageInfo
	{
		// url
		FString url;

		//
		FString path;

		// id
		int id = 0;

		// 组件池
		TArray<UObject*> componentList;

		// 对象
		TWeakObjectPtr<UTexture2D> texture2D;
	};

	// 队列数据
	struct CvQueueInfo
	{
		RingBuffer* ringBuffer;

		float lastTime = 0;
	};

	// 线程数据
	struct CvChildThreadInfo
	{
		// 线程状态
		int state;

		// 下载组件指针
		std::shared_ptr<DownloadSaveFiles> downloadSaveFiles;
	};

	// 反查数据
	struct CvEntityInfo
	{
		//
		FString url;

		//
		int32 entityId;

		// 操作类型
		CallBackEventType type;
	};

	/*------------------------------------------------------------------*/

	// owner
	UBaseGameInstance* gameInstance;

	// 主线程事件队列
	CvQueueInfo mainQueueInfo;

	// 子线程事件队列
	CvQueueInfo childQueueInfo;

	// 子线程
	CvChildThreadInfo childThreadInfo;

	// 磁盘加载照片
	ULoadImageObject* loadImageObject;

	// 本地图片数据管理,key是url,value是数据
	TMap<FString, CvFLocalImageInfo> urlDataMap;

	// 本地图片数据管理,key是component指针,value是数据
	TMap<UObject*, CvEntityInfo> componentUrlMap;

	/*------------------------------------------------------------------*/

	// Sets default values for this component's properties
	UOnlineResourcesComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// 主线程队列 处理来自网络的消息
	UFUNCTION(BlueprintCallable)
		void TickFrame(float DeltaTime);
	void MastrThreadReadData(std::string& readData);

	//
	void Start(UBaseGameInstance* gameInstance_);

	//
	void End();

	//
	void ShowLog(std::string str);

	// 获取
	float GetTime();

	// 获取 时间字符串
	std::string GetTimeStr();
	CvFLocalImageInfo* GetImageInfo(FString url);
	CvFLocalImageInfo* GetImageInfo(UObject* component);
	CvEntityInfo* GetEntityInfo(UObject* component);
	bool IsLocalImageExist(FString path);
	void ChildDownloadImageAndPreview(FString url);
	void MasterDownloadImageAndPreview(FString localPath);
	void DownloadImage(FString url);
	void LoadImageOk(FString url);
	void LoadSingleImage(UObject* component, CvFLocalImageInfo* imageInfo);
	void LoadImageToUI(UObject* component, CvFLocalImageInfo* imageInfo, CvEntityInfo* entityInfo);
	void LoadImageToEntity(UObject* component, CvFLocalImageInfo* imageInfo, CvEntityInfo* entityInfo);
	UFUNCTION(BlueprintCallable)
		void GameShowImage(UObject* component, FString url, int32 entityId, CallBackEventType type);
	UFUNCTION(BlueprintCallable)
		void GameCloseImage(UObject* component);
	FString GetJsonStr(TSharedPtr<FJsonObject> jsonObject);
	FString MakeEventJsonStr(int type, int id, FString value);
	void ParsingEventJson(FString& str, int& type, int& id, FString& value);
	void ChildController(bool flag);
	static unsigned ChildThread(void* arg);
	void ChildThreadFunc();
	void ChildThreadReadData(std::string& readData);

	/*------------------------------------------------------------------*/
};
