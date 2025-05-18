#include "GameObject.h"
#include "Command.h"
//#include "MoveComponent.h"
//#include "LivesAndPoints.h"
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
	//MoveComponent* moveComponent = GetGameActor()->GetComponent<MoveComponent>();
	//if (moveComponent != nullptr)
	//{
	//	moveComponent->SetMoveDirection(m_direction);
	//}
}

dae::DieCommand::DieCommand(GameObject* actor) :
	GameActorCommand(actor)
{
}

void dae::DieCommand::Execute()
{
	//HealthComponent* healthComponent = GetGameActor()->GetComponent<HealthComponent>();
	//if (healthComponent != nullptr)
	//{
	//	healthComponent->TakeDamage(healthComponent->GetHealth());
	//}
}

dae::TriggerEventCommand::TriggerEventCommand(Event event) :
	m_event{ event }
{
}

void dae::TriggerEventCommand::Execute()
{
	Subject::Notify(m_event);
}
