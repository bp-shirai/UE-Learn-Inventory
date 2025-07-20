#include "Base/SharedCrafting.h"

#include "Net/UnrealNetwork.h"

#include "Components/CraftingComponent.h"

ASharedCrafting::ASharedCrafting()
{
	SetReplicates(true);

	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>("CraftingComponent");
	CraftingComponent->Inventory = StorageComponent;
}

void ASharedCrafting::BeginPlay()
{
	Super::BeginPlay();
}

void ASharedCrafting::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CraftingComponent);
}
