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
	//CastSphere(GetActorLocation(), 300);
	//CastSphere(GetActorLocation() + FVector(0, 0, 1000), 300);
}

// Called every frame
void ATile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}


void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawned, int MaxSpawned, float Radius,float MinScale, float MaxScale) {
	int NumToSpawn = FMath::RandRange(MinSpawned, MaxSpawned);
	for (int i = 0; i < NumToSpawn; i++) {
		FVector SpawnPoint;
		float Scale = FMath::RandRange(MinScale, MaxScale);
		bool Found = GetEmptyLocation(SpawnPoint, Radius*Scale);
		if (Found) {
			float RandomRotation = FMath::RandRange(-180.f, 180.f);
			
			PlaceActor(ToSpawn, SpawnPoint, RandomRotation,Scale);
		}
	}
}

bool ATile::GetEmptyLocation(FVector& OutLocation, float Radius) {
	FVector SpawnPoint;
	const int MAX_ATTEMPTS = 30;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
		FVector Min = FVector(-2000, -2000, 0);
		FVector Max = FVector(2000, 2000, 0);
		FBox Bounds = FBox(Min, Max);

		SpawnPoint = FMath::RandPointInBox(Bounds);
		if (CastSphere(SpawnPoint, Radius) == false) {
			OutLocation = SpawnPoint;
			return true;
		}

	}
	return false;

}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale) {
	AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);

	Spawned->SetActorRelativeLocation(SpawnPoint);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	Spawned->SetActorRotation(FRotator(0, Rotation, 0));
	Spawned->SetActorRelativeScale3D(FVector(Scale, Scale, Scale));
}


bool ATile::CastSphere(FVector Location, float Radius) {

	FHitResult HitResult;

	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	//FColor ResultColour = HasHit ? FColor::Red : FColor::Green;
	//DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColour, true, 100);
	return HasHit;

}
