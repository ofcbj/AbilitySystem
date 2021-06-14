// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "GameplayAbilityBase.h"
#include "GATargetActorGroundSelect.h"

AGATargetActorGroundSelect::AGATargetActorGroundSelect()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGATargetActorGroundSelect::ConfirmTargetingAndContinue()
{
	FVector ViewLocation;
	GetPlayerLookingPoint(ViewLocation);

	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AActor>> OverlapedActors;
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = false;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn)
	{
		CollisionQueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());
	}

	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps, 
															ViewLocation, 
															FQuat::Identity, 
															FCollisionObjectQueryParams(ECC_Pawn), 
															FCollisionShape::MakeSphere(Radius), 
															CollisionQueryParams);

	if (TryOverlap)
	{
		for (int32 i = 0; i < Overlaps.Num(); ++i)
		{
			APawn* PawnOverlaped = Cast<APawn>(Overlaps[i].GetActor());
			if (PawnOverlaped && !OverlapedActors.Contains(PawnOverlaped))
			{
				OverlapedActors.Add(PawnOverlaped);
			}
		}
	}

	if (OverlapedActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlapedActors);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
}

void AGATargetActorGroundSelect::Tick(float DeltaSeconds)
{
	//static FVector PrevLookPoint;
	Super::Tick(DeltaSeconds);
	FVector LookPoint;
	GetPlayerLookingPoint(LookPoint);
	//if (LookPoint != PrevLookPoint)
	//{
		DrawDebugSphere(GetWorld(), LookPoint, Radius, 32, FColor::Red, false, -1, 0, 5.0f);
	//}
	//PrevLookPoint = LookPoint;
}

bool AGATargetActorGroundSelect::GetPlayerLookingPoint(FVector& OutViewPoint)
{
	FVector ViewPoint;
	FRotator ViewRotation;
	MasterPC->GetPlayerViewPoint(ViewPoint, ViewRotation);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn)
	{
		QueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());
	}

	FVector LookAtPoint = FVector();
	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPoint, ViewPoint + ViewRotation.Vector()*10000.0f, ECC_Visibility, QueryParams);
	if (TryTrace)
	{
		OutViewPoint = HitResult.ImpactPoint;
	}

	return TryTrace;
}
