// Fill out your copyright notice in the Description page of Project Settings.

#include "CalculationObject.h"

#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"

/*------------------------------------------------------------------*/

UCalculationObject::UCalculationObject()
{
}

/*------------------------------------------------------------------*/

// 相对变换转为世界变换
FTransform UCalculationObject::RelativeToWorldTransform(FTransform originWTransform, FTransform rTransform)
{
	// 相对坐标转世界坐标
	FVector worldLocation = UKismetMathLibrary::TransformLocation(originWTransform
		, rTransform.GetLocation());
	// 相对旋转转世界旋转
	FRotator worldRotator = UKismetMathLibrary::TransformRotation(originWTransform,
		rTransform.Rotator());

	FTransform wTransform;
	wTransform.SetLocation(worldLocation);
	wTransform.SetRotation(worldRotator.Quaternion());


	auto scale1 = originWTransform.GetScale3D() * rTransform.GetScale3D();
	wTransform.SetScale3D(scale1);

	return wTransform;
}

// 相对平移转为世界平移
FVector UCalculationObject::RelativeToWorldTransform(FTransform originWTransform, FVector rLocation)
{
	FTransform rTransform1;
	rTransform1.SetLocation(rLocation);
	rTransform1.SetRotation(FRotator::ZeroRotator.Quaternion());
	rTransform1.SetScale3D(FVector(1, 1, 1));
	FTransform wTransform2 = UCalculationObject::RelativeToWorldTransform(originWTransform, rTransform1);
	return wTransform2.GetLocation();
}

// 相对旋转转为世界旋转
FRotator UCalculationObject::RelativeToWorldTransform(FTransform originWTransform, FRotator rRotator)
{
	FRotator worldRotator = UKismetMathLibrary::TransformRotation(originWTransform, rRotator);
	return worldRotator;
}

// 世界位置转为相对平移
FVector UCalculationObject::WorldToRelativeTransform(FTransform originWTransform, FVector worldLocation)
{
	FTransform wTransform1;
	wTransform1.SetLocation(worldLocation);
	wTransform1.SetRotation(FRotator::ZeroRotator.Quaternion());
	wTransform1.SetScale3D(FVector(1, 1, 1));
	FTransform rTransform2 = wTransform1.GetRelativeTransform(originWTransform);
	return rTransform2.GetLocation();
}

// 世界旋转转为相对旋转
FRotator UCalculationObject::WorldToRelativeTransform(FTransform originWTransform, FRotator worldRotator)
{
	FTransform wTransform1;
	wTransform1.SetLocation(FVector(0, 0, 0));
	wTransform1.SetRotation(worldRotator.Quaternion());
	wTransform1.SetScale3D(FVector(1, 1, 1));
	FTransform rTransform2 = wTransform1.GetRelativeTransform(originWTransform);
	return rTransform2.GetRotation().Rotator();
}

/*------------------------------------------------------------------*/

// 向量夹角
float UCalculationObject::GetVectorAngle(FVector a, FVector b)
{
	a = UKismetMathLibrary::Normal(a);
	b = UKismetMathLibrary::Normal(b);
	float c = FVector::DotProduct(a, b);
	return UKismetMathLibrary::DegAcos(c);
}

// 四舍五入
int32 UCalculationObject::RoundFloat(float number)
{
	return (number > 0.0) ? (number + 0.5) : (number - 0.5);
}

// 四舍五入位置
FTransform UCalculationObject::RoundTransform(FTransform oldTransform)
{
	FTransform newTransform;

	// 四舍五入,将float转为int方便传输.
	FVector location = oldTransform.GetLocation();
	FRotator rotator = oldTransform.Rotator();
	FVector newLocation(RoundFloat(location.X),
		RoundFloat(location.Y),
		RoundFloat(location.Z));
	FRotator newRotator(RoundFloat(rotator.Pitch),
		RoundFloat(rotator.Yaw),
		RoundFloat(rotator.Roll));
	newTransform.SetLocation(newLocation);
	newTransform.SetRotation(newRotator.Quaternion());

	UE_LOG(LogTemp, Log, TEXT("[RoundTransform] [newRotator:%s] [transform:%s]"),
		*newRotator.ToString(), *newTransform.ToString());

	return newTransform;
}

// 保留两位小数
float UCalculationObject::TwoDecimalPlaces(float var)
{
	return (var * 10 + 5) / 10.0f;
}

/*------------------------------------------------------------------*/

