#include "PlayerState.h"
#include "PlayerComponent.h"
#include "ServiceLocator.h"
#include "SpriteRenderComponent.h"
#include "MoveComponent.h"
#include "PumpComponent.h"
#include "GameObject.h"
#include "DaeTime.h"
#include "PookaComponent.h"
#include "ColliderComponent.h"
#include "GameCommands.h"

using namespace PlayerStates;

std::unique_ptr<PlayerStates::PlayerState> IdleState::Update(PlayerComponent* player)
{
	if (player->GetMoveComponent()->IsMoving())
	{
		return std::make_unique<PlayerStates::MovingState>();
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
    dae::ServiceLocator::GetSoundSystem().PauseChannel(MUSIC_CHANNEL);
}

std::unique_ptr<PlayerStates::PlayerState> MovingState::Update(PlayerComponent* player)
{
	if (!player->GetMoveComponent()->IsMoving())
	{
		return std::make_unique<PlayerStates::IdleState>();
	}
	else if (player->GetMoveComponent()->IsHittingWall())
	{
		return std::make_unique<PlayerStates::DiggingState>();
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
	if (dae::ServiceLocator::GetSoundSystem().CheckChannel(MUSIC_CHANNEL))
		dae::ServiceLocator::GetSoundSystem().ResumeChannel(MUSIC_CHANNEL);
	else
		dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/walkmusic.wav", 128, true, MUSIC_CHANNEL);
}

std::unique_ptr<PlayerStates::PlayerState> AttackingState::Update(PlayerComponent* player)
{
	m_attackTimer += dae::Time::GetInstance().GetDeltaTime();
	if (m_attackTimer >= ATTACK_COOLDOWN / 2 && m_pumpConnected)
	{
		player->GetSpriteRenderComponent()->Reset(); // Lil pumping animation
	}
	// Going back to Idle
	if (m_pumpComponent == nullptr || !m_pumpComponent->IsActive())
	{
		return std::make_unique<PlayerStates::IdleState>();
	}
	else if (m_pumpConnected && m_pEnemy && m_pEnemy->IsDead())
	{
		m_pumpComponent->Deactivate();
		return std::make_unique<PlayerStates::IdleState>();
	}

	// Attacking an enemy
	if (!m_pumpConnected && (m_pEnemyCollider = m_pumpComponent->GetHitEnemy()) != nullptr)
	{
		player->GetMoveComponent()->SetActive(true);
		m_pumpConnected = true;
		m_attackTimer = 0.f;
		dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/PumpToEnemy.wav", 128, false, -1);
		player->GetSpriteRenderComponent()->SetTexture("DigDug" + std::to_string(player->GetPlayerNumber()) + "/Pumping.png");
	}
	else if (m_attackTimer >= ATTACK_COOLDOWN && m_pumpConnected && m_pEnemyCollider && player->IsAttacking())
	{
		m_pEnemy = m_pEnemyCollider->GetOwner()->GetComponent<EnemyComponent>();
		if (m_pEnemy) m_pEnemy->Inflate();
		m_attackTimer = 0.f;
		player->GetSpriteRenderComponent()->NextFrame();
		dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/PumpToEnemy.wav", 128, false, -1);
	}
	if (player->GetMoveComponent()->IsMoving())
	{
		m_pumpComponent->Deactivate();
		return std::make_unique<PlayerStates::MovingState>();
	}
    return nullptr;
}

void AttackingState::Exit(PlayerComponent* player)
{
	player->GetMoveComponent()->SetActive(true);
	player->GetSpriteRenderComponent()->Play();
	dae::ServiceLocator::GetSoundSystem().StopChannel(PUMP_SOUND_CHANNEL);
}

void AttackingState::Enter(PlayerComponent* player)
{
	const std::string folder = "DigDug";
	const std::string filename = "/Walking.png";
	player->GetSpriteRenderComponent()->SetTexture(folder + std::to_string(player->GetPlayerNumber()) + filename);
	player->GetSpriteRenderComponent()->Reset();
	player->GetSpriteRenderComponent()->Pause();
	player->GetMoveComponent()->SetActive(false);

	m_pumpComponent = player->GetOwner()->GetChildren()[0]->GetComponent<PumpComponent>();
	if (m_pumpComponent != nullptr)
	{
		m_pumpComponent->Activate();
		player->GetMoveComponent()->SetActive(false);
		dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/PumpShoot.wav", 128, true, PUMP_SOUND_CHANNEL);
	}
	dae::ServiceLocator::GetSoundSystem().PauseChannel(MUSIC_CHANNEL);
}

std::unique_ptr<PlayerStates::PlayerState> DyingState::Update(PlayerComponent* player)
{
	m_deathTimer += dae::Time::GetInstance().GetDeltaTime();
	if (!m_isFullyDead && m_deathTimer >= DEATH_DURATION)
	{
		m_isFullyDead = true;
		player->GetOwner()->SetRenderLayer(-1);
	}
    return nullptr;
}

void DyingState::Exit(PlayerComponent* player)
{
	const std::string folder = "DigDug";
	const std::string filename = "/Walking.png";
	auto* spriteRender = player->GetSpriteRenderComponent();
	spriteRender->SetTexture(folder + std::to_string(player->GetPlayerNumber()) + filename);
	spriteRender->Reset();
	spriteRender->SetSpriteGrid(1, 2);
	spriteRender->SetLooping(true);
	player->GetMoveComponent()->SetActive(true);
}

void DyingState::Enter(PlayerComponent* player)
{
	const std::string folder = "DigDug";
	const std::string filename = "/Death.png";
	auto* spriteRender = player->GetSpriteRenderComponent();
	spriteRender->SetTexture(folder + std::to_string(player->GetPlayerNumber()) + filename);
	spriteRender->Reset();
	spriteRender->SetSpriteGrid(1, 4);
	spriteRender->SetLooping(false);
	player->GetMoveComponent()->SetActive(false);
	auto dieCommand = std::make_unique<DieCommand>(player->GetOwner());
	dieCommand->Execute();
	dae::ServiceLocator::GetSoundSystem().PauseChannel(MUSIC_CHANNEL);
}

std::unique_ptr<PlayerStates::PlayerState> CrushedState::Update(PlayerComponent* )
{
    return nullptr;
}

void CrushedState::Exit(PlayerComponent* )
{
}

void CrushedState::Enter(PlayerComponent* player)
{
	const std::string folder = "DigDug";
	const std::string filename = "/CrushedByRock.png";
	player->GetSpriteRenderComponent()->SetTexture(folder + std::to_string(player->GetPlayerNumber()) + filename);
	player->GetSpriteRenderComponent()->Reset();
	dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/RockHit.wav", 128, false, -1);
}

std::unique_ptr<PlayerStates::PlayerState> DiggingState::Update(PlayerComponent* player)
{
	shovelTimer += dae::Time::GetInstance().GetDeltaTime();
	if (!player->GetMoveComponent()->IsMoving())
	{
		return std::make_unique<PlayerStates::IdleState>();
	}
	else if (shovelTimer >= MAX_SHOVEL_DURATION)
	{
		return std::make_unique<PlayerStates::MovingState>();
	}
	else if (player->GetMoveComponent()->IsHittingWall())
	{
		shovelTimer = 0.f;
	}
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
	if (dae::ServiceLocator::GetSoundSystem().CheckChannel(MUSIC_CHANNEL))
		dae::ServiceLocator::GetSoundSystem().ResumeChannel(MUSIC_CHANNEL);
	else
		dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/walkmusic.wav", 128, true, MUSIC_CHANNEL);
}
