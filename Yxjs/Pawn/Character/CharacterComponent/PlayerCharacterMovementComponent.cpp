// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterMovementComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Character.h>
#include "Yxjs/Pawn/Character/BaseCharacter.h"
#include "Yxjs/GameMode/BaseGameMode.h"
#include "Yxjs/GameMode/Component/BaseGameModeController.h"
#include "Yxjs/FunctionalModule/ActorTrajectory/ActorTrajectoryView.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"

UPlayerCharacterMovementComponent::UPlayerCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

/*

//
void UPlayerCharacterMovementComponent::SmoothCorrection(const FVector& OldLocation, const FQuat& OldRotation, const FVector& NewLocation, const FQuat& NewRotation)
{
	//SCOPE_CYCLE_COUNTER(STAT_CharacterMovementSmoothCorrection);
	if (!HasValidData())
	{
		return;
	}

	int i = 0;

	// We shouldn't be running this on a server that is not a listen server.
	checkSlow(GetNetMode() != NM_DedicatedServer);
	checkSlow(GetNetMode() != NM_Standalone);

	// Only client proxies or remote clients on a listen server should run this code.
	const bool bIsSimulatedProxy = (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy);
	const bool bIsRemoteAutoProxy = (CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy);
	//ensure(bIsSimulatedProxy || bIsRemoteAutoProxy);

	// Getting a correction means new data, so smoothing needs to run.
	bNetworkSmoothingComplete = false;

	// Handle selected smoothing mode.
	if (NetworkSmoothingMode == ENetworkSmoothingMode::Replay)
	{
		// Replays use pure interpolation in this mode, all of the work is done in SmoothClientPosition_Interpolate
		return;
	}
	else if (NetworkSmoothingMode == ENetworkSmoothingMode::Disabled)
	{
		UpdatedComponent->SetWorldLocationAndRotation(NewLocation, NewRotation, false, nullptr, ETeleportType::TeleportPhysics);
		bNetworkSmoothingComplete = true;
	}
	else if (FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character())
	{
		const UWorld* MyWorld = GetWorld();
		if (!ensure(MyWorld != nullptr))
		{
			return;
		}

		// The mesh doesn't move, but the capsule does so we have a new offset.
		FVector NewToOldVector = (OldLocation - NewLocation);
		if (bIsNavWalkingOnServer && FMath::Abs(NewToOldVector.Z) < NavWalkingFloorDistTolerance)
		{
			// ignore smoothing on Z axis
			// don't modify new location (local simulation result), since it's probably more accurate than server data
			// and shouldn't matter as long as difference is relatively small
			NewToOldVector.Z = 0;
		}

		const float DistSq = NewToOldVector.SizeSquared();
		if (DistSq > FMath::Square(ClientData->MaxSmoothNetUpdateDist))
		{
			ClientData->MeshTranslationOffset = (DistSq > FMath::Square(ClientData->NoSmoothNetUpdateDist))
				? FVector::ZeroVector
				: ClientData->MeshTranslationOffset + ClientData->MaxSmoothNetUpdateDist * NewToOldVector.GetSafeNormal();
		}
		else
		{
			ClientData->MeshTranslationOffset = ClientData->MeshTranslationOffset + NewToOldVector;
		}

		//UE_LOG(LogCharacterNetSmoothing, Verbose, TEXT("Proxy %s SmoothCorrection(%.2f)"), *GetNameSafe(CharacterOwner), FMath::Sqrt(DistSq));
		if (NetworkSmoothingMode == ENetworkSmoothingMode::Linear)
		{
			ClientData->OriginalMeshTranslationOffset = ClientData->MeshTranslationOffset;

			// Remember the current and target rotation, we're going to lerp between them
			ClientData->OriginalMeshRotationOffset = OldRotation;
			ClientData->MeshRotationOffset = OldRotation;
			ClientData->MeshRotationTarget = NewRotation;

			// Move the capsule, but not the mesh.
			// Note: we don't change rotation, we lerp towards it in SmoothClientPosition.
			if (NewLocation != OldLocation)
			{
				const FScopedPreventAttachedComponentMove PreventMeshMove(CharacterOwner->GetMesh());
				UpdatedComponent->SetWorldLocation(NewLocation, false, nullptr, GetTeleportType());
			}
		}
		else
		{
			// Calc rotation needed to keep current world rotation after UpdatedComponent moves.
			// Take difference between where we were rotated before, and where we're going
			ClientData->MeshRotationOffset = (NewRotation.Inverse() * OldRotation) * ClientData->MeshRotationOffset;
			ClientData->MeshRotationTarget = FQuat::Identity;

			const FScopedPreventAttachedComponentMove PreventMeshMove(CharacterOwner->GetMesh());
			UpdatedComponent->SetWorldLocationAndRotation(NewLocation, NewRotation, false, nullptr, GetTeleportType());
		}

		//////////////////////////////////////////////////////////////////////////
		// Update smoothing timestamps

		// If running ahead, pull back slightly. This will cause the next delta to seem slightly longer, and cause us to lerp to it slightly slower.
		if (ClientData->SmoothingClientTimeStamp > ClientData->SmoothingServerTimeStamp)
		{
			const double OldClientTimeStamp = ClientData->SmoothingClientTimeStamp;
			ClientData->SmoothingClientTimeStamp = FMath::LerpStable(ClientData->SmoothingServerTimeStamp, OldClientTimeStamp, 0.5);

			//UE_LOG(LogCharacterNetSmoothing, VeryVerbose, TEXT("SmoothCorrection: Pull back client from ClientTimeStamp: %.6f to %.6f, ServerTimeStamp: %.6f for %s"),
			//	OldClientTimeStamp, ClientData->SmoothingClientTimeStamp, ClientData->SmoothingServerTimeStamp, *GetNameSafe(CharacterOwner));
		}

		// Using server timestamp lets us know how much time actually elapsed, regardless of packet lag variance.
		double OldServerTimeStamp = ClientData->SmoothingServerTimeStamp;
		//if (bIsSimulatedProxy)
		//{
		//	// This value is normally only updated on the server, however some code paths might try to read it instead of the replicated value so copy it for proxies as well.
		//	ServerLastTransformUpdateTimeStamp = CharacterOwner->GetReplicatedServerLastTransformUpdateTimeStamp();
		//}
		ClientData->SmoothingServerTimeStamp = ServerLastTransformUpdateTimeStamp;

		// Initial update has no delta.
		if (ClientData->LastCorrectionTime == 0)
		{
			ClientData->SmoothingClientTimeStamp = ClientData->SmoothingServerTimeStamp;
			OldServerTimeStamp = ClientData->SmoothingServerTimeStamp;
		}

		// Don't let the client fall too far behind or run ahead of new server time.
		const double ServerDeltaTime = ClientData->SmoothingServerTimeStamp - OldServerTimeStamp;
		const double MaxOffset = ClientData->MaxClientSmoothingDeltaTime;
		const double MinOffset = FMath::Min(double(ClientData->SmoothNetUpdateTime), MaxOffset);

		// MaxDelta is the farthest behind we're allowed to be after receiving a new server time.
		const double MaxDelta = FMath::Clamp(ServerDeltaTime * 1.25, MinOffset, MaxOffset);
		ClientData->SmoothingClientTimeStamp = FMath::Clamp(ClientData->SmoothingClientTimeStamp, ClientData->SmoothingServerTimeStamp - MaxDelta, ClientData->SmoothingServerTimeStamp);

		// Compute actual delta between new server timestamp and client simulation.
		ClientData->LastCorrectionDelta = ClientData->SmoothingServerTimeStamp - ClientData->SmoothingClientTimeStamp;
		ClientData->LastCorrectionTime = MyWorld->GetTimeSeconds();

		//UE_LOG(LogCharacterNetSmoothing, VeryVerbose, TEXT("SmoothCorrection: WorldTime: %.6f, ServerTimeStamp: %.6f, ClientTimeStamp: %.6f, Delta: %.6f for %s"),
		//	MyWorld->GetTimeSeconds(), ClientData->SmoothingServerTimeStamp, ClientData->SmoothingClientTimeStamp, ClientData->LastCorrectionDelta, *GetNameSafe(CharacterOwner));

//#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
//		if (CharacterMovementCVars::NetVisualizeSimulatedCorrections >= 2)
//		{
//			const float Radius = 4.0f;
//			const bool	bPersist = false;
//			const float Lifetime = 10.0f;
//			const int32	Sides = 8;
//			const float ArrowSize = 4.0f;
//
//			const FVector SimulatedLocation = OldLocation;
//			const FVector ServerLocation = NewLocation + FVector(0, 0, 0.5f);
//
//			const FVector SmoothLocation = CharacterOwner->GetMesh()->GetComponentLocation() - CharacterOwner->GetBaseTranslationOffset() + FVector(0, 0, 1.0f);
//
//			//DrawDebugCoordinateSystem( GetWorld(), ServerLocation + FVector( 0, 0, 300.0f ), UpdatedComponent->GetComponentRotation(), 45.0f, bPersist, Lifetime );
//
//			// Draw simulated location
//			DrawCircle(GetWorld(), SimulatedLocation, FVector(1, 0, 0), FVector(0, 1, 0), FColor(255, 0, 0, 255), Radius, Sides, bPersist, Lifetime);
//
//			// Draw server (corrected location)
//			DrawCircle(GetWorld(), ServerLocation, FVector(1, 0, 0), FVector(0, 1, 0), FColor(0, 255, 0, 255), Radius, Sides, bPersist, Lifetime);
//
//			// Draw smooth simulated location
//			FRotationMatrix SmoothMatrix(CharacterOwner->GetMesh()->GetComponentRotation());
//			DrawDebugDirectionalArrow(GetWorld(), SmoothLocation, SmoothLocation + SmoothMatrix.GetScaledAxis(EAxis::Y) * 5, ArrowSize, FColor(255, 255, 0, 255), bPersist, Lifetime);
//			DrawCircle(GetWorld(), SmoothLocation, FVector(1, 0, 0), FVector(0, 1, 0), FColor(0, 0, 255, 255), Radius, Sides, bPersist, Lifetime);
//
//			if (ClientData->LastServerLocation != FVector::ZeroVector)
//			{
//				// Arrow showing simulated line
//				DrawDebugDirectionalArrow(GetWorld(), ClientData->LastServerLocation, SimulatedLocation, ArrowSize, FColor(255, 0, 0, 255), bPersist, Lifetime);
//
//				// Arrow showing server line
//				DrawDebugDirectionalArrow(GetWorld(), ClientData->LastServerLocation, ServerLocation, ArrowSize, FColor(0, 255, 0, 255), bPersist, Lifetime);
//
//				// Arrow showing smooth location plot
//				DrawDebugDirectionalArrow(GetWorld(), ClientData->LastSmoothLocation, SmoothLocation, ArrowSize, FColor(0, 0, 255, 255), bPersist, Lifetime);
//
//				// Line showing correction
//				DrawDebugDirectionalArrow(GetWorld(), SimulatedLocation, ServerLocation, ArrowSize, FColor(128, 0, 0, 255), bPersist, Lifetime);
//
//				// Line showing smooth vector
//				DrawDebugDirectionalArrow(GetWorld(), ServerLocation, SmoothLocation, ArrowSize, FColor(0, 0, 128, 255), bPersist, Lifetime);
//			}
//
//			ClientData->LastServerLocation = ServerLocation;
//			ClientData->LastSmoothLocation = SmoothLocation;
//		}
//#endif
	}
}

//
void UPlayerCharacterMovementComponent::SmoothClientPosition(float DeltaSeconds)
{
	//if (!HasValidData() || NetworkSmoothingMode == ENetworkSmoothingMode::Disabled)
	//{
	//	return;
	//}

	//// We shouldn't be running this on a server that is not a listen server.
	//checkSlow(GetNetMode() != NM_DedicatedServer);
	//checkSlow(GetNetMode() != NM_Standalone);

	//// Only client proxies or remote clients on a listen server should run this code.
	//const bool bIsSimulatedProxy = (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy);
	//const bool bIsRemoteAutoProxy = (CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy);
	////if (!ensure(bIsSimulatedProxy || bIsRemoteAutoProxy))
	////{
	////	return;
	////}

	SmoothClientPosition_Interpolate(DeltaSeconds);
	SmoothClientPosition_UpdateVisuals();
}

*/

