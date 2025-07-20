// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Base/InventoryItem.h"
#include "InventoryPlayerController.generated.h"

class UStorageComponent;

/**
 *
 */
UCLASS()
class INVENTORYPLUGIN_API AInventoryPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void AddItem_Server(UStorageComponent* Storage, FInventoryItem Item);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void AddItemAtIndex_Server(UStorageComponent* Storage, FInventoryItem Item, int32 Index);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void RemoveItem_Server(UStorageComponent* Storage, FInventoryItem Item);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	UStorageComponent* GetPlayerInventory() const;
};
