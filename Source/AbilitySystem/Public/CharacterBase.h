// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "PlayerControllerBase.h"
#include "GameplayAbilityBase.h"
#include "CharacterBase.generated.h"

class UAttributeSetBase;

UCLASS()
class ABILITYSYSTEM_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	UAttributeSetBase* AttributeSetBaseComp;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void AcquireAbilities(TMap<FString, TSubclassOf<UGameplayAbility>> AbilityToAquire);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities);

	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION()
	void OnManaChanged(float Mana, float MaxMana);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnManaChanged"))
	void BP_OnManaChanged(float Mana, float MaxMana);

	UFUNCTION()
	void OnStaminaChanged(float Stamina, float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnStaminaChanged"))
	void BP_OnStaminaChanged(float Stamina, float MaxStamina);

	UFUNCTION()
	void OnPowerChanged(float Power);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnPowerChanged"))
	void BP_OnPowerChanged(float Power);

	UFUNCTION()
	void OnArmorChanged(float Armor);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnArmorChanged"))
	void BP_OnArmorChanged(float Armor);

	UFUNCTION()
	void OnCriticalChanged(float Critical);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnCriticalChanged"))
	void BP_OnCriticalChanged(float Critical);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnDamaged"))
	void BP_OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "Die"))
	void BP_Die();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void AddGameplayTag(FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void RemoveGameplayTag(FGameplayTag TagToRemove);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	FGameplayTag FullHealthTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	FGameplayTag AliveTag;

	UFUNCTION(BlueprintPure, Category = "CharacterBase")
	bool IsOtherHostile(ACharacterBase* Other);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	int TeamID;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void HitStun(float StunDruation);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	FVector GetLookingPoint();

	UFUNCTION(BlueprintPure, Category = "CharacterBase")
	bool IsDead();

	UFUNCTION(BlueprintPure, Category = "CharacterBase")
	bool IsAlive();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void ApplyGESpecHandleToTargetDataSpecHandle(const FGameplayEffectSpecHandle GESpecHandle, const FGameplayAbilityTargetDataHandle TargetDataHandle);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	FName ClassName;

	void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser);
protected:
	bool bDead = false;
	void Dead();
	void DisableInputControl();
	void EnableInputControl();
	FTimerHandle StunTimeHandle;
	void AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd);
};
