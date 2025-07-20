// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "Base/InventoryItem.h"

#include "StorageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStorageComponentUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStorageComponentFull);

USTRUCT(BlueprintType, Category = Inventory)
struct FItemTimer
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FName UniqueName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FTimerHandle Handle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	float Cooldown = 0.f;

	bool operator==(const FItemTimer rhs) const { return (UniqueName == rhs.UniqueName); }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYPLUGIN_API UStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStorageComponent();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region ============== Variables ========================

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	int32 Capacity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	int32 SlotsFilled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_StorageUpdated, Category = Inventory)
	TArray<FInventoryItem> Items;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	class UDataTable* DataTable;

	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnStorageComponentUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnStorageComponentFull OnInventoryFull;

#pragma endregion

#pragma region ============== Functions ========================

	UFUNCTION()
	void OnRep_StorageUpdated();
	void UpdateUI();

	int32 GetEmpty() const;
	int32 AddEmpty(int32 Index);

	bool AddItem(FInventoryItem& Item);
	bool AddItemAtIndex(FInventoryItem& Item, int32 Index);
	bool RemoveItem(FInventoryItem& Item);
	bool HasItems(FName UniqueName, int32 StackSize);
	bool RemoveItemStack(FName UniqueName, int32 StackSize);

#pragma endregion

#pragma region ============== BP Functions ========================

	UFUNCTION(BlueprintCallable, Category = Inventory)
	TArray<FInventoryItem> GetItems() { return Items; }

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void AddItem_Server(FInventoryItem Item);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void AddItemAtIndex_Server(FInventoryItem Item, int32 Index);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void RemoveItem_Server(FInventoryItem Item);

	UFUNCTION(BlueprintCallable, Category = Inventory, meta = (DisplayName = "HasItems"))
	bool HasItems_BP(FName UniqueName, int32 StackSize);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void RemoveItemStack_Server(FName UniqueName, int32 StackSize);

#pragma endregion

#pragma region ============== Cooldown ========================

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_TimersUpdated, Category = Inventory)
	TArray<FItemTimer> ItemTimers;

	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnStorageComponentUpdated OnTimersUpdated;

	UFUNCTION()
	void OnRep_TimersUpdated();

	void SetTimer(FName UniqueName, float Cooldown);
	void ClearTimer(FName UniqueName);
	float SetTimeRemaining(FName UniqueName);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void SetTimer_Server(FName UniqueName, float Cooldown);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void ClearTimer_Server(FName UniqueName);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	float GetTimeRemaining(FName UniqueName);

#pragma endregion
};
