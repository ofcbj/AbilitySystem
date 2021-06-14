// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "AttributeSet.h"

#include "PyFunction.h"

using namespace std;

static UWorld* s_World = nullptr;

void UPyFunction::SetWorld(UWorld* World)
{
	s_World = World;
}

FString UPyFunction::GetPropStr(FProperty* PropIt, UObject* Object)
{
	if (PropIt->IsA(FBoolProperty::StaticClass()))
	{
		FBoolProperty* Prop = CastChecked<FBoolProperty>(PropIt);
		bool CurValue = Prop->GetPropertyValue_InContainer(Object);
		return FString::Format(TEXT("{0}"), { CurValue ? TEXT("true") : TEXT("false") });
	}
	else if (PropIt->IsA(FFloatProperty::StaticClass()))
	{
		FFloatProperty* Prop = CastChecked<FFloatProperty>(PropIt);
		float CurValue = Prop->GetPropertyValue_InContainer(Object);
		return FString::Format(TEXT("{0}"), { CurValue });
	}
	else if (PropIt->IsA(FIntProperty::StaticClass()))
	{
		FIntProperty* Prop = CastChecked<FIntProperty>(PropIt);
		int CurValue = Prop->GetPropertyValue_InContainer(Object);
		return FString::Format(TEXT("{0}"), { CurValue });
	}
	else if (PropIt->IsA(FNameProperty::StaticClass()))
	{
		FNameProperty* Prop = CastChecked<FNameProperty>(PropIt);
		FName CurValue = Prop->GetPropertyValue_InContainer(Object);
		return FString::Format(TEXT("{0}"), { CurValue.ToString() });
	}
	else if (PropIt->IsA(FTextProperty::StaticClass()))
	{
		FTextProperty* Prop = CastChecked<FTextProperty>(PropIt);
		FText CurValue = Prop->GetPropertyValue_InContainer(Object);
		return FString::Format(TEXT("{0}"), { CurValue.ToString() });
	}
	else if (PropIt->IsA(FStrProperty::StaticClass()))
	{
		FStrProperty* Prop = CastChecked<FStrProperty>(PropIt);
		FString CurValue = Prop->GetPropertyValue_InContainer(Object);
		return CurValue;
	}
	else if (PropIt->IsA(FArrayProperty::StaticClass()))
	{
		FArrayProperty* Prop = CastChecked<FArrayProperty>(PropIt);
		//return FString::Format(TEXT("{0}"), { Prop->Inner->GetClass()->GetName() });
		FString TypeName = Prop->Inner->GetClass()->GetName();
		FString ForDecl = Prop->GetCPPTypeForwardDeclaration();
		if (ForDecl.Len() > 0)
		{
			ForDecl = ForDecl.Replace(TEXT("class "), TEXT(""));
			ForDecl = ForDecl.Replace(TEXT(";"), TEXT(""));
		}
		const FScriptArray& CurValue = Prop->GetPropertyValue_InContainer(Object);
		return FString::Format(TEXT("{0}: {1}"), { ForDecl.Len() == 0 ?  TypeName:ForDecl, CurValue.Num() });
	}
	else if (PropIt->IsA(FStructProperty::StaticClass()))
	{
		FStructProperty* Prop = CastChecked<FStructProperty>(PropIt);
		if (Prop->Struct->GetName() == "Vector")
		{
			FVector* Vec = Prop->ContainerPtrToValuePtr<FVector>(Object);
			return Vec->ToString();
			//return FString::Format(TEXT("({0}, {1}, {2})"), { Vec->X, Vec->Y, Vec->Z });
		}
		else if (Prop->Struct->GetName() == "Rotator")
		{
			FRotator* Rot = Prop->ContainerPtrToValuePtr<FRotator>(Object);
			return Rot->ToString();
			//return FString::Format(TEXT("({0}, {1}, {2})"), { Rot->Yaw, Rot->Pitch, Rot->Roll });
		}
		else if (Prop->Struct->GetName() == "Quat")
		{
			FQuat* Quat = Prop->ContainerPtrToValuePtr<FQuat>(Object);
			return Quat->ToString();
			//return FString::Format(TEXT("({0}, {1}, {2}, {3})"), { Quat->X, Quat->Y, Quat->Z, Quat->W });
		}
		else if (Prop->Struct->GetName() == "GameplayAttributeData")
		{
			FGameplayAttributeData* Data = Prop->ContainerPtrToValuePtr<FGameplayAttributeData>(Object);
			return FString::Format(TEXT("Base: {0}, Current: {1}"), { Data->GetBaseValue(), Data->GetCurrentValue() });
		}
		else
		{
			//return FString::Format(TEXT("{0}"), { Prop->Struct->GetStructCPPName() });
			return TEXT("Struct");
		}
	}
	else if (PropIt->IsA(FObjectProperty::StaticClass()))
	{
		FObjectProperty* Prop = CastChecked<FObjectProperty>(PropIt);
		UObject* ChildObject = *Prop->GetPropertyValuePtr_InContainer(Object);
		FString ObjectName = TEXT("NULL");
		if (ChildObject != nullptr)
		{
			ChildObject->GetName(ObjectName);
		}
		return FString::Format(TEXT("{0}"), { ObjectName });
	}
	else
	{
		FProperty* Prop = CastChecked<FProperty>(PropIt);
		return FString::Format(TEXT("{0}"), { Prop->GetNameCPP() });
	}
}

