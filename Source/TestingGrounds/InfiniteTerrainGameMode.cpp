// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "GameFramework/Volume.h"
#include "EngineUtils.h"
#include "ActorPool.h"


AInfiniteTerrainGameMode::AInfiniteTerrainGameMode() {
	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh Bounds Volume Pool"));
}


void AInfiniteTerrainGameMode::PopulateBoundsVolumePool() {

	TActorIterator <AVolume> VolumeIterator = TActorIterator <AVolume>(GetWorld());
	while (VolumeIterator) {
		AddToPool(*VolumeIterator);		
		++VolumeIterator;
	}
}



void AInfiniteTerrainGameMode::AddToPool(AVolume* VolumeToAdd) {
	if (VolumeToAdd->GetName().Contains(FString("NavMesh"),ESearchCase::CaseSensitive,ESearchDir::FromStart)) {
		NavMeshBoundsVolumePool->AddActorToPool(VolumeToAdd);
	}


}