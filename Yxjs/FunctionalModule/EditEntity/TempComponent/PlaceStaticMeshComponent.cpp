//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "PlaceStaticMeshComponent.h"
//#include "string"
//
//// 增加实例
//int32 UPlaceStaticMeshComponent::AddInstanceWorld(const FTransform& WorldTransform)
//{
//	FTransform RelativeTM = WorldTransform.GetRelativeTransform(GetComponentTransform());
//	int index = AddInstance(RelativeTM);
//
//	int thiOnlyId = onlyId;
//	onlyId++;
//	onlyIdIndexMap.Add(thiOnlyId, index);
//	onlyIdList.Add(thiOnlyId);
//	return thiOnlyId;
//}
//
//// 现实log
//void UPlaceStaticMeshComponent::ShowLog() {
//
//	std::string str;
//	for (int i = 0; i < GetInstanceCount(); i++) {
//		str = str + " [(" + std::to_string(i) + ")";
//		FTransform transform;
//		if (GetInstanceTransform(i, transform, true)) {
//			auto location = transform.GetLocation();
//			str = str + " " + std::to_string(int(location.X)) + "_" + std::to_string(int(location.Y)) + "_" + std::to_string(int(location.Z));
//		}
//		str = str + "]";
//	}
//	FString str2 = UTF8_TO_TCHAR(str.c_str());
//	UE_LOG(LogTemp, Log, TEXT("[%x] [ShowLog]  %s"), this,
//		*str2);
//}
//
//// 移除实例
//bool UPlaceStaticMeshComponent::RemoveInstanceWorld(int32 onlyId_)
//{
//	bool flag = false;
//	if (onlyIdIndexMap.Find(onlyId_)) {
//
//		//ShowLog();
//		int index = onlyIdIndexMap[onlyId_];
//
//		//bool removeLast = false;
//		//// 删除最后一个
//		//if (GetInstanceCount()-1 == index) {
//		//	removeLast=true;
//		//}
//		flag = RemoveInstance(index);
//
//
//		int thisNum = onlyIdList.IndexOfByKey(onlyId_);
//		int lastNum = onlyIdList.Num() - 1;
//		onlyIdList[thisNum] = onlyIdList[lastNum];
//
//		int lastOnlyId = onlyIdList[thisNum];
//		onlyIdIndexMap[lastOnlyId] = thisNum;
//
//		onlyIdIndexMap.Remove(onlyId_);
//		onlyIdList.RemoveAt(lastNum);
//
//		// 理论上也不应该出现
//		if (flag==false) {
//
//
//		}
//
//		//ShowLog();
//	}
//	// 重复删除的问题
//	else {
//	
//	}
//
//	return flag;
//}
//
//// 更新实例
//void UPlaceStaticMeshComponent::UpdateInstanceTransformWorld(int32 onlyId_, const FTransform& WorldTransform) {
//
//	if (onlyIdIndexMap.Find(onlyId_)) {
//		int meshInstanceIndex = onlyIdIndexMap[onlyId_];
//		UpdateInstanceTransform(meshInstanceIndex, WorldTransform, true,true, true);
//	}
//}