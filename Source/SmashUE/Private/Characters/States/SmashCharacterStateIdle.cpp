// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateIdle.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "Characters/SmashCharacterSettings.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateIdle::GetStateID()
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
}

void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if(Character->GetInputJump() && !Character->GetCharacterMovement()->IsFalling())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
	}

	else if(Character->GetInputBasicAttack())
	{
		float XValue = FMath::Abs(Character->GetInputMoveX());
		float YValue = FMath::Abs(Character->GetInputMoveY());

		bool XAttack = XValue > YValue && XValue > CharacterSettings->InputMoveXThreshold;
		bool YAttack = YValue > XValue && YValue > CharacterSettings->InputMoveYThreshold;

		ESmashCharacterStateID Attack = ESmashCharacterStateID::None;

		if(XAttack)
		{
			Attack = Character->GetInputMoveX() < 0.0f ? ESmashCharacterStateID::LeftBasicAttack : ESmashCharacterStateID::RightBasicAttack;
		}
		else if(YAttack)
		{
			Attack = Character->GetInputMoveY() < 0.0f ? ESmashCharacterStateID::DownBasicAttack : ESmashCharacterStateID::UpBasicAttack;
		}
		else
		{
			Attack = ESmashCharacterStateID::NeutralBasicAttack;
		}

		StateMachine->ChangeState(Attack);
	}

	else if(Character->GetInputSpecialAttack())
	{
		float XValue = FMath::Abs(Character->GetInputMoveX());
		float YValue = FMath::Abs(Character->GetInputMoveY());

		bool XAttack = XValue > YValue && XValue > CharacterSettings->InputMoveXThreshold;
		bool YAttack = YValue > XValue && YValue > CharacterSettings->InputMoveYThreshold;

		ESmashCharacterStateID Attack = ESmashCharacterStateID::None;

		if(XAttack)
		{
			Attack = Character->GetInputMoveX() < 0.0f ? ESmashCharacterStateID::LeftSpecialAttack : ESmashCharacterStateID::RightSpecialAttack;
		}
		else if(YAttack)
		{
			Attack = Character->GetInputMoveY() < 0.0f ? ESmashCharacterStateID::DownSpecialAttack : ESmashCharacterStateID::UpSpecialAttack;
		}
		else
		{
			Attack = ESmashCharacterStateID::NeutralSpecialAttack;
		}

		StateMachine->ChangeState(Attack);
	}

	else if(FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
}

void USmashCharacterStateIdle::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}
