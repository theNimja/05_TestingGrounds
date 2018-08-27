// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "GameFramework/Actor.h"
#include "WorldCollision.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "ActorPool.h"



// Sets default values
ATile::ATile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MaxExtent = FVector(2000, 2000, 0);
	MinExtent = FVector(-2000, -2000, 0);

	NavigationBoundsOffset = FVector(2000, 0, 0);
}

void ATile::SetPool(UActorPool* InPool) {
	Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == NULL) {
		UE_LOG(LogTemp, Error, TEXT("[%s]Pool is empty."), *GetName());
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("[%s] Checked out {%s}."), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem())->Build();
}



// Called when the game starts or when spawned
void ATile::BeginPlay() {
	Super::BeginPlay();
}


void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason) {

	Pool->Return(NavMeshBoundsVolume);

	//Super::EndPlay(EndPlayReason);


}

// Called every frame
void ATile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawned, int MaxSpawned, float Radius, float MinScale, float MaxScale) {
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawned, MaxSpawned, MinScale, MaxScale, Radius);

	for (FSpawnPosition SpawnPosition : SpawnPositions) {
		PlaceActor(ToSpawn, SpawnPosition);

	}
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawned, int MaxSpawned, float Radius) {
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawned, MaxSpawned, 1, 1, Radius);

	for (FSpawnPosition SpawnPosition : SpawnPositions) {
		PlaceAIPawn(ToSpawn, SpawnPosition);
	
	}
}

TArray<FSpawnPosition>  ATile::RandomSpawnPositions(int MinSpawned, int MaxSpawned, float MinScale, float MaxScale, float Radius)
{
	TArray<FSpawnPosition> SpawnPositions;
	int NumToSpawn = FMath::RandRange(MinSpawned, MaxSpawned);
	for (int i = 0; i < NumToSpawn; i++) {
		FSpawnPosition SpawnPosition;

		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool Found = GetEmptyLocation(SpawnPosition.Location, Radius*SpawnPosition.Scale);
		if (Found) {
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			SpawnPositions.Add(SpawnPosition);
		}
	}

	return SpawnPositions;
}

bool ATile::GetEmptyLocation(FVector& OutLocation, float Radius) {
	FVector SpawnPoint;
	const int MAX_ATTEMPTS = 30;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++) {

		FBox Bounds = FBox(MinExtent, MaxExtent);

		SpawnPoint = FMath::RandPointInBox(Bounds);
		if (CastSphere(SpawnPoint, Radius) == false) {
			OutLocation = SpawnPoint;
			return true;
		}

	}
	return false;

}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition) {
	AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);

	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
	Spawned->SetActorRelativeScale3D(FVector(SpawnPosition.Scale, SpawnPosition.Scale, SpawnPosition.Scale));
}


void ATile::PlaceAIPawn(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition) {
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);

	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
	
	Spawned->SpawnDefaultController();
	Spawned->Tags.Add(FName("Enemy"));
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
