// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GATargetActorCommon.h"
#include "GATargetActorGetAround.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API AGATargetActorGetAround : public AGATargetActorCommon
{
	GENERATED_BODY()
	
public:
	void ConfirmTargetingAndContinue() override;
};
