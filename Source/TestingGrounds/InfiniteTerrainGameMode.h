// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestingGroundsGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 *
 */
UCLASS()
class TESTINGGROUNDS_API AInfiniteTerrainGameMode : public ATestingGroundsGameMode
{
	GENERATED_BODY()

public:

	AInfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
		void PopulateBoundsVolumePool();

private:

	void AddToPool(class AVolume* VolumeToAdd);


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
		class UActorPool* NavMeshBoundsVolumePool;


};
