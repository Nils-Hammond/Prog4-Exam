#include "PlayerComponent.h"
#include "PlayerState.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "SpriteRenderComponent.h"
#include <iostream>
#include <exception>

PlayerComponent::PlayerComponent(dae::GameObject* owner, int playerNumber)
	: dae::BaseComponent(owner), m_pState(std::make_unique<PlayerStates::IdleState>())
	, m_pMoveComponent(nullptr)
	, m_pSpriteRenderComponent(nullptr)
	, m_playerNumber(playerNumber)
	, m_isAttacking(false)
	, m_isDead(false)
	, m_initialPosition()
{
	m_initialPosition = GetOwner()->GetTransform()->GetWorldPosition();
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

PlayerComponent::~PlayerComponent() = default;

void PlayerComponent::Update()
{
	auto newState = m_pState->Update(this);
	if (newState != nullptr)
	{
		SetState(std::move(newState));
	}
	m_isAttacking = false;
}

void PlayerComponent::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("OnCollision"))
	{
		try
		{
			auto* collider = std::any_cast<const dae::ColliderComponent*>(event.data);
			if (collider && collider->GetTag() == dae::make_sdbm_hash("Enemy"))
			{
				if (!m_isDead)
				{
					m_isDead = true;
					SetState(std::make_unique<PlayerStates::DyingState>());
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
	if (!m_isDead && dynamic_cast<PlayerStates::AttackingState*>(m_pState.get()) == nullptr)
	{
		SetState(std::make_unique<PlayerStates::AttackingState>());
	}
	else if (!m_isDead)
	{
		m_isAttacking = true;
	}
}

bool PlayerComponent::IsDead() const
{
	return m_isDead;
}

void PlayerComponent::Reset()
{
	GetOwner()->GetTransform()->SetLocalPosition(m_initialPosition);
	GetOwner()->SetRenderLayer(3);
	SetState(std::make_unique<PlayerStates::IdleState>());
	m_isDead = false;
}

void PlayerComponent::UpdateRespawn()
{
	m_initialPosition = GetOwner()->GetTransform()->GetWorldPosition();
}

void PlayerComponent::SetState(std::unique_ptr<PlayerStates::PlayerState> newState)
{
	m_pState.get()->Exit(this);
	m_pState = std::move(newState);
	m_pState.get()->Enter(this);
}