bool UPyFunction::SetPropStr(FProperty* PropIt, UObject* Object, FString Value)
{
	if (PropIt->IsA(FBoolProperty::StaticClass()))
	{
		FBoolProperty* Prop = CastChecked<FBoolProperty>(PropIt);
		Prop->SetPropertyValue_InContainer(Object, Value == TEXT("true") ? true : false);
	}
	else if (PropIt->IsA(FFloatProperty::StaticClass()))
	{
		FFloatProperty* Prop = CastChecked<FFloatProperty>(PropIt);
		Prop->SetPropertyValue_InContainer(Object, FCString::Atof(*Value));
	}
	else if (PropIt->IsA(FIntProperty::StaticClass()))
	{
		FIntProperty* Prop = CastChecked<FIntProperty>(PropIt);
		Prop->SetPropertyValue_InContainer(Object, FCString::Atoi(*Value));
	}
	else if (PropIt->IsA(FNameProperty::StaticClass()))
	{
		FNameProperty* Prop = CastChecked<FNameProperty>(PropIt);
		Prop->SetPropertyValue_InContainer(Object, FName(*Value));
	}
	else if (PropIt->IsA(FTextProperty::StaticClass()))
	{
		FTextProperty* Prop = CastChecked<FTextProperty>(PropIt);
		Prop->SetPropertyValue_InContainer(Object, FText::FromString(Value));
	}
	else if (PropIt->IsA(FStrProperty::StaticClass()))
	{
		FStrProperty* Prop = CastChecked<FStrProperty>(PropIt);
		Prop->SetPropertyValue_InContainer(Object, Value);
	}
	else if (PropIt->IsA(FByteProperty::StaticClass()))
	{
		FByteProperty* Prop = CastChecked<FByteProperty>(PropIt);
		Prop->SetPropertyValue_InContainer(Object, FCString::Atoi(*Value));
	}

	return true;
}

FString UPyFunction::GetObjectPropListStr(UObject* Object)
{
	UClass* Class = Object->GetClass();

	FString StrRet = TEXT("{");
	bool isFirst = true;
	for (TFieldIterator<FProperty> PropIt(Class); PropIt; ++PropIt)
	{
		if (isFirst == true)
			isFirst = false;
		else
			StrRet += TEXT(",");

		FProperty* Prop = *PropIt;
		FName PropName = Prop->GetFName();
		if (PropName.ToString()== TEXT("Tags"))
		{
			int x = 0;
		}
		StrRet += TEXT("'");
		StrRet += PropName.ToString();
		StrRet += TEXT("': ['");
		StrRet += Prop->GetCPPType();
		StrRet += TEXT("','");
		StrRet += GetPropStr(Prop, Object);
		StrRet += TEXT("']");
	}
	StrRet += TEXT("}");

	return StrRet;
}

