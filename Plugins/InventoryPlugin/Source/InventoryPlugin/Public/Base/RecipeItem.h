#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Base/InventoryItem.h"
#include "RecipeItem.generated.h"

USTRUCT(BlueprintType, Category = Inventory)
struct FRecipeItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FName UniqueName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FString ItemDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool canChain = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 LevelOrSkill = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 OutputStackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	float BuildTime = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<FInventoryItem> Recipe;

	bool operator==(const FRecipeItem rhs) const { return (UniqueName == rhs.UniqueName); }
};
