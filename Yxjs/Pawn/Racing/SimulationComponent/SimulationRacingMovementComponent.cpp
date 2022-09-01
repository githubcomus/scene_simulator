// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationRacingMovementComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Character.h>
#include "Yxjs/Pawn/Character/BaseCharacter.h"
#include "Yxjs/GameMode/BaseGameMode.h"
#include "Yxjs/GameMode/Component/BaseGameModeController.h"
#include "Yxjs/FunctionalModule/ActorTrajectory/ActorTrajectoryView.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"


USimulationRacingMovementComponent::USimulationRacingMovementComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}


//void USimulationRacingMovementComponent::SmoothCorrection(const FVector& OldLocation, const FQuat& OldRotation, const FVector& NewLocation, const FQuat& NewRotation)
//{
//	//SCOPE_CYCLE_COUNTER(STAT_CharacterMovementSmoothCorrection);
//	//if (!HasValidData())
//	//{
//	//	return;
//	//}
//
//	{
//
//		FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character();
//
//	}
//
//
//	// We shouldn't be running this on a server that is not a listen server.
//	checkSlow(GetNetMode() != NM_DedicatedServer);
//	checkSlow(GetNetMode() != NM_Standalone);
//
//	// Only client proxies or remote clients on a listen server should run this code.
//	const bool bIsSimulatedProxy = (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy);
//	const bool bIsRemoteAutoProxy = (CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy);
//	ensure(bIsSimulatedProxy || bIsRemoteAutoProxy);
//
//	// Getting a correction means new data, so smoothing needs to run.
//	bNetworkSmoothingComplete = false;
//
//	// Handle selected smoothing mode.
//	if (NetworkSmoothingMode == ENetworkSmoothingMode::Replay)
//	{
//		// Replays use pure interpolation in this mode, all of the work is done in SmoothClientPosition_Interpolate
//		return;
//	}
//	else if (NetworkSmoothingMode == ENetworkSmoothingMode::Disabled)
//	{
//		UpdatedComponent->SetWorldLocationAndRotation(NewLocation, NewRotation, false, nullptr, ETeleportType::TeleportPhysics);
//		bNetworkSmoothingComplete = true;
//	}
//	else if (FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character())
//	{
//		const UWorld* MyWorld = GetWorld();
//		if (!ensure(MyWorld != nullptr))
//		{
//			return;
//		}
//
//		// The mesh doesn't move, but the capsule does so we have a new offset.
//		FVector NewToOldVector = (OldLocation - NewLocation);
//		if (bIsNavWalkingOnServer && FMath::Abs(NewToOldVector.Z) < NavWalkingFloorDistTolerance)
//		{
//			// ignore smoothing on Z axis
//			// don't modify new location (local simulation result), since it's probably more accurate than server data
//			// and shouldn't matter as long as difference is relatively small
//			NewToOldVector.Z = 0;
//		}
//
//		const float DistSq = NewToOldVector.SizeSquared();
//		if (DistSq > FMath::Square(ClientData->MaxSmoothNetUpdateDist))
//		{
//			ClientData->MeshTranslationOffset = (DistSq > FMath::Square(ClientData->NoSmoothNetUpdateDist))
//				? FVector::ZeroVector
//				: ClientData->MeshTranslationOffset + ClientData->MaxSmoothNetUpdateDist * NewToOldVector.GetSafeNormal();
//		}
//		else
//		{
//			ClientData->MeshTranslationOffset = ClientData->MeshTranslationOffset + NewToOldVector;
//		}
//
//		//UE_LOG(LogCharacterNetSmoothing, Verbose, TEXT("Proxy %s SmoothCorrection(%.2f)"), *GetNameSafe(CharacterOwner), FMath::Sqrt(DistSq));
//		if (NetworkSmoothingMode == ENetworkSmoothingMode::Linear)
//		{
//			ClientData->OriginalMeshTranslationOffset = ClientData->MeshTranslationOffset;
//
//			// Remember the current and target rotation, we're going to lerp between them
//			ClientData->OriginalMeshRotationOffset = OldRotation;
//			ClientData->MeshRotationOffset = OldRotation;
//			ClientData->MeshRotationTarget = NewRotation;
//
//			// Move the capsule, but not the mesh.
//			// Note: we don't change rotation, we lerp towards it in SmoothClientPosition.
//			if (NewLocation != OldLocation)
//			{
//				const FScopedPreventAttachedComponentMove PreventMeshMove(CharacterOwner->GetMesh());
//				UpdatedComponent->SetWorldLocation(NewLocation, false, nullptr, GetTeleportType());
//			}
//		}
//		else
//		{
//			// Calc rotation needed to keep current world rotation after UpdatedComponent moves.
//			// Take difference between where we were rotated before, and where we're going
//			ClientData->MeshRotationOffset = (NewRotation.Inverse() * OldRotation) * ClientData->MeshRotationOffset;
//			ClientData->MeshRotationTarget = FQuat::Identity;
//
//			const FScopedPreventAttachedComponentMove PreventMeshMove(CharacterOwner->GetMesh());
//			UpdatedComponent->SetWorldLocationAndRotation(NewLocation, NewRotation, false, nullptr, GetTeleportType());
//		}
//
//		//////////////////////////////////////////////////////////////////////////
//		// Update smoothing timestamps
//
//		// If running ahead, pull back slightly. This will cause the next delta to seem slightly longer, and cause us to lerp to it slightly slower.
//		if (ClientData->SmoothingClientTimeStamp > ClientData->SmoothingServerTimeStamp)
//		{
//			const double OldClientTimeStamp = ClientData->SmoothingClientTimeStamp;
//			ClientData->SmoothingClientTimeStamp = FMath::LerpStable(ClientData->SmoothingServerTimeStamp, OldClientTimeStamp, 0.5);
//
//			//UE_LOG(LogCharacterNetSmoothing, VeryVerbose, TEXT("SmoothCorrection: Pull back client from ClientTimeStamp: %.6f to %.6f, ServerTimeStamp: %.6f for %s"),
//			//	OldClientTimeStamp, ClientData->SmoothingClientTimeStamp, ClientData->SmoothingServerTimeStamp, *GetNameSafe(CharacterOwner));
//		}
//
//		// Using server timestamp lets us know how much time actually elapsed, regardless of packet lag variance.
//		double OldServerTimeStamp = ClientData->SmoothingServerTimeStamp;
//		if (bIsSimulatedProxy)
//		{
//			// This value is normally only updated on the server, however some code paths might try to read it instead of the replicated value so copy it for proxies as well.
//			ServerLastTransformUpdateTimeStamp = CharacterOwner->GetReplicatedServerLastTransformUpdateTimeStamp();
//		}
//		ClientData->SmoothingServerTimeStamp = ServerLastTransformUpdateTimeStamp;
//
//		// Initial update has no delta.
//		if (ClientData->LastCorrectionTime == 0)
//		{
//			ClientData->SmoothingClientTimeStamp = ClientData->SmoothingServerTimeStamp;
//			OldServerTimeStamp = ClientData->SmoothingServerTimeStamp;
//		}
//
//
//
//
//		// Don't let the client fall too far behind or run ahead of new server time.
//		const double ServerDeltaTime = ClientData->SmoothingServerTimeStamp - OldServerTimeStamp;
//		const double MaxOffset = ClientData->MaxClientSmoothingDeltaTime;
//		const double MinOffset = FMath::Min(double(ClientData->SmoothNetUpdateTime), MaxOffset);
//
//		// MaxDelta is the farthest behind we're allowed to be after receiving a new server time.
//		const double MaxDelta = FMath::Clamp(ServerDeltaTime * 1.25, MinOffset, MaxOffset);
//		ClientData->SmoothingClientTimeStamp = FMath::Clamp(ClientData->SmoothingClientTimeStamp, ClientData->SmoothingServerTimeStamp - MaxDelta, ClientData->SmoothingServerTimeStamp);
//
//		// Compute actual delta between new server timestamp and client simulation.
//		ClientData->LastCorrectionDelta = ClientData->SmoothingServerTimeStamp - ClientData->SmoothingClientTimeStamp;
//		ClientData->LastCorrectionTime = MyWorld->GetTimeSeconds();
//
//		//// todo
//		//ClientData->SmoothingServerTimeStamp = GetWorld()->TimeSeconds ;
//		//ClientData->SmoothingClientTimeStamp = GetWorld()->TimeSeconds-0.5;
//		//ClientData->LastCorrectionDelta = ClientData->SmoothingServerTimeStamp - ClientData->SmoothingClientTimeStamp;
//
//
//
//		UE_LOG(LogTemp, Log, TEXT("SmoothCorrection: WorldTime: %.6f, ServerTimeStamp: %.6f, ClientTimeStamp: %.6f, Delta: %.6f for %s"),
//			MyWorld->GetTimeSeconds(), ClientData->SmoothingServerTimeStamp, ClientData->SmoothingClientTimeStamp, ClientData->LastCorrectionDelta, *GetNameSafe(CharacterOwner));
//
////#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
////		if (CharacterMovementCVars::NetVisualizeSimulatedCorrections >= 2)
////		{
////			const float Radius = 4.0f;
////			const bool	bPersist = false;
////			const float Lifetime = 10.0f;
////			const int32	Sides = 8;
////			const float ArrowSize = 4.0f;
////
////			const FVector SimulatedLocation = OldLocation;
////			const FVector ServerLocation = NewLocation + FVector(0, 0, 0.5f);
////
////			const FVector SmoothLocation = CharacterOwner->GetMesh()->GetComponentLocation() - CharacterOwner->GetBaseTranslationOffset() + FVector(0, 0, 1.0f);
////
////			//DrawDebugCoordinateSystem( GetWorld(), ServerLocation + FVector( 0, 0, 300.0f ), UpdatedComponent->GetComponentRotation(), 45.0f, bPersist, Lifetime );
////
////			// Draw simulated location
////			DrawCircle(GetWorld(), SimulatedLocation, FVector(1, 0, 0), FVector(0, 1, 0), FColor(255, 0, 0, 255), Radius, Sides, bPersist, Lifetime);
////
////			// Draw server (corrected location)
////			DrawCircle(GetWorld(), ServerLocation, FVector(1, 0, 0), FVector(0, 1, 0), FColor(0, 255, 0, 255), Radius, Sides, bPersist, Lifetime);
////
////			// Draw smooth simulated location
////			FRotationMatrix SmoothMatrix(CharacterOwner->GetMesh()->GetComponentRotation());
////			DrawDebugDirectionalArrow(GetWorld(), SmoothLocation, SmoothLocation + SmoothMatrix.GetScaledAxis(EAxis::Y) * 5, ArrowSize, FColor(255, 255, 0, 255), bPersist, Lifetime);
////			DrawCircle(GetWorld(), SmoothLocation, FVector(1, 0, 0), FVector(0, 1, 0), FColor(0, 0, 255, 255), Radius, Sides, bPersist, Lifetime);
////
////			if (ClientData->LastServerLocation != FVector::ZeroVector)
////			{
////				// Arrow showing simulated line
////				DrawDebugDirectionalArrow(GetWorld(), ClientData->LastServerLocation, SimulatedLocation, ArrowSize, FColor(255, 0, 0, 255), bPersist, Lifetime);
////
////				// Arrow showing server line
////				DrawDebugDirectionalArrow(GetWorld(), ClientData->LastServerLocation, ServerLocation, ArrowSize, FColor(0, 255, 0, 255), bPersist, Lifetime);
////
////				// Arrow showing smooth location plot
////				DrawDebugDirectionalArrow(GetWorld(), ClientData->LastSmoothLocation, SmoothLocation, ArrowSize, FColor(0, 0, 255, 255), bPersist, Lifetime);
////
////				// Line showing correction
////				DrawDebugDirectionalArrow(GetWorld(), SimulatedLocation, ServerLocation, ArrowSize, FColor(128, 0, 0, 255), bPersist, Lifetime);
////
////				// Line showing smooth vector
////				DrawDebugDirectionalArrow(GetWorld(), ServerLocation, SmoothLocation, ArrowSize, FColor(0, 0, 128, 255), bPersist, Lifetime);
////			}
////
////			ClientData->LastServerLocation = ServerLocation;
////			ClientData->LastSmoothLocation = SmoothLocation;
////		}
////#endif
//	}
//}
//
//void USimulationRacingMovementComponent::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
//{
//	Super::CalcVelocity(DeltaTime, Friction, bFluid, BrakingDeceleration);
//	
//	//if (oldV.Equals(Velocity) == false) {
//		//oldV = Velocity;
//		//UE_LOG(LogTemp, Log, TEXT("[%x] [移动] [CalcVelocity] [DeltaTime:%f] [Friction:%f] [bFluid:%f] [BrakingDeceleration:%f] [Velocity:%s] [speed:%f]"),
//		//	this, DeltaTime, Friction, bFluid, BrakingDeceleration, *Velocity.ToString(), Velocity.Size());
//	//}
//}
//
//void USimulationRacingMovementComponent::MoveSmooth(const FVector& InVelocity, const float DeltaSeconds, FStepDownResult* OutStepDownResult )
//{
//	Super::MoveSmooth(InVelocity, DeltaSeconds, OutStepDownResult);
//
//	//if (oldV.Equals(Velocity) == false) {
//		//oldV = Velocity;
//		UE_LOG(LogTemp, Log, TEXT("[%x] [移动] [MoveSmooth] [DeltaSeconds:%f] [InVelocity:%s] [speed:%f]"),
//			this,  DeltaSeconds, *InVelocity.ToString(), InVelocity.Size());
//	//}
//}
//
//
//void USimulationRacingMovementComponent::SimulateMovement(float DeltaSeconds)
//{
//	if (!HasValidData() || UpdatedComponent->Mobility != EComponentMobility::Movable || UpdatedComponent->IsSimulatingPhysics())
//	{
//		return;
//	}
//
//	const bool bIsSimulatedProxy = (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy);
//
//	const FRepMovement& ConstRepMovement = CharacterOwner->GetReplicatedMovement();
//
//	// Workaround for replication not being updated initially
//	if (bIsSimulatedProxy &&
//		ConstRepMovement.Location.IsZero() &&
//		ConstRepMovement.Rotation.IsZero() &&
//		ConstRepMovement.LinearVelocity.IsZero())
//	{
//		return;
//	}
//
//	// If base is not resolved on the client, we should not try to simulate at all
//	if (CharacterOwner->GetReplicatedBasedMovement().IsBaseUnresolved())
//	{
//		//UE_LOG(LogCharacterMovement, Verbose, TEXT("Base for simulated character '%s' is not resolved on client, skipping SimulateMovement"), *CharacterOwner->GetName());
//		return;
//	}
//
//	FVector OldVelocity;
//	FVector OldLocation;
//
//	// Scoped updates can improve performance of multiple MoveComponent calls.
//	{
//		FScopedMovementUpdate ScopedMovementUpdate(UpdatedComponent, bEnableScopedMovementUpdates ? EScopedUpdate::DeferredUpdates : EScopedUpdate::ImmediateUpdates);
//
//		bool bHandledNetUpdate = false;
//		if (bIsSimulatedProxy)
//		{
//			// Handle network changes
//			if (bNetworkUpdateReceived)
//			{
//				bNetworkUpdateReceived = false;
//				bHandledNetUpdate = true;
//				//UE_LOG(LogCharacterMovement, Verbose, TEXT("Proxy %s received net update"), *CharacterOwner->GetName());
//				if (bNetworkMovementModeChanged)
//				{
//					ApplyNetworkMovementMode(CharacterOwner->GetReplicatedMovementMode());
//					bNetworkMovementModeChanged = false;
//				}
//				else if (bJustTeleported || bForceNextFloorCheck)
//				{
//					// Make sure floor is current. We will continue using the replicated base, if there was one.
//					bJustTeleported = false;
//					UpdateFloorFromAdjustment();
//				}
//			}
//			else if (bForceNextFloorCheck)
//			{
//				UpdateFloorFromAdjustment();
//			}
//		}
//
//		UpdateCharacterStateBeforeMovement(DeltaSeconds);
//
//		if (MovementMode != MOVE_None)
//		{
//			//TODO: Also ApplyAccumulatedForces()?
//			HandlePendingLaunch();
//		}
//		ClearAccumulatedForces();
//
//		if (MovementMode == MOVE_None)
//		{
//			return;
//		}
//
//		const bool bSimGravityDisabled = (bIsSimulatedProxy && CharacterOwner->bSimGravityDisabled);
//		const bool bZeroReplicatedGroundVelocity = (bIsSimulatedProxy && IsMovingOnGround() && ConstRepMovement.LinearVelocity.IsZero());
//
//		// bSimGravityDisabled means velocity was zero when replicated and we were stuck in something. Avoid external changes in velocity as well.
//		// Being in ground movement with zero velocity, we cannot simulate proxy velocities safely because we might not get any further updates from the server.
//		if (bSimGravityDisabled || bZeroReplicatedGroundVelocity)
//		{
//			Velocity = FVector::ZeroVector;
//		}
//
//		MaybeUpdateBasedMovement(DeltaSeconds);
//
//		// simulated pawns predict location
//		OldVelocity = Velocity;
//		OldLocation = UpdatedComponent->GetComponentLocation();
//
//		UpdateProxyAcceleration();
//
//		// May only need to simulate forward on frames where we haven't just received a new position update.
//		if (!bHandledNetUpdate || !bNetworkSkipProxyPredictionOnNetUpdate ) //|| !CharacterMovementCVars::NetEnableSkipProxyPredictionOnNetUpdate
//		{
//			//UE_LOG(LogCharacterMovement, Verbose, TEXT("Proxy %s simulating movement"), *GetNameSafe(CharacterOwner));
//			FStepDownResult StepDownResult;
//			MoveSmooth(Velocity, DeltaSeconds, &StepDownResult);
//
//			// find floor and check if falling
//			if (IsMovingOnGround() || MovementMode == MOVE_Falling)
//			{
//				if (StepDownResult.bComputedFloor)
//				{
//					CurrentFloor = StepDownResult.FloorResult;
//				}
//				else if (Velocity.Z <= 0.f)
//				{
//					FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, Velocity.IsZero(), NULL);
//				}
//				else
//				{
//					CurrentFloor.Clear();
//				}
//
//				if (!CurrentFloor.IsWalkableFloor())
//				{
//					if (!bSimGravityDisabled)
//					{
//						// No floor, must fall.
//						if (Velocity.Z <= 0.f || bApplyGravityWhileJumping || !CharacterOwner->IsJumpProvidingForce())
//						{
//							Velocity = NewFallVelocity(Velocity, FVector(0.f, 0.f, GetGravityZ()), DeltaSeconds);
//						}
//					}
//					SetMovementMode(MOVE_Falling);
//				}
//				else
//				{
//					// Walkable floor
//					if (IsMovingOnGround())
//					{
//						AdjustFloorHeight();
//						SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
//					}
//					else if (MovementMode == MOVE_Falling)
//					{
//						if (CurrentFloor.FloorDist <= MIN_FLOOR_DIST || (bSimGravityDisabled && CurrentFloor.FloorDist <= MAX_FLOOR_DIST))
//						{
//							// Landed
//							SetPostLandedPhysics(CurrentFloor.HitResult);
//						}
//						else
//						{
//							if (!bSimGravityDisabled)
//							{
//								// Continue falling.
//								Velocity = NewFallVelocity(Velocity, FVector(0.f, 0.f, GetGravityZ()), DeltaSeconds);
//							}
//							CurrentFloor.Clear();
//						}
//					}
//				}
//			}
//		}
//		else
//		{
//			//UE_LOG(LogCharacterMovement, Verbose, TEXT("Proxy %s SKIPPING simulate movement"), *GetNameSafe(CharacterOwner));
//		}
//
//		UpdateCharacterStateAfterMovement(DeltaSeconds);
//
//		// consume path following requested velocity
//		bHasRequestedVelocity = false;
//
//		OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
//	} // End scoped movement update
//
//	// Call custom post-movement events. These happen after the scoped movement completes in case the events want to use the current state of overlaps etc.
//	CallMovementUpdateDelegate(DeltaSeconds, OldLocation, OldVelocity);
//
//	SaveBaseLocation();
//	UpdateComponentVelocity();
//	bJustTeleported = false;
//
//	LastUpdateLocation = UpdatedComponent ? UpdatedComponent->GetComponentLocation() : FVector::ZeroVector;
//	LastUpdateRotation = UpdatedComponent ? UpdatedComponent->GetComponentQuat() : FQuat::Identity;
//	LastUpdateVelocity = Velocity;
//}
