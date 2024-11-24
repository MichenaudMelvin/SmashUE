// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateFall.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/States/SmashCharacterStateJump.h"

#if WITH_EDITORONLY_DATA
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#endif

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	// idle shouldn't be possible but still
	if(PreviousStateID == ESmashCharacterStateID::Idle || PreviousStateID == ESmashCharacterStateID::Walk || PreviousStateID == ESmashCharacterStateID::Run)
	{
		bCanDoCoyoteTime = true;
		CoyoteTime = 0.0f;
		JumpCount = 0;
	}
	else
	{
		bCanDoCoyoteTime = false;
	}

	Character->GetCharacterMovement()->MaxWalkSpeed = FallHorizontalMoveSpeed;
	Character->GetCharacterMovement()->AirControl = FallAirControl;
	bReleasedJumpKey = false;

}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	if(NextStateID != ESmashCharacterStateID::Jump)
	{
		return;
	}

	USmashCharacterStateJump* JumpState = Cast<USmashCharacterStateJump>(StateMachine->GetState(NextStateID));
	if(JumpState == nullptr)
	{
		return;
	}

	JumpState->SetJumpCount(JumpCount);
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	const float MoveXValue = Character->GetInputMoveX();
	const float MoveYValue = Character->GetInputMoveY();

	if(bCanDoCoyoteTime)
	{
		CoyoteTime += DeltaTime;
		if(CoyoteTime >= CoyoteTimeMaxDuration)
		{
			// only consume one jump
			JumpCount++;
			bCanDoCoyoteTime = false;

#if WITH_EDITORONLY_DATA
			if(bDebugState)
			{
				// cause a weird camera effect
				FVector CapsuleLocation = Character->GetCapsuleComponent()->GetComponentLocation();
				FRotator CapsuleRotation = Character->GetCapsuleComponent()->GetComponentRotation();
				float CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				float CapsuleRadius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
				UKismetSystemLibrary::DrawDebugCapsule(Character, CapsuleLocation, CapsuleHalfHeight, CapsuleRadius, CapsuleRotation, FLinearColor::Red, 5.0f);
			}
#endif
		}
	}

	if(!Character->GetInputJump())
	{
		bReleasedJumpKey = true;
	}

	else if(bReleasedJumpKey && Character->GetInputJump())
	{
		if(JumpCount >= MaxJumpCount)
		{
			return;
		}

		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
		return;
	}

	if(MoveYValue < -CharacterSettings->InputMoveXThreshold)
	{
		Character->GetCharacterMovement()->GravityScale = FallFastGravityScale;
	}
	else
	{
		Character->GetCharacterMovement()->GravityScale = FallGravityScale;
	}

	if(FMath::Abs(MoveXValue) > CharacterSettings->InputMoveXThreshold)
	{
		Character->AddMovementInput(FVector::ForwardVector, MoveXValue);
	}

	if(Character->GetCharacterMovement()->IsFalling())
	{
		return;
	}

	StateMachine->ChangeState(ESmashCharacterStateID::Idle);

	USmashCharacterStateJump* JumpState = Cast<USmashCharacterStateJump>(StateMachine->GetState(ESmashCharacterStateID::Jump));
	if(JumpState == nullptr)
	{
		return;
	}

	JumpState->SetJumpCount(0);
}

