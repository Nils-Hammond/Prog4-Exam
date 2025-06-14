#include "FygarState.h"
#include "FygarComponent.h"
#include "SpriteRenderComponent.h"
#include "ServiceLocator.h"
#include "DaeTime.h"
#include "MoveComponent.h"
#include "GameObject.h"
#include "GameConstants.h"
#include "ColliderComponent.h"

using namespace FygarStates;

glm::vec3 MovingState::GetRandomGridDirection()
{
	std::uniform_int_distribution<int> axisDist(0, 1);
	std::uniform_int_distribution<int> directionDist(0, 1);

	int axis = axisDist(m_randomNumGen);
	int direction = directionDist(m_randomNumGen) == 0 ? -1 : 1;
	if (axis == 0)
	{
		return glm::vec3(direction, 0.f, 0.f);
	}
	else
	{
		return glm::vec3(0.f, direction, 0.f);
	}
}

std::unique_ptr<FygarStates::FygarState> MovingState::Update(FygarComponent* fygar)
{
	m_timeSinceLastDirectionChange += dae::Time::GetInstance().GetDeltaTime();
	m_ghostCooldownTimer += dae::Time::GetInstance().GetDeltaTime();


	if (m_ghostCooldownTimer >= m_ghostCooldownDuration)
	{
		return std::make_unique<FygarStates::GhostState>();
	}

	if (BreakBounceLoop(fygar)) {}
	else if (fygar->GetMoveComponent()->IsHittingWall())
	{
		++m_bouncesSinceLastDirectionChange;
		m_desiredDirection = -m_desiredDirection;
		m_moveCommand = std::make_unique<MoveCharacterCommand>(fygar->GetOwner(), m_desiredDirection, fygar->GetSpeed());
	}
	else if (m_timeSinceLastDirectionChange >= DIRECTION_CHANGE_INTERVAL)
	{
		m_desiredDirection = GetRandomGridDirection();
		m_moveCommand = std::make_unique<MoveCharacterCommand>(fygar->GetOwner(), m_desiredDirection, fygar->GetSpeed());
		m_timeSinceLastDirectionChange = 0.f;
	}
	if (m_moveCommand)
		m_moveCommand->Execute();
	return nullptr;
}

void MovingState::Exit(FygarComponent*)
{
	m_moveCommand.reset();
}

void MovingState::Enter(FygarComponent* fygar)
{
	static std::random_device rd;
	m_randomNumGen.seed(rd());

	std::uniform_real_distribution<float> dist(MIN_GHOST_COOLDOWN_DURATION, MAX_GHOST_COOLDOWN_DURATION);
	m_ghostCooldownDuration = dist(m_randomNumGen);

	fygar->GetSpriteRenderComponent()->SetTexture("Fygar/Default.png");
	fygar->GetSpriteRenderComponent()->SetSpriteGrid(1, 2);
	fygar->GetSpriteRenderComponent()->Reset();
	fygar->GetSpriteRenderComponent()->Play();
	fygar->GetSpriteRenderComponent()->SetOffset(0, 0);
	m_desiredDirection = GetRandomGridDirection();
	m_moveCommand = std::make_unique<MoveCharacterCommand>(fygar->GetOwner(), m_desiredDirection, fygar->GetSpeed());
}

bool FygarStates::MovingState::BreakBounceLoop(FygarComponent* fygar)
{
	if (m_bouncesSinceLastDirectionChange > 2)
	{
		if (m_desiredDirection.x != 0.f)
		{
			m_desiredDirection = glm::vec3(0.f, m_desiredDirection.x, 0.f);
		}
		else if (m_desiredDirection.y != 0.f)
		{
			m_desiredDirection = glm::vec3(m_desiredDirection.y, 0.f, 0.f);
		}
		m_bouncesSinceLastDirectionChange = 0;
		m_timeSinceLastDirectionChange = 0.f;
		m_ghostCooldownTimer += DIRECTION_CHANGE_INTERVAL; // Speed up ghosting if it starts getting stuck
		m_moveCommand = std::make_unique<MoveCharacterCommand>(fygar->GetOwner(), m_desiredDirection, fygar->GetSpeed());
		return true;
	}
	return false;
}

std::unique_ptr<FygarStates::FygarState> InflatedState::Update(FygarComponent* fygar)
{
	m_deflationTimer += dae::Time::GetInstance().GetDeltaTime();
	if (fygar->IsInflating())
	{
		Inflate(fygar);
		m_deflationTimer = 0.f;
	}
	else if (m_deflationTimer >= DEFLATION_COOLDOWN)
	{
		Deflate(fygar);
		m_deflationTimer = 0.f;
	}
	if (m_inflationProgress < 0)
	{
		return std::make_unique<FygarStates::MovingState>();
	}
	else if (m_inflationProgress >= MAX_INFLATION_PROGRESS)
	{
		return std::make_unique<FygarStates::DyingState>();
	}
	return nullptr;
}

void FygarStates::InflatedState::Exit(FygarComponent* fygar)
{
	fygar->GetOwner()->GetComponent<dae::ColliderComponent>()->ChangeTag(dae::make_sdbm_hash("Enemy"));
}

