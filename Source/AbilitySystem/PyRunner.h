#pragma once

#include "GameFramework/Character.h"


#include "PyRunner.generated.h"

UCLASS()
class APyRunner : public ACharacter
{
	GENERATED_BODY()

	bool ShouldTickIfViewportsOnly() const override { return true; };
public:
	void Tick(float DeltaTime) override;
};

