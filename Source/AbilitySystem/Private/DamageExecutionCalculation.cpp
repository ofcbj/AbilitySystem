// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageExecutionCalculation.h"
#include "Math/UnrealMathUtility.h"
#include "AttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "CharacterBase.h"

struct DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Power)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Critical)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)

	DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Power, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Critical, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Armor, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Damage, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Health, Target, true);

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
	RelevantAttributesToCapture.Add(DamageStaticsInst().PowerDef);
	RelevantAttributesToCapture.Add(DamageStaticsInst().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStaticsInst().CriticalDef);
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
	if (Curve == NULL)
	{
		check(Curve);
		return;
	}
	FKeyHandle KeyHandle = Curve->FindKey(Combo);
	float Damage = Curve->GetKeyValue(KeyHandle);

	float PowerMagnitude = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStaticsInst().PowerDef, FAggregatorEvaluateParameters(), PowerMagnitude);
	
	float ArmorMagnitude = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStaticsInst().ArmorDef, FAggregatorEvaluateParameters(), ArmorMagnitude);

	float CriticalMagnitude = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStaticsInst().CriticalDef, FAggregatorEvaluateParameters(), CriticalMagnitude);

	Damage += Damage*(PowerMagnitude/100.0f);
	Damage -= Damage*(ArmorMagnitude/100.0f);

	if (FMath::RandRange(0.0f, 100.0f) < CriticalMagnitude)
	{
		Damage *= 2.0f;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStaticsInst().DamageProperty, EGameplayModOp::Override, Damage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStaticsInst().HealthProperty, EGameplayModOp::Additive, -1.0f*Damage));
}
