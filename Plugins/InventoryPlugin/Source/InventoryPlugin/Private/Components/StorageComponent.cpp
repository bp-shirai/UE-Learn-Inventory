// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StorageComponent.h"

#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"

#include "Base/BaseItem.h"

UStorageComponent::UStorageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	ItemTimers = TArray<FItemTimer>();
}

void UStorageComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Items);
	DOREPLIFETIME(ThisClass, ItemTimers);
	DOREPLIFETIME(ThisClass, SlotsFilled);
	DOREPLIFETIME(ThisClass, Capacity);
}

void UStorageComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < Capacity; i++)
	{
		FInventoryItem Item = FInventoryItem();
		Item.Index			= i;
		Item.StackSize		= 0;
		Item.ItemOwner		= this;
		Item.bIsEmpty		= true;
		Items.Add(Item);
	}
}

void UStorageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->HasAuthority())
	{
		for (FItemTimer& Timer : ItemTimers)
		{
			Timer.Cooldown = SetTimeRemaining(Timer.UniqueName);
		}
	}
}

void UStorageComponent::OnRep_StorageUpdated()
{
	// OnInventoryUpdated.Broadcast();
	UpdateUI();
}

void UStorageComponent::UpdateUI()
{
	OnInventoryUpdated.Broadcast();
}

int32 UStorageComponent::GetEmpty() const
{
	for (const FInventoryItem& Item : Items)
	{
		if (Item.bIsEmpty)
		{
			return Item.Index;
		}
	}
	return -1;
}

int32 UStorageComponent::AddEmpty(int32 Index)
{
	FInventoryItem item = FInventoryItem();
	item.Index			= Index;
	item.bIsEmpty		= true;
	item.ItemOwner		= this;
	Items.Insert(item, Index);
	UpdateUI();
	return Index;
}

bool UStorageComponent::AddItem(FInventoryItem& Item)
{
	if (SlotsFilled >= Capacity)
	{
		OnInventoryFull.Broadcast();
		return false;
	}

	// proccess if the item is stackable
	if (!DataTable) return false;

	FItemRow* ItemRow = DataTable->FindRow<FItemRow>(Item.UniqueName, "");

	int32 PendingStackSize = Item.StackSize;

	if (ItemRow->bIsStackable)
	{
		for (FInventoryItem& item : Items)
		{
			if (item == Item)
			{
				if (!ItemRow->bHasMaxStackSize)
				{
					item.StackSize += PendingStackSize;
					UpdateUI();
					return true;
				}
				else
				{
					if (item.StackSize < ItemRow->MaxStackSize)
					{
						int32 OldStackSize = item.StackSize;

						if ((OldStackSize + PendingStackSize) > ItemRow->MaxStackSize)
						{
							int32 Diff		 = (OldStackSize + PendingStackSize) - ItemRow->MaxStackSize;
							item.StackSize	 = ItemRow->MaxStackSize;
							PendingStackSize = Diff;
						}
						else
						{
							item.StackSize += PendingStackSize;
							PendingStackSize = 0;
						}
					}
				}
			}

			if (PendingStackSize == 0)
			{
				UpdateUI();
				return true;
			}
		}
	}

	// new item slot
	int32 NewIndex = GetEmpty();
	Items.RemoveAt(NewIndex);
	Item.Index	   = NewIndex;
	Item.StackSize = PendingStackSize;
	Item.ItemOwner = this;
	Item.bIsEnable = GetTimeRemaining(Item.UniqueName) == 0;
	Items.Insert(Item, NewIndex);
	SlotsFilled++;

	UpdateUI();
	return true;
}

bool UStorageComponent::AddItemAtIndex(FInventoryItem& Item, int32 Index)
{
	if (!DataTable) return false;
	FItemRow* ItemRow = DataTable->FindRow<FItemRow>(Item.UniqueName, "");

	FInventoryItem& item = Items[Index];
	if (item.bIsEmpty)
	{
		Items.RemoveAt(Index);
		Item.Index	   = Index;
		Item.ItemOwner = this;
		Items.Insert(Item, Index);
		SlotsFilled++;
		UpdateUI();
		return true;
	}
	else
	{
		AddItem(Item);
		return true;
	}
}

bool UStorageComponent::RemoveItem(FInventoryItem& Item)
{
	int32 Index = Item.Index;
	Items.RemoveAt(Index);
	AddEmpty(Index);
	SlotsFilled--;
	UpdateUI();
	return true;
}

