// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSetBase.h"

UAttributeSetBase::UAttributeSetBase()
	:Health(200.0f)
{
	
}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Took damage: %f"), Health.GetCurrentValue());
	}

}
