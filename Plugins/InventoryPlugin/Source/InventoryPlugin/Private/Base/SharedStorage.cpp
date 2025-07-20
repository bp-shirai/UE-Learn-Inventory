
#include "Base/SharedStorage.h"

#include "Net/UnrealNetwork.h"

#include "Components/StorageComponent.h"

ASharedStorage::ASharedStorage()
{
	SetReplicates(true);

	StorageComponent = CreateDefaultSubobject<UStorageComponent>("StorageComponent");
	StorageComponent->Capacity = 20;
}

void ASharedStorage::BeginPlay()
{
	Super::BeginPlay();
}

void ASharedStorage::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StorageComponent);
}

void ASharedStorage::AddItem_Server_Implementation(FInventoryItem Item)
{
	StorageComponent->AddItem(Item);
}

void ASharedStorage::RemoveItem_Server_Implementation(FInventoryItem Item)
{
	StorageComponent->RemoveItem(Item);
}