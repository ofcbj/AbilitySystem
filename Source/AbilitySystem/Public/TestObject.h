// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TestObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ABILITYSYSTEM_API UTestObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Unit)
	TSubclassOf<UObject> UClassOfPlayer;

	UPROPERTY(EditAnywhere, meta=(MetaClass="GameMode"), Category = Unit)
	FStringClassReference UClassGameMode;
};
