// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CalculationObject.generated.h"

/**
 *
 */
UCLASS()
class  UCalculationObject : public UObject
{
	GENERATED_BODY()

public:
	UCalculationObject();

	/// <summary>
	///  相对坐标转世界坐标
	/// </summary>
	static FTransform RelativeToWorldTransform(FTransform originWTransform, FTransform rTransform);

	/// <summary>
	///  相对坐标转世界坐标
	/// </summary>
	static FVector RelativeToWorldTransform(FTransform originWTransform, FVector rLocation);

	//
	static FRotator RelativeToWorldTransform(FTransform originWTransform, FRotator rRotator);

	/// <summary>
	///  世界平移转为相对位置
	/// </summary>
	static FVector WorldToRelativeTransform(FTransform originWTransform, FVector worldLocation);

	//
	static FRotator WorldToRelativeTransform(FTransform originWTransform, FRotator worldRotator);

	/// <summary>
	///  向量夹角
	/// </summary>
	static float GetVectorAngle(FVector a, FVector b);

	/// <summary>
	///  四舍五入
	/// </summary>
	static int32 RoundFloat(float number);

	/// <summary>
	///  获取顶点位置
	/// </summary>
	static FVector GetVertexLocation(FVector location, FRotator rotator, FVector extent, int32 vrtexNum);

	/// <summary>
	///  获取顶点位置,可以调整距离
	/// </summary>
	static FVector GetVertexLocationSet(FVector location, FRotator rotator, FVector extent, int32 vrtexNum,
		int32 bottomstanceX, int32 bottomstanceY, int32 bottomstanceZ);

	/// <summary>
	///  四舍五入位置
	/// </summary>
	static FTransform RoundTransform(FTransform oldTransform);
	static float TwoDecimalPlaces(float var);

	/// <summary>
	///  Float转Int64
	/// </summary>
	static int64 FloatToInt64(float val);


	/// <summary>
	///  矩形与矩形是否重叠(通过8个点与最大最小点判断)
	/// </summary>
	static bool WhetherItIsInARectangle2(FVector myLocation, FVector min, FVector max);
	static bool IsRectangularOverlap1(FVector spawnWTransform, FVector myExtent, FVector min, FVector max);


	/// <summary>
	///  矩形与矩形是否重叠(xyz)
	/// </summary>
	static bool IsRectangularOverlap2(FVector stayLocation, FVector stayExten, FVector activityLocation, FVector activityExten);

	/// <summary>
	///  点与矩形是否重叠
	/// </summary>
	static bool IsPointBoxOverlap(FTransform boxTransform, FVector boxExten, FVector pointLocation);

	/// <summary>
	///  物体指定轴旋转
	/// </summary>
	static FTransform SpecifiedAxisRotation(FTransform spawnWTransform, FTransform meshRTransform, FVector customRotationPoint, float angle);

	// 旋转 旋转值指定世界旋转轴旋转
	static FRotator RotatorInWorldSpin(int type, FRotator fixedRotator, float angle);

	//
	static FTransform GetTransformWithLine(bool isX, FVector location, FVector line1, FVector line2);
	static bool StrIsInt(std::string strValue);
	static bool StrIsFloat(std::string strValue);
	static bool StrIsNumber(std::string strValue);
};
