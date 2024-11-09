// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/SmashCharacterSettings.h"

void USmashCharacterStateJump::BeginPlay()
{
	Super::BeginPlay();

	float TargetJumpDuration = bUseAnimDuration ? StateAnimation->GetPlayLength() : JumpDuration;

	JumpVelocity = (2.0 * JumpMaxHeight) / TargetJumpDuration;
	JumpGravity = (-2.0 * JumpMaxHeight) / FMath::Pow(TargetJumpDuration, 2);
}

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->GetCharacterMovement()->MaxWalkSpeed = JumpWalkSpeed;
	Character->GetCharacterMovement()->AirControl = JumpAirControl;
	Character->GetCharacterMovement()->GravityScale = 0.0f;

	Character->GetCharacterMovement()->Velocity.Z = JumpVelocity;
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	Character->GetCharacterMovement()->Velocity.Z += JumpGravity * DeltaTime;

	if(FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold)
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}

	if(!Character->GetInputJump() || Character->GetCharacterMovement()->Velocity.Z <= 0.0f)
	{
		Character->GetCharacterMovement()->Velocity.Z = 0.0f;
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}