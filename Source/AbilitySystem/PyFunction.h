// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PyFunction.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UPyFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "PyFunction")
	static FString GetProp(FString ActorLabel, FString PropHierarchy);
	
	static FString GetPropStr(FProperty* PropIt, UObject* Object);
	static FString GetPropStr(UObject* Object, FString PropName);

	UFUNCTION(BlueprintCallable, Category = "PyFunction")
	static bool SetProp(FString ActorLabel, FString PropHierachy, FString Value);

	static bool SetPropStr(FProperty* PropIt, UObject* Object, FString Value);
	static bool SetPropStr(UObject* Object, FString PropName, FString Value);
	
	static FString GetObjectPropListStr(UObject* Object);

	static UObject* GetLeaf(UObject* Root, FString PropHierarchy, FString& OutLeafName);
	static UObject* GetChildObject(UObject* Parent, FString Name);

	UFUNCTION(BlueprintCallable, Category = "PyFunction")
	static bool CallBP(FString ActorLabel, FString FuncAndArg);
	
	static void SetWorld(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "PyFunction")
	static AActor* FindActor(FString ActorLabel);
};
