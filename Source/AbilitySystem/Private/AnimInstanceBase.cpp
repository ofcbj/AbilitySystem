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

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto OwnedCharacter = TryGetPawnOwner();
	if (!::IsValid(OwnedCharacter))
		return;

	TArray<USkeletalMeshComponent*> MeshComp;
	OwnedCharacter->GetComponents<USkeletalMeshComponent>(MeshComp);

	USkeletalMeshComponent* Mesh = MeshComp[0];

	FTransform TM = Mesh->K2_GetComponentToWorld();
	FVector Velocity = OwnedCharacter->GetVelocity();

	MeshSpaceVel = UKismetMathLibrary::InverseTransformDirection(TM, Velocity);
	MeshSpaceVel.X *= -1.0;

	TArray<UNavMovementComponent*> MoveComp;
	OwnedCharacter->GetComponents<UNavMovementComponent>(MoveComp);
	IsFalling = MoveComp[0]->IsFalling();
}