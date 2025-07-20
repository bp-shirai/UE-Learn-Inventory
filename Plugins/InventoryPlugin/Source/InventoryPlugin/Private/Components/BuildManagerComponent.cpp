#include "Components/BuildManagerComponent.h"

#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/ActorChannel.h"
#include "Camera/CameraComponent.h"

#include "Player/InventoryCharacter.h"
#include "Components/StorageComponent.h"
#include "Base/BaseItem.h"
#include "Base/RecipeItem.h"
#include "Base/Placeable.h"

UBuildManagerComponent::UBuildManagerComponent()
{
	// Component must be replicated to replicate sub-objects
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick		 = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	PrimaryComponentTick.TickGroup			 = TG_PrePhysics;
	SetComponentTickEnabled(true);

	bIsBuilding		= false;
	bIsActorSpawned = false;
}

void UBuildManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bIsBuilding);
	DOREPLIFETIME(ThisClass, BuildDistance);
	DOREPLIFETIME(ThisClass, BuildingTemplate);
	DOREPLIFETIME(ThisClass, CurrentBuilding);
	DOREPLIFETIME(ThisClass, bIsActorSpawned);
	DOREPLIFETIME(ThisClass, PlaceDistance);
	DOREPLIFETIME(ThisClass, RotSpeed);
	DOREPLIFETIME(ThisClass, CurrentBuildingRotation);
	DOREPLIFETIME_CONDITION(ThisClass, DataTable, COND_InitialOnly);
}

void UBuildManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsBuilding)
	{
		ProcessBuildLocation();
	}
}

// Called when the game starts or when spawned
void UBuildManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	Parent = Cast<AInventoryCharacter>(GetOwner());
	if (!Parent)
	{
		// DO("No Parent Found");
	}
	else
	{
		Inventory = Parent->GetInventory();
	}
}

void UBuildManagerComponent::SetBuilding_Implementation(bool Building, TSubclassOf<APlaceable> Template)
{
	if (Parent->HasAuthority())
	{
		bIsBuilding		 = Building;
		BuildingTemplate = Template;
	}
}

void UBuildManagerComponent::ToggleBuildMode_Implementation(bool StartBuild, TSubclassOf<APlaceable> Template)
{
	SetBuilding(StartBuild, Template);

	if (bIsBuilding)
	{
		FVector Location  = GetNextBuildLocation();
		FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

		SpawnActor(Location, Rotation);
	}
	else
	{
		ClearBuild();
	}
}

void UBuildManagerComponent::RequestBuild_Implementation()
{
	if (Parent->HasAuthority())
	{
		if (bIsBuilding && bIsActorSpawned)
		{
			if (Inventory && CurrentBuilding->Definition.Recipe.Num() > 0)
			{
				bool CanBuild = true;

				for (FInventoryItem& a : CurrentBuilding->Definition.Recipe)
				{
					if (!Inventory->HasItems(a.UniqueName, a.StackSize))
					{
						CanBuild = false;
					}
				}

				if (!CanBuild)
				{
					return;
				}

				for (FInventoryItem& a : CurrentBuilding->Definition.Recipe)
				{
					Inventory->RemoveItemStack(a.UniqueName, a.StackSize);
				}

				Inventory->UpdateUI();

				CurrentBuilding->Build();
				bIsBuilding		= false;
				bIsActorSpawned	= false;
				CurrentBuilding = nullptr;
			}
			else
			{
				CurrentBuilding->Build();
				bIsBuilding		= false;
				bIsActorSpawned	= false;
				CurrentBuilding = nullptr;
			}
		}
	}
}

void UBuildManagerComponent::SpawnActor_Implementation(FVector Location, FRotator Rotation)
{
	if (BuildingTemplate)
	{

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorSpawnParams.Owner							= GetOwner();
		ActorSpawnParams.Instigator						= Cast<APawn>(GetOwner());

		CurrentBuilding = GetWorld()->SpawnActor<APlaceable>(BuildingTemplate, Location, Rotation, ActorSpawnParams);
		bIsActorSpawned	= CurrentBuilding != nullptr;

		if (bIsActorSpawned)
		{
			FRecipeItem* Item = DataTable->FindRow<FRecipeItem>(CurrentBuilding->UniqueName, "");
			if (Item)
			{
				CurrentBuilding->Definition = *Item;
			}
			else
			{
				ClearBuild();
			}
		}
	}
}

void UBuildManagerComponent::ClearBuild_Implementation()
{
	if (Parent->HasAuthority())
	{
		if (CurrentBuilding != nullptr)
		{
			bIsBuilding = false;
			CurrentBuilding->Destroy();
			CurrentBuilding = nullptr;
			bIsActorSpawned	= false;
		}
	}
}

void UBuildManagerComponent::UpdateActorPosition_Implementation(FVector Location)
{
	if (bIsActorSpawned)
	{
		if (LocationSnapGrid)
		{
			FVector v = Location.GridSnap(100);
			CurrentBuilding->SetActorLocationAndRotation(FVector(v.X, v.Y, Location.Z), CurrentBuildingRotation);
		}
		else
		{
			CurrentBuilding->SetActorLocationAndRotation(Location, CurrentBuildingRotation);
		}
	}
}

void UBuildManagerComponent::ProcessBuildLocation_Implementation()
{
	FVector Location = GetNextBuildLocation();
	UpdateActorPosition(Location);
}

FVector UBuildManagerComponent::GetNextBuildLocation()
{
	FVector l = FindLookAtLocation();
	return l;
}

FVector UBuildManagerComponent::FindLookAtLocation()
{
	TArray<AActor*> IgnoreList;
	IgnoreList.Add(CurrentBuilding);
	IgnoreList.Add(Parent);

	FVector HeadPos			= Parent->GetCameraLocation();
	FRotator PlayerRotation = Parent->GetCameraRotation();

	FVector End = HeadPos + (PlayerRotation.Vector() * PlaceDistance);

	FHitResult OutHit;
	bool HasHit = GetHitLoc(HeadPos, End, IgnoreList, OUT OutHit);
	if (HasHit)
	{
		return OutHit.Location;
	}

	return FVector();
}

FRotator UBuildManagerComponent::GetNextBuildRotation()
{
	return CurrentBuildingRotation;
}

void UBuildManagerComponent::RotateBuilding_Implementation(float AxisValue)
{
	if (bIsBuilding)
	{
		CurrentBuildingRotation.Yaw += AxisValue * RotSpeed;
		CurrentBuildingRotation = FRotator(CurrentBuildingRotation.Pitch, FMath::GridSnap(CurrentBuildingRotation.Yaw, RotSpeed), CurrentBuildingRotation.Roll);
	}
}

bool UBuildManagerComponent::GetHitLoc(FVector Start, FVector End, TArray<AActor*> IgnoreList, FHitResult& OutHit)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	bool result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		Start,
		End,
		TraceObjectTypes,
		false,
		IgnoreList,
		EDrawDebugTrace::None,
		OUT OutHit,
		true);

	return result;
}
