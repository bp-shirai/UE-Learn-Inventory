#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Base/RecipeItem.h"

#include "Placeable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaced);

class UBoxComponent;

UCLASS()
class INVENTORYPLUGIN_API APlaceable : public APawn
{
	GENERATED_BODY()

public:
	APlaceable();

	//	virtual void Tick(float DeltaTime) override;
	//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Placeable)
	TObjectPtr<UStaticMeshComponent> StaticBuildMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Placeable)
	TObjectPtr<UBoxComponent> CollisionVolume;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Placeable)
	FName UniqueName = "";

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Placeable)
	FRecipeItem Definition;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = Building)
	void Build();
};