// tick
void UPlayerCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (false)
	{
		//if (movePoint.isMove) {
		//float nowTime = GetWorld()->TimeSeconds - movePoint.moveStartTime;

		//	{
		//		auto tempLocation = FMath::VInterpConstantTo(
		//			CharacterOwner->GetActorLocation(),
		//			movePoint.endMoveLocation,
		//			nowTime,
		//			movePoint.speed);
		//		CharacterOwner->SetActorLocation(tempLocation);

		//	}

		//	//{
		//	//	auto tempVar = FMath::FInterpConstantTo(CharacterOwner->GetActorRotation().Yaw, movePoint.yaw, nowTime, 100);
		//	//	auto tempR = CharacterOwner->GetActorRotation();
		//	//	tempR.Yaw = tempVar;
		//	//	CharacterOwner->SetActorRotation(tempR);
		//	//}

		//	//if (CharacterOwner->GetActorLocation().Equals(movePoint.endMoveLocation, 1) ) {
		//	//	movePoint.isMove = false;
		//	//}
		//}
	}

	// 消费队列

	{
		//if (movePoint.goToLocationList.size() > 1) {
		//
		//	if (movePoint.runIndex< movePoint.goToLocationList.size()) {
		//		auto& goToLocation = movePoint.goToLocationList[movePoint.runIndex];
		//

		//		if (goToLocation.moveStartTime == 0) {
		//			goToLocation.moveStartTime = GetWorld()->TimeSeconds;
		//		}

		//		float nowTime = GetWorld()->TimeSeconds - goToLocation.moveStartTime;

		//		auto tempLocation = FMath::VInterpConstantTo(
		//			CharacterOwner->GetActorLocation(),
		//			goToLocation.location,
		//			nowTime,
		//			goToLocation.speed);
		//		CharacterOwner->SetActorLocation(tempLocation);

		//		if (CharacterOwner->GetActorLocation().Equals(goToLocation.location, 1)) {
		//			movePoint.runIndex++;
		//			UE_LOG(LogTemp, Log, TEXT("[%x] [移动] [TickComponent]  移动中 [index:%d] [time:%f] "), this, movePoint.runIndex, GetWorld()->TimeSeconds);

		//			if (goToLocation.moveType==MoveType_end) {
		//					movePoint.runIndex = 0;
		//					movePoint.goToLocationList.clear();

		//					Velocity = FVector::ZeroVector;
		//			}
		//		}
		//	}
		//}
	}
}

