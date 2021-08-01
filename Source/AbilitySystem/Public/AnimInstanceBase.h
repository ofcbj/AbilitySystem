// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void AnimNotify_StartFullBody();

	UFUNCTION()
	void AnimNotify_StartHalfBody();

	UFUNCTION(BlueprintPure, Category = "CharacterBase")
	bool IsOwnerAlive();

	UFUNCTION(BlueprintPure, Category = "CharacterBase")
	bool IsOwnerDead();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool ShouldDoFullBody;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FVector MeshSpaceVel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	APawn* OwnedCharacter;

	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

	class USkeletalMeshComponent* SkeletalMeshComp;
	class UNavMovementComponent* NavMoveComp;
};
