#pragma once
#include "EnemyComponent.h"
#include "GameConstants.h"

namespace PookaStates {
	class PookaState;
}
class PookaComponent : public EnemyComponent
{
public:
	PookaComponent(dae::GameObject* owner, float speed = 115.f);
	~PookaComponent() = default;
	PookaComponent(const PookaComponent& other) = delete;
	PookaComponent(PookaComponent&& other) = delete;
	PookaComponent& operator=(const PookaComponent& other) = delete;
	PookaComponent& operator=(PookaComponent&& other) = delete;
	void Update() override;

	void Kill();
	void Inflate();
	void OnNotify(dae::Event event) override;
	void Reset();
private:
	void SetState(std::unique_ptr<PookaStates::PookaState> newState);
	std::unique_ptr<PookaStates::PookaState> m_pState;
};

