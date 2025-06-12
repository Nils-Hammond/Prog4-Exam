#include "GameCommands.h"
#include "GameObject.h"
#include "LivesAndPoints.h"
#include <algorithm>
#include "RenderComponent.h"
#include "DaeTime.h"
#include "ServiceLocator.h"
#include "PlayerComponent.h"

GameActorCommand::GameActorCommand(dae::GameObject* actor) :
	m_actor{ actor }
{
}

MoveCommand::MoveCommand(dae::GameObject* actor, glm::vec3 direction, float speed)
	: GameActorCommand(actor), m_direction(direction), m_speed(speed)
{
}

void MoveCommand::Execute()
{
	GetGameActor()->GetTransform()->SetLocalPosition(GetGameActor()->GetTransform()->GetLocalPosition() + m_direction * m_speed * dae::Time::GetInstance().GetDeltaTime());
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

PlaySoundCommand::PlaySoundCommand(const std::string& soundFile, int volume, bool loop, int channel)
	: m_soundFile(soundFile), m_volume(volume), m_loop(loop), m_channel(channel)
{
}

void PlaySoundCommand::Execute()
{
	dae::ServiceLocator::GetSoundSystem().PlaySound(m_soundFile, m_volume, m_loop, m_channel);
}

AttackCommand::AttackCommand(dae::GameObject* actor) :
	GameActorCommand(actor)
{
}

void AttackCommand::Execute()
{
	PlayerComponent* playerComponent = GetGameActor()->GetComponent<PlayerComponent>();
	if (playerComponent != nullptr)
	{
		playerComponent->Attack();
	}
}