// 获取盒体顶点位置
FVector UCalculationObject::GetVertexLocation(FVector location, FRotator rotator, FVector extent, int32 vrtexNum)
{
	// 移动单位
	FVector moveX, moveY, moveZ;
	FRotator moveR1, moveR2, moveR3;
	moveX = moveY = moveZ = FVector(0, 0, 0);
	moveR1 = moveR2 = moveR3 = FRotator(0, 0, 0);
	moveR2.Yaw += 90;
	moveR3.Pitch += 90;
	moveX = moveR1.Vector() * extent.X;
	moveY = moveR2.Vector() * extent.Y;
	moveZ = moveR3.Vector() * extent.Z;

	// 顶点相对坐标(基址 0 0 0)
	FVector move(0, 0, 0);
	// (底1) 0 0 0
	if (vrtexNum == 1)
	{
		move -= moveX;
		move -= moveY;
		move -= moveZ;
	}
	// (底2) 1 0 0
	else if (vrtexNum == 2)
	{
		move += moveX;
		move -= moveY;
		move -= moveZ;
	}
	// (底3) 1 1 0
	else if (vrtexNum == 3)
	{
		move += moveX;
		move += moveY;
		move -= moveZ;
	}
	// (底4) 0 1 0
	else if (vrtexNum == 4)
	{
		move -= moveX;
		move += moveY;
		move -= moveZ;
	}
	// (顶1) 0 0 1
	if (vrtexNum == 5)
	{
		move -= moveX;
		move -= moveY;
		move += moveZ;
	}
	// (顶2)1 0 1
	else if (vrtexNum == 6)
	{
		move += moveX;
		move -= moveY;
		move += moveZ;
	}
	// (顶3)1 1 1
	else if (vrtexNum == 7)
	{
		move += moveX;
		move += moveY;
		move += moveZ;
	}
	// (顶4)0 1 1
	else if (vrtexNum == 8)
	{
		move -= moveX;
		move += moveY;
		move += moveZ;
	}

	FTransform rTransform1;
	rTransform1.SetLocation(location);
	rTransform1.SetRotation(rotator.Quaternion());
	rTransform1.SetScale3D(FVector(1, 1, 1));

	FTransform rTransform2;
	rTransform2.SetLocation(move);
	rTransform2.SetRotation(FRotator(0, 0, 0).Quaternion());
	rTransform2.SetScale3D(FVector(1, 1, 1));
	FTransform wTransform3 = UCalculationObject::RelativeToWorldTransform(rTransform1, rTransform2);
	FVector result = wTransform3.GetLocation();

	return result;
}

// 获取顶点位置,设置中心点距离
FVector UCalculationObject::GetVertexLocationSet(FVector location, FRotator rotator, FVector extent, int32 vrtexNum,
	int32 bottomstanceX, int32 bottomstanceY, int32 bottomstanceZ)
{
	// 移动单位
	FVector moveX, moveY, moveZ;
	FRotator moveR1, moveR2, moveR3;
	moveX = moveY = moveZ = FVector(0, 0, 0);
	moveR1 = moveR2 = moveR3 = FRotator(0, 0, 0);
	moveR2.Yaw += 90;
	moveR3.Pitch += 90;
	moveX = moveR1.Vector() * (extent.X + bottomstanceX);
	moveY = moveR2.Vector() * (extent.Y + bottomstanceY);
	moveZ = moveR3.Vector() * (extent.Z + bottomstanceZ);

	// 顶点相对坐标(基址 0 0 0)
	FVector move(0, 0, 0);
	// (底1) 0 0 0
	if (vrtexNum == 1)
	{
		move -= moveX;
		move -= moveY;
		move -= moveZ;
	}
	// (底2) 1 0 0
	else if (vrtexNum == 2)
	{
		move += moveX;
		move -= moveY;
		move -= moveZ;
	}
	// (底3) 1 1 0
	else if (vrtexNum == 3)
	{
		move += moveX;
		move += moveY;
		move -= moveZ;
	}
	// (底4) 0 1 0
	else if (vrtexNum == 4)
	{
		move -= moveX;
		move += moveY;
		move -= moveZ;
	}
	// (顶1) 0 0 1
	if (vrtexNum == 5)
	{
		move -= moveX;
		move -= moveY;
		move += moveZ;
	}
	// (顶2)1 0 1
	else if (vrtexNum == 6)
	{
		move += moveX;
		move -= moveY;
		move += moveZ;
	}
	// (顶3)1 1 1
	else if (vrtexNum == 7)
	{
		move += moveX;
		move += moveY;
		move += moveZ;
	}
	// (顶4)0 1 1
	else if (vrtexNum == 8)
	{
		move -= moveX;
		move += moveY;
		move += moveZ;
	}

	FTransform rTransform1;
	rTransform1.SetLocation(location);
	rTransform1.SetRotation(rotator.Quaternion());
	rTransform1.SetScale3D(FVector(1, 1, 1));

	FTransform rTransform2;
	rTransform2.SetLocation(move);
	rTransform2.SetRotation(FRotator(0, 0, 0).Quaternion());
	rTransform2.SetScale3D(FVector(1, 1, 1));
	FTransform wTransform3 = UCalculationObject::RelativeToWorldTransform(rTransform1, rTransform2);
	FVector result = wTransform3.GetLocation();

	return result;
}

