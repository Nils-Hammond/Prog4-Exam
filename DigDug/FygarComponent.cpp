#include "FygarComponent.h"
#include "ColliderComponent.h"
#include "MoveComponent.h"
#include "FygarState.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"

FygarComponent::FygarComponent(dae::GameObject* owner, float speed) :
	EnemyComponent(owner, speed)
	, m_pState(std::make_unique<FygarStates::MovingState>())
{
	m_pState->Enter(this);
}

void FygarComponent::Update()
{
	auto newState = m_pState->Update(this);
	if (newState != nullptr)
	{
		SetState(std::move(newState));
	}
	m_isInflating = false;
}

void FygarComponent::Kill()
{
	m_isDead = true;
	m_pDeathSubject->Notify(dae::Event{ dae::make_sdbm_hash("OnEnemyDeath"), GetOwner() });
}

void FygarComponent::Inflate()
{
	m_isInflating = true;
}

void FygarComponent::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("OnCollision"))
	{
		try
		{
			auto* collider = std::any_cast<const dae::ColliderComponent*>(event.data);
			if (collider && collider->GetTag() == dae::make_sdbm_hash("Pump"))
			{
				if (dynamic_cast<FygarStates::InflatedState*>(m_pState.get()) == nullptr)
				{
					SetState(std::make_unique<FygarStates::InflatedState>());
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

void FygarComponent::Reset()
{
	if (!m_isDead)
	{
		GetOwner()->GetTransform()->SetLocalPosition(m_initialPosition);
		GetOwner()->SetRenderLayer(4);
		SetState(std::make_unique<FygarStates::MovingState>());
	}
}

void FygarComponent::SetState(std::unique_ptr<FygarStates::FygarState> newState)
{
	m_pState->Exit(this);
	m_pState = std::move(newState);
	m_pState->Enter(this);
}
