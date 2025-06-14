#pragma once
#include "EnemyComponent.h"

namespace FygarStates {
	class FygarState;
}
class FygarComponent : public EnemyComponent
{
public:
	FygarComponent(dae::GameObject* owner, float speed = 115.f);
	~FygarComponent() = default;
	FygarComponent(const FygarComponent& other) = delete;
	FygarComponent(FygarComponent&& other) = delete;
	FygarComponent& operator=(const FygarComponent& other) = delete;
	FygarComponent& operator=(FygarComponent&& other) = delete;
	void Update() override;

	void Kill();
	void Inflate();
	void OnNotify(dae::Event event) override;
	void Reset();
private:
	void SetState(std::unique_ptr<FygarStates::FygarState> newState);
	std::unique_ptr<FygarStates::FygarState> m_pState;
};

