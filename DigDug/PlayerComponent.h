#pragma once
#include "BaseComponent.h"
#include "PlayerState.h"
#include "SubjectObserver.h"

class PlayerComponent : public dae::BaseComponent, public dae::Observer
{
public:
	PlayerComponent(dae::GameObject* owner, PlayerState* state);
	~PlayerComponent() = default;
	PlayerComponent(const PlayerComponent& other) = delete;
	PlayerComponent(PlayerComponent&& other) = delete;
	PlayerComponent& operator=(const PlayerComponent& other) = delete;
	PlayerComponent& operator=(PlayerComponent&& other) = delete;

	void Update() override;
	void OnNotify(dae::Event event) override;
private:
	PlayerState* m_pState;
};

