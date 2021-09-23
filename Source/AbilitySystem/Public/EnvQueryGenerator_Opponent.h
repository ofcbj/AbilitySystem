// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ActorsOfClass.h"
#include "EnvQueryGenerator_Opponent.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UEnvQueryGenerator_Opponent : public UEnvQueryGenerator_ActorsOfClass
{
	GENERATED_BODY()
	
	void ProcessItems(FEnvQueryInstance& QueryInstance, TArray<AActor*>& MatchingActors) const override;
};
