// Fill out your copyright notice in the Description page of Project Settings.


#include "GATargetActorGetAround.h"

void AGATargetActorGetAround::ConfirmTargetingAndContinue()
{
	TArray<TWeakObjectPtr<AActor>> OverlapedActors;

	APawn* OwningPawn = MasterPC->GetPawn();
	if (!OwningPawn)
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
	GetAroundActor(OwningPawn->GetActorLocation(), OverlapedActors);

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
