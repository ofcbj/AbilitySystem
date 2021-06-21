// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityTypes.generated.h"

class UGameplayAbilityBase;

UENUM(BlueprintType)
enum class EAbilityCostType : uint8
{
	Health,
	Mana,
	Strength
};

USTRUCT(BlueprintType)
struct FGameplayAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	float CooldownDuration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	float Cost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	EAbilityCostType CostType = EAbilityCostType::Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	UMaterialInstance* UIMat = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	TSubclassOf<UGameplayAbilityBase> AbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	FString Name;

	FGameplayAbilityInfo();
	FGameplayAbilityInfo(FString InName, float InCooldownDuration, float InCost, EAbilityCostType InCostType, UMaterialInstance* InUIMat, TSubclassOf<UGameplayAbilityBase> InAbilityClass);
};