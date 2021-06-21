// Fill out your copyright notice in the Description page of Project Settings.

#include "GATargetActorGroundSelect.h"

#include "DrawDebugHelpers.h"
#include "GameplayAbilityBase.h"
#include "CharacterBase.h"
#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"

AGATargetActorGroundSelect::AGATargetActorGroundSelect()
{
	PrimaryActorTick.bCanEverTick = true;

	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	SetRootComponent(RootComp);
	Decal->SetupAttachment(RootComp);
	Radius = 200.0f;
	Decal->DecalSize = FVector(Radius);
}

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	Decal->DecalSize = FVector(Radius);
}

void AGATargetActorGroundSelect::ConfirmTargetingAndContinue()
{
	FVector ViewLocation = GetPlayerLookingPoint();

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

	FGameplayAbilityTargetData_LocationInfo* CenterLocation = new FGameplayAbilityTargetData_LocationInfo();
	if (Decal)
	{
		CenterLocation->TargetLocation.LiteralTransform = Decal->GetComponentTransform();
		CenterLocation->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	}

	if (OverlapedActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlapedActors);
		TargetData.Add(CenterLocation);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
}

void AGATargetActorGroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FVector LookPoint = GetPlayerLookingPoint();
	//DrawDebugSphere(GetWorld(), LookPoint, Radius, 32, FColor::Red, false, -1, 0, 5.0f);
	Decal->SetWorldLocation(LookPoint);
}

FVector AGATargetActorGroundSelect::GetPlayerLookingPoint()
{
	APawn* MasterPawn = MasterPC->GetPawn();
	ACharacterBase* CharacterBase = Cast<ACharacterBase>(MasterPawn);
	return CharacterBase->GetLookingPoint();
}
