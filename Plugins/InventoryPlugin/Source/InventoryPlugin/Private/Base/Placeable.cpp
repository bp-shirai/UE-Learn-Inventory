#include "Base/Placeable.h"

#include "Net/UnrealNetwork.h"
#include "components/BoxComponent.h"
#include "Engine/CollisionProfile.h"

APlaceable::APlaceable()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	StaticBuildMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticBuildMesh");
	StaticBuildMesh->SetupAttachment(RootComponent);
	//StaticBuildMesh->SetIsReplicated(true);

	CollisionVolume = CreateDefaultSubobject<UBoxComponent>("CollisionVolume");
	CollisionVolume->SetupAttachment(RootComponent);
	CollisionVolume->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	//CollisionVolume->SetIsReplicated(true);
}

void APlaceable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(ThisClass, StaticBuildMesh);
//	DOREPLIFETIME(ThisClass, CollisionVolume);
	DOREPLIFETIME(ThisClass, UniqueName);
	DOREPLIFETIME(ThisClass, Definition);

}

void APlaceable::BeginPlay()
{
	Super::BeginPlay();
}

void APlaceable::Build_Implementation()
{
	StaticBuildMesh->SetVisibility(true);
	CollisionVolume->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
}
