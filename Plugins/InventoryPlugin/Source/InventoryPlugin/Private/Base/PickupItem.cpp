#include "Base/PickupItem.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#include "Components/StorageComponent.h"

APickupItem::APickupItem()
{
	SetReplicates(true);
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();
}

void APickupItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(ThisClass, PickupMesh);
}

void APickupItem::PickupItem_Implementation(UStorageComponent* Storage)
{
	// if (!Character) return;

	// TArray<UStorageComponent*> Comps;
	// Character->GetComponents<UStorageComponent>(Comps);

	// if (Comps.Num() > 0)
	//{
	//	if (UStorageComponent* First = Comps[0])
	if (IsValid(Storage))
	{
		FInventoryItem item = FInventoryItem();
		item.UniqueName		= UniqueName;
		item.StackSize		= StackSize;
		item.ItemOwner		= Storage;
		Storage->AddItem(item);

		Destroy();
	}
	//}
}