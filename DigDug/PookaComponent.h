#pragma once
#include "BaseComponent.h"
#include "glm.hpp"
#include "GameConstants.h"
#include "SubjectObserver.h"

namespace dae {
	class SpriteRenderComponent;
}
namespace PookaStates {
	class PookaState;
}
class MoveComponent;
class PookaComponent : public dae::BaseComponent, public dae::Observer
{
public:
	PookaComponent(dae::GameObject* owner, float speed = 115.f);
	~PookaComponent() = default;
	PookaComponent(const PookaComponent& other) = delete;
	PookaComponent(PookaComponent&& other) = delete;
	PookaComponent& operator=(const PookaComponent& other) = delete;
	PookaComponent& operator=(PookaComponent&& other) = delete;
	void Update() override;

	float GetSpeed() const { return m_speed; }
	dae::SpriteRenderComponent* GetSpriteRenderComponent() const;
	MoveComponent* GetMoveComponent() const;
	void OnNotify(dae::Event event) override;
	const dae::GameObject* GetClosestPlayer() const;
	void SetPlayers(const std::vector<const dae::GameObject*>& players);
private:
	void SetState(std::unique_ptr<PookaStates::PookaState> newState);
	float m_speed;
	bool m_hitByPump;
	std::unique_ptr<PookaStates::PookaState> m_pState;
	MoveComponent* m_pMoveComponent;
	dae::SpriteRenderComponent* m_pSpriteRenderComponent;
	std::vector<const dae::GameObject*> m_pPlayers;
};

