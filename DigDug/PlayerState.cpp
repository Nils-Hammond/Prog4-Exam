#include "PlayerState.h"

PlayerState* IdleState::Update(PlayerComponent* )
{
    return nullptr;
}

void IdleState::Exit(PlayerComponent* )
{
}

void IdleState::Enter(PlayerComponent* )
{
}

PlayerState* MovingState::Update(PlayerComponent* )
{
    return nullptr;
}

void MovingState::Exit(PlayerComponent* )
{
}

void MovingState::Enter(PlayerComponent* )
{
}

PlayerState* AttackingState::Update(PlayerComponent* )
{
    return nullptr;
}

void AttackingState::Exit(PlayerComponent* )
{
}

void AttackingState::Enter(PlayerComponent* )
{
}

PlayerState* DyingState::Update(PlayerComponent* )
{
    return nullptr;
}

void DyingState::Exit(PlayerComponent* )
{
}

void DyingState::Enter(PlayerComponent* )
{
}

PlayerState* CrushedState::Update(PlayerComponent* )
{
    return nullptr;
}

void CrushedState::Exit(PlayerComponent* )
{
}

void CrushedState::Enter(PlayerComponent* )
{
}
