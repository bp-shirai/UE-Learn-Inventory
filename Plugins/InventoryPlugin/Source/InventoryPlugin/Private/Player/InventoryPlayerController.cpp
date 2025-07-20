// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InventoryPlayerController.h"

#include "Components/StorageComponent.h"
#include "Player/InventoryCharacter.h"

void AInventoryPlayerController::AddItem_Server_Implementation(UStorageComponent* Storage, FInventoryItem Item)
{
    Storage->AddItem(Item);
}

void AInventoryPlayerController::AddItemAtIndex_Server_Implementation(UStorageComponent* Storage, FInventoryItem Item, int32 Index)
{
    Storage->AddItemAtIndex(Item, Index);
}


void AInventoryPlayerController::RemoveItem_Server_Implementation(UStorageComponent* Storage, FInventoryItem Item)
{
    Storage->RemoveItem(Item);
}

UStorageComponent* AInventoryPlayerController::GetPlayerInventory() const
{
    const AInventoryCharacter* MyPC = GetPawn<AInventoryCharacter>();
    return MyPC ? MyPC->GetInventory() : nullptr;
}