// Fill out your copyright notice in the Description page of Project Settings.


#include "ZwJsonObject.h"


#include "Json.h"
#include "Json/Public/Serialization/JsonSerializer.h"

#include <string>
//using namespace std;


UZwJsonObject::UZwJsonObject()
{

}

bool UZwJsonObject::GetJsonArray(FString& josnStr, class TArray<class TSharedPtr<class FJsonValue>>& jsonArray)
{
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(josnStr);
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, jsonArray);
	return BFlag;
}

void UZwJsonObject::GetJsonArrayString(class TArray<class TSharedPtr<class FJsonValue>> jsonArray, int32 index, FString key, FString& value)
{
	value = jsonArray[index]->AsObject()->GetStringField(key);
}

void UZwJsonObject::GetJsonArrayInt(class TArray<class TSharedPtr<class FJsonValue>> jsonArray, int32 index, FString key, int32& value)
{
	value = jsonArray[index]->AsObject()->GetIntegerField(key);
}



//************************************************************************
// 功能:	解码
// 返回:	FString
//************************************************************************
void UZwJsonObject::DecodeJson(FString& jsonStr)
{

}

//************************************************************************
// 功能:	编码
// 返回:	FString
//************************************************************************
bool UZwJsonObject::EncodeJson(const TSharedPtr<class FJsonObject>& jsonObject, FString& jsonStr)
{
	if (jsonObject.IsValid() && jsonObject->Values.Num() > 0)
	{
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&jsonStr);
		FJsonSerializer::Serialize(jsonObject.ToSharedRef(), t_writer);
		return true;
	}
	return false;
}



void UZwJsonObject::TestFunc()
{


	// 编码
	{
		TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);
		jsonObject->SetNumberField(TEXT("aaa"), 12345);
		jsonObject->SetStringField(TEXT("bbb"), TEXT("asdfasdf"));
		FString jsonStr;
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&jsonStr);
		bool result = FJsonSerializer::Serialize(jsonObject.ToSharedRef(), t_writer);
		if (result)
		{
			UE_LOG(LogTemp, Log, TEXT("[TestFunc] [jsonStr:%s]"), *jsonStr);
		}
	}

	// 解码
	{
		std::string strData = "{ \"aaa\":\"asdfasdf\", \"bbb\":123 }";
		FString jsonStr = UTF8_TO_TCHAR(strData.c_str());
		TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonStr);
		TSharedPtr<class FJsonObject> JsonParsed;
		bool flag = FJsonSerializer::Deserialize(JsonReader, JsonParsed);
		if (flag)
		{
			FString aaa = JsonParsed->GetStringField(TEXT("aaa"));
			int32 bbb = JsonParsed->GetIntegerField(TEXT("bbb"));
			UE_LOG(LogTemp, Log, TEXT("[TestFunc]  [aaa: %s] [bbb:%d]"), *aaa, bbb);
		}
	}

	// 编码json数组
	{
		TArray<TSharedPtr<FJsonValue>> jsonArray;
		for (int i = 0; i < 5; i++)
		{
			TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);
			jsonObject->SetNumberField(TEXT("aaa"), i);
			jsonObject->SetStringField(TEXT("bbb"), TEXT("asdfasdf"));

			TSharedPtr<FJsonValueObject>t_objValue = MakeShareable(new FJsonValueObject(jsonObject));
			jsonArray.Add(t_objValue);
		}

		FString jsonStr;
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&jsonStr);
		bool result = FJsonSerializer::Serialize(jsonArray, t_writer);
		if (result)
		{
			UE_LOG(LogTemp, Log, TEXT("[TestFunc] [jsonStr:%s]"), *jsonStr);
		}
	}


	// 解析json数组
	{
		FString jsonArrayStr = TEXT("[{\"ServerName\":\"asdflkj111\",\"ServerIP\":\"127.0.0.1\"},{\"ServerName\":\"asdfas222\",\"ServerIP\":\"127.0.0.2\"}]");
		TArray< TSharedPtr< FJsonValue>> jsonArray;
		bool jsonFlag = GetJsonArray(jsonArrayStr, jsonArray);
		if (jsonFlag)
		{
			if (jsonArray.Num() > 0)
			{
				FString serverIP1, serverIP2;
				GetJsonArrayString(jsonArray, 0, TEXT("ServerIP"), serverIP1);
				GetJsonArrayString(jsonArray, 1, TEXT("ServerIP"), serverIP2);
				UE_LOG(LogTemp, Log, TEXT("[TestFunc]  [ServerIP: %s] [ServerIP: %s]"), *serverIP1, *serverIP2);
			}
		}
	}
}




//TSharedPtr<FJsonObject>  UZwJsonObject::NewJsonObject()
//{
//    TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);
//    return jsonObject;
//}
//void UZwJsonObject::DeleteJsonObject(TSharedPtr<FJsonObject> jsonObject)
//{
//	jsonObject
//}