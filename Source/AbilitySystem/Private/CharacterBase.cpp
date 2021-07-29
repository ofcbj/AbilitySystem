// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "AttributeSetBase.h"
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
	AttributeSetBaseComp->OnManaChange.AddDynamic(this, &ACharacterBase::OnManaChanged);
	AttributeSetBaseComp->OnStaminaChange.AddDynamic(this, &ACharacterBase::OnStaminaChanged);

	AddGameplayTag(FullHealthTag);
	
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

void ACharacterBase::AcquireAbilities(TMap<FString, TSubclassOf<UGameplayAbility>> AbilityToAquire)
{
	for (auto& Elem: AbilityToAquire)
	{
		AcquireAbility(Elem.Value);
		if (Elem.Value->IsChildOf(UGameplayAbility::StaticClass()))
		{
			TSubclassOf<UGameplayAbilityBase> AbilityBaseClass = *Elem.Value;
			if (AbilityBaseClass != nullptr)
			{
				AddAbilityToUI(AbilityBaseClass);
			}
		}
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

void ACharacterBase::OnManaChanged(float Mana, float MaxMana)
{
	BP_OnManaChanged(Mana, MaxMana);
}

void ACharacterBase::OnStaminaChanged(float Stamina, float MaxStamina)
{
	BP_OnStaminaChanged(Stamina, MaxStamina);
}

void ACharacterBase::AddGameplayTag(FGameplayTag TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
}

void ACharacterBase::RemoveGameplayTag(FGameplayTag TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
}

bool ACharacterBase::IsOtherHostile(ACharacterBase* Other)
{
	if (this->TeamID == Other->TeamID)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void ACharacterBase::Dead()
{
	DisableInputControl();
}

void ACharacterBase::DisableInputControl()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->DisableInput(PC);
	}
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC && AIC->GetBrainComponent())
	{
		AIC->GetBrainComponent()->StopLogic(TEXT("Dead"));
	}
}

void ACharacterBase::EnableInputControl()
{
	if (!bDead)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->EnableInput(PC);
		}
		AAIController* AIC = Cast<AAIController>(GetController());
		if (AIC && AIC->GetBrainComponent())
		{
			AIC->GetBrainComponent()->RestartLogic();
		}
	}
}

void ACharacterBase::AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd)
{
	APlayerControllerBase* PlayerControllerBase = Cast<APlayerControllerBase>(GetController());
	if (PlayerControllerBase)
	{
		UGameplayAbilityBase* AbilityInstance = AbilityToAdd.Get()->GetDefaultObject<UGameplayAbilityBase>();
		if (AbilityInstance)
		{
			FGameplayAbilityInfo AbilityInfo = AbilityInstance->GetAbilityInfo();
			PlayerControllerBase->AddAbilityToUI(AbilityInfo);
		}
	}
}

void ACharacterBase::HitStun(float StunDruation)
{
	DisableInputControl();
	GetWorldTimerManager().SetTimer(StunTimeHandle, this, &ACharacterBase::EnableInputControl, StunDruation, false);
}

FVector ACharacterBase::GetLookingPoint()
{
	FVector ViewPoint;
	FRotator ViewRotation;
	GetController()->GetPlayerViewPoint(ViewPoint, ViewRotation);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetUniqueID());

	FVector LookAtPoint = FVector();
	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPoint, ViewPoint + ViewRotation.Vector()*10000.0f, ECC_Visibility, QueryParams);
	if (TryTrace)
	{
		return HitResult.ImpactPoint;
	}
	else
	{
		return FVector::ZeroVector;
	}
}

bool ACharacterBase::IsDead()
{
	return bDead;
}

void ACharacterBase::ApplyGESpecHandleToTargetDataSpecHandle(const FGameplayEffectSpecHandle GESpecHandle, const FGameplayAbilityTargetDataHandle TargetDataHandle)
{
	for (TSharedPtr<FGameplayAbilityTargetData> Data : TargetDataHandle.Data)
	{
		Data->ApplyGameplayEffectSpec(*GESpecHandle.Data.Get());
	}
}

void ACharacterBase::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	BP_OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}