bool UStorageComponent::HasItems(FName UniqueName, int32 StackSize)
{
	int32 PendingCount = StackSize;
	for (FInventoryItem& item : Items)
	{
		if (item.UniqueName == UniqueName)
		{
			PendingCount -= item.StackSize;
			if (PendingCount <= 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool UStorageComponent::RemoveItemStack(FName UniqueName, int32 StackSize)
{
	int32 PendingCount = StackSize;
	for (FInventoryItem& item : Items)
	{
		if (item.UniqueName == UniqueName)
		{
			if (item.StackSize <= PendingCount)
			{
				PendingCount -= item.StackSize;
				RemoveItem(item);
			}
			else
			{
				item.StackSize -= PendingCount;
				PendingCount = 0;
			}
			if (PendingCount <= 0)
			{
				UpdateUI();
				return true;
			}
		}
	}
	UpdateUI();
	return false;
}

void UStorageComponent::AddItem_Server_Implementation(FInventoryItem Item)
{
	AddItem(Item);
}

void UStorageComponent::AddItemAtIndex_Server_Implementation(FInventoryItem Item, int32 Index)
{
	AddItemAtIndex(Item, Index);
}

void UStorageComponent::RemoveItem_Server_Implementation(FInventoryItem Item)
{
	RemoveItem(Item);
}

bool UStorageComponent::HasItems_BP(FName UniqueName, int32 StackSize)
{
	return HasItems(UniqueName, StackSize);
}

void UStorageComponent::RemoveItemStack_Server_Implementation(FName UniqueName, int32 StackSize)
{
	RemoveItemStack(UniqueName, StackSize);
}

void UStorageComponent::OnRep_TimersUpdated()
{
	OnTimersUpdated.Broadcast();
}

void UStorageComponent::SetTimer(FName UniqueName, float Cooldown)
{
	if (UniqueName.GetStringLength() == 0) return;

	for (FInventoryItem& item : Items)
	{
		if (!item.bIsEmpty && item.UniqueName == UniqueName)
		{
			item.bIsEnable = false;
		}
	}

	bool bTimerExists = false;
	for (FItemTimer& t : ItemTimers)
	{
		if (t.UniqueName == UniqueName)
		{
			bTimerExists = true;
			break;
		}
	}

	int32 NewIndex = -1;
	if (!bTimerExists)
	{
		FItemTimer t				 = FItemTimer();
		t.UniqueName				 = UniqueName;
		FTimerDelegate ClearDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::ClearTimer, UniqueName);
		GetWorld()->GetTimerManager().SetTimer(t.Handle, ClearDelegate, Cooldown, false);
		//GetWorld()->GetTimerManager().SetTimer(t.Handle, [&, UniqueName]() { ClearTimer(UniqueName); }, Cooldown, false);

		NewIndex = ItemTimers.Add(t);
		UpdateUI();
	}
}

void UStorageComponent::ClearTimer(FName UniqueName)
{
	bool bTimerExists = false;
	int32 Index		  = 0;

	for (int32 i = 0; i < ItemTimers.Num(); i++)
	{
		if (ItemTimers[i].UniqueName == UniqueName)
		{
			Index		 = i;
			bTimerExists = true;
			break;
		}
	}

	if (bTimerExists)
	{
		FItemTimer& t = ItemTimers[Index];
		GetWorld()->GetTimerManager().ClearTimer(t.Handle);
		ItemTimers.RemoveAt(Index);
		UpdateUI();
	}

	for (FInventoryItem& item : Items)
	{
		if (!item.bIsEmpty && item.UniqueName == UniqueName)
		{
			item.bIsEnable = true;
		}
	}
}

float UStorageComponent::SetTimeRemaining(FName UniqueName)
{
	bool bTimerExists = false;
	int32 Index		  = 0;

	for (int32 i = 0; i < ItemTimers.Num(); i++)
	{
		if (ItemTimers[i].UniqueName == UniqueName)
		{
			Index		 = i;
			bTimerExists = true;
			break;
		}
	}

	if (bTimerExists)
	{
		FItemTimer& t = ItemTimers[Index];
		return GetWorld()->GetTimerManager().GetTimerRemaining(t.Handle);
	}

	return 0.f;
}

void UStorageComponent::SetTimer_Server_Implementation(FName UniqueName, float Cooldown)
{
	SetTimer(UniqueName, Cooldown);
}

void UStorageComponent::ClearTimer_Server_Implementation(FName UniqueName)
{
	ClearTimer(UniqueName);
}

float UStorageComponent::GetTimeRemaining(FName UniqueName)
{
	for (FItemTimer& t : ItemTimers)
	{
		if (t.UniqueName == UniqueName)
		{
			return t.Cooldown;
		}
	}
	return 0.0f;
}