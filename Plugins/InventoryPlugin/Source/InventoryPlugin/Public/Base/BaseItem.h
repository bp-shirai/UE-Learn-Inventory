#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "BaseItem.generated.h"

UENUM(BlueprintType)
enum EItemType : uint8
{
	Unset UMETA(DisplayName = "Unset"),
	Weapon UMETA(DisplayName = "Weapon"),
	Spell UMETA(DisplayName = "Spell"),
	Food UMETA(DisplayName = "Food"),
	Potion UMETA(DisplayName = "Postion"),
	Armour UMETA(DisplayName = "Armour"),
	Material UMETA(DisplayName = "Material"),
};

USTRUCT(BlueprintType, Category = Inventory)
struct FItemRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FName UniqueName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FText ItemDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (Multiline = true))
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FText UseActionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TObjectPtr<UTexture2D> Thumbnail = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TObjectPtr<UStaticMesh> StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TObjectPtr<USkeletalMesh> SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TEnumAsByte<EItemType> ItemType = EItemType::Unset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (ClampMin = 0.0))
	float Weight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (ClampMin = 1))
	int32 StackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (ClampMin = 1))
	int32 MaxStackSize = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (ClampMin = 0.0))
	float Cost = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (ClampMin = 0.0))
	float SellCost = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bIsShopItem = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FString CostType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bIsConsumable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bHasMaxStackSize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bIsRare = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bIsBound = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bIsEnable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bIsAltEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bHasCooldown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (ClampMin = 0.0))
	float CooldownTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bHasAltCooldown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (ClampMin = 0.0))
	float AltCooldownTime = 5.0f;
};