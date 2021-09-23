// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryGenerator_Opponent.h"
#include "CharacterBase.h"

void UEnvQueryGenerator_Opponent::ProcessItems(FEnvQueryInstance& QueryInstance, TArray<AActor*>& MatchingActors) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	ACharacterBase* Querier = Cast<ACharacterBase>(QueryOwner);

	MatchingActors.RemoveAll([Querier](AActor* Actor) {
		return Querier->IsOtherHostile(Cast<ACharacterBase>(Actor)) == false || Cast<ACharacterBase>(Actor)->IsDead();
	});
}
