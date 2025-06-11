#include "PlayerComponent.h"
#include "PlayerState.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "SpriteRenderComponent.h"
#include <iostream>
#include <exception>

PlayerComponent::PlayerComponent(dae::GameObject* owner)
	: dae::BaseComponent(owner), m_pState(std::make_unique<IdleState>())
	, m_pMoveComponent(nullptr)
	, m_pSpriteRenderComponent(nullptr)
	, m_playerNumber(0)
{
	GetOwner()->GetComponent<dae::ColliderComponent>()->AddObserver(this);
	m_pMoveComponent = GetOwner()->GetComponent<MoveComponent>();
	if (!m_pMoveComponent)
	{
		assert(false && "PlayerComponent requires a MoveComponent to function properly.");
	}
	m_pSpriteRenderComponent = GetOwner()->GetComponent<dae::SpriteRenderComponent>();
	if (!m_pSpriteRenderComponent)
	{
		assert(false && "PlayerComponent requires a SpriteRenderComponent to function properly.");
	}
	m_pState->Enter(this);
}

void PlayerComponent::Update()
{
	auto newState = m_pState->Update(this);
	if (newState != nullptr)
	{
		SetState(std::move(newState));
	}
}

void PlayerComponent::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("OnCollision"))
	{
		try
		{
			auto* collider = std::any_cast<const dae::ColliderComponent*>(event.data);
			if (collider && collider->GetTag() == dae::make_sdbm_hash("Rock"))
			{
				std::cout << "Player collided with a rock" << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error getting event data: " << e.what() << std::endl;
			return;
		}
	}
}

dae::SpriteRenderComponent* PlayerComponent::GetSpriteRenderComponent() const
{
	return m_pSpriteRenderComponent;
}

MoveComponent* PlayerComponent::GetMoveComponent() const
{
	return m_pMoveComponent;
}

void PlayerComponent::Attack()
{
	SetState(std::make_unique<AttackingState>());
}

void PlayerComponent::SetState(std::unique_ptr<PlayerState> newState)
{
	m_pState.get()->Exit(this);
	m_pState = std::move(newState);
	m_pState.get()->Enter(this);
}