// Float转Int64 float乘以1000000而不损失任何精度,直接float乘会出现偏差.
int64 UCalculationObject::FloatToInt64(float val)
{
	int64 newVal;

	// 整数部分 * 1000000
	int64 val1 = (int64)val * 1000000;

	// 小数部分float
	float val2 = (val - (int64)val);

	// 小数部分*1000000转化为整数
	int64 val3 = val2 * 1000000;

	// 合并
	newVal = val1 + val3;

	//UE_LOG(LogTemp, Log, TEXT("[FloatToInt64]  [%f] ---> [%f] [ [%I64d] + [%I64d] ]  ---> [%I64d]"),
	//       val,  val2, val1, val3, newVal
	//      );

	return newVal;
}

/*------------------------------------------------------------------*/

// 点是否与矩形重叠
bool UCalculationObject::WhetherItIsInARectangle2(FVector myLocation, FVector min, FVector max)
{
	bool flag = false;
	if (myLocation.X <= max.X
		&& myLocation.Y <= max.Y
		&& myLocation.Z <= max.Z
		&& myLocation.X >= min.X
		&& myLocation.Y >= min.Y
		&& myLocation.Z >= min.Z
		)
	{
		//UE_LOG(LogTemp, Log, TEXT("[WhetherItIsInARectangle2] 重叠 [%s] < [%s] < [%s] "),
		//       *min.ToString(),
		//       *myLocation.ToString(),
		//       *max.ToString() );
		flag = true;
	}

	return flag;
}

// 矩形与矩形是否重叠(通过8个点与最大最小点判断)
bool UCalculationObject::IsRectangularOverlap1(FVector spawnWTransform, FVector myExtent, FVector min, FVector max)
{
	// 返回:	flag: true有重叠, false 无重叠
	bool flag = false;
	FVector bottom1 = UCalculationObject::GetVertexLocation(spawnWTransform, FRotator(0, 0, 0), myExtent, 1);
	FVector bottom2 = UCalculationObject::GetVertexLocation(spawnWTransform, FRotator(0, 0, 0), myExtent, 2);
	FVector bottom3 = UCalculationObject::GetVertexLocation(spawnWTransform, FRotator(0, 0, 0), myExtent, 3);
	FVector bottom4 = UCalculationObject::GetVertexLocation(spawnWTransform, FRotator(0, 0, 0), myExtent, 4);
	FVector top1 = UCalculationObject::GetVertexLocation(spawnWTransform, FRotator(0, 0, 0), myExtent, 5);
	FVector top2 = UCalculationObject::GetVertexLocation(spawnWTransform, FRotator(0, 0, 0), myExtent, 6);
	FVector top3 = UCalculationObject::GetVertexLocation(spawnWTransform, FRotator(0, 0, 0), myExtent, 7);
	FVector top4 = UCalculationObject::GetVertexLocation(spawnWTransform, FRotator(0, 0, 0), myExtent, 8);

	bool flag1 = UCalculationObject::WhetherItIsInARectangle2(bottom1, min, max);
	bool flag2 = UCalculationObject::WhetherItIsInARectangle2(bottom2, min, max);
	bool flag3 = UCalculationObject::WhetherItIsInARectangle2(bottom3, min, max);
	bool flag4 = UCalculationObject::WhetherItIsInARectangle2(bottom4, min, max);
	bool flag5 = UCalculationObject::WhetherItIsInARectangle2(top1, min, max);
	bool flag6 = UCalculationObject::WhetherItIsInARectangle2(top2, min, max);
	bool flag7 = UCalculationObject::WhetherItIsInARectangle2(top3, min, max);
	bool flag8 = UCalculationObject::WhetherItIsInARectangle2(top4, min, max);

	// 全部不重叠
	if (flag1 == false
		&& flag2 == false
		&& flag3 == false
		&& flag4 == false
		&& flag5 == false
		&& flag6 == false
		&& flag7 == false
		&& flag8 == false)
	{
	}
	// 有重叠
	else
	{
		flag = true;
	}
	return flag;
}

