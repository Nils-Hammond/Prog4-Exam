#include "PookaComponent.h"
#include "ColliderComponent.h"
#include "MoveComponent.h"
#include "PookaState.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"

PookaComponent::PookaComponent(dae::GameObject* owner, float speed) :
	EnemyComponent(owner, speed)
	, m_pState(std::make_unique<PookaStates::MovingState>())
{
	m_pState->Enter(this);
}

void PookaComponent::Update()
{
	auto newState = m_pState->Update(this);
	if (newState != nullptr)
	{
		SetState(std::move(newState));
	}
	m_isInflating = false;
}

void PookaComponent::Kill()
{
	m_isDead = true;
	m_pDeathSubject->Notify(dae::Event{ dae::make_sdbm_hash("OnEnemyDeath"), GetOwner()});
}

void PookaComponent::Inflate()
{
	m_isInflating = true;
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
				if (dynamic_cast<PookaStates::InflatedState*>(m_pState.get()) == nullptr)
				{
					SetState(std::make_unique<PookaStates::InflatedState>());
				}
				else
				{
					Inflate();
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

void PookaComponent::Reset()
{
	if (!m_isDead)
	{
		GetOwner()->GetTransform()->SetLocalPosition(m_initialPosition);
		GetOwner()->SetRenderLayer(4);
		SetState(std::make_unique<PookaStates::MovingState>());
	}
}

void PookaComponent::SetState(std::unique_ptr<PookaStates::PookaState> newState)
{
	m_pState->Exit(this);
	m_pState = std::move(newState);
	m_pState->Enter(this);
}
