// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeSetBase.h"
#include "CharacterBase.h"

UAttributeSetBase::UAttributeSetBase()
	:Health(200.0f), MaxHealth(200.0f),
	Mana(100.0f), MaxMana(100.0f),
	Stamina(250.0f), MaxStamina(250.0f),
	Power(0.0f), Armor(0.0f),
	Combo(1.0f)
{
}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	AActor* TargetActor = nullptr;
	ACharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ACharacterBase>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		AActor* SourceActor = nullptr;
		ACharacterBase* SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceCharacter = Cast<ACharacterBase>(SourceActor);
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		const float LocalDamageDone = GetDamage();
		SetDamage(0.f); 

		if (LocalDamageDone > 0)
		{
			if (TargetCharacter)
			{
				TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.0f, MaxHealth.GetCurrentValue()));
		OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());

		ACharacterBase* CharacterOwner = Cast<ACharacterBase>(GetOwningActor());
		if (CharacterOwner)
		{
			if (Health.GetCurrentValue() == MaxHealth.GetCurrentValue())
			{
				CharacterOwner->AddGameplayTag(CharacterOwner->FullHealthTag);
			}
			else
			{
				CharacterOwner->RemoveGameplayTag(CharacterOwner->FullHealthTag);
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.0f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.0f, MaxMana.GetCurrentValue()));
		OnManaChange.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		Stamina.SetCurrentValue(FMath::Clamp(Stamina.GetCurrentValue(), 0.0f, MaxStamina.GetCurrentValue()));
		Stamina.SetBaseValue(FMath::Clamp(Stamina.GetBaseValue(), 0.0f, MaxStamina.GetCurrentValue()));
		OnStaminaChange.Broadcast(Stamina.GetCurrentValue(), MaxStamina.GetCurrentValue());
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.0f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.0f, MaxMana.GetCurrentValue()));
		OnManaChange.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
	}

	if (Data.EvaluatedData.Attribute == GetPowerAttribute())
	{
		Power.SetCurrentValue(FMath::Clamp(Power.GetCurrentValue(), 0.0f, Power.GetCurrentValue()));
		OnPowerChange.Broadcast(Power.GetCurrentValue());
	}

	if (Data.EvaluatedData.Attribute == GetArmorAttribute())
	{
		Armor.SetCurrentValue(FMath::Clamp(Armor.GetCurrentValue(), 0.0f, 100.0f));
		OnArmorChange.Broadcast(Power.GetCurrentValue());
	}

	if (Data.EvaluatedData.Attribute == GetCriticalAttribute())
	{
		Critical.SetCurrentValue(FMath::Clamp(Critical.GetCurrentValue(), 0.0f, 100.0f));
		OnCriticalChange.Broadcast(Critical.GetCurrentValue());
	}
}
