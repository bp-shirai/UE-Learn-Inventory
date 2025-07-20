#pragma once

#include "CoreMinimal.h"

#include "Base/SharedStorage.h"

#include "SharedCrafting.generated.h"

class UCraftingComponent;

UCLASS()
class INVENTORYPLUGIN_API ASharedCrafting : public ASharedStorage
{
	GENERATED_BODY()

public:
	ASharedCrafting();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Inventory)
	TObjectPtr<UCraftingComponent> CraftingComponent;
};