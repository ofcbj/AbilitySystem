// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATargetActorCommon.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API AGATargetActorCommon : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "GroundSelect")
	float Radius;

	void StartTargeting(UGameplayAbility* Ability) override;
	void GetAroundActor(FVector Center, TArray<TWeakObjectPtr<AActor>>& OverlapedActors);
};