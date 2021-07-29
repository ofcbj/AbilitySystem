// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageExecutionCalculation.h"
#include "AttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "CharacterBase.h"

struct DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)

	DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, AttackDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Armor, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Health, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Damage, Target, true);

		AttackDamageTable = LoadObject<UCurveTable>(NULL, UTF8_TO_TCHAR("CurveTable'/Game/Blueprints/Table/AttackDamage.AttackDamage'"));
	}

	UCurveTable* AttackDamageTable;
};

static DamageStatics& DamageStaticsInst()
{
	static DamageStatics DamageStaticsInst;
	return DamageStaticsInst;
}

UDamageExecutionCalculation::UDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStaticsInst().AttackDamageDef);
	RelevantAttributesToCapture.Add(DamageStaticsInst().ArmorDef);

}

void UDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;

	ACharacterBase* SourceCharacter = Cast<ACharacterBase>(SourceActor);
	ACharacterBase* TargetCharacter = Cast<ACharacterBase>(TargetActor);

	float Combo = ExecutionParams.GetOwningSpecForPreExecuteMod()->GetLevel();

	FString ContextString;
	FRealCurve* Curve = DamageStaticsInst().AttackDamageTable->FindCurve(SourceCharacter->ClassName, ContextString);
	FKeyHandle KeyHandle = Curve->FindKey(Combo);
	float Damage = Curve->GetKeyValue(KeyHandle);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStaticsInst().DamageProperty, EGameplayModOp::Override, Damage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStaticsInst().HealthProperty, EGameplayModOp::Additive, -1.0f*Damage));
}