// 设置目标位置
void UPlayerCharacterMovementComponent::SetTargetLocation(FVector velocity, FVector endMoveLocation)
{
	auto gameMode = CharacterOwner->GetWorld()->GetAuthGameMode();
	auto gameMode_ = Cast<ABaseGameMode>(gameMode);
	auto character = Cast<ABaseCharacter>(CharacterOwner);

	// 移动落后一个消息包?

	//if(false)
	//{
	//
	//	// 动画速度
	//	Velocity = velocity;

	//	movePoint.moveStartTime = GetWorld()->TimeSeconds;
	//	movePoint.startMoveLocation = CharacterOwner->GetActorLocation();
	//	movePoint.endMoveLocation = endMoveLocation;

	//	auto moveSize = (movePoint.endMoveLocation - movePoint.startMoveLocation).Size();
	//	//auto speedTime = moveSize / velocity.Size();

	//	// 起步
	//	if (movePoint.moveType == MoveType_none && velocity.Size() != 0) {
	//		//movePoint.speed = velocity.Size();
	//		//movePoint.isMove = true;
	//		movePoint.moveType = MoveType_start;
	//	}
	//	// 持续运动(规划目标点,动态控制各个模拟客户端的速度保持移动轨迹稳定)
	//	else if (movePoint.moveType == MoveType_start && velocity.Size() != 0) {
	//		movePoint.moveType = MoveType_run;
	//	}
	//	// 止步
	//	else if ((movePoint.moveType == MoveType_start || movePoint.moveType == MoveType_run) && velocity.Size() == 0) {
	//
	//	}

	//auto tempR = CharacterOwner->GetActorRotation();
	//tempR.Yaw = (endMoveLocation - CharacterOwner->GetActorLocation()).Rotation().Yaw;
	//CharacterOwner->SetActorRotation(tempR);
	////}

	//// 攒积队列(假定该列队一次只服务于一次移动路径)
	//{
	//	CvGoToLocation goToLocation;
	//	goToLocation.location = endMoveLocation;
	//	goToLocation.time = GetWorld()->TimeSeconds;

	//	// 开始
	//	if (movePoint.goToLocationList.size() == 0 && velocity.Size() != 0) {
	//		{
	//			CvGoToLocation goToLocation2;
	//			goToLocation2.location = endMoveLocation;
	//			goToLocation2.time = GetWorld()->TimeSeconds-0.5;
	//			goToLocation2.moveType = MoveType_start;
	//			goToLocation2.speed = 300;
	//			movePoint.goToLocationList.push_back(goToLocation2);
	//
	//		}
	//		goToLocation.moveType = MoveType_run;

	//		auto moveSize = (goToLocation.location - CharacterOwner->GetActorLocation()).Size();
	//		auto time = 0.5;
	//		goToLocation.speed = moveSize / time;
	//		UE_LOG(LogTemp, Log, TEXT("[%x] [移动] [SetTargetLocation]  开始 [index:%d] [time:%f] "), this, 0, GetWorld()->TimeSeconds);

	//	}
	//	// 持续运动(规划目标点,动态控制各个模拟客户端的速度保持移动轨迹稳定)
	//	else if (movePoint.goToLocationList.size() > 0 && velocity.Size() != 0) {
	//		Velocity = velocity;

	//		goToLocation.moveType = MoveType_run;

	//		auto previousMovePoint = movePoint.goToLocationList[movePoint.goToLocationList.size() - 1];

	//		auto time = goToLocation.time - previousMovePoint.time;
	//		auto moveSize = (goToLocation.location - CharacterOwner->GetActorLocation()).Size();
	//		goToLocation.speed = moveSize / time;
	//		UE_LOG(LogTemp, Log, TEXT("[%x] [移动] [SetTargetLocation]  移动中 [index:%d] [time:%f] [距离:%f] [时长:%f] [速度:%f] "),
	//			this, movePoint.goToLocationList.size() + 1, GetWorld()->TimeSeconds, moveSize, time, goToLocation.speed);
	//	}
	//	// 止步
	//	else if (movePoint.goToLocationList.size() > 0 && velocity.Size() == 0) {
	//		goToLocation.moveType = MoveType_end;
	//		auto previousMovePoint = movePoint.goToLocationList[movePoint.goToLocationList.size() - 1];

	//		auto time = goToLocation.time - previousMovePoint.time;
	//		auto moveSize = (goToLocation.location - CharacterOwner->GetActorLocation()).Size();
	//		goToLocation.speed = moveSize / time;

	//		UE_LOG(LogTemp, Log, TEXT("[%x] [移动] [SetTargetLocation]  结束 [index:%d] [time:%f] [距离:%f] [时长:%f] [速度:%f] "),
	//			this, movePoint.goToLocationList.size() + 1, GetWorld()->TimeSeconds, moveSize, time, goToLocation.speed);
	//	}

		//movePoint.goToLocationList.push_back(goToLocation);

		//vector<FVector> locationList;
		//for (auto i = 0; i < movePoint.goToLocationList.size(); i++) {
		//	locationList.push_back(movePoint.goToLocationList[i].location);
		//}

		//gameMode_->actorTrajectoryView->OnUpdatePoint(character->dataComp->playerId, locationList);

		//Cast<ABaseCharacter>(CharacterOwner)->SplineCreateLine(locationList);
	//}

	gameMode_->actorTrajectoryView->OnUpdatePoint(character->myData->playerId, endMoveLocation);
}

