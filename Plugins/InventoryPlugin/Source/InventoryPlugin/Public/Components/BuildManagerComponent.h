#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BuildManagerComponent.generated.h"

class UCameraComponent;
class UStorageComponent;
class AInventoryCharacter;
class APlaceable;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYPLUGIN_API UBuildManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UBuildManagerComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

#pragma region =============== Variables ==========================

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TObjectPtr<UDataTable> DataTable;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Building)
	bool bIsBuilding;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Building)
	float BuildDistance = 500.0f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Movement)
	float PlaceDistance = 1000.0f;

	TObjectPtr<UCameraComponent> PlayerCamera;
	TObjectPtr<UStorageComponent> Inventory;
	TObjectPtr<AInventoryCharacter> Parent;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Building)
	TSubclassOf<APlaceable> BuildingTemplate;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Building)
	TObjectPtr<APlaceable> CurrentBuilding;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Building)
	bool bIsActorSpawned;

	bool LocationSnapGrid = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Movement)
	float RotSpeed = 45;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Building)
	FRotator CurrentBuildingRotation;

#pragma endregion

#pragma region =============== Functions ====================

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ProcessBuildLocation();

	FVector GetNextBuildLocation();
	FRotator GetNextBuildRotation();
	FVector FindLookAtLocation();

	bool GetHitLoc(FVector Start, FVector End, TArray<AActor*> IgnoreList, FHitResult& OutHit);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ToggleBuildMode(bool StartBuild, TSubclassOf<APlaceable> Template);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetBuilding(bool Building, TSubclassOf<APlaceable> Template);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RequestBuild();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SpawnActor(FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UpdateActorPosition(FVector Location);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ClearBuild();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RotateBuilding(float AxisValue);

#pragma endregion
};