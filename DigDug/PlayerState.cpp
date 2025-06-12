#include "PlayerState.h"
#include "PlayerComponent.h"
#include "ServiceLocator.h"
#include "SpriteRenderComponent.h"
#include "MoveComponent.h"
#include "PumpComponent.h"
#include "GameObject.h"

std::unique_ptr<PlayerState> IdleState::Update(PlayerComponent* player)
{
	if (player->GetMoveComponent()->IsMoving())
	{
		return std::make_unique<MovingState>();
	}
    return nullptr;
}

void IdleState::Exit(PlayerComponent* player)
{
    player->GetSpriteRenderComponent()->Play();
}

void IdleState::Enter(PlayerComponent* player)
{
    player->GetSpriteRenderComponent()->Pause();
    dae::ServiceLocator::GetSoundSystem().PauseChannel(0);
}

std::unique_ptr<PlayerState> MovingState::Update(PlayerComponent* player)
{
	if (!player->GetMoveComponent()->IsMoving())
	{
		return std::make_unique<IdleState>();
	}
	return nullptr;
}

void MovingState::Exit(PlayerComponent* )
{
}

void MovingState::Enter(PlayerComponent* player)
{
	const std::string folder = "DigDug";
	const std::string filename = "/Walking.png";
	player->GetSpriteRenderComponent()->SetTexture(folder + std::to_string(player->GetPlayerNumber()) + filename);
	player->GetSpriteRenderComponent()->Reset();
	dae::ServiceLocator::GetSoundSystem().ResumeChannel(0);
}

std::unique_ptr<PlayerState> AttackingState::Update(PlayerComponent* )
{
	if (m_pumpComponent == nullptr)
		return std::make_unique<IdleState>();
	if (!m_pumpComponent->IsActive())
		return std::make_unique<IdleState>();
    return nullptr;
}

void AttackingState::Exit(PlayerComponent* player)
{
	player->GetMoveComponent()->SetActive(true);
	dae::ServiceLocator::GetSoundSystem().StopChannel(1);
}

void AttackingState::Enter(PlayerComponent* player)
{
	const std::string folder = "DigDug";
	const std::string filename = "/Pumping.png";
	player->GetSpriteRenderComponent()->SetTexture(folder + std::to_string(player->GetPlayerNumber()) + filename);
	player->GetSpriteRenderComponent()->Pause();
	player->GetMoveComponent()->SetActive(false);

	m_pumpComponent = player->GetOwner()->GetChildren()[0]->GetComponent<PumpComponent>();
	if (m_pumpComponent != nullptr)
	{
		m_pumpComponent->Activate();
		player->GetMoveComponent()->SetActive(false);
		dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/PumpShoot.wav", 128, false, 1);
	}
}

std::unique_ptr<PlayerState> DyingState::Update(PlayerComponent* )
{
    return nullptr;
}

void DyingState::Exit(PlayerComponent* )
{
}

void DyingState::Enter(PlayerComponent* )
{
}

std::unique_ptr<PlayerState> CrushedState::Update(PlayerComponent* )
{
    return nullptr;
}

void CrushedState::Exit(PlayerComponent* )
{
}

void CrushedState::Enter(PlayerComponent* )
{
}

std::unique_ptr<PlayerState> DiggingState::Update(PlayerComponent* )
{
    return nullptr;
}

void DiggingState::Exit(PlayerComponent* )
{
}

void DiggingState::Enter(PlayerComponent* player)
{
    const std::string folder = "DigDug";
	const std::string filename = "/Digging.png";
	player->GetSpriteRenderComponent()->SetTexture(folder + std::to_string(player->GetPlayerNumber()) + filename);
	dae::ServiceLocator::GetSoundSystem().ResumeChannel(0);
}
