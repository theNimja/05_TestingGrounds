// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrollingGuard.h" //TODO remove coupling


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	//TODO protect against empty patrol routes


	//get list of patrol points by getting controlled pawn
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();

	APatrollingGuard* Guard = Cast<APatrollingGuard>(ControlledPawn);


	//set next waypoint
	int Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(Waypoint.SelectedKeyName, Guard->PatrolPointsCPP[Index]);

	//cycle index
	Index++;
	Index = Index % (Guard->PatrolPointsCPP.Num());

	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, Index);


	return EBTNodeResult::Succeeded;
}