FString UPyFunction::GetPropStr(UObject* Object, FString PropName)
{
	FString ValRet;
	for (TFieldIterator<FProperty> PropIt(Object->GetClass()); PropIt; ++PropIt)
	{
		FString ItrName = PropIt->GetNameCPP();
		if (PropName != ItrName)
			continue;

		if (PropIt->IsA(FObjectProperty::StaticClass()))
		{
			FProperty* Property = *PropIt;
			FObjectProperty* Prop = CastChecked<FObjectProperty>(Property);
			UObject* ChildObject = *Prop->GetPropertyValuePtr_InContainer(Object);
			if (ChildObject)
				ValRet = GetObjectPropListStr(ChildObject);
		}
		else
		{
			ValRet = GetPropStr(*PropIt, Object);
		}
	}

	return ValRet;
}

bool UPyFunction::SetPropStr(UObject* Object, FString PropName, FString Value)
{
	FString ValRet;
	for (TFieldIterator<FProperty> PropIt(Object->GetClass()); PropIt; ++PropIt)
	{
		FString ItrName = PropIt->GetNameCPP();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ItrName);
		if (PropName != ItrName)
			continue;

		return SetPropStr(*PropIt, Object, Value);
	}
	return false;
}

UObject* UPyFunction::GetChildObject(UObject* Parent, FString Name)
{
	UClass* Class = Parent->GetClass();
	FProperty* Prop = Class->FindPropertyByName(FName(Name));
	UObject* Child = nullptr;
	if (Prop->IsA(FObjectProperty::StaticClass()))
	{
		FObjectProperty* ObjetProp = CastChecked<FObjectProperty>(Prop);
		Child = *ObjetProp->GetPropertyValuePtr_InContainer(Parent);
	}

	return Child;
}

FString UPyFunction::GetProp(FString FindObjectName, FString PropHierarchy)
{
	UObject* Object = nullptr;
	TArray<AActor*> ArrOutActors;
	UGameplayStatics::GetAllActorsOfClass(s_World, AActor::StaticClass(), ArrOutActors);
	for (int i = 0; i < ArrOutActors.Num(); ++i)
	{
		AActor* Actor = ArrOutActors[i];
		if (Actor->GetActorLabel() == FindObjectName)
			Object = Actor;
	}

	if (Object == nullptr)
	{
		return FString("None");
	}

	if (PropHierarchy.Len() == 0)
	{
		FString Val = GetObjectPropListStr(Object);
		return Val;
	}
	else
	{
		FString LeafName;
		UObject* LeafObject = GetLeaf(Object, PropHierarchy, LeafName);
		FString Val = GetPropStr(LeafObject, LeafName);
		return Val;
	}
}

UObject* UPyFunction::GetLeaf(UObject* Root, FString PropHierarchy, FString& OutLeafName)
{
	UObject* LeafObject = Root;
	TArray<FString> NameListRaw = {};
	TArray<FString> NameList = {};
	PropHierarchy.ParseIntoArray(NameListRaw, TEXT(","));

	for (auto Name : NameListRaw)
	{
		NameList.Add(Name.TrimStartAndEnd());
	}

	if (NameList.Num() == 1)
	{
		OutLeafName = NameList[0];
	}
	else
	{
		for (int i = 0; i < NameList.Num() - 1; ++i)
		{
			FString Name = NameList[i].TrimStartAndEnd();
			UObject* Child = GetChildObject(LeafObject, Name);
			if (Child != nullptr)
			{
				LeafObject = Child;
			}
		}

		OutLeafName = NameList[NameList.Num() - 1];
	}
	return LeafObject;
}

bool UPyFunction::SetProp(FString FindObjectName, FString PropHierarchy, FString Value)
{
	UObject* Object = nullptr;
	TArray<AActor*> ArrOutActors;
	UGameplayStatics::GetAllActorsOfClass(s_World, AActor::StaticClass(), ArrOutActors);
	for (int i = 0; i < ArrOutActors.Num(); ++i)
	{
		AActor* Actor = ArrOutActors[i];
		if (Actor->GetActorLabel() == FindObjectName)
			Object = Actor;
	}

	if (Object == nullptr)
	{
		return false;
	}

	if (PropHierarchy.Len() == 0)
	{
		return false;
	}
	else
	{
		FString LeafName;
		UObject* LeafObject = GetLeaf(Object, PropHierarchy, LeafName);
		return SetPropStr(LeafObject, LeafName, Value);
	}
}