// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonlyObject.h"

#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"

/*----------------------------------------------------*/

// 转换位置
FTransform UCommonlyObject::SetTransform(FVector location, FVector rotator, FVector scale)
{
	FRotator r(rotator.X, rotator.Y, rotator.Z);

	FTransform t;
	t.SetLocation(location);
	t.SetRotation(r.Quaternion());
	t.SetScale3D(scale);

	return t;
}

// 获取 九宫格
FString UCommonlyObject::GetJiugongge(float x, float y, int syncSize)
{
	int x2 = (int)(x / syncSize);
	if (x >= 0)
	{
	}
	else
	{
		x2 -= 1;
	}
	int y2 = (int)(y / syncSize);
	if (y >= 0)
	{
	}
	else
	{
		y2 -= 1;
	}

	FString key = FString::Printf(TEXT("%d_%d"), x2, y2);
	return key;
}

/*----------------------------------------------------*/

// 字符串转float
float UCommonlyObject::StringToFloat(FString var)
{
	if (var.Len() == 0)return 0;

	var = var.Replace(TEXT(","), TEXT(""));

	std::string str2(TCHAR_TO_UTF8(*var));
	const char* str3 = str2.c_str();

	float f = std::stof(str3);
	return f;
}

// ------------------------------------------------------

// pb location转换ue4
FVector UCommonlyObject::PBToUe4Vector(Gamedata::FVector* source)
{
	FVector target(source->x(), source->y(), source->z());
	return target;
}
FVector UCommonlyObject::PBToUe4Vector(const Gamedata::FVector* source)
{
	FVector target(source->x(), source->y(), source->z());
	return target;
}

// pb rotator转换ue4
FRotator UCommonlyObject::PBToUe4Rotator(Gamedata::FRotator* sourceRotator)
{
	FRotator targetFRotator(sourceRotator->pitch(), sourceRotator->yaw(), sourceRotator->roll());
	return targetFRotator;
}
FRotator UCommonlyObject::PBToUe4Rotator(const Gamedata::FRotator* sourceRotator)
{
	FRotator targetFRotator(sourceRotator->pitch(), sourceRotator->yaw(), sourceRotator->roll());
	return targetFRotator;
}

// pb transform转换ue4
FTransform UCommonlyObject::PBToUe4Transform(Gamedata::FTransform* sourceTransform)
{
	FTransform transform;

	auto location = &sourceTransform->location();
	auto rotator = &sourceTransform->rotator();
	auto scale = &sourceTransform->scale();
	FVector location2(location->x(), location->y(), location->z());
	FRotator rotator2(rotator->pitch(), rotator->yaw(), rotator->roll());
	FVector scale2(scale->x(), scale->y(), scale->z());
	transform.SetLocation(location2);
	transform.SetRotation(rotator2.Quaternion());
	transform.SetScale3D(scale2);

	return transform;
}

// pb ue4转location
void UCommonlyObject::Ue4ToPbVector(FVector source, Gamedata::FVector* target)
{
	target->set_x(source.X);
	target->set_y(source.Y);
	target->set_z(source.Z);
}
Gamedata::FVector UCommonlyObject::Ue4ToPbVector(FVector source)
{
	Gamedata::FVector target;
	target.set_x(source.X);
	target.set_y(source.Y);
	target.set_z(source.Z);
	return target;
}

// pb ue4转rotator
void UCommonlyObject::Ue4ToPbRotator(FRotator source, Gamedata::FRotator* target)
{
	target->set_pitch(source.Pitch);
	target->set_yaw(source.Yaw);
	target->set_roll(source.Roll);
}

// pb ue4转rotator
Gamedata::FRotator UCommonlyObject::Ue4ToPbRotator(FRotator source)
{
	Gamedata::FRotator target;
	target.set_pitch(source.Pitch);
	target.set_yaw(source.Yaw);
	target.set_roll(source.Roll);
	return target;
}

// pb ue4转transform
void UCommonlyObject::Ue4ToPbTransform(FTransform source, Gamedata::FTransform* target)
{
	auto location = target->mutable_location();
	auto rotator = target->mutable_rotator();
	auto scale = target->mutable_scale();
	location->set_x(source.GetLocation().X);
	location->set_y(source.GetLocation().Y);
	location->set_z(source.GetLocation().Z);

	rotator->set_pitch(source.GetRotation().Rotator().Pitch);
	rotator->set_yaw(source.GetRotation().Rotator().Yaw);
	rotator->set_roll(source.GetRotation().Rotator().Roll);

	scale->set_x(source.GetScale3D().X);
	scale->set_y(source.GetScale3D().Y);
	scale->set_z(source.GetScale3D().Z);
}

// fstring string
std::string UCommonlyObject::FStringToString(FString& str)
{
	std::string str2(TCHAR_TO_UTF8(*str));
	return str2;
}

// fstring string
FString UCommonlyObject::StringToFString(std::string& str)
{
	FString str2 = UTF8_TO_TCHAR(str.c_str());
	return str2;
}

// ------------------------------------------------------

// 保留显示两位小数
std::string UCommonlyObject::FloatToString(float val)
{
	char* chCode;
	chCode = new char[64];
	sprintf_s(chCode, 64, "%.2lf", val);
	std::string str(chCode);
	delete[]chCode;
	return str;
}

// 保留显示两位小数
std::string UCommonlyObject::IntToString(int val)
{
	char* chCode;
	chCode = new char[64];
	sprintf_s(chCode, 64, "%d", val);
	std::string str(chCode);
	delete[]chCode;
	return str;
}


// 调试 数据对比
void UCommonlyObject::DataComparison2(std::list<int>& beforeList, std::list<int>& nowList, std::list<int>& addList, std::list<int>& removeList, std::list<int>& constantList)
{

	// 移除
	{
		std::list<int > list2;
		std::copy(beforeList.begin(), beforeList.end(), std::back_inserter(removeList));
		for (auto it = nowList.begin(); it != nowList.end(); ++it)list2.push_back(*it);

		for (auto it = list2.begin(); it != list2.end(); ++it)
		{
			removeList.remove(*it);
		}

	}

	// 增加
	{
		std::list<int> list1;
		std::copy(beforeList.begin(), beforeList.end(), std::back_inserter(list1));
		std::copy(nowList.begin(), nowList.end(), std::back_inserter(addList));
		for (auto it = list1.begin(); it != list1.end(); ++it)
		{
			auto node = *it;
			addList.remove(node);
		}
	}

	// 不变
	{
		std::copy(nowList.begin(), nowList.end(), std::back_inserter(constantList));
		for (auto node : removeList)
		{
			constantList.remove(node);
		}
		for (auto node : addList)
		{
			constantList.remove(node);
		}
	}
}
