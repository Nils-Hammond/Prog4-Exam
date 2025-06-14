#include "EnemyComponent.h"
#include "GameObject.h"
#include "ColliderComponent.h"
#include "MoveComponent.h"
#include "SpriteRenderComponent.h"

EnemyComponent::EnemyComponent(dae::GameObject* owner, float speed) :
	dae::BaseComponent(owner)
	, m_pMoveComponent(nullptr)
	, m_pSpriteRenderComponent(nullptr)
	, m_speed(speed)
	, m_isInflating(false)
	, m_isDead(false)
	, m_pDeathSubject(std::make_unique<dae::Subject>())
	, m_initialPosition()
{
	auto colliderComponent = GetOwner()->GetComponent<dae::ColliderComponent>();
	if (colliderComponent) colliderComponent->AddObserver(this);
	m_pMoveComponent = GetOwner()->GetComponent<MoveComponent>();
	if (!m_pMoveComponent)
	{
		assert(false && "EnemyComponent requires a MoveComponent to function properly.");
	}
	m_pSpriteRenderComponent = GetOwner()->GetComponent<dae::SpriteRenderComponent>();
	if (!m_pSpriteRenderComponent)
	{
		assert(false && "EnemyComponent requires a SpriteRenderComponent to function properly.");
	}
	m_initialPosition = GetOwner()->GetTransform()->GetLocalPosition();
}

dae::SpriteRenderComponent* EnemyComponent::GetSpriteRenderComponent() const
{
	return m_pSpriteRenderComponent;
}

MoveComponent* EnemyComponent::GetMoveComponent() const
{
	return m_pMoveComponent;
}

const dae::GameObject* EnemyComponent::GetClosestPlayer() const
{
	const dae::GameObject* closestPlayer = nullptr;
	float closestDistance = std::numeric_limits<float>::max(); // Large value so that first closest found is always closer
	for (const auto& player : m_pPlayers)
	{
		const auto& playerPos = player->GetTransform()->GetWorldPosition();
		const auto& currentPos = GetOwner()->GetTransform()->GetWorldPosition();
		float distance = glm::distance(playerPos, currentPos);
		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestPlayer = player;
		}
	}
	return closestPlayer;
}

void EnemyComponent::SetPlayers(const std::vector<const dae::GameObject*>& players)
{
	m_pPlayers = players;
}

void EnemyComponent::RegisterObserver(dae::Observer* observer)
{
	m_pDeathSubject->AddObserver(observer);
}

void EnemyComponent::UnregisterObserver(dae::Observer* observer)
{
	m_pDeathSubject->RemoveObserver(observer);
}
