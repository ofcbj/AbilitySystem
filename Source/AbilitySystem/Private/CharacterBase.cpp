// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "AttributeSetBase.h"
#include "GameFramework/PlayerController.h"
#include "AiController.h"
#include "BrainComponent.h"
// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	AttributeSetBaseComp= CreateDefaultSubobject<UAttributeSetBase>("AttributeSetBaseComp");
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	AttributeSetBaseComp->OnHealthChange.AddDynamic(this, &ACharacterBase::OnHealthChanged);
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ACharacterBase::AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire)
{
	if (AbilitySystemComp)
	{
		if (HasAuthority() && AbilityToAquire)
		{
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityToAquire, 1, 0));
		}
		AbilitySystemComp->InitAbilityActorInfo(this, this);

	}
}

void ACharacterBase::OnHealthChanged(float Health, float MaxHealth)
{
	if (Health <= 0.0f && bDead == false)
	{
		bDead = true;
		BP_Die();
		Dead();
	}
	BP_OnHealthChanged(Health, MaxHealth);
}

void ACharacterBase::Dead()
{
	APlayerController* Pc =Cast<APlayerController>(GetController());
	if (Pc)
	{
		Pc->DisableInput(Pc);
	}
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBrainComponent()->StopLogic(TEXT("Dead"));
	}
}
