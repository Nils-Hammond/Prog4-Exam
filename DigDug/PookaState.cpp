#include "PookaState.h"
#include "PookaComponent.h"
#include "SpriteRenderComponent.h"
#include "ServiceLocator.h"
#include "DaeTime.h"
#include "MoveComponent.h"
#include "GameObject.h"
#include "GameConstants.h"
#include "ColliderComponent.h"

using namespace PookaStates;

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

std::unique_ptr<PookaStates::PookaState> MovingState::Update(PookaComponent* pooka)
{
	m_timeSinceLastDirectionChange += dae::Time::GetInstance().GetDeltaTime();
	m_ghostCooldownTimer += dae::Time::GetInstance().GetDeltaTime();


	if (m_ghostCooldownTimer >= m_ghostCooldownDuration)
	{
		return std::make_unique<PookaStates::GhostState>();
	}

	if (BreakBounceLoop(pooka)) {}
	else if (pooka->GetMoveComponent()->IsHittingWall())
	{
		++m_bouncesSinceLastDirectionChange;
		m_desiredDirection = -m_desiredDirection;
		m_moveCommand = std::make_unique<MoveCharacterCommand>(pooka->GetOwner(), m_desiredDirection, pooka->GetSpeed());
	}
	else if (m_timeSinceLastDirectionChange >= DIRECTION_CHANGE_INTERVAL)
	{
		m_desiredDirection = GetRandomGridDirection();
		m_moveCommand = std::make_unique<MoveCharacterCommand>(pooka->GetOwner(), m_desiredDirection, pooka->GetSpeed());
		m_timeSinceLastDirectionChange = 0.f;
	}
	if (m_moveCommand)
		m_moveCommand->Execute();
	return nullptr;
}

void MovingState::Exit(PookaComponent* )
{
	m_moveCommand.reset();
}

void MovingState::Enter(PookaComponent* pooka)
{
	static std::random_device rd;
	m_randomNumGen.seed(rd());

	std::uniform_real_distribution<float> dist(MIN_GHOST_COOLDOWN_DURATION, MAX_GHOST_COOLDOWN_DURATION);
	m_ghostCooldownDuration = dist(m_randomNumGen);

	pooka->GetSpriteRenderComponent()->SetTexture("Pooka/Default.png");
	pooka->GetSpriteRenderComponent()->SetSpriteGrid(1, 2);
	pooka->GetSpriteRenderComponent()->Reset();
	pooka->GetSpriteRenderComponent()->Play();
	pooka->GetSpriteRenderComponent()->SetOffset(0, 0);
	m_desiredDirection = GetRandomGridDirection();
	m_moveCommand = std::make_unique<MoveCharacterCommand>(pooka->GetOwner(), m_desiredDirection, pooka->GetSpeed());
}

bool PookaStates::MovingState::BreakBounceLoop(PookaComponent* pooka)
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
		m_moveCommand = std::make_unique<MoveCharacterCommand>(pooka->GetOwner(), m_desiredDirection, pooka->GetSpeed());
		return true;
	}
	return false;
}

std::unique_ptr<PookaStates::PookaState> InflatedState::Update(PookaComponent* pooka)
{
	m_deflationTimer += dae::Time::GetInstance().GetDeltaTime();
	if (pooka->IsInflating())
	{
		Inflate(pooka);
		m_deflationTimer = 0.f;
	}
	else if (m_deflationTimer >= DEFLATION_COOLDOWN)
	{
		Deflate(pooka);
		m_deflationTimer = 0.f;
	}
	if (m_inflationProgress < 0)
	{
		return std::make_unique<PookaStates::MovingState>();
	}
	else if (m_inflationProgress >= MAX_INFLATION_PROGRESS)
	{
		return std::make_unique<PookaStates::DyingState>();
	}
	return nullptr;
}

void PookaStates::InflatedState::Exit(PookaComponent* pooka)
{
	pooka->GetOwner()->GetComponent<dae::ColliderComponent>()->ChangeTag(dae::make_sdbm_hash("Enemy"));
}

void PookaStates::InflatedState::Enter(PookaComponent* pooka)
{
	auto* spriteRender = pooka->GetSpriteRenderComponent();
	spriteRender->SetTexture("Pooka/Pumped.png");
	spriteRender->SetSpriteGrid(1, 4);
	spriteRender->Pause();
	spriteRender->SetOffset(-GRID_SIZE / 2, -GRID_SIZE / 2);
	pooka->GetOwner()->GetComponent<dae::ColliderComponent>()->ChangeTag(dae::make_sdbm_hash("Balloon"));
}

