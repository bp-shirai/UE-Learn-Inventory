#pragma once

#include "CoreMinimal.h"

#include "Base/Placeable.h"

#include "SharedStorage.generated.h"

class UStorageComponent;

UCLASS()
class INVENTORYPLUGIN_API ASharedStorage : public APlaceable
{
	GENERATED_BODY()

public:
	ASharedStorage();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Inventory)
	TObjectPtr<UStorageComponent> StorageComponent;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void AddItem_Server(FInventoryItem Item);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void RemoveItem_Server(FInventoryItem Item);
};
