// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}




AActor* UActorPool::Checkout() {
	if (Pool.Num() == 0) {
		return NULL;
	}
	return Pool.Pop();
}
void UActorPool::Return(AActor* ActorToReturn) {
	if (ActorToReturn == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("[%s] Returned null actor"), *this->GetName());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Actor Returned: %s"), *this->GetName(), *ActorToReturn->GetName());
	Pool.Push(ActorToReturn);

}
void UActorPool::AddActorToPool(AActor* ActorToAdd) {
	if (ActorToAdd == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("[%s] Added null actor"), *this->GetName());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Actor added: %s"), *this->GetName(), *ActorToAdd->GetName());
	Pool.Push(ActorToAdd);
}