void PookaStates::InflatedState::Inflate(PookaComponent* pooka)
{
	++m_inflationProgress;
	pooka->GetSpriteRenderComponent()->NextFrame();
}

void PookaStates::InflatedState::Deflate(PookaComponent* pooka)
{
	--m_inflationProgress;
	pooka->GetSpriteRenderComponent()->PreviousFrame();
}

std::unique_ptr<PookaStates::PookaState> CrushedState::Update(PookaComponent* )
{
	return nullptr;
}

void PookaStates::CrushedState::Exit(PookaComponent* )
{
}

void PookaStates::CrushedState::Enter(PookaComponent* pooka)
{
	auto* spriteRender = pooka->GetSpriteRenderComponent();
	spriteRender->SetTexture("Pooka/CrushedByRock.png");
	spriteRender->SetSpriteGrid(1, 2);
	spriteRender->Reset();
	spriteRender->Pause();
	pooka->GetSpriteRenderComponent()->SetOffset(0, 0);
	dae::ServiceLocator::GetSoundSystem().PlaySound("Pooka/RockHit.wav", 128, false, -1);
	pooka->Kill();
}

std::unique_ptr<PookaStates::PookaState> DyingState::Update(PookaComponent* pooka)
{
	m_deathTimer += dae::Time::GetInstance().GetDeltaTime();
	if (m_deathTimer >= DEATH_DURATION)
	{
		pooka->GetOwner()->Destroy();
		return nullptr;
	}
	return nullptr;
}

void PookaStates::DyingState::Exit(PookaComponent* )
{
}

void PookaStates::DyingState::Enter(PookaComponent* pooka)
{
	pooka->GetOwner()->GetComponent<dae::ColliderComponent>()->SetActive(false);
	dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/EnemyDeath.wav", 128, false, -1);
	m_deathTimer = 0.f;
	pooka->Kill();
}

std::unique_ptr<PookaStates::PookaState> GhostState::Update(PookaComponent* pooka)
{
	// Move in the direction of the target player
	m_ghostingDuration += dae::Time::GetInstance().GetDeltaTime();
	if (m_ghostingDuration >= MIN_GHOSTING_DURATION && !pooka->GetMoveComponent()->IsHittingWall())
	{
		glm::vec3 position = pooka->GetOwner()->GetTransform()->GetWorldPosition();
		position.x = std::round(position.x / GRID_SIZE) * GRID_SIZE;
		position.y = std::round(position.y / GRID_SIZE) * GRID_SIZE;
		pooka->GetOwner()->GetTransform()->SetLocalPosition(position);
		return std::make_unique<PookaStates::MovingState>();
	}
	else if (m_pTargetPlayer)
	{
		const glm::vec3& targetPos = m_pTargetPlayer->GetTransform()->GetWorldPosition();
		const glm::vec3& pookaPos = pooka->GetOwner()->GetTransform()->GetWorldPosition();
		const glm::vec3& direction = glm::normalize(targetPos - pookaPos);
		pooka->GetOwner()->GetTransform()->SetLocalPosition(pooka->GetOwner()->GetTransform()->GetLocalPosition() + direction * ENEMY_GHOST_SPEED * dae::Time::GetInstance().GetDeltaTime());
	}
	else
	{
		m_pTargetPlayer = pooka->GetClosestPlayer();
	}
	return nullptr;
}

void PookaStates::GhostState::Exit(PookaComponent* pooka)
{
	pooka->GetMoveComponent()->SetActive(true);
}

void PookaStates::GhostState::Enter(PookaComponent* pooka)
{
	auto* spriteRender = pooka->GetSpriteRenderComponent();
	spriteRender->SetTexture("Pooka/Ghost.png");
	spriteRender->SetSpriteGrid(1, 2);
	spriteRender->Reset();
	spriteRender->SetAngle(0.f);
	spriteRender->SetScale(glm::abs(spriteRender->GetScale()));
	spriteRender->Play();
	pooka->GetMoveComponent()->SetActive(false);
	m_pTargetPlayer = pooka->GetClosestPlayer();
}
