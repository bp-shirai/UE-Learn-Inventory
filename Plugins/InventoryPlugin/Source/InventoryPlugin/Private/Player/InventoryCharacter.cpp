// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InventoryCharacter.h"

#include "Components/StorageComponent.h"


AInventoryCharacter::AInventoryCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	StorageComponent = CreateDefaultSubobject<UStorageComponent>("StorageComponent");
	StorageComponent->Capacity = 20;
}

void AInventoryCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void AInventoryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInventoryCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

