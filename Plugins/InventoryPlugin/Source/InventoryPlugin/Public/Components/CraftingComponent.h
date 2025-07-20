#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CraftingComponent.generated.h"

class UStorageComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYPLUGIN_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCraftingComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

#pragma region ============ Variables ===================

	UPROPERTY(Replicated, BlueprintReadWrite, Category = Crafting)
	FTimerHandle CraftingTimerHandle;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Crafting)
	bool bIsCrafting;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Crafting)
	FName UniqueName = "";

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Crafting)
	float CraftingTimeLeft = 0.0f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = Crafting)
	TObjectPtr<UStorageComponent> Inventory;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Crafting)
	TObjectPtr<UDataTable> DataTable;

#pragma endregion

#pragma region ============== BP Functions ====================

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void StartCrafting();

	UFUNCTION()
	void ProcessCrafting();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UpdateCraftingTimer();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetIsCrafting(bool IsCrafting);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ClearCrafting();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void CancelCrafting();

#pragma endregion
};