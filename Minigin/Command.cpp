#include "GameObject.h"
#include "Command.h"
#include "MoveComponent.h"
#include <iostream>

dae::GameActorCommand::GameActorCommand(GameObject* actor) :
	m_actor{actor}
{
}

dae::TestCommand::TestCommand(const std::string& testText)
	: m_testText{testText}
{
}

void dae::TestCommand::Execute()
{
	std::cout << m_testText << std::endl;
}

dae::MoveCommand::MoveCommand(GameObject* actor, glm::vec3 direction)
	: GameActorCommand(actor), m_direction(direction)
{
}

void dae::MoveCommand::Execute()
{
	MoveComponent* moveComponent = GetGameActor()->GetComponent<MoveComponent>();
	if (moveComponent != nullptr)
	{
		moveComponent->SetMoveDirection(m_direction);
	}
}