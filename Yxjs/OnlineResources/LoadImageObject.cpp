// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadImageObject.h"

#include "HAL/FileManagerGeneric.h"
#include "Runtime/ImageWrapper/Public/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"

#include <string>
////using namespace std;

//
ULoadImageObject::ULoadImageObject()
{
}

//
void ULoadImageObject::TestFunc()
{
}

//
static TSharedPtr<IImageWrapper> GetImageWrapperByExtention(const FString InImagePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(
		FName("ImageWrapper"));
	if (InImagePath.EndsWith(".png"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	}
	else if (InImagePath.EndsWith(".jpg") || InImagePath.EndsWith(".jpeg"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	}
	else if (InImagePath.EndsWith(".bmp"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	}
	else if (InImagePath.EndsWith(".ico"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO);
	}
	else if (InImagePath.EndsWith("exr"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
	}
	else if (InImagePath.EndsWith(".icns"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS);
	}
	return nullptr;
}

//
UTexture2D* ULoadImageObject::LoadTexture2D(const FString path, bool& IsValid, int32& OutWidth, int32& OutHeight)
{
	UTexture2D* Texture = nullptr;
	IsValid = false;
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*path))
	{
		return nullptr;
	}
	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *path))
	{
		return nullptr;
	}
	TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(path);

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		int32 SourceBitDepth = ImageWrapper->GetBitDepth();
		TArray64<uint8> RawImageData;
		if (ImageWrapper->GetRaw(ERGBFormat::RGBA, SourceBitDepth, RawImageData))
		{
			Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);
			if (Texture != nullptr)
			{
				IsValid = true;
				OutWidth = ImageWrapper->GetWidth();
				OutHeight = ImageWrapper->GetHeight();
				void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, RawImageData.GetData(), RawImageData.Num());
				Texture->PlatformData->Mips[0].BulkData.Unlock();
				Texture->UpdateResource();
			}
		}
	}
	return Texture;
}