void FygarStates::InflatedState::Enter(FygarComponent* fygar)
{
	auto* spriteRender = fygar->GetSpriteRenderComponent();
	spriteRender->SetTexture("Fygar/Pumped.png");
	spriteRender->SetSpriteGrid(1, 4);
	spriteRender->Pause();
	spriteRender->SetOffset(-GRID_SIZE / 2, -GRID_SIZE / 2);
	fygar->GetOwner()->GetComponent<dae::ColliderComponent>()->ChangeTag(dae::make_sdbm_hash("Balloon"));
}

void FygarStates::InflatedState::Inflate(FygarComponent* fygar)
{
	++m_inflationProgress;
	fygar->GetSpriteRenderComponent()->NextFrame();
}

void FygarStates::InflatedState::Deflate(FygarComponent* fygar)
{
	--m_inflationProgress;
	fygar->GetSpriteRenderComponent()->PreviousFrame();
}

std::unique_ptr<FygarStates::FygarState> CrushedState::Update(FygarComponent*)
{
	return nullptr;
}

void FygarStates::CrushedState::Exit(FygarComponent*)
{
}

void FygarStates::CrushedState::Enter(FygarComponent* fygar)
{
	auto* spriteRender = fygar->GetSpriteRenderComponent();
	spriteRender->SetTexture("Fygar/CrushedByRock.png");
	spriteRender->SetSpriteGrid(1, 2);
	spriteRender->Reset();
	spriteRender->Pause();
	fygar->GetSpriteRenderComponent()->SetOffset(0, 0);
	dae::ServiceLocator::GetSoundSystem().PlaySound("Fygar/RockHit.wav", 128, false, -1);
	fygar->Kill();
}

std::unique_ptr<FygarStates::FygarState> DyingState::Update(FygarComponent* fygar)
{
	m_deathTimer += dae::Time::GetInstance().GetDeltaTime();
	if (m_deathTimer >= DEATH_DURATION)
	{
		fygar->GetOwner()->Destroy();
		return nullptr;
	}
	return nullptr;
}

void FygarStates::DyingState::Exit(FygarComponent*)
{
}

void FygarStates::DyingState::Enter(FygarComponent* fygar)
{
	fygar->GetOwner()->GetComponent<dae::ColliderComponent>()->SetActive(false);
	dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/EnemyDeath.wav", 128, false, -1);
	m_deathTimer = 0.f;
	fygar->Kill();
}

std::unique_ptr<FygarStates::FygarState> GhostState::Update(FygarComponent* fygar)
{
	// Move in the direction of the target player
	m_ghostingDuration += dae::Time::GetInstance().GetDeltaTime();
	if (m_ghostingDuration >= MIN_GHOSTING_DURATION && !fygar->GetMoveComponent()->IsHittingWall())
	{
		glm::vec3 position = fygar->GetOwner()->GetTransform()->GetWorldPosition();
		position.x = std::round(position.x / GRID_SIZE) * GRID_SIZE;
		position.y = std::round(position.y / GRID_SIZE) * GRID_SIZE;
		fygar->GetOwner()->GetTransform()->SetLocalPosition(position);
		return std::make_unique<FygarStates::MovingState>();
	}
	else if (m_pTargetPlayer)
	{
		const glm::vec3& targetPos = m_pTargetPlayer->GetTransform()->GetWorldPosition();
		const glm::vec3& FygarPos = fygar->GetOwner()->GetTransform()->GetWorldPosition();
		const glm::vec3& direction = glm::normalize(targetPos - FygarPos);
		fygar->GetOwner()->GetTransform()->SetLocalPosition(fygar->GetOwner()->GetTransform()->GetLocalPosition() + direction * ENEMY_GHOST_SPEED * dae::Time::GetInstance().GetDeltaTime());
	}
	else
	{
		m_pTargetPlayer = fygar->GetClosestPlayer();
	}
	return nullptr;
}

void FygarStates::GhostState::Exit(FygarComponent* fygar)
{
	fygar->GetMoveComponent()->SetActive(true);
}

void FygarStates::GhostState::Enter(FygarComponent* fygar)
{
	auto* spriteRender = fygar->GetSpriteRenderComponent();
	spriteRender->SetTexture("Fygar/Ghost.png");
	spriteRender->SetSpriteGrid(1, 2);
	spriteRender->Reset();
	spriteRender->SetAngle(0.f);
	spriteRender->SetScale(glm::abs(spriteRender->GetScale()));
	spriteRender->Play();
	fygar->GetMoveComponent()->SetActive(false);
	m_pTargetPlayer = fygar->GetClosestPlayer();
}

std::unique_ptr<FygarState> FygarStates::IdleState::Update(FygarComponent* )
{
	return nullptr;
}

void FygarStates::IdleState::Exit(FygarComponent* )
{
}

void FygarStates::IdleState::Enter(FygarComponent* )
{
}

std::unique_ptr<FygarState> FygarStates::AttackingState::Update(FygarComponent* )
{
	return nullptr;
}

void FygarStates::AttackingState::Exit(FygarComponent* )
{
}

void FygarStates::AttackingState::Enter(FygarComponent* )
{
}
