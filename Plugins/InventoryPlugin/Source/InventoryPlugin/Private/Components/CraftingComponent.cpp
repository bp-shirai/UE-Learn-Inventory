#include "Components/CraftingComponent.h"

#include "Components/StorageComponent.h"
#include "Base/InventoryItem.h"
#include "Base/RecipeItem.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"

UCraftingComponent::UCraftingComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCraftingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsCrafting);
	DOREPLIFETIME(ThisClass, CraftingTimerHandle);
	DOREPLIFETIME(ThisClass, CraftingTimeLeft);
	DOREPLIFETIME(ThisClass, Inventory);
	DOREPLIFETIME(ThisClass, UniqueName);
	DOREPLIFETIME_CONDITION(ThisClass, DataTable, COND_InitialOnly);
}

void UCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCrafting)
	{
		UpdateCraftingTimer();
	}
}

void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCraftingComponent::StartCrafting_Implementation()
{
	bIsCrafting = !bIsCrafting;

	if (bIsCrafting)
	{
		if (UniqueName.GetStringLength() > 0 && DataTable)
		{
			// Get Recipe
			FRecipeItem* Item = DataTable->FindRow<FRecipeItem>(UniqueName, "");
			if (Inventory)
			{
				// process whether the player has the ingredients
				bool HasIngredients = true;

				for (FInventoryItem& a : Item->Recipe)
				{
					if (!Inventory->HasItems(a.UniqueName, a.StackSize))
					{
						HasIngredients = false;
					}
				}

				if (HasIngredients)
				{
					for (FInventoryItem& a : Item->Recipe)
					{
						Inventory->RemoveItemStack(a.UniqueName, a.StackSize);
					}

					if (Item->BuildTime > 0.0f)
					{
						GetWorld()->GetTimerManager().SetTimer(CraftingTimerHandle, this, &ThisClass::ProcessCrafting, Item->BuildTime, false);
					}
					else
					{
						ProcessCrafting();
					}
				}
			}
		}
	}
	else
	{
		ClearCrafting();
	}
}

void UCraftingComponent::ProcessCrafting()
{
	FRecipeItem* Item = DataTable->FindRow<FRecipeItem>(UniqueName, "");
	if (Item && Inventory)
	{
		FInventoryItem NewItem = FInventoryItem();
		NewItem.UniqueName	   = Item->UniqueName;
		NewItem.StackSize	   = Item->OutputStackSize;
		Inventory->AddItem(NewItem);
	}
}

void UCraftingComponent::UpdateCraftingTimer_Implementation()
{
	CraftingTimeLeft = GetWorld()->GetTimerManager().GetTimerRemaining(CraftingTimerHandle);
}

void UCraftingComponent::SetIsCrafting_Implementation(bool IsCrafting)
{
	bIsCrafting = IsCrafting;
}

void UCraftingComponent::ClearCrafting_Implementation()
{
	if (CraftingTimerHandle.IsValid())
	{
		CraftingTimeLeft = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(CraftingTimerHandle);
	}
}

void UCraftingComponent::CancelCrafting_Implementation()
{
	FRecipeItem* Item = DataTable->FindRow<FRecipeItem>(UniqueName, "");
	if (Item && Inventory)
	{
		GetWorld()->GetTimerManager().ClearTimer(CraftingTimerHandle);

		for (FInventoryItem& a : Item->Recipe)
		{
			Inventory->AddItem(a);
		}
	}
}
