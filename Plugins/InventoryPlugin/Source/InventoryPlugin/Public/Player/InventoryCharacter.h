// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryCharacter.generated.h"

class UStorageComponent;

UCLASS()
class INVENTORYPLUGIN_API AInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AInventoryCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TObjectPtr<UStorageComponent> StorageComponent;

	virtual FVector GetCameraLocation() { return FVector(); }
	virtual FRotator GetCameraRotation() { return FRotator(); }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual UStorageComponent* GetInventory() const { return StorageComponent; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
