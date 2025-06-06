#pragma once
#include "BaseComponent.h"
#include "PlayerState.h"

class PlayerComponent : public dae::BaseComponent
{
public:
	PlayerComponent(dae::GameObject* owner, PlayerState* state);
	~PlayerComponent() = default;
	PlayerComponent(const PlayerComponent& other) = delete;
	PlayerComponent(PlayerComponent&& other) = delete;
	PlayerComponent& operator=(const PlayerComponent& other) = delete;
	PlayerComponent& operator=(PlayerComponent&& other) = delete;

	void Update() override;
private:
	PlayerState* m_pState;
};

