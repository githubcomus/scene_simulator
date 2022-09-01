// Fill out your copyright notice in the Description page of Project Settings.

#include "ZwXlsDataActorComponent.h"
#include "Core/BaseGameInstance.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"

#include <windows.h>
#include <wchar.h>

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "Yxjs/Tools/FileTools.h"

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values for this component's properties
UZwXlsDataActorComponent::UZwXlsDataActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UZwXlsDataActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

//
void UZwXlsDataActorComponent::Start(UBaseGameInstance* gameInstance_)
{
	gameInstance = gameInstance_;

	Test();
}

//
void UZwXlsDataActorComponent::End()
{
}

// Called every frame
void UZwXlsDataActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/*----------------------------------------------------*/

//
std::string UZwXlsDataActorComponent::GBKToUTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

//
std::string UZwXlsDataActorComponent::UTF8TOGBK(std::string strValue)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	std::string resultStr(str);
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return resultStr;
}

/*----------------------------------------------------*/

//
void UZwXlsDataActorComponent::Test()
{
	// 数据的资源路径
	FString xlsDataPath;

	bool isEdit = false;
#if WITH_EDITORONLY_DATA
	isEdit = true;
#endif

	//if (isEdit == false)
	//{
 //       //xlsDataPath = FPaths::GameDevelopersDir();

	//	//projectDir += "Content/TextFile.txt";
 //       xlsDataPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	//	xlsDataPath += TEXT("Resources/XlsData/XlsData.bin");

	//}
	//else {
	//}

	xlsDataPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	xlsDataPath += TEXT("OtherData/XlsData/XlsData.bin");
	FPaths::CollapseRelativeDirectories(xlsDataPath);
	std::string xlsDataPath2 = (TCHAR_TO_UTF8(*xlsDataPath));

	// 加载资源中的数据
	TArray<uint8> data;
	if (FFileHelper::LoadFileToArray(data, *xlsDataPath) == false)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [表格数据] ERR"), this);
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[%x] [表格数据] [Test] _ [size:%d]"), this, data.Num());

	// 中转临时文件
	//FString profilingDir = FPaths::LaunchDir();
	//profilingDir = profilingDir.Replace(TEXT("/"), TEXT(""));
	//profilingDir += TEXT("\\tempFile.bin");

  //  // 转存到中间文件
  //  {
  //      bool writeFlag = false;
		//char* fileBuf = (char*)malloc(1024 * 1024 * 5);
		//int fileSize = 0;

  //      // 文件不存在
  //      if (FileTools::MyReadFile((char*)xlsDataPath2.c_str(), fileBuf, fileSize)==false) {
  //          writeFlag = true;
  //      }
  //      // 文件大小不对
  //      if (fileSize!= data.Num()) {
  //          writeFlag = true;
  //      }

  //      // MD5不对
  //      {
  //
  //      }

  //      FString str2 = TEXT("无");
  //      bool flag2 = false;
  //      if (writeFlag) {
  //          //char tempbuf[] = {1,1,1,1,1,11,1};
		//	//flag2 = FileTools::MyWriteFile(tempbuf, sizeof(tempbuf), (char*)xlsDataPath2.c_str());

		//	flag2 = FileTools::MyWriteFile((char*)data.GetData(), (int)data.Num(), (char*)xlsDataPath2.c_str());
		//	str2 = flag2 ? TEXT("成功") : TEXT("失败");
  //      }
		//FString str1 = writeFlag ? TEXT("替换") : TEXT("不变");
		//UE_LOG(LogTemp, Log, TEXT("[%x] [表格数据] [Test] 文件判定 path:%s] [文件:%s] [写入:%s] "), this, *profilingDir,*str1,*str2);

		//free(fileBuf);
  //  }

	std::ifstream stream(xlsDataPath2.c_str(), std::ios::binary);
	tabtoy::DataReader reader(stream);
	if (!reader.ReadHeader())
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [表格文件] [Test] 读取文件错误 "), this);
	}
	else
	{
		xlsdata::XlsData::Deserialize(xlsData, reader);
		SetData();

		//// 访问中文测试
		//for (int i = 0; i < xlsData.WeaponBulletTable_.size(); i++)
		//{
		//	int id = (int)xlsData.WeaponBulletTable_[i].ID_;

		//	std::string name = xlsData.WeaponBulletTable_[i].Name_;
		//	auto rangedWeaponBulletType_ = xlsData.WeaponBulletTable_[i].RangedWeaponBulletType_;
		//	auto meshPath = xlsData.WeaponBulletTable_[i].MeshPath_;

		//	FString name2 = UTF8_TO_TCHAR(name.c_str());
		//	UE_LOG(LogTemp, Log, TEXT("[%x] [UZwXlsDataActorComponent::Test]  [id:%d] [name:%s]"),
		//		this, id, *name2);
		//}
	}
}

// 遍历数据并且存入map
void UZwXlsDataActorComponent::SetData()
{
	// 菜单栏
	{
		auto& table = xlsData.MenuBarTable_;
		int count = table.size();
		for (int i = 0; i < count; i++)
		{
			auto& node = table[i];
			menuBarMap.emplace(node.ID_, node);
		}
	}

	// 背包
	{
		auto& table = xlsData.BackpackTable_;
		int count = table.size();
		for (int i = 0; i < count; i++)
		{
			auto& node = table[i];
			backpacktMap.emplace(node.ID_, node);
		}
	}

	// 摆放系列 网格,粒子效果,激光,烟雾
	{
		auto& table = xlsData.PlaceTable_;
		int count = table.size();
		for (int i = 0; i < count; i++)
		{
			auto& node = table[i];
			placeMap.emplace(node.ID_, node);
		}
	}

	// 材质
	{
		auto& table = xlsData.MaterialTable_;
		int count = table.size();
		for (int i = 0; i < count; i++)
		{
			auto& node = table[i];
			materialMap.emplace(node.ID_, node);
		}
	}

	// 图片

	// 视频\

	// 其他

	// 音效
	{
		auto& table = xlsData.SoundEffectTable_;
		int count = table.size();
		for (int i = 0; i < count; i++)
		{
			auto& node = table[i];
			soundEffectMap.emplace(node.ID_, node);
		}
	}
}

/*----------------------------------------------------*/
