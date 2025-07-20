#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PickupItem.generated.h"

class UStorageComponent;

UCLASS()
class INVENTORYPLUGIN_API APickupItem : public AActor
{
	GENERATED_BODY()

public:
	APickupItem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, Meta = (ExposeOnSpawn = true))
	FName UniqueName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, Meta = (ExposeOnSpawn = true))
	int StackSize = 1;

//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Inventory)
//	TObjectPtr<UStaticMesh> PickupMesh;

//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)


	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory")
	void PickupItem(UStorageComponent* Storage);


	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnHovered();
};