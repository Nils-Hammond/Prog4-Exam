#include "PookaComponent.h"
#include "ColliderComponent.h"
#include "MoveComponent.h"
#include "PookaState.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"

PookaComponent::PookaComponent(dae::GameObject* owner, float speed) :
	dae::BaseComponent(owner), m_pState(std::make_unique<PookaStates::MovingState>()),
	m_pMoveComponent(nullptr), m_pSpriteRenderComponent(nullptr), m_speed(speed), m_hitByPump(false)
{
	GetOwner()->GetComponent<dae::ColliderComponent>()->AddObserver(this);
	m_pMoveComponent = GetOwner()->GetComponent<MoveComponent>();
	if (!m_pMoveComponent)
	{
		assert(false && "PookaComponent requires a MoveComponent to function properly.");
	}
	m_pSpriteRenderComponent = GetOwner()->GetComponent<dae::SpriteRenderComponent>();
	if (!m_pSpriteRenderComponent)
	{
		assert(false && "PookaComponent requires a SpriteRenderComponent to function properly.");
	}
	m_pState->Enter(this);
}

void PookaComponent::Update()
{
	auto newState = m_pState->Update(this);
	if (newState != nullptr)
	{
		SetState(std::move(newState));
	}
}

dae::SpriteRenderComponent* PookaComponent::GetSpriteRenderComponent() const
{
	return m_pSpriteRenderComponent;
}

MoveComponent* PookaComponent::GetMoveComponent() const
{
	return m_pMoveComponent;
}

void PookaComponent::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("OnCollision"))
	{
		try
		{
			auto* collider = std::any_cast<const dae::ColliderComponent*>(event.data);
			if (collider && collider->GetTag() == dae::make_sdbm_hash("Pump"))
			{
				std::cout << "Pooka collided with a pump" << std::endl;
				if (!m_hitByPump)
				{
					m_hitByPump = true;
					SetState(std::make_unique<PookaStates::InflatedState>());
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error getting event data: " << e.what() << std::endl;
			return;
		}
	}
}

const dae::GameObject* PookaComponent::GetClosestPlayer() const
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

void PookaComponent::SetPlayers(const std::vector<const dae::GameObject*>& players)
{
	m_pPlayers = players;
}

void PookaComponent::SetState(std::unique_ptr<PookaStates::PookaState> newState)
{
	m_pState->Exit(this);
	m_pState = std::move(newState);
	m_pState->Enter(this);
}
