#include "PookaState.h"
#include "PookaComponent.h"
#include "SpriteRenderComponent.h"
#include "ServiceLocator.h"
#include "DaeTime.h"
#include "MoveComponent.h"
#include "GameObject.h"
#include "GameConstants.h"

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
		m_timeSinceLastDirectionChange -= DIRECTION_CHANGE_INTERVAL;
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
	pooka->GetSpriteRenderComponent()->Reset();
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

std::unique_ptr<PookaStates::PookaState> InflatedState::Update(PookaComponent* )
{
	return nullptr;
}

void PookaStates::InflatedState::Exit(PookaComponent* pooka)
{
	pooka->GetSpriteRenderComponent()->Play();
	pooka->GetSpriteRenderComponent()->SetSpriteGrid(1, 2);
}

void PookaStates::InflatedState::Enter(PookaComponent* pooka)
{
	auto* spriteRender = pooka->GetSpriteRenderComponent();
	spriteRender->SetTexture("Pooka/Pumped.png");
	spriteRender->SetSpriteGrid(1, 4);
	spriteRender->Pause();
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
	spriteRender->Reset();
	spriteRender->Pause();
	dae::ServiceLocator::GetSoundSystem().PlaySound("Pooka/RockHit.wav", 128, false, -1);
}

std::unique_ptr<PookaStates::PookaState> DyingState::Update(PookaComponent* )
{
	return nullptr;
}

void PookaStates::DyingState::Exit(PookaComponent* )
{
}

void PookaStates::DyingState::Enter(PookaComponent* )
{
}

std::unique_ptr<PookaStates::PookaState> GhostState::Update(PookaComponent* pooka)
{
	// Move in the direction of the target player
	m_ghostingDuration += dae::Time::GetInstance().GetDeltaTime();
	if (m_ghostingDuration >= MIN_GHOSTING_DURATION && !pooka->GetMoveComponent()->IsHittingWall())
	{
		glm::vec3 position = pooka->GetOwner()->GetTransform()->GetLocalPosition();
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
		pooka->GetOwner()->GetTransform()->SetLocalPosition(pooka->GetOwner()->GetTransform()->GetLocalPosition() + direction * (pooka->GetSpeed() / 2) * dae::Time::GetInstance().GetDeltaTime());
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
	spriteRender->Reset();
	spriteRender->SetAngle(0.f);
	spriteRender->SetScale(glm::abs(spriteRender->GetScale()));
	pooka->GetMoveComponent()->SetActive(false);
	m_pTargetPlayer = pooka->GetClosestPlayer();
}
