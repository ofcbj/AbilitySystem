// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectExtension.h"
#include "AttributeSetBase.generated.h"

/**
 * 
 */
 DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangeDelegate, float, Health, float, MaxHealth);
 DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChangeDelegate, float, Mana, float, MaxMana);
 DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrengthChangeDelegate, float, Strength, float, MaxStrength);

UCLASS()
class ABILITYSYSTEM_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSetBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData MaxStrength;

	FOnHealthChangeDelegate OnHealthChange;
	FOnManaChangeDelegate OnManaChange;
	FOnStrengthChangeDelegate OnStrengthChange;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
