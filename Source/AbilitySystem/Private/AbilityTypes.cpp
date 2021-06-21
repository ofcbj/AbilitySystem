// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTypes.h"

FGameplayAbilityInfo::FGameplayAbilityInfo()
{
}

FGameplayAbilityInfo::FGameplayAbilityInfo(FString InName, float InCooldownDuration, float InCost, EAbilityCostType InCostType, UMaterialInstance* InUIMat, TSubclassOf<UGameplayAbilityBase> InAbilityClass)
{
	Name = InName;
	CooldownDuration = InCooldownDuration;
	Cost = InCost;
	CostType = InCostType;
	UIMat = InUIMat;
	AbilityClass = InAbilityClass;

}