// 矩形与矩形是否重叠(通过xyz半径判断)
bool UCalculationObject::IsRectangularOverlap2(FVector stayLocation, FVector stayExten, FVector activityLocation,
	FVector activityExten)
{
	// 返回:	flag: true有重叠,false 无重叠
	bool flag = false;
	float xSize = abs(stayLocation.X - activityLocation.X);
	float ySize = abs(stayLocation.Y - activityLocation.Y);
	float zSize = abs(stayLocation.Z - activityLocation.Z);
	// 重叠
	if (xSize <= (stayExten.X + activityExten.X)
		&& ySize <= (stayExten.Y + activityExten.Y)
		&& zSize <= (stayExten.Z + activityExten.Z)
		)
	{
		flag = true;
	}
	// 无重叠
	else
	{
	}

	return flag;
}

// 点与矩形是否重叠
bool UCalculationObject::IsPointBoxOverlap(FTransform boxTransform, FVector boxExten, FVector pointLocation)
{
	// 返回:	flag: true有重叠,false 无重叠
	bool flag = false;

	FVector pointRLocation = UCalculationObject::WorldToRelativeTransform(boxTransform, pointLocation);

	// 有重叠
	if (abs(pointRLocation.X) <= boxExten.X
		&& abs(pointRLocation.Y) <= boxExten.Y
		&& abs(pointRLocation.Z) <= boxExten.Z
		)
	{
		flag = true;
	}
	// 无重叠
	else
	{
	}

	return flag;
}

/*------------------------------------------------------------------*/

// 旋转 物体指定轴旋转
FTransform UCalculationObject::SpecifiedAxisRotation(FTransform spawnWTransform, FTransform meshRTransform,
	FVector customRotationPoint, float angle)
{
	// 返回:	meshWTransform:新的mesh世界位置
	// 参数:	spawnWTransform:原点位置 ,meshRTransform:旋转物体相对平移,customRotationCenterPoint:自定义旋转点,angle:旋转角度
	// 简单计算方法
	// x1 = x0 * cosB - y0 * sinB
	//
	// y1 = y0 * cosB + x0 * sinB
	//
	//

	FRotator meshRotator = meshRTransform.GetRotation().Rotator();
	FVector meshLocation = meshRTransform.GetLocation();

	// 网格平移到旋转点
	{
		// 移动x
		FRotator moveR1 = meshRotator;
		FVector moveX = moveR1.Vector() * (meshRTransform.GetLocation().X - customRotationPoint.X);
		meshLocation -= moveX;
		// 移动y
		FRotator moveR2 = meshRotator;
		moveR2.Yaw -= 90;
		FVector moveY = moveR2.Vector() * (meshRTransform.GetLocation().Y - customRotationPoint.Y);
		meshLocation += moveY;
	}

	// 网格旋转
	{
		meshRotator.Yaw += angle;
	}

	// 网格回退中心点
	{
		// 移动x
		FRotator moveR1 = meshRotator;
		FVector moveX = moveR1.Vector() * (meshRTransform.GetLocation().X - customRotationPoint.X);
		meshLocation += moveX;
		// 移动y
		FRotator moveR2 = meshRotator;
		moveR2.Yaw += 90;
		FVector moveY = moveR2.Vector() * (meshRTransform.GetLocation().Y - customRotationPoint.Y);
		meshLocation += moveY;
	}

	FTransform wTransform2;
	wTransform2.SetLocation(meshLocation);
	wTransform2.SetRotation(meshRotator.Quaternion());
	wTransform2.SetScale3D(meshRTransform.GetScale3D());
	wTransform2 = RelativeToWorldTransform(spawnWTransform, wTransform2);
	return wTransform2;
}

