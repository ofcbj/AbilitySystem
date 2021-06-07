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
	UFUNCTION(BlueprintCallable, Category = "Unreal Python")
	static FString GetProp(FString ObjectName, FString PropHierarchy);
	
	static FString GetPropStr(FProperty* PropIt, UObject* Object);
	static FString GetPropStr(UObject* Object, FString PropName);

	UFUNCTION(BlueprintCallable, Category = "Unreal Python")
	static bool SetProp(FString ObjectName, FString PropHierachy, FString Value);

	static bool SetPropStr(FProperty* PropIt, UObject* Object, FString Value);
	static bool SetPropStr(UObject* Object, FString PropName, FString Value);
	
	static FString GetObjectPropListStr(UObject* Object);

	static UObject* GetLeaf(UObject* Root, FString PropHierarchy, FString& OutLeafName);
	static UObject* GetChildObject(UObject* Parent, FString Name);

	static void SetWorld(UWorld* World);
};
