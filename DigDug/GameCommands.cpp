#include "GameCommands.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "LivesAndPoints.h"
#include <algorithm>
#include "RenderComponent.h"
#include "DaeTime.h"

GameActorCommand::GameActorCommand(dae::GameObject* actor) :
	m_actor{ actor }
{
}

MoveCommand::MoveCommand(dae::GameObject* actor, glm::vec3 direction)
	: GameActorCommand(actor), m_direction(direction)
{
}

void MoveCommand::Execute()
{
	MoveComponent* moveComponent = GetGameActor()->GetComponent<MoveComponent>();
	if (moveComponent != nullptr)
	{
		moveComponent->SetMoveDirection(m_direction);
	}
}

DieCommand::DieCommand(dae::GameObject* actor) :
	GameActorCommand(actor)
{
}

void DieCommand::Execute()
{
	HealthComponent* healthComponent = GetGameActor()->GetComponent<HealthComponent>();
	if (healthComponent != nullptr)
	{
		healthComponent->TakeDamage(healthComponent->GetHealth());
	}
}