// 旋转 旋转值指定世界旋转轴旋转
FRotator UCalculationObject::RotatorInWorldSpin(int type, FRotator fixedRotator, float angle)
{
	// type 1:yaw  2:pitch  3:rool
	// yaw是以世界坐标为原点

	FRotator wRotator;
	if (type == 1)
	{
		// yaw为世界坐标原点旋转(旋转轴为0,0,1)
		wRotator = fixedRotator;
		wRotator.Yaw += angle;
	}
	else if (type == 2)
	{
		// pitch为朝右的坐标轴为原点(旋转轴为,0,1,0),需要原坐标系旋转90度,使得z轴倒下来到达0,1,0
		FTransform newT;
		newT.SetLocation(FVector(0, 0, 0));
		newT.SetScale3D(FVector(1, 1, 1));
		FRotator rRotator2(0, 0, 90);
		newT.SetRotation(rRotator2.Quaternion());

		auto rRotator = UCalculationObject::WorldToRelativeTransform(newT, fixedRotator);
		rRotator.Yaw += angle;
		wRotator = UCalculationObject::RelativeToWorldTransform(newT, rRotator);
	}
	else if (type == 3)
	{
		// rool需要环绕向前的坐标轴旋转(旋转轴为1,0,0)
		FTransform newT;
		newT.SetLocation(FVector(0, 0, 0));
		newT.SetScale3D(FVector(1, 1, 1));
		newT.SetRotation(FVector::DownVector.Rotation().Quaternion());

		auto rRotator = UCalculationObject::WorldToRelativeTransform(newT, fixedRotator);
		rRotator.Yaw += angle;
		wRotator = UCalculationObject::RelativeToWorldTransform(newT, rRotator);
	}
	return wRotator;
}

/*------------------------------------------------------------------*/

// 通过射线a和射线b 确定变换
FTransform UCalculationObject::GetTransformWithLine(bool isX, FVector location, FVector line1, FVector line2)
{
	// 莫名其妙的函数在处理非世界平移为原点的数据时异常
	// line1 和 line2 需要是90度相交的线段

	FTransform t;

	auto r1 = line1.Rotation();
	t.SetLocation(location);
	t.SetRotation(r1.Quaternion());
	t.SetScale3D(FVector(1, 1, 1));

	// yaw轴线段
	FRotator r3(0, 90, 0);
	auto worldRotator = UCalculationObject::RelativeToWorldTransform(t, r3);
	auto v3 = location + worldRotator.Vector() * 100;

	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), v3, startLocation6, FColor(22, 222, 22), 10.0f, 2.2f);

	// rool相差角度
	auto line3 = v3 - location;
	float angle1 = UCalculationObject::GetVectorAngle(line3, line2);

	UE_LOG(LogTemp, Log, TEXT("[GetTransformWithLine] [角度] [%f] "), angle1);

	FRotator r2(0, 0, 0);
	r2.Yaw = 0;
	r2.Pitch = 0;
	if (isX)
	{
		r2.Roll = angle1;
	}
	else
	{
		r2.Roll = -angle1;
	}
	auto r4 = RelativeToWorldTransform(t, r2);
	t.SetRotation(r4.Quaternion());

	// x+ y- z+-

	return t;
}

/*------------------------------------------------------------------*/

// 字符串是整数
bool UCalculationObject::StrIsInt(std::string strValue)
{
	const char* str = strValue.c_str();
	bool isNum = false;

	int index = 0;
	for (; *str != '\0'; str++, index++)
	{
		switch (*str)
		{
		case '0':case'1':case'2':case'3':case'4':case'5':
		case'6':case'7':case'8':case'9':
			isNum = true;
			break;
		case '-':case '+':
			if (index != 0)
			{
				return false;
			}
			break;
		default:
			return false;
		}
	}

	if (!isNum)
	{
		return false;
	}

	return true;

}

// 字符串是浮点数
bool UCalculationObject::StrIsFloat(std::string strValue)
{
	const char* str = strValue.c_str();
	bool isE = false,
		isPoint = false,
		numBefore = false,
		numBehind = false;

	int index = 0;
	for (; *str != '\0'; str++, index++)
	{
		switch (*str)
		{
		case '0':case'1':case'2':case'3':case'4':case'5':
		case'6':case'7':case'8':case'9':
			if (isE)
			{
				numBehind = true;
			}
			else
			{
				numBefore = true;
			}
			break;
		case '+':case '-':
			if (index != 0)
			{
				return false;
			}
			break;
		case 'e':case 'E':
			if (isE || !numBefore)
			{
				return false;
			}
			else
			{
				isPoint = true;
				index = -1;
				isE = true;
			}
			break;
		case '.':
			if (isPoint)
			{
				return false;
			}
			else
			{
				isPoint = true;
			}
			break;
		default:
			return false;
		}
	}

	if (!numBefore)
	{
		return false;
	}
	else if (isE && !numBehind)
	{
		return false;
	}

	return true;

}

// 判断字符串是否为数字
bool UCalculationObject::StrIsNumber(std::string strValue)
{
	bool flag = false;
	if (StrIsInt(strValue))
	{
		flag = true;
	}
	else if (StrIsFloat(strValue))
	{
		flag = true;
	}
	return flag;
}


/*------------------------------------------------------------------*/