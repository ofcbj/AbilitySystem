// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GATargetActorCommon.h"
#include "GATargetActorGroundSelect.generated.h"

class UDecalComponent;
class USceneComponent;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API AGATargetActorGroundSelect : public AGATargetActorCommon
{
	GENERATED_BODY()

public:
	AGATargetActorGroundSelect();
	void StartTargeting(UGameplayAbility* Ability) override;
	void ConfirmTargetingAndContinue() override;
	void Tick(float DeltaSeconds) override;
	
	FVector GetPlayerLookingPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GroundBlast")
	UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GroundBlast")
	USceneComponent* RootComp;
};
