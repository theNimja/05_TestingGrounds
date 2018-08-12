// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "GameFramework/Actor.h"
#include "WorldCollision.h"
#include "DrawDebugHelpers.h"
// Sets default values
ATile::ATile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay() {
	Super::BeginPlay();
	CastSphere(GetActorLocation(), 300);
	CastSphere(GetActorLocation()+FVector(0,0,1000), 300);
}

// Called every frame
void ATile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}


void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawned, int MaxSpawned) {


	FVector Min = FVector(0, -2000, 0);
	FVector Max = FVector(4000, 2000, 0);
	FBox Bounds = FBox(Min, Max);

	int NumToSpawn = FMath::RandRange(MinSpawned, MaxSpawned);

	for (int i = 0; i < NumToSpawn; i++) {
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);

		Spawned->SetActorRelativeLocation(SpawnPoint);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));


	}
}


bool ATile::CastSphere(FVector Location, float Radius) {


	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		Location,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	FColor ResultColour = HasHit ? FColor::Red : FColor::Green;
	DrawDebugCapsule(GetWorld(), Location,0, Radius,FQuat::Identity, ResultColour, true, 100);
	return HasHit;

}
