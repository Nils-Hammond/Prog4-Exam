#include "PlayerComponent.h"

PlayerComponent::PlayerComponent(dae::GameObject* owner, PlayerState* state)
	: dae::BaseComponent(owner), m_pState(state)
{
}

void PlayerComponent::Update()
{
	PlayerState* state = m_pState->Update(this);
	if (state != nullptr)
	{
		m_pState->Exit(this);
		delete m_pState;
		m_pState = state;
		m_pState->Enter(this);
	}
}
