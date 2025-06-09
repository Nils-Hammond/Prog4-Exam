#include "PlayerComponent.h"
#include "PlayerState.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include <iostream>

PlayerComponent::PlayerComponent(dae::GameObject* owner, PlayerState* state)
	: dae::BaseComponent(owner), m_pState(state)
{
	GetOwner()->GetComponent<dae::ColliderComponent>()->AddObserver(this);
}

void PlayerComponent::Update()
{
	/*
	PlayerState* state = m_pState->Update(this);
	if (state != nullptr)
	{
		m_pState->Exit(this);
		delete m_pState;
		m_pState = state;
		m_pState->Enter(this);
	}
	*/
}

void PlayerComponent::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("Collision"))
	{
		auto* collider = std::any_cast<dae::ColliderComponent*>(event.data);
		if (collider && collider->GetTag() == dae::make_sdbm_hash("Rock"))
		{
			std::cout << "Player collided with a rock" << std::endl;
		}
	}
}
