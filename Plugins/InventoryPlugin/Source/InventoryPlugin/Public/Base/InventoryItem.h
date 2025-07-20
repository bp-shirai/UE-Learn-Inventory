#pragma once

#include "CoreMinimal.h"

#include "Base/BaseItem.h"

#include "InventoryItem.generated.h"

class UStorageComponent;

USTRUCT(BlueprintType, Category = Inventory)
struct FInventoryItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FName UniqueName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (ClampMin = 1))
	int32 StackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 Index = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bIsEmpty = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bIsEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TObjectPtr<UStorageComponent> ItemOwner;

	bool operator==(const FInventoryItem rhs) const { return (UniqueName == rhs.UniqueName); }
};