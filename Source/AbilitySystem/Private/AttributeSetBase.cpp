// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeSetBase.h"
#include "CharacterBase.h"

UAttributeSetBase::UAttributeSetBase()
	:Health(200.0f), MaxHealth(200.0f),
	Mana(100.0f), MaxMana(100.0f),
	Stamina(250.0f), MaxStamina(250.0f),
	AttackDamage(20.0f), Armor(5.0f),
	Combo(1.0f)
{
}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)))
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.0f, MaxHealth.GetCurrentValue()));
		OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());

		ACharacterBase* CharacterOwnner = Cast<ACharacterBase>(GetOwningActor());
		if (CharacterOwnner)
		{
			if (Health.GetCurrentValue() == MaxHealth.GetCurrentValue())
			{
				CharacterOwnner->AddGameplayTag(CharacterOwnner->FullHealthTag);
			}
			else
			{
				CharacterOwnner->RemoveGameplayTag(CharacterOwnner->FullHealthTag);
			}
		}
	}

	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Mana)))
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.0f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.0f, MaxMana.GetCurrentValue()));
		OnManaChange.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
	}

	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Stamina)))
	{
		Stamina.SetCurrentValue(FMath::Clamp(Stamina.GetCurrentValue(), 0.0f, MaxStamina.GetCurrentValue()));
		Stamina.SetBaseValue(FMath::Clamp(Stamina.GetBaseValue(), 0.0f, MaxStamina.GetCurrentValue()));
		OnStaminaChange.Broadcast(Stamina.GetCurrentValue(), MaxStamina.GetCurrentValue());
	}


}
