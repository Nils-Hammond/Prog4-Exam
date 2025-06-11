#pragma once
#include "BaseComponent.h"
#include "SubjectObserver.h"
#include "glm.hpp"

namespace dae {
	class SpriteRenderComponent;
}

class PlayerState;
class MoveComponent;
class PlayerComponent : public dae::BaseComponent, public dae::Observer
{
public:
	PlayerComponent(dae::GameObject* owner);
	~PlayerComponent() = default;
	PlayerComponent(const PlayerComponent& other) = delete;
	PlayerComponent(PlayerComponent&& other) = delete;
	PlayerComponent& operator=(const PlayerComponent& other) = delete;
	PlayerComponent& operator=(PlayerComponent&& other) = delete;

	void Update() override;
	void OnNotify(dae::Event event) override;
	dae::SpriteRenderComponent* GetSpriteRenderComponent() const;
	MoveComponent* GetMoveComponent() const;
	int GetPlayerNumber() const { return m_playerNumber; }
	void Attack();
private:
	void SetState(std::unique_ptr<PlayerState> newState);
	std::unique_ptr<PlayerState> m_pState;
	MoveComponent* m_pMoveComponent;
	dae::SpriteRenderComponent* m_pSpriteRenderComponent;
	int m_playerNumber;
};

