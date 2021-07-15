// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstanceBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "Gameframework/NavMovementComponent.h"

void UAnimInstanceBase::AnimNotify_StartFullBody()
{
	ShouldDoFullBody = true;
}

void UAnimInstanceBase::AnimNotify_StartHalfBody()
{
	ShouldDoFullBody = false;
}

void UAnimInstanceBase::NativeBeginPlay()
{
	OwnedCharacter = TryGetPawnOwner();

	TArray<USkeletalMeshComponent*> MeshComp;
	OwnedCharacter->GetComponents<USkeletalMeshComponent>(MeshComp);
	SkeletalMeshComp = MeshComp[0];

	TArray<UNavMovementComponent*> MoveComp;
	OwnedCharacter->GetComponents<UNavMovementComponent>(MoveComp);
	NavMoveComp = MoveComp[0];
}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!::IsValid(OwnedCharacter))
		return;

	FTransform TM = SkeletalMeshComp->K2_GetComponentToWorld();
	FVector Velocity = OwnedCharacter->GetVelocity();

	MeshSpaceVel = UKismetMathLibrary::InverseTransformDirection(TM, Velocity);
	MeshSpaceVel.X *= -1.0;

	IsFalling = NavMoveComp->IsFalling